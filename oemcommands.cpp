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
#include "oemcommands.hpp"

#include <ipmid/api.hpp>
#include <phosphor-logging/log.hpp>
#include <xyz/openbmc_project/Software/Activation/server.hpp>
#include <xyz/openbmc_project/Software/Version/server.hpp>

static constexpr auto softwareRoot = "/xyz/openbmc_project/software";
static constexpr auto biosObj = "/xyz/openbmc_project/software/bios_active";
static constexpr auto versionIntf = "xyz.openbmc_project.Software.Version";
static constexpr auto redundancyIntf =
    "xyz.openbmc_project.Software.RedundancyPriority";
static constexpr auto activationIntf =
    "xyz.openbmc_project.Software.Activation";
using Version = sdbusplus::xyz::openbmc_project::Software::server::Version;
using Activation =
    sdbusplus::xyz::openbmc_project::Software::server::Activation;

namespace ipmi
{
using namespace phosphor::logging;

namespace nuvoton
{
ipmi::RspType<uint8_t> ipmiOEMGetPwmMode()
{
    uint8_t rc, value;
    rc = pwm_control->getPwmMode(&value);
    return ipmi::response(rc, value);
}

ipmi::RspType<> ipmiOEMSetManualPwm(uint8_t enabled)
{
    uint8_t rc;
    rc = pwm_control->setManualPwm(enabled);
    return ipmi::response(rc);
}

ipmi::RspType<uint8_t> ipmiOEMGetPwm(uint8_t pwm_id)
{
    uint8_t value, rc;
    rc = pwm_control->getPwmValue(pwm_id, &value);
    return ipmi::response(rc, value);
}

ipmi::RspType<uint8_t> ipmiOEMSetPwm(uint8_t pwm_id, uint8_t value)
{
    uint8_t rc = pwm_control->setPwmValue(pwm_id, value);
    return ipmi::response(rc, value);
}

void createPwmControl()
{
    pwm_control = std::make_unique<IpmiPwmcontrol>();
}

ipmi::RspType<uint8_t> ipmiOEMGetPostCode(ipmi::Context::ptr& ctx)
{
    using postcode_t = std::tuple<uint64_t, std::vector<uint8_t>>;
    auto conn = getSdBus();

    try
    {
        auto method =
            conn->new_method_call("xyz.openbmc_project.State.Boot.Raw",
                                  "/xyz/openbmc_project/state/boot/raw0",
                                  "org.freedesktop.DBus.Properties", "Get");

        method.append("xyz.openbmc_project.State.Boot.Raw", "Value");

        auto reply = conn->call(method);
        std::variant<postcode_t> postCode;
        reply.read(postCode);

        return ipmi::responseSuccess(
            std::get<0>(std::get<postcode_t>(postCode)));
    }
    catch (std::exception& e)
    {
        log<level::ERR>("ipmiOEMGetPostCode err");
        return ipmi::responseUnspecifiedError();
    }

    return ipmi::responseSuccess();
}
namespace postcode
{
static constexpr auto postCodeIntf = "xyz.openbmc_project.State.Boot.PostCode";
static constexpr auto postCodeObj = "/xyz/openbmc_project/State/Boot/PostCode0";
static constexpr auto MAX_CODE_BYTES = 240;
using postcode_t = std::tuple<uint64_t, std::vector<uint8_t>>;

int getPostCodes(uint16_t index, std::string service,
                 std::vector<uint8_t>& codes)
{
    auto conn = getSdBus();
    std::vector<postcode_t> post_codes;
    try
    {
        auto method = conn->new_method_call(service.c_str(), postCodeObj,
                                            postCodeIntf, "GetPostCodes");
        method.append(index);
        auto reply = conn->call(method);
        reply.read(post_codes);
    }
    catch (const std::exception& e)
    {
        log<level::ERR>(e.what());
        return -1;
    }
    if (post_codes.empty())
    {
        return 0;
    }
    // convert a(tay) to at, we only need the code and ignore desc
    std::transform(post_codes.begin(), post_codes.end(),
                   std::back_inserter(codes),
                   [](const auto& pc) { return std::get<0>(pc); });
    // according to spec we only can send 240 bytes, remove the front data
    if (codes.size() > MAX_CODE_BYTES)
    {
        size_t rm_count = codes.size() - MAX_CODE_BYTES;
        codes.erase(codes.begin(), codes.begin() + rm_count);
    }
    return 0;
}
// if previous == 0, use current post code
ipmi::RspType<std::vector<uint8_t>> ipmiOEMGetPostCodes(uint8_t previous)
{
    std::string msg, service;
    std::vector<uint8_t> codes;
    int ret;
    if (previous > 1)
    {
        return ipmi::responseParmOutOfRange();
    }
    try
    {
        service = ipmi::getService(*getSdBus(), postCodeIntf, postCodeObj);
    }
    catch (const std::exception& e)
    {
        log<level::ERR>("Cannot get post code manager service");
        return ipmi::responseUnspecifiedError();
    }

    // if previous post code not exist, we will get empty codes, so we don't
    // need handle index error
    ret = getPostCodes(previous + 1, service, codes);
    if (ret)
    {
        return ipmi::responseUnspecifiedError();
    }
    return ipmi::responseSuccess(codes);
}
} // namespace postcode

int getBMCVersionInfo(ipmi::Context::ptr& ctx, std::string& ver)
{
    ipmi::ObjectTree objectTree;

    try
    {
        objectTree =
            ipmi::getAllDbusObjects(*ctx->bus, softwareRoot, versionIntf);
    }
    catch (const sdbusplus::exception::exception& e)
    {
        log<level::ERR>("Failed to fetch software object from dbus",
                        entry("INTERFACE=%s", versionIntf),
                        entry("ERRMSG=%s", e.what()));
        return -1;
    }

    auto objectFound = false;
    for (auto& softObject : objectTree)
    {
        auto service =
            ipmi::getService(*ctx->bus, versionIntf, softObject.first);
        auto objValueTree =
            ipmi::getManagedObjects(*ctx->bus, service, softwareRoot);

        auto minPriority = 0xFF;
        for (const auto& objIter : objValueTree)
        {
            try
            {
                auto& intfMap = objIter.second;
                auto& redundancyPriorityProps = intfMap.at(redundancyIntf);
                auto& versionProps = intfMap.at(versionIntf);
                auto& activationProps = intfMap.at(activationIntf);
                auto priority =
                    std::get<uint8_t>(redundancyPriorityProps.at("Priority"));
                auto purpose =
                    std::get<std::string>(versionProps.at("Purpose"));
                auto activation =
                    std::get<std::string>(activationProps.at("Activation"));
                auto version =
                    std::get<std::string>(versionProps.at("Version"));
                if ((Version::convertVersionPurposeFromString(purpose) ==
                     Version::VersionPurpose::BMC) &&
                    (Activation::convertActivationsFromString(activation) ==
                     Activation::Activations::Active))
                {
                    if (priority < minPriority)
                    {
                        minPriority = priority;
                        objectFound = true;
                        ver = std::move(version);
                    }
                }
            }
            catch (const std::exception& e)
            {
                log<level::ERR>(e.what());
            }
        }
    }

    if (!objectFound)
    {
        log<level::ERR>("Could not found an BMC software Object");
        return -1;
    }

    return 0;
}

int getBIOSVersionInfo(ipmi::Context::ptr& ctx, std::string& ver)
{
    // read software manager object first
    auto service = ipmi::getService(*ctx->bus, versionIntf, biosObj);
    try
    {
        auto objProperty = ipmi::getDbusProperty(*ctx->bus, service, biosObj,
                                                 versionIntf, "Version");
        auto version = std::get<std::string>(objProperty);
        ver = std::move(version);
    }
    catch (const sdbusplus::exception::exception& e)
    {
        log<level::ERR>("Failed to fetch BIOS object from dbus",
                        entry("ERRMSG=%s", e.what()));
        return -1;
    }
    return 0;
}

ipmi::RspType<std::vector<char>>
    ipmiOEMGetFirmwareVersion(ipmi::Context::ptr ctx, uint8_t type)
{
    int res = 0;
    std::string version{};

    log<level::INFO>("ipmiOEMGetFirmwareVersion");

    switch (type)
    {
        case as_int(FirmwareType::BMC):
            res = getBMCVersionInfo(ctx, version);
            break;
        case as_int(FirmwareType::BIOS):
            res = getBIOSVersionInfo(ctx, version);
            break;
        // TODO: wait for implement
        case as_int(FirmwareType::CPLD):
        case as_int(FirmwareType::SCM_CPLD):
            res = cpld::getCpldVersionInfo(ctx, version, type);
            // return ipmi::responseDestinationUnavailable();
            break;
        case as_int(FirmwareType::PSU):
            res = psu::getPsuVersionInfo(ctx, version);
            break;
        default:
            log<level::ERR>("GetFirmwareVersion: invalid type");
            return ipmi::responseParmOutOfRange();
    }
    if (res)
        return ipmi::responseUnspecifiedError();

    // convert string to vector<char> to avoid byte count
    std::vector<char> v(version.length());
    std::copy(version.begin(), version.end(), v.begin());

    return ipmi::responseSuccess(v);
}

ipmi::RspType<uint8_t, uint8_t> ipmiOEMGetGpio(uint8_t pinNum)
{
    return ipmiOEMGetGpioStatus(pinNum);
}

ipmi::RspType<uint8_t> ipmiFwUpdate(uint8_t target, uint8_t region,
                                    uint8_t action,
                                    std::optional<std::vector<uint8_t>> image)
{
    std::string img;
    std::string msg;
    // convert std::vector<uint8_t> to std::string
    if (image.has_value())
    {
        auto image_bytes = image.value();
        img = std::string(image_bytes.begin(), image_bytes.end());
    }
    msg = "target: " + std::to_string(target) +
          ", region: " + std::to_string(region) +
          ", action:" + std::to_string(action);
    log<level::INFO>(msg.c_str());
    switch (target)
    {
        case as_int(FirmwareTarget::PSU):
            return psu::ipmiOemPsuFwUpdate(region, action, img);
    }
    return ipmi::responseUnspecifiedError();
}

} // namespace nuvoton

static void registerOEMFunctions() __attribute__((constructor));

static void registerOEMFunctions(void)
{
    log<level::INFO>("Registering Nuvoton IPMI OEM commands");

    nuvoton::createPwmControl();

    // <Get Manual PWM command>
    registerHandler(prioOemBase, netFnOemTwo, nuvoton::fan::cmdGetPwmMode,
                    Privilege::Callback, nuvoton::ipmiOEMGetPwmMode);

    // <Set Manual PWM command>
    registerHandler(prioOemBase, netFnOemTwo, nuvoton::fan::cmdSetManualPwm,
                    Privilege::Callback, nuvoton::ipmiOEMSetManualPwm);

    // <Get PWM value command>
    registerHandler(prioOemBase, netFnOemTwo, nuvoton::fan::cmdGetPwm,
                    Privilege::Callback, nuvoton::ipmiOEMGetPwm);

    // <Set PWM value command>
    registerHandler(prioOemBase, netFnOemTwo, nuvoton::fan::cmdSetPwm,
                    Privilege::Callback, nuvoton::ipmiOEMSetPwm);

    // Get BIOS post code
    registerHandler(prioOemBase, netFnOemTwo, nuvoton::cmdGetPostCode,
                    Privilege::User, nuvoton::postcode::ipmiOEMGetPostCodes);

    // Get firmware version
    registerHandler(prioOemBase, netFnOemFive, nuvoton::cmdGetFimwareVer,
                    Privilege::User, nuvoton::ipmiOEMGetFirmwareVersion);

    // <Get GPIO status command>
    registerHandler(prioOemBase, netFnOemOne, nuvoton::cmdGetGpioStatus,
                    Privilege::User, nuvoton::ipmiOEMGetGpio);

    // <OEM firmware update command>
    registerHandler(prioOemBase, netFnOemFive, nuvoton::cmdFirmwareUpdate,
                    Privilege::User, nuvoton::ipmiFwUpdate);
    // Master Phase Write Read
    registerHandler(prioOemBase, netFnOemFive, nuvoton::cmdPsuPhase,
                    Privilege::User, nuvoton::psu::masterPhase);
}

} // namespace ipmi
