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
    registerHandler(prioOemBase, netFnOemTwo, nuvoton::postcode::cmdGetPostCode,
                    Privilege::User, nuvoton::ipmiOEMGetPostCode);
}

} // namespace ipmi
