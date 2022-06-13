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

#include <ipmid/utils.hpp>

namespace ipmi
{
namespace nuvoton
{
enum FanServiceType
{
    fanServiceInvalid = 0,
    fanServiceHwmon = 1,
    fanServiceDbusSensor = 2,
};
using ZoneInfo = std::pair<DbusService, std::vector<DbusObjectPath>>;
class IpmiPwmcontrol
{
  public:
    IpmiPwmcontrol() {};
    ~IpmiPwmcontrol() = default;

    uint8_t getFanServiceType();
    uint8_t getPwmValue(uint8_t instance, uint8_t* value);
    uint8_t setPwmValue(uint8_t instance, uint8_t value);
    uint8_t setPwmProperty(const std::string& path,
                           const std::string& interface,
                           const std::string& property, ipmi::Value value);
    uint8_t setManualPwm(uint8_t enabled);
    uint8_t getPwmMode(uint8_t* value);

  private:
    std::string _fan_service = "";
    uint8_t dbusSetPwm(uint8_t instance, uint8_t value);
    uint8_t hwmonSetPwm(uint8_t instance, uint8_t value);
    std::string getFanService();
    ZoneInfo getPidZoneInfo();
};

} // namespace nuvoton

} // namespace ipmi