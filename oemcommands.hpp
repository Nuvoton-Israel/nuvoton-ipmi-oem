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

#pragma once

#include "ipmi_pwm.hpp"

#include <ipmid/api-types.hpp>

namespace ipmi
{
namespace nuvoton
{

typedef struct
{
    char major;
    char minor;
} Revision;

enum FirmwareType : uint8_t
{
    BIOS = 0,
    CPLD,
    BMC,
    PSU,
};

namespace fan
{
static constexpr Cmd cmdSetManualPwm = 0x90;
static constexpr Cmd cmdGetPwm = 0x92;
static constexpr Cmd cmdSetPwm = 0x91;
} // namespace fan

static constexpr Cmd cmdGetFimwareVer = 0x0b;
static constexpr Cmd cmdGetPostCode = 0x73;

std::unique_ptr<IpmiPwmcontrol> pwm_control;
void createPwmControl();
ipmi::RspType<> ipmiOEMSetManualPwm(uint8_t enabled);
ipmi::RspType<uint8_t> ipmiOEMGetPwm(uint8_t pwm_id);
ipmi::RspType<uint8_t> ipmiOEMSetPwm(uint8_t pwm_id, uint8_t value);

} // namespace nuvoton

} // namespace ipmi
