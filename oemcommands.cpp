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
#include <xyz/openbmc_project/Software/Version/server.hpp>

static constexpr auto softwareRoot = "/xyz/openbmc_project/software";
static constexpr auto versionIntf = "xyz.openbmc_project.Software.Version";
using Version = sdbusplus::xyz::openbmc_project::Software::server::Version;

namespace ipmi
{
using namespace phosphor::logging;

namespace nuvoton
{
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
        auto method = conn->new_method_call(
            "xyz.openbmc_project.State.Boot.Raw", "/xyz/openbmc_project/state/boot/raw0",
            "org.freedesktop.DBus.Properties", "Get");

        method.append("xyz.openbmc_project.State.Boot.Raw", "Value");

        auto reply = conn->call(method);
        std::variant<postcode_t> postCode;
        reply.read(postCode);

        return ipmi::responseSuccess(std::get<0>(std::get<postcode_t>(postCode)));
    }
    catch (std::exception& e)
    {
        log<level::ERR>("ipmiOEMGetPostCode err");
        return ipmi::responseUnspecifiedError();
    }

    return ipmi::responseSuccess();
}

int convertVersion(std::string s, Revision& rev)
{
    std::string token;

    auto location = s.find_first_of('v');
    if (location != std::string::npos)
    {
        s = s.substr(location + 1);
    }

    if (!s.empty())
    {
        location = s.find_first_of(".");
        if (location != std::string::npos)
        {
            rev.major =
                static_cast<char>(std::stoi(s.substr(0, location), 0, 10));
            token = s.substr(location + 1);
        }

        if (!token.empty())
        {
            location = token.find_first_of(".-");
            if (location != std::string::npos)
            {
                rev.minor = static_cast<char>(
                    std::stoi(token.substr(0, location), 0, 10));
                token = token.substr(location + 1);
            }
        }

    } else
        return -1;

    return 0;
}

std::string getBMCVersionInfo(ipmi::Context::ptr& ctx)
{
    std::string revision{};
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
    }

    auto objectFound = false;
    for (auto& softObject : objectTree)
    {
        auto service =
            ipmi::getService(*ctx->bus, versionIntf, softObject.first);
        auto objValueTree =
            ipmi::getManagedObjects(*ctx->bus, service, softwareRoot);

        for (const auto& objIter : objValueTree)
        {
            try
            {
                auto& intfMap = objIter.second;
                auto& versionProps = intfMap.at(versionIntf);
                auto purpose =
                    std::get<std::string>(versionProps.at("Purpose"));
                auto version =
                    std::get<std::string>(versionProps.at("Version"));
                if (Version::convertVersionPurposeFromString(purpose) ==
                     Version::VersionPurpose::BMC)
                {
                        objectFound = true;
                        revision = std::move(version);
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
    }

    return revision;
}

ipmi::RspType<uint8_t, // major
              uint8_t  // minor
          >
      ipmiOEMGetFirmwareVersion(ipmi::Context::ptr ctx, uint8_t type)
{
    int r;
    Revision rev = {0};
    std::string version{};

    log<level::INFO>("ipmiOEMGetFirmwareVersion");

    switch (type)
    {
        case ipmi::nuvoton::FirmwareType::BMC:
            version = getBMCVersionInfo(ctx);
            break;

        default:
            log<level::ERR>("GetFirmwareVersion: invalid type");
            return ipmi::responseUnspecifiedError();
    }
    r = convertVersion(version, rev);
    if (r < 0)
    {
        log<level::INFO>("convertVersion error");
        return ipmi::responseUnspecifiedError();
    }

    return ipmi::responseSuccess(rev.major, rev.minor);
}
} // namespace nuvoton

static void registerOEMFunctions() __attribute__((constructor));

static void registerOEMFunctions(void)
{
    log<level::INFO>("Registering Nuvoton IPMI OEM commands");

    nuvoton::createPwmControl();

    // <Set Manual PWM command>
    registerHandler(prioOemBase, netFnOemThree, nuvoton::fan::cmdSetManualPwm,
                    Privilege::Callback, nuvoton::ipmiOEMSetManualPwm);

    // <Get PWM value command>
    registerHandler(prioOemBase, netFnOemThree, nuvoton::fan::cmdGetPwm,
                    Privilege::Callback, nuvoton::ipmiOEMGetPwm);

    // <Set PWM value  command>
    registerHandler(prioOemBase, netFnOemThree, nuvoton::fan::cmdSetPwm,
                    Privilege::Callback, nuvoton::ipmiOEMSetPwm);

    // Get BIOS post code
    registerHandler(prioOemBase, netFnOemTwo, nuvoton::cmdGetPostCode,
                    Privilege::User, nuvoton::ipmiOEMGetPostCode);

    // Get firmware version
    registerHandler(prioOemBase, netFnOemFive, nuvoton::cmdGetFimwareVer,
                    Privilege::User, nuvoton::ipmiOEMGetFirmwareVersion);
}

} // namespace ipmi
