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
#include <cstdint>

#include "nuvoton_defs.hpp"

namespace ipmi
{
namespace nuvoton
{
#define GPIO_NUM  256
#define DEF_NONE  254
#define DEF_GPIO  255

struct gpio_lookup
{
    // The value of the pin is expected to be configured as a gpio
    uint8_t  refValue;

    // The number of bits for the pin to be checked under the bitwise mask
    // operation
    uint8_t  maskBit;

    uint32_t reg;
    uint8_t  offSet;

    struct gpio_lookup *next;
};

<% data %>

} // namespace nuvoton

} // namespace ipmi