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


struct gpio_lookup gpio_20_1 =
{
    0,
    1,
    MFSEL2,24,
    NULL
};

struct gpio_lookup gpio_21_1 =
{
    0,
    1,
    MFSEL2,25,
    NULL
};

struct gpio_lookup gpio_22_1 =
{
    0,
    1,
    MFSEL2,26,
    NULL
};

struct gpio_lookup gpio_23_1 =
{
    0,
    1,
    MFSEL2,27,
    NULL
};

struct gpio_lookup gpio_24_1 =
{
    0,
    1,
    MFSEL3,18,
    NULL
};

struct gpio_lookup gpio_25_1 =
{
    0,
    1,
    MFSEL3,18,
    NULL
};

struct gpio_lookup gpio_43_44_2 =
{
    0,
    1,
    MFSEL1,10,
    NULL
};

struct gpio_lookup gpio_43_44_1 =
{
    0,
    1,
    MFSEL4,0,
    &gpio_43_44_2
};

struct gpio_lookup gpio_45_1 =
{
    0,
    1,
    MFSEL1,10,
    NULL
};

struct gpio_lookup gpio_46_47_1 =
{
    0,
    1,
    MFSEL1,10,
    NULL
};

struct gpio_lookup gpio_48_1 =
{
    0,
    1,
    MFSEL1,11,
    NULL
};

struct gpio_lookup gpio_49_1 =
{
    0,
    1,
    MFSEL1,11,
    NULL
};

struct gpio_lookup gpio_62_1 =
{
    0,
    1,
    MFSEL1,10,
    NULL
};

struct gpio_lookup gpio_63_1 =
{
    0,
    1,
    MFSEL1,10,
    NULL
};

struct gpio_lookup gpio_110_113_1 =
{
    0,
    1,
    MFSEL4,24,
    NULL
};

struct gpio_lookup gpio_155_1 =
{
     0,
     1,
     MFSEL3,25,
     NULL
};

struct gpio_lookup gpio_168_1 =
{
    0,
    1,
    MFSEL4,8,
    NULL
};

struct gpio_lookup gpio_188_1 =
{
     0,
     1,
     MFSEL4,20,
     NULL
};

struct gpio_lookup gpio_189_1 =
{
     0,
     1,
     MFSEL4,20,
     NULL
};

struct gpio_lookup gpioLUT[GPIO_NUM] =
{
    // gpio 0-3
    {
     0,
     1,
     MFSEL1,30,
     NULL
    },

    {
     0,
     1,
     MFSEL1,30,
     NULL
    },

    {
     0,
     1,
     MFSEL1,30,
     NULL
    },

    {
     0,
     1,
     MFSEL1,30,
     NULL
    },

    // gpio 4-7
    {
     0,
     1,
     MFSEL3,14,
     NULL
    },

    {
     0,
     1,
     MFSEL3,14,
     NULL
    },

    {
     0,
     1,
     MFSEL3,14,
     NULL
    },

    {
     0,
     1,
     MFSEL3,14,
     NULL
    },

    // gpio 8
    {
     0,
     1,
     FLOCKR1,4,
     NULL
    },

    // gpio 9
    {
     0,
     1,
     FLOCKR1,8,
     NULL
    },

    // gpio 10-11
    {
     0,
     1,
     MFSEL3,18,
     NULL
    },

    {
     0,
     1,
     MFSEL3,18,
     NULL
    },

    // gpio 12-15
    {
     0,
     1,
     MFSEL1,24,
     NULL
    },

    {
     0,
     1,
     MFSEL1,24,
     NULL
    },

    {
     0,
     1,
     MFSEL1,24,
     NULL
    },

    {
     0,
     1,
     MFSEL1,24,
     NULL
    },

    // gpio 16
    {
     0,
     1,
     FLOCKR1,0,
     NULL
    },

    // gpio 17-19
    {
     0,
     1,
     MFSEL3,13,
     NULL
    },

    {
     0,
     1,
     MFSEL3,13,
     NULL
    },

    {
     0,
     1,
     MFSEL3,13,
     NULL
    },

    // gpio 20
    {
     0,
     1,
     MFSEL3,8,
     &gpio_20_1
    },

    // gpio 21
    {
     0,
     1,
     MFSEL3,8,
     &gpio_21_1
    },

    // gpio 22
    {
     0,
     1,
     MFSEL3,7,
     &gpio_22_1
    },

    // gpio 23
    {
     0,
     1,
     MFSEL3,7,
     &gpio_23_1
    },

    // gpio 24
    {
     0,
     1,
     MFSEL2,28,
     &gpio_24_1
    },

    // gpio 25
    {
     0,
     1,
     MFSEL2,29,
     &gpio_25_1
    },

    // gpio 26-27
    {
     0,
     1,
     MFSEL1,2,
     NULL
    },

    {
     0,
     1,
     MFSEL1,2,
     NULL
    },

    // gpio 28-29
    {
     0,
     1,
     MFSEL1,1,
     NULL
    },

    {
     0,
     1,
     MFSEL1,1,
     NULL
    },

    // gpio 30-31
    {
     0,
     1,
     MFSEL1,0,
     NULL
    },

    {
     0,
     1,
     MFSEL1,0,
     NULL
    },

    // gpio 32
    {
     0,
     1,
     MFSEL1,3,
     NULL
    },

    // gpio 33-36
    // not connected to pins
    {
     DEF_NONE,
     0,
     0,0,
     NULL
    },

    {
     DEF_NONE,
     0,
     0,0,
     NULL
    },

    {
     DEF_NONE,
     0,
     0,0,
     NULL
    },

    {
     DEF_NONE,
     0,
     0,0,
     NULL
    },

    // gpio 37-40
    // no mfsel
    {
     DEF_GPIO,
     0,
     0,0,
     NULL
    },

    {
     DEF_GPIO,
     0,
     0,0,
     NULL
    },

    {
     DEF_GPIO,
     0,
     0,0,
     NULL
    },

    {
     DEF_GPIO,
     0,
     0,0,
     NULL
    },

    // gpio 41-42
    {
     0,
     1,
     MFSEL1,9,
     NULL
    },

    {
     0,
     1,
     MFSEL1,9,
     NULL
    },

    // gpio 43-44
    {
     0,
     1,
     MFSEL3,24,
     &gpio_43_44_1
    },

    {
     0,
     1,
     MFSEL3,24,
     &gpio_43_44_1
    },

    // gpio 45
    {
     0,
     1,
     MFSEL4,0,
     &gpio_45_1
    },

    // gpio 46-47
    {
     0,
     1,
     MFSEL4,0,
     &gpio_46_47_1
    },

    {
     0,
     1,
     MFSEL4,0,
     &gpio_46_47_1
    },

    // gpio  48
    {
     0,
     1,
     MFSEL4,1,
     &gpio_48_1
    },

    // gpio 49-55
    {
     0,
     1,
     MFSEL4,1,
     &gpio_49_1
    },

    {
     0,
     1,
     MFSEL1,11,
     NULL
    },

    {
     0,
     1,
     MFSEL1,11,
     NULL
    },

    {
     0,
     1,
     MFSEL1,11,
     NULL
    },

    {
     0,
     1,
     MFSEL1,11,
     NULL
    },

    {
     0,
     1,
     MFSEL1,11,
     NULL
    },

    {
     0,
     1,
     MFSEL1,11,
     NULL
    },

    // gpio 56
    {
     0,
     1,
     MFSEL1,12,
     NULL
    },

    // gpio 57-58
    {
     0,
     1,
     MFSEL1,13,
     NULL
    },

    {
     0,
     1,
     MFSEL1,13,
     NULL
    },

    // gpio 59
    {
     0,
     1,
     MFSEL2,30,
     NULL
    },

    // gpio 60
    {
     0,
     1,
     MFSEL2,31,
     NULL
    },

    // gpo 61
    {
     0,
     1,
     MFSEL1,10,
     NULL
    },

    // gpo 62
    {
     0,
     1,
     MFSEL3,24,
     &gpio_62_1
    },

    // gpo 63
    {
     0,
     1,
     MFSEL3,24,
     &gpio_63_1
    },

    // gpio 64
    {
     0,
     1,
     MFSEL2,0,
     NULL
    },

    // gpio 65
    {
     0,
     1,
     MFSEL2,1,
     NULL
    },

    // gpio 66
    {
     0,
     1,
     MFSEL2,2,
     NULL
    },

    // gpio 67
    {
     0,
     1,
     MFSEL2,3,
     NULL
    },

    // gpio 68
    {
     0,
     1,
     MFSEL2,4,
     NULL
    },

    // gpio 69
    {
     0,
     1,
     MFSEL2,5,
     NULL
    },

    // gpio 70
    {
     0,
     1,
     MFSEL2,6,
     NULL
    },

    // gpio 71
    {
     0,
     1,
     MFSEL2,7,
     NULL
    },

    // gpio 72
    {
     0,
     1,
     MFSEL2,8,
     NULL
    },

    // gpio 73
    {
     0,
     1,
     MFSEL2,9,
     NULL
    },

    // gpio 74
    {
     0,
     1,
     MFSEL2,10,
     NULL
    },

    // gpio 75
    {
     0,
     1,
     MFSEL2,11,
     NULL
    },

    // gpio 76
    {
     0,
     1,
     MFSEL2,12,
     NULL
    },

    // gpio 77
    {
     0,
     1,
     MFSEL2,13,
     NULL
    },

    // gpio 78
    {
     0,
     1,
     MFSEL2,14,
     NULL
    },

    // gpio 79
    {
     0,
     1,
     MFSEL2,15,
     NULL
    },


    // gpio 80
    {
     0,
     1,
     MFSEL2,16,
     NULL
    },

    // gpio 81
    {
     0,
     1,
     MFSEL2,17,
     NULL
    },

    // gpio 82
    {
     0,
     1,
     MFSEL2,18,
     NULL
    },

    // gpio 83
    {
     0,
     1,
     MFSEL2,19,
     NULL
    },

    // gpio 84-89
    {
     0,
     1,
     MFSEL1,14,
     NULL
    },

    {
     0,
     1,
     MFSEL1,14,
     NULL
    },

    {
     0,
     1,
     MFSEL1,14,
     NULL
    },

    {
     0,
     1,
     MFSEL1,14,
     NULL
    },

    {
     0,
     1,
     MFSEL1,14,
     NULL
    },

    {
     0,
     1,
     MFSEL1,14,
     NULL
    },

    // gpio 90
    {
     0,
     1,
     MFSEL1,15,
     NULL
    },

    // gpio 91-92
    {
     0,
     1,
     MFSEL1,16,
     NULL
    },

    {
     0,
     1,
     MFSEL1,16,
     NULL
    },

    // gpio 93-94
    {
     0,
     1,
     MFSEL1,17,
     NULL
    },

    {
     0,
     1,
     MFSEL1,17,
     NULL
    },

    // gpio 95
    {
     1,
     1,
     MFSEL1,26,
     NULL
    },

    // gpio 96-107
    {
     0,
     1,
     MFSEL4,22,
     NULL
    },

    {
     0,
     1,
     MFSEL4,22,
     NULL
    },

    {
     0,
     1,
     MFSEL4,22,
     NULL
    },

    {
     0,
     1,
     MFSEL4,22,
     NULL
    },

    {
     0,
     1,
     MFSEL4,22,
     NULL
    },

    {
     0,
     1,
     MFSEL4,22,
     NULL
    },

    {
     0,
     1,
     MFSEL4,22,
     NULL
    },

    {
     0,
     1,
     MFSEL4,22,
     NULL
    },

    {
     0,
     1,
     MFSEL4,22,
     NULL
    },

    {
     0,
     1,
     MFSEL4,22,
     NULL
    },

    {
     0,
     1,
     MFSEL4,22,
     NULL
    },

    {
     0,
     1,
     MFSEL4,22,
     NULL
    },

    // gpio 108-109
    {
     0,
     1,
     MFSEL4,21,
     NULL
    },

    {
     0,
     1,
     MFSEL4,21,
     NULL
    },

    // gpio 110-113
    {
     0,
     1,
     MFSEL3,26,
     &gpio_110_113_1
    },

    {
     0,
     1,
     MFSEL3,26,
     &gpio_110_113_1
    },

    {
     0,
     1,
     MFSEL3,26,
     &gpio_110_113_1
    },

    {
     0,
     1,
     MFSEL3,26,
     &gpio_110_113_1
    },

    // gpio 114-115
    {
     0,
     1,
     MFSEL1,6,
     NULL
    },

    {
     0,
     1,
     MFSEL1,6,
     NULL
    },

    // gpio 116-117
    {
     0,
     1,
     MFSEL1,7,
     NULL
    },

    {
     0,
     1,
     MFSEL1,7,
     NULL
    },

    // gpio 118-119
    {
     0,
     1,
     MFSEL1,8,
     NULL
    },

    {
     0,
     1,
     MFSEL1,8,
     NULL
    },

    // gpio 120-127
    {
     DEF_GPIO,
     0,
     0,0,
     NULL
    },

    {
     DEF_GPIO,
     0,
     0,0,
     NULL
    },

    {
     DEF_GPIO,
     0,
     0,0,
     NULL
    },

    {
     DEF_GPIO,
     0,
     0,0,
     NULL
    },

    {
     DEF_GPIO,
     0,
     0,0,
     NULL
    },

    {
     DEF_GPIO,
     0,
     0,0,
     NULL
    },

    {
     DEF_GPIO,
     0,
     0,0,
     NULL
    },

    {
     DEF_GPIO,
     0,
     0,0,
     NULL
    },

    // gpio 128-129
    {
     0,
     1,
     MFSEL4,11,
     NULL
    },

    {
     0,
     1,
     MFSEL4,11,
     NULL
    },

    // gpio 130-131
    {
     0,
     1,
     MFSEL4,12,
     NULL
    },

    {
     0,
     1,
     MFSEL4,12,
     NULL
    },

    // gpio 132-133
    {
     0,
     1,
     MFSEL4,13,
     NULL
    },

    {
     0,
     1,
     MFSEL4,13,
     NULL
    },

    // gpio 134-135
    {
     0,
     1,
     MFSEL4,14,
     NULL
    },

    {
     0,
     1,
     MFSEL4,14,
     NULL
    },

    // gpio 136-139
    {
     0,
     1,
     MFSEL3,12,
     NULL
    },

    {
     0,
     1,
     MFSEL3,12,
     NULL
    },

    {
     0,
     1,
     MFSEL3,12,
     NULL
    },

    {
     0,
     1,
     MFSEL3,12,
     NULL
    },

    // gpio 140-143
    {
     0,
     1,
     MFSEL3,12,
     NULL
    },

    {
     0,
     1,
     MFSEL3,12,
     NULL
    },

    {
     0,
     1,
     MFSEL3,12,
     NULL
    },

    {
     0,
     1,
     MFSEL3,12,
     NULL
    },

    // gpio 144
    {
     0,
     1,
     MFSEL2,20,
     NULL
    },

    // gpio 145
    {
     0,
     1,
     MFSEL2,21,
     NULL
    },

    // gpio 146
    {
     0,
     1,
     MFSEL2,22,
     NULL
    },

    // gpio 147
    {
     0,
     1,
     MFSEL2,23,
     NULL
    },

    // gpio 148-151
    {
     0,
     1,
     MFSEL3,11,
     NULL
    },

    {
     0,
     1,
     MFSEL3,11,
     NULL
    },

    {
     0,
     1,
     MFSEL3,11,
     NULL
    },

    {
     0,
     1,
     MFSEL3,11,
     NULL
    },

    // gpio 152
    {
     0,
     1,
     MFSEL3,10,
     NULL
    },

    // gpio 153
    {
     0,
     1,
     FLOCKR1,24,
     NULL
    },

    // gpio 154
    {
     0,
     1,
     MFSEL3,10,
     NULL
    },

    // gpio 155
    {
     0,
     1,
     MFSEL4,6,
     &gpio_155_1
    },

    // gpio 156-159
    {
     0,
     1,
     MFSEL3,10,
     NULL
    },

    {
     0,
     1,
     MFSEL3,10,
     NULL
    },

    {
     0,
     1,
     MFSEL3,10,
     NULL
    },

    {
     0,
     1,
     MFSEL3,10,
     NULL
    },

    // gpio 160
    {
     0,
     1,
     MFSEL1,21,
     NULL
    },

    // gpio 161
    {
     1,
     1,
     MFSEL1,26,
     NULL
    },

    // gpio 162
    {
     1,
     1,
     MFSEL1,31,
     NULL
    },

    // gpio 163-167
    {
     1,
     1,
     MFSEL1,26,
     NULL
    },

    {
     1,
     1,
     MFSEL1,26,
     NULL
    },

    {
     1,
     1,
     MFSEL1,26,
     NULL
    },

    {
     1,
     1,
     MFSEL1,26,
     NULL
    },

    {
     1,
     1,
     MFSEL1,26,
     NULL
    },

    // gpio 168
    {
     1,
     1,
     MFSEL3,16,
     &gpio_168_1
    },

    // gpio 169
    {
     0,
     1,
     MFSEL3,0,
     NULL
    },

    // gpio 170
    {
     0,
     1,
     MFSEL1,22,
     NULL
    },

    // gpio 171-172
    {
     0,
     1,
     MFSEL3,1,
     NULL
    },

    {
     0,
     1,
     MFSEL3,1,
     NULL
    },

    // gpio 173-174
    {
     0,
     1,
     MFSEL3,2,
     NULL
    },

    {
     0,
     1,
     MFSEL3,2,
     NULL
    },

    // gpio 175-177
    {
     0,
     2,
     MFSEL3,3,
     NULL
    },

    {
     0,
     2,
     MFSEL3,3,
     NULL
    },

    {
     0,
     2,
     MFSEL3,3,
     NULL
    },

    // gpio 178-182
    {
     0,
     1,
     MFSEL3,9,
     NULL
    },

    {
     0,
     1,
     MFSEL3,9,
     NULL
    },

    {
     0,
     1,
     MFSEL3,9,
     NULL
    },

    {
     0,
     1,
     MFSEL3,9,
     NULL
    },

    {
     0,
     1,
     MFSEL3,9,
     NULL
    },

    // gpio 183-186
    {
     0,
     1,
     MFSEL4,16,
     NULL
    },

    {
     0,
     1,
     MFSEL4,16,
     NULL
    },

    {
     0,
     1,
     MFSEL4,16,
     NULL
    },

    {
     0,
     1,
     MFSEL4,16,
     NULL
    },

    // gpio 187
    {
     0,
     1,
     MFSEL4,17,
     NULL
    },

    // gpio 188
    {
     0,
     1,
     MFSEL4,18,
     &gpio_188_1
    },

    // gpio 189
    {
     0,
     1,
     MFSEL4,19,
     &gpio_189_1
    },

    // gpio 190
    {
     1,
     1,
     FLOCKR1,20,
     NULL
    },

    // gpio 191-192
    {
     DEF_GPIO,
     0,
     0,0,
     NULL
    },

    {
     DEF_GPIO,
     0,
     0,0,
     NULL
    },

    // gpio 193
    {
     0,
     1,
     MFSEL3,9,
     NULL
    },

    // gpio 194-199
    {
     DEF_GPIO,
     0,
     0,0,
     NULL
    },

    {
     DEF_GPIO,
     0,
     0,0,
     NULL
    },

    {
     DEF_GPIO,
     0,
     0,0,
     NULL
    },

    {
     DEF_GPIO,
     0,
     0,0,
     NULL
    },

    {
     DEF_GPIO,
     0,
     0,0,
     NULL
    },

    {
     DEF_GPIO,
     0,
     0,0,
     NULL
    },

    // gpio 200
    {
     0,
     1,
     MFSEL1,14,
     NULL
    },

    // gpio 201
    {
     0,
     1,
     MFSEL3,9,
     NULL
    },

    // gpio 202
    {
     DEF_GPIO,
     0,
     0,0,
     NULL
    },

    // gpio 203
    {
     0,
     1,
     MFSEL3,3,
     NULL
    },

    // gpio 204-207
    {
     1,
     1,
     MFSEL3,22,
     NULL
    },

    {
     1,
     1,
     MFSEL3,22,
     NULL
    },

    {
     1,
     1,
     MFSEL3,22,
     NULL
    },

    {
     1,
     1,
     MFSEL3,22,
     NULL
    },

    // gpio 208-215
    {
     0,
     1,
     MFSEL3,26,
     &gpio_110_113_1
    },

    {
     0,
     1,
     MFSEL3,26,
     &gpio_110_113_1
    },

    {
     0,
     1,
     MFSEL3,26,
     &gpio_110_113_1
    },

    {
     0,
     1,
     MFSEL3,26,
     &gpio_110_113_1
    },

    {
     0,
     1,
     MFSEL3,26,
     &gpio_110_113_1
    },

    {
     0,
     1,
     MFSEL3,26,
     &gpio_110_113_1
    },

    {
     0,
     1,
     MFSEL3,26,
     &gpio_110_113_1
    },

    {
     0,
     1,
     MFSEL3,26,
     &gpio_110_113_1
    },


    // gpio 216
    {
     0,
     1,
     MFSEL4,23,
     NULL
    },

    // gpio 217
    {
     0,
     1,
     MFSEL4,23,
     NULL
    },

    // gpio 218
    {
     0,
     1,
     MFSEL3,19,
     NULL
    },

    // gpio 219
    {
     0,
     1,
     MFSEL3,20,
     NULL
    },

    // gpio 220-221
    {
     0,
     1,
     MFSEL3,5,
     NULL
    },

    {
     0,
     1,
     MFSEL3,5,
     NULL
    },

    // gpio 222-223
    {
     0,
     1,
     MFSEL3,6,
     NULL
    },

    {
     0,
     1,
     MFSEL3,6,
     NULL
    },

    // gpio 224-227
    {
     0,
     1,
     MFSEL4,27,
     NULL
    },

    {
     0,
     1,
     MFSEL4,27,
     NULL
    },

    {
     0,
     1,
     MFSEL4,27,
     NULL
    },

    {
     0,
     1,
     MFSEL4,27,
     NULL
    },

    // gpio 228
    {
     0,
     1,
     MFSEL4,28,
     NULL
    },

    // gpio 229-230
    {
     0,
     1,
     MFSEL4,27,
     NULL
    },

    {
     0,
     1,
     MFSEL4,27,
     NULL
    },

    // gpio 231
    {
     0,
     1,
     MFSEL4,9,
     NULL
    },

    // gpio 232-255
    {
     DEF_NONE,
     0,
     0,0,
     NULL
    },

    {
     DEF_NONE,
     0,
     0,0,
     NULL
    },

    {
     DEF_NONE,
     0,
     0,0,
     NULL
    },

    {
     DEF_NONE,
     0,
     0,0,
     NULL
    },

    {
     DEF_NONE,
     0,
     0,0,
     NULL
    },

    {
     DEF_NONE,
     0,
     0,0,
     NULL
    },

    {
     DEF_NONE,
     0,
     0,0,
     NULL
    },

    {
     DEF_NONE,
     0,
     0,0,
     NULL
    },

    {
     DEF_NONE,
     0,
     0,0,
     NULL
    },

    {
     DEF_NONE,
     0,
     0,0,
     NULL
    },

    {
     DEF_NONE,
     0,
     0,0,
     NULL
    },

    {
     DEF_NONE,
     0,
     0,0,
     NULL
    },

    {
     DEF_NONE,
     0,
     0,0,
     NULL
    },

    {
     DEF_NONE,
     0,
     0,0,
     NULL
    },

    {
     DEF_NONE,
     0,
     0,0,
     NULL
    },

    {
     DEF_NONE,
     0,
     0,0,
     NULL
    },

    {
     DEF_NONE,
     0,
     0,0,
     NULL
    },

    {
     DEF_NONE,
     0,
     0,0,
     NULL
    },

    {
     DEF_NONE,
     0,
     0,0,
     NULL
    },

    {
     DEF_NONE,
     0,
     0,0,
     NULL
    },

    {
     DEF_NONE,
     0,
     0,0,
     NULL
    },

    {
     DEF_NONE,
     0,
     0,0,
     NULL
    },

    {
     DEF_NONE,
     0,
     0,0,
     NULL
    },

    {
     DEF_NONE,
     0,
     0,0,
     NULL
    }

};

} // namespace nuvoton

} // namespace ipmi