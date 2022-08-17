/*
// Copyright (c) 2020 Nuvoton Technology Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
*/

#include "ipmi_pwm.hpp"

#include <ipmid/api.h>

#include <ipmid/api.hpp>
#include <phosphor-logging/log.hpp>

namespace ipmi
{
namespace nuvoton
{
using namespace phosphor::logging;
// The command object path both hwmon and dbus sensor contains
constexpr auto FAN_COMMON_OBJ_PATH = "/xyz/openbmc_project/sensors/fan_tach";
// PWM fan sensor object path
// constexpr auto FAN_PWM_PATH = "/xyz/openbmc_project/sensors/fan_pwm/Pwm_";
// Dbus sensor PWM object path which implement control interface
constexpr auto FAN_PWM_CTRL_PATH = "/xyz/openbmc_project/control/fanpwm/Pwm_";
constexpr auto FAN_PWM_CTRL_PATH_U = "/xyz/openbmc_project/control/fanpwm/PWM_";
// TODO: check the object FanX is fixed or setting by conf?
// Hwmon PWM object path
constexpr auto FAN_TACH_PATH = "/xyz/openbmc_project/sensors/fan_tach/Fan";
// Fan Pwm control interface
constexpr auto FAN_CTRL_PWM_INTF = "xyz.openbmc_project.Control.FanPwm";
constexpr auto FAN_PWM_PROPERTY = "Target";
// constexpr auto SENSOR_INTF = "xyz.openbmc_project.Sensor.Value";
// PID PWM manual mode interface and object path root
constexpr auto PID_FAN_INTF = "xyz.openbmc_project.Control.Mode";
constexpr auto PID_FAN_OBJ_ROOT = "/xyz/openbmc_project/settings/fanctrl";
constexpr auto PID_MANUAL_PROPERTY = "Manual";

/**
 * @brief get the fan sensor service, which create by hwmon or dbus sensor
 *
 *  _fan_service:
 *   Hwmon       => xyz.openbmc_project.Hwmon-[377141964.Hwmon1]
 *   Dbus sensor => xyz.openbmc_project.FanSensor
 * @return std::string the fan sensor service
 */
std::string IpmiPwmcontrol::getFanService()
{
    std::string msg, fan_service;
    try
    {
        fan_service =
            ipmi::getService(*getSdBus(), ipmi::PROP_INTF, FAN_COMMON_OBJ_PATH);
    }
    catch (const std::exception& e)
    {
        msg = "Cannot get fan tach sensor service, " + std::string(e.what());
        log<level::ERR>(msg.c_str());
    }
    // check PWM object is all upper case or normal case
    if (fan_service.find("xyz.openbmc_project.FanSensor") != std::string::npos)
    {
        try
        {
            std::string fullPath = FAN_PWM_CTRL_PATH_U + std::string("1");
            ipmi::getAllDbusProperties(*getSdBus(), fan_service, fullPath,
                                       FAN_CTRL_PWM_INTF);
            _is_pwm_upper = true;
            log<level::DEBUG>("use upper case pwm object");
        }
        catch (const std::exception& e)
        {
            _is_pwm_upper = false;
        }
    }

    return fan_service;
}

uint8_t IpmiPwmcontrol::getFanServiceType()
{
    std::string msg;
    // check fan service is valid first
    if (_fan_service.empty())
    {
        log<level::INFO>("fan service is not valid, try to get it");
        _fan_service = getFanService();
    }
    if (_fan_service.find("xyz.openbmc_project.Hwmon") != std::string::npos)
    {
        return fanServiceHwmon;
    }
    else if (_fan_service.find("xyz.openbmc_project.FanSensor") !=
             std::string::npos)
    {
        return fanServiceDbusSensor;
    }
    log<level::ERR>("Cannot get valid fan service");
    return fanServiceInvalid;
}

/**
 * @brief simple util for convert double to uint8_t safe
 *
 * @param input the double data from sensor reading
 * @return uint8_t the true value with type uint8_t
 */
uint8_t double_to_uint8(double input)
{
    uint8_t output;
    char temp[8];

    std::snprintf(temp, 8, "%.0f", std::round(input));
    temp[7] = 0x00;
    std::sscanf(temp, "%hhu", &output);
    std::string msg =
        "convert " + std::to_string(input) + " to " + std::to_string(output);
    log<level::DEBUG>(msg.c_str());
    return output;
}

uint8_t IpmiPwmcontrol::setPwmProperty(const std::string& path,
                                       const std::string& interface,
                                       const std::string& property,
                                       ipmi::Value value)
{
    std::string msg;
    try
    {
        ipmi::setDbusProperty(*getSdBus(), _fan_service, path, interface,
                              property, value);
    }
    catch (const sdbusplus::exception::exception& e)
    {
        msg = "Cannot set fan property, path:" + path + ", " +
              std::string(e.what());
        log<level::ERR>(msg.c_str());
        return IPMI_CC_UNSPECIFIED_ERROR;
    }
    return IPMI_CC_OK;
}

/**
 * @brief Get PWM value from Hwmon or Dbus sensor service
 * We use the FanPwm target as value, we also need take care
 * 1. TARGET_MODE="PWM" must set in sensor conf environment for hwmon
 * 2. return data type is uint64, range 0~255
 * 3. this function may not work before execute set pwm command(hwmon)
 * ex:
 * obj: /xyz/openbmc_project/sensors/fan_tach/Fan4 (hwmon)
 *      /xyz/openbmc_project/control/fanpwm/Pwm_4 (dbus sensor)
 *   xyz.openbmc_project.Control.FanPwm    interface -     -
 *   .Target                               property  t     100
 *
 * 4. for the MS PWM spec, we should set target value as percentage,
 *    so map the value to 0~255
 * @param instance the pwm id
 * @param value the value reference, the PWM data will return here
 * @return uint8_t IPMI status code
 */
uint8_t IpmiPwmcontrol::getPwmValue(uint8_t instance, uint8_t* value)
{
    std::string fullPath;
    std::string msg;
    uint64_t reading = 0;
    switch (getFanServiceType())
    {
        case fanServiceHwmon:
            fullPath = FAN_TACH_PATH + std::to_string(instance + 1);
            break;
        case fanServiceDbusSensor:
            if (_is_pwm_upper)
                fullPath = FAN_PWM_CTRL_PATH_U + std::to_string(instance + 1);
            else
                fullPath = FAN_PWM_CTRL_PATH + std::to_string(instance + 1);
            break;
        default:
            return IPMI_CC_UNSPECIFIED_ERROR;
    }

    msg = "full path: " + fullPath + ", instance:" + std::to_string(instance);
    log<level::DEBUG>(msg.c_str());
    try
    {
        auto propValue =
            ipmi::getDbusProperty(*getSdBus(), _fan_service, fullPath,
                                  FAN_CTRL_PWM_INTF, FAN_PWM_PROPERTY);
        reading = std::get<uint64_t>(propValue);
        double convert_value = reading * 100.0 / 255;
        *value = double_to_uint8(convert_value);
    }
    catch (const sdbusplus::exception::exception& ex)
    {
        msg = "Cannot get PWM from " + fullPath + ", " + std::string(ex.what());
        log<level::ERR>(msg.c_str());
        return IPMI_CC_INVALID;
    }
    return IPMI_CC_OK;
}

uint8_t IpmiPwmcontrol::setPwmValue(uint8_t instance, uint8_t percentage)
{
    double convert_value;
    uint8_t value;
    if (percentage > 0x64)
    {
        return IPMI_CC_PARM_OUT_OF_RANGE;
    }
    convert_value = (percentage * 255.0) / 100;
    value = double_to_uint8(convert_value);
    switch (getFanServiceType())
    {
        case fanServiceHwmon:
            return hwmonSetPwm(instance, value);
        case fanServiceDbusSensor:
            return dbusSetPwm(instance, value);
        default:
            return IPMI_CC_UNSPECIFIED_ERROR;
    }
}

/**
 * @brief set PWM by dbus pwm sensor dbus object property
 * use the PWM control object path
 * ex: /xyz/openbmc_project/control/fanpwm/Pwm_1
 * xyz.openbmc_project.Control.FanPwm  interface -   -
 * .Target                             property  t   255
 *
 * Note: the PWM sensor /xyz/openbmc_project/sensors/fan_pwm/Pwm_x
 * is also good to get sensor reading, but it present value as percentage
 * @param instance the PWM controller instance
 * @param value PWM value we want to set
 * @return uint8_t IPMI status code
 */
uint8_t IpmiPwmcontrol::dbusSetPwm(uint8_t instance, uint8_t value)
{
    std::string fullPath = FAN_PWM_CTRL_PATH + std::to_string(instance + 1);
    std::string msg =
        "full path: " + fullPath + ", instance:" + std::to_string(instance);
    log<level::DEBUG>(msg.c_str());
    uint8_t rc;
    rc = setPwmProperty(fullPath, FAN_CTRL_PWM_INTF, FAN_PWM_PROPERTY,
                        (uint64_t)value);

    return rc;
}

uint8_t IpmiPwmcontrol::hwmonSetPwm(uint8_t instance, uint8_t value)
{
    std::string fullPath = FAN_TACH_PATH + std::to_string(instance + 1);
    std::string msg =
        "full path: " + fullPath + ", instance:" + std::to_string(instance);
    log<level::DEBUG>(msg.c_str());
    uint8_t rc;
    log<level::INFO>(msg.c_str());
    rc = setPwmProperty(fullPath, FAN_CTRL_PWM_INTF, FAN_PWM_PROPERTY,
                        (uint64_t)value);

    return rc;
}

uint8_t IpmiPwmcontrol::setManualPwm(uint8_t enabled)
{
    //  Get service and zone object paths
    std::string service;
    std::vector<std::string> objs;
    ZoneInfo info = getPidZoneInfo();
    if (info.first.empty())
    {
        log<level::WARNING>(
            "Cannot get zone info, PID service may get some error");
        return IPMI_CC_INVALID;
    }

    // Set manual mode for each zone
    std::string msg;
    try
    {
        for (auto it = info.second.begin(); it != info.second.end(); ++it)
        {
            // msg =
            //     "set " + *it + " manual mode " +
            //     std::to_string((bool)enabled);
            // log<level::DEBUG>(msg.c_str());
            ipmi::setDbusProperty(*getSdBus(), info.first, *it, PID_FAN_INTF,
                                  PID_MANUAL_PROPERTY, (bool)enabled);
        }
    }
    catch (const std::exception& e)
    {
        msg = "Set fan property failed, " + std::string(e.what());
        log<level::ERR>(msg.c_str());
        return IPMI_CC_UNSPECIFIED_ERROR;
    }

    return IPMI_CC_OK;
}

uint8_t IpmiPwmcontrol::getPwmMode(uint8_t* value)
{
    //  Get service and zone object paths
    std::string service;
    std::vector<std::string> objs;
    ZoneInfo info = getPidZoneInfo();
    bool reading = false;

    if (info.first.empty())
    {
        log<level::WARNING>(
            "Cannot get zone info, PID service may get some error");
        return IPMI_CC_INVALID;
    }

    // Return non-manual if all zones are not manual
    try
    {
        for (auto it = info.second.begin(); it != info.second.end(); ++it)
        {
            auto propValue =
                ipmi::getDbusProperty(*getSdBus(), info.first, *it,
                                      PID_FAN_INTF, PID_MANUAL_PROPERTY);
            reading = std::get<bool>(propValue) || reading;
        }
        *value = (uint8_t)reading;
    }
    catch (const std::exception& e)
    {
        std::string msg;
        msg = "Get fan property failed, " + std::string(e.what());
        log<level::ERR>(msg.c_str());
        return IPMI_CC_UNSPECIFIED_ERROR;
    }

    return IPMI_CC_OK;
}

/**
 * @brief Get the Pid Zone Info object
 * The getDbusObject function in ipmid will return only the
 * first object we found, but we need all object paths.
 *
 * @return ZoneInfo the zone service name and object paths
 */
ZoneInfo IpmiPwmcontrol::getPidZoneInfo()
{
    ZoneInfo info =
        std::make_pair(std::string(), std::vector<DbusObjectPath>());
    std::string service;
    std::vector<DbusObjectPath> objs;
    // call dbus command to get zone object path
    ObjectTree objectTree;
    try
    {
        objectTree = ipmi::getAllDbusObjects(*getSdBus(), PID_FAN_OBJ_ROOT,
                                             PID_FAN_INTF, "zone");
    }
    catch (const std::exception& e)
    {
        log<level::ERR>("Cannot get call dbus command to get zone object");
        return info;
    }

    if (objectTree.empty())
    {
        log<level::ERR>("Cannot get any zone object");
        return info;
    }

    // save dbus reply to zone info
    service = objectTree.begin()->second.begin()->first;
    for (auto it = objectTree.begin(); it != objectTree.end(); ++it)
    {
        objs.push_back(it->first);
    }
    return std::make_pair(service, objs);
}

} // namespace nuvoton

} // namespace ipmi
