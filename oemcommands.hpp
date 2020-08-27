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

#include <ipmid/api-types.hpp>

#define GPIO_NUM  256

struct gpio_lookup
{
    uint32_t mfselReg;
    uint8_t  offset;
};

struct gpio_lookup gpioLUT[GPIO_NUM] =
{
    // gpio 0 - 3
    {MFSEL1, 30},
    {MFSEL1, 30},
    {MFSEL1, 30},
    {MFSEL1, 30},

    // gpio 12 - 15
    {MFSEL1, 24},
    {MFSEL1, 24},
    {MFSEL1, 24},
    {MFSEL1, 24},

    // gpio 20
    {MFSEL2, 24},

    // gpio 21
    {MFSEL2, 25},

    // gpio 22
    {MFSEL2, 26},

    // gpio 23
    {MFSEL2, 27},


    // gpio 26-27
    {MFSEL1, 2},
    {MFSEL1, 2},

    // gpio 28-29
    {MFSEL1, 1},
    {MFSEL1, 1},

    // gpio 30-31
    {MFSEL1, 0},
    {MFSEL1, 0},

    // gpio 32
    {MFSEL1, 3},

    // gpio 41-42
    {MFSEL1, 9},
    {MFSEL1, 9},

    // gpo  45
    {MFSEL1, 10},

    // gpio 46-47
    {MFSEL1, 10},
    {MFSEL1, 10},


    // gpo  48
    {MFSEL1, 11},

    // gpio 49-55
    {MFSEL1, 11},
    {MFSEL1, 11},
    {MFSEL1, 11},
    {MFSEL1, 11},
    {MFSEL1, 11},
    {MFSEL1, 11},
    {MFSEL1, 11},

    // gpio 56
    {MFSEL1, 12},

    // gpio 57-58
    {MFSEL1, 13},
    {MFSEL1, 13},

    // gpio 59
    {MFSEL2, 30},

    // gpio 60
    {MFSEL2, 31},

    // gpo  61
    {MFSEL1, 10},

    // gpio 64
    {MFSEL2, 0},

    // gpio 65
    {MFSEL2, 1},

    // gpio 66
    {MFSEL2, 2},

    // gpio 67
    {MFSEL2, 3},

    // gpio 68
    {MFSEL2, 4},

    // gpio 69
    {MFSEL2, 5},

    // gpio 70
    {MFSEL2, 6},

    // gpio 71
    {MFSEL2, 7},

    // gpio 72
    {MFSEL2, 8},

    // gpio 73
    {MFSEL2, 9},

    // gpio 74
    {MFSEL2, 10},

    // gpio 75
    {MFSEL2, 11},

    // gpio 76
    {MFSEL2, 12},

    // gpio 77
    {MFSEL2, 13},

    // gpio 78
    {MFSEL2, 14},

    // gpio 79
    {MFSEL2, 15},

    // gpio 80
    {MFSEL2, 16},

    // gpio 81
    {MFSEL2, 17},

    // gpio 82
    {MFSEL2, 18},

    // gpio 83
    {MFSEL2, 19},

    // gpio 84-89
    {MFSEL1, 14},
    {MFSEL1, 14},
    {MFSEL1, 14},
    {MFSEL1, 14},
    {MFSEL1, 14},
    {MFSEL1, 14},

    // gpio 90
    {MFSEL1, 15},

    // gpio 91-92
    {MFSEL1, 16},
    {MFSEL1, 16},

    // gpio 93-94
    {MFSEL1, 17},
    {MFSEL1, 17},

    // gpio 95
    {MFSEL1, 26},

    // gpio 114-115
    {MFSEL1, 6},
    {MFSEL1, 6},

    // gpio 116-117
    {MFSEL1, 7},
    {MFSEL1, 7},

    // gpio 118-119
    {MFSEL1, 8},
    {MFSEL1, 8},

    // gpio 144
    {MFSEL2, 20},

    // gpio 145
    {MFSEL2, 21},

    // gpio 146
    {MFSEL2, 22},

    // gpio 147
    {MFSEL2, 23},

    // gpio 160
    {MFSEL1, 21},

    // gpio 161
    {MFSEL1, 26},

    // gpio 163-167
    {MFSEL1, 26},
    {MFSEL1, 26},
    {MFSEL1, 26},
    {MFSEL1, 26},
    {MFSEL1, 26},

    // gpio 170
    {MFSEL1, 22},

    // gpio 200
    {MFSEL1, 14},

};

namespace ipmi
{
namespace nuvoton
{

static constexpr NetFn netFnGeneral = netFnOemOne;
static constexpr NetFn netFnPlatform = netFnOemTwo;
static constexpr NetFn netFnApp = netFnOemEight;

namespace general
{
static constexpr Cmd cmdGetStrapPinStatus = 0x02;

static constexpr Cmd cmdGetUartMode = 0x04;

static constexpr Cmd cmdGetUsbDeviceStatus = 0x06;

static constexpr Cmd cmdGetGpioStatus = 0x08;

static constexpr Cmd cmdGetBmcRebootReason = 0x0a;
} // namespace general

} // namespace nuvoton

} // namespace ipmi
