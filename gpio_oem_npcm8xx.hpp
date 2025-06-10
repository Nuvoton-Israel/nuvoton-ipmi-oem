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

struct gpio_lookup gpio_0_2 =
{0, 1, MFSEL5, 26, NULL};

struct gpio_lookup gpio_0_1 =
{0, 1, I2CSEGSEL, 25, &gpio_0_2};

struct gpio_lookup gpio_1_2 =
{0, 1, MFSEL5, 26, NULL};

struct gpio_lookup gpio_1_1 =
{0, 1, I2CSEGSEL, 25, &gpio_1_2};

struct gpio_lookup gpio_2_2 =
{0, 1, MFSEL5, 25, NULL};

struct gpio_lookup gpio_2_1 =
{0, 1, I2CSEGSEL, 24, &gpio_2_2};

struct gpio_lookup gpio_3_2 =
{0, 1, MFSEL5, 25, NULL};

struct gpio_lookup gpio_3_1 =
{0, 1, I2CSEGSEL, 24, &gpio_3_2};

struct gpio_lookup gpio_4_1 =
{0, 1, I2CSEGSEL, 7, NULL};

struct gpio_lookup gpio_5_1 =
{0, 1, I2CSEGSEL, 7, NULL};

struct gpio_lookup gpio_6_1 =
{0, 1, I2CSEGSEL, 10, NULL};

struct gpio_lookup gpio_7_1 =
{0, 1, I2CSEGSEL, 10, NULL};

struct gpio_lookup gpio_8_1 =
{0, 1, MFSEL7, 8, NULL};

struct gpio_lookup gpio_9_1 =
{0, 1, MFSEL7, 9, NULL};

struct gpio_lookup gpio_10_2 =
{0, 1, MFSEL5, 24, NULL};

struct gpio_lookup gpio_10_1 =
{0, 1, I2CSEGSEL, 26, &gpio_10_2};

struct gpio_lookup gpio_11_2 =
{0, 1, MFSEL5, 24, NULL};

struct gpio_lookup gpio_11_1 =
{0, 1, I2CSEGSEL, 26, &gpio_11_2};

struct gpio_lookup gpio_12_1 =
{0, 1, I2CSEGSEL, 19, NULL};

struct gpio_lookup gpio_13_1 =
{0, 1, I2CSEGSEL, 19, NULL};

struct gpio_lookup gpio_14_1 =
{0, 1, I2CSEGSEL, 20, NULL};

struct gpio_lookup gpio_15_1 =
{0, 1, I2CSEGSEL, 20, NULL};

struct gpio_lookup gpio_16_2 =
{0, 1, MFSEL7, 10, NULL};

struct gpio_lookup gpio_16_1 =
{0, 1, I2CSEGSEL, 27, &gpio_16_2};

struct gpio_lookup gpio_17_1 =
{0, 1, MFSEL6, 7, NULL};

struct gpio_lookup gpio_18_1 =
{0, 1, I2CSEGSEL, 14, NULL};

struct gpio_lookup gpio_19_1 =
{0, 1, I2CSEGSEL, 14, NULL};

struct gpio_lookup gpio_20_2 =
{0, 1, I2CSEGSEL, 15, NULL};

struct gpio_lookup gpio_20_1 =
{0, 1, MFSEL3, 8, &gpio_20_2};

struct gpio_lookup gpio_21_2 =
{0, 1, I2CSEGSEL, 15, NULL};

struct gpio_lookup gpio_21_1 =
{0, 1, MFSEL3, 8, &gpio_21_2};

struct gpio_lookup gpio_22_2 =
{0, 1, I2CSEGSEL, 16, NULL};

struct gpio_lookup gpio_22_1 =
{0, 1, MFSEL3, 7, &gpio_22_2};

struct gpio_lookup gpio_23_2 =
{0, 1, I2CSEGSEL, 16, NULL};

struct gpio_lookup gpio_23_1 =
{0, 1, MFSEL3, 7, &gpio_23_2};

struct gpio_lookup gpio_24_3 =
{0, 1, MFSEL7, 28, NULL};

struct gpio_lookup gpio_24_2 =
{0, 1, I2CSEGSEL, 28, &gpio_24_3};

struct gpio_lookup gpio_24_1 =
{0, 1, MFSEL3, 18, &gpio_24_2};

struct gpio_lookup gpio_25_3 =
{0, 1, MFSEL7, 28, NULL};

struct gpio_lookup gpio_25_2 =
{0, 1, I2CSEGSEL, 28, &gpio_25_3};

struct gpio_lookup gpio_25_1 =
{0, 1, MFSEL3, 18, &gpio_25_2};

struct gpio_lookup gpio_32_1 =
{0, 1, MFSEL7, 26, NULL};

struct gpio_lookup gpio_37_1 =
{0, 1, MFSEL5, 31, NULL};

struct gpio_lookup gpio_38_1 =
{0, 1, MFSEL5, 31, NULL};

struct gpio_lookup gpio_39_1 =
{0, 1, MFSEL5, 30, NULL};

struct gpio_lookup gpio_40_1 =
{0, 1, MFSEL5, 30, NULL};

struct gpio_lookup gpio_41_1 =
{0, 1, MFSEL6, 31, NULL};

struct gpio_lookup gpio_42_1 =
{0, 1, MFSEL6, 1, NULL};

struct gpio_lookup gpio_43_1 =
{0, 1, MFSEL3, 24, NULL};

struct gpio_lookup gpio_44_3 =
{0, 1, MFSEL7, 13, NULL};

struct gpio_lookup gpio_44_2 =
{0, 1, MFSEL4, 0, &gpio_44_3};

struct gpio_lookup gpio_44_1 =
{0, 1, MFSEL6, 15, &gpio_44_2};

struct gpio_lookup gpio_45_3 =
{0, 1, MFSEL7, 13, NULL};

struct gpio_lookup gpio_45_2 =
{0, 1, MFSEL5, 2, &gpio_45_3};

struct gpio_lookup gpio_45_1 =
{0, 1, MFSEL4, 0, &gpio_45_2};

struct gpio_lookup gpio_46_3 =
{0, 1, MFSEL7, 13, NULL};

struct gpio_lookup gpio_46_2 =
{0, 1, MFSEL5, 2, &gpio_46_3};

struct gpio_lookup gpio_46_1 =
{0, 1, MFSEL4, 0, &gpio_46_2};

struct gpio_lookup gpio_48_1 =
{0, 1, MFSEL4, 1, NULL};

struct gpio_lookup gpio_49_1 =
{0, 1, MFSEL4, 1, NULL};

struct gpio_lookup gpio_50_2 =
{0, 1, MFSEL7, 12, NULL};

struct gpio_lookup gpio_50_1 =
{0, 1, MFSEL5, 6, &gpio_50_2};

struct gpio_lookup gpio_51_2 =
{0, 1, MFSEL7, 12, NULL};

struct gpio_lookup gpio_51_1 =
{0, 1, MFSEL5, 6, &gpio_51_2};

struct gpio_lookup gpio_52_1 =
{0, 1, MFSEL5, 7, NULL};

struct gpio_lookup gpio_53_1 =
{0, 1, MFSEL5, 7, NULL};

struct gpio_lookup gpio_54_1 =
{0, 1, MFSEL5, 8, NULL};

struct gpio_lookup gpio_55_1 =
{0, 1, MFSEL5, 8, NULL};

struct gpio_lookup gpio_56_1 =
{0, 1, MFSEL5, 9, NULL};

struct gpio_lookup gpio_57_1 =
{0, 1, MFSEL5, 20, NULL};

struct gpio_lookup gpio_58_1 =
{0, 1, MFSEL5, 22, NULL};

struct gpio_lookup gpio_59_2 =
{0, 1, MFSEL5, 27, NULL};

struct gpio_lookup gpio_59_1 =
{0, 1, I2CSEGSEL, 13, &gpio_59_2};

struct gpio_lookup gpio_60_2 =
{0, 1, MFSEL5, 27, NULL};

struct gpio_lookup gpio_60_1 =
{0, 1, I2CSEGSEL, 13, &gpio_60_2};

struct gpio_lookup gpio_62_3 =
{0, 1, MFSEL6, 15, NULL};

struct gpio_lookup gpio_62_2 =
{0, 1, MFSEL5, 2, &gpio_62_3};

struct gpio_lookup gpio_62_1 =
{0, 1, MFSEL4, 0, &gpio_62_2};

struct gpio_lookup gpio_63_1 =
{0, 1, MFSEL3, 24, NULL};

struct gpio_lookup gpio_90_1 =
{0, 1, MFSEL5, 10, NULL};

struct gpio_lookup gpio_91_2 =
{0, 1, MFSEL7, 0, NULL};

struct gpio_lookup gpio_91_1 =
{0, 1, MFSEL6, 8, &gpio_91_2};

struct gpio_lookup gpio_92_2 =
{0, 1, MFSEL7, 1, NULL};

struct gpio_lookup gpio_92_1 =
{0, 1, MFSEL6, 9, &gpio_92_2};

struct gpio_lookup gpio_93_1 =
{0, 1, I2CSEGSEL, 21, NULL};

struct gpio_lookup gpio_94_1 =
{0, 1, I2CSEGSEL, 21, NULL};

struct gpio_lookup gpio_95_1 =
{0, 1, MFSEL4, 8, NULL};

struct gpio_lookup gpio_96_1 =
{0, 1, MFSEL7, 7, NULL};

struct gpio_lookup gpio_97_1 =
{0, 1, MFSEL7, 6, NULL};

struct gpio_lookup gpio_98_2 =
{0, 1, MFSEL7, 5, NULL};

struct gpio_lookup gpio_98_1 =
{0, 1, MFSEL6, 26, &gpio_98_2};

struct gpio_lookup gpio_99_2 =
{0, 1, MFSEL7, 4, NULL};

struct gpio_lookup gpio_99_1 =
{0, 1, MFSEL6, 27, &gpio_99_2};

struct gpio_lookup gpio_100_2 =
{0, 1, MFSEL7, 3, NULL};

struct gpio_lookup gpio_100_1 =
{0, 1, MFSEL6, 28, &gpio_100_2};

struct gpio_lookup gpio_101_2 =
{0, 1, MFSEL7, 2, NULL};

struct gpio_lookup gpio_101_1 =
{0, 1, MFSEL6, 29, &gpio_101_2};

struct gpio_lookup gpio_110_2 =
{0, 1, MFSEL5, 11, NULL};

struct gpio_lookup gpio_110_1 =
{0, 1, MFSEL3, 26, &gpio_110_2};

struct gpio_lookup gpio_111_2 =
{0, 1, MFSEL5, 11, NULL};

struct gpio_lookup gpio_111_1 =
{0, 1, MFSEL3, 26, &gpio_111_2};

struct gpio_lookup gpio_112_1 =
{0, 1, MFSEL3, 26, NULL};

struct gpio_lookup gpio_113_1 =
{0, 1, MFSEL3, 26, NULL};

struct gpio_lookup gpio_124_1 =
{0, 1, MFSEL6, 23, NULL};

struct gpio_lookup gpio_125_1 =
{0, 1, MFSEL6, 22, NULL};

struct gpio_lookup gpio_126_1 =
{0, 1, MFSEL6, 21, NULL};

struct gpio_lookup gpio_127_1 =
{0, 1, MFSEL6, 20, NULL};

struct gpio_lookup gpio_134_1 =
{0, 1, MFSEL6, 0, NULL};

struct gpio_lookup gpio_135_1 =
{0, 1, MFSEL6, 0, NULL};

struct gpio_lookup gpio_142_1 =
{0, 1, MFSEL7, 11, NULL};

struct gpio_lookup gpio_143_1 =
{0, 1, MFSEL7, 11, NULL};

struct gpio_lookup gpio_155_1 =
{0, 1, MFSEL4, 6, NULL};

struct gpio_lookup gpio_161_1 =
{0, 1, MFSEL4, 8, NULL};

struct gpio_lookup gpio_163_1 =
{0, 1, MFSEL4, 8, NULL};

struct gpio_lookup gpio_164_1 =
{0, 1, MFSEL4, 8, NULL};

struct gpio_lookup gpio_165_1 =
{0, 1, MFSEL4, 8, NULL};

struct gpio_lookup gpio_166_1 =
{0, 1, MFSEL4, 8, NULL};

struct gpio_lookup gpio_167_1 =
{0, 1, MFSEL4, 8, NULL};

struct gpio_lookup gpio_168_1 =
{0, 1, MFSEL4, 8, NULL};

struct gpio_lookup gpio_169_1 =
{0, 1, MFSEL5, 29, NULL};

struct gpio_lookup gpio_170_1 =
{0, 1, MFSEL5, 29, NULL};

struct gpio_lookup gpio_175_2 =
{0, 1, MFSEL6, 17, NULL};

struct gpio_lookup gpio_175_1 =
{0, 1, MFSEL3, 3, &gpio_175_2};

struct gpio_lookup gpio_176_2 =
{0, 1, MFSEL6, 17, NULL};

struct gpio_lookup gpio_176_1 =
{0, 1, MFSEL3, 3, &gpio_176_2};

struct gpio_lookup gpio_177_2 =
{0, 1, MFSEL6, 17, NULL};

struct gpio_lookup gpio_177_1 =
{0, 1, MFSEL3, 3, &gpio_177_2};

struct gpio_lookup gpio_191_3 =
{0, 1, MFSEL7, 27, NULL};

struct gpio_lookup gpio_191_2 =
{0, 1, MFSEL6, 17, &gpio_191_3};

struct gpio_lookup gpio_191_1 =
{0, 1, MFSEL5, 4, &gpio_191_2};

struct gpio_lookup gpio_192_3 =
{0, 1, MFSEL7, 27, NULL};

struct gpio_lookup gpio_192_2 =
{0, 1, MFSEL6, 17, &gpio_192_3};

struct gpio_lookup gpio_192_1 =
{0, 1, MFSEL5, 5, &gpio_192_2};

struct gpio_lookup gpio_194_1 =
{0, 1, MFSEL6, 16, NULL};

struct gpio_lookup gpio_195_1 =
{0, 1, MFSEL6, 16, NULL};

struct gpio_lookup gpio_196_1 =
{0, 1, MFSEL6, 16, NULL};

struct gpio_lookup gpio_197_1 =
{0, 1, MFSEL6, 16, NULL};

struct gpio_lookup gpio_198_1 =
{0, 1, MFSEL6, 16, NULL};

struct gpio_lookup gpio_199_1 =
{0, 1, MFSEL6, 16, NULL};

struct gpio_lookup gpio_202_1 =
{0, 1, MFSEL6, 16, NULL};

struct gpio_lookup gpio_203_2 =
{0, 1, MFSEL6, 17, NULL};

struct gpio_lookup gpio_203_1 =
{0, 1, MFSEL3, 4, &gpio_203_2};

struct gpio_lookup gpio_208_1 =
{0, 1, MFSEL3, 26, NULL};

struct gpio_lookup gpio_209_2 =
{0, 1, MFSEL5, 11, NULL};

struct gpio_lookup gpio_209_1 =
{0, 1, MFSEL3, 26, &gpio_209_2};

struct gpio_lookup gpio_210_2 =
{0, 1, MFSEL5, 11, NULL};

struct gpio_lookup gpio_210_1 =
{0, 1, MFSEL3, 26, &gpio_210_2};

struct gpio_lookup gpio_211_2 =
{0, 1, MFSEL5, 11, NULL};

struct gpio_lookup gpio_211_1 =
{0, 1, MFSEL3, 26, &gpio_211_2};

struct gpio_lookup gpio_212_2 =
{0, 1, MFSEL6, 30, NULL};

struct gpio_lookup gpio_212_1 =
{0, 1, MFSEL3, 26, &gpio_212_2};

struct gpio_lookup gpio_213_2 =
{0, 1, MFSEL5, 14, NULL};

struct gpio_lookup gpio_213_1 =
{0, 1, MFSEL3, 26, &gpio_213_2};

struct gpio_lookup gpio_214_2 =
{0, 1, MFSEL5, 11, NULL};

struct gpio_lookup gpio_214_1 =
{0, 1, MFSEL3, 26, &gpio_214_2};

struct gpio_lookup gpio_215_2 =
{0, 1, MFSEL5, 11, NULL};

struct gpio_lookup gpio_215_1 =
{0, 1, MFSEL3, 26, &gpio_215_2};

struct gpio_lookup gpio_216_1 =
{0, 1, MFSEL3, 26, NULL};

struct gpio_lookup gpio_217_1 =
{0, 1, MFSEL3, 26, NULL};

struct gpio_lookup gpio_218_1 =
{0, 1, MFSEL7, 30, NULL};

struct gpio_lookup gpio_219_1 =
{0, 1, MFSEL7, 30, NULL};

struct gpio_lookup gpio_220_1 =
{0, 1, MFSEL6, 11, NULL};

struct gpio_lookup gpio_221_1 =
{0, 1, MFSEL6, 12, NULL};

struct gpio_lookup gpio_224_1 =
{0, 1, MFSEL6, 18, NULL};

struct gpio_lookup gpio_225_1 =
{0, 1, MFSEL6, 18, NULL};

struct gpio_lookup gpio_226_1 =
{0, 1, MFSEL6, 18, NULL};

struct gpio_lookup gpio_227_1 =
{0, 1, MFSEL6, 18, NULL};

struct gpio_lookup gpio_228_1 =
{0, 1, MFSEL6, 18, NULL};

struct gpio_lookup gpio_229_1 =
{0, 1, MFSEL6, 18, NULL};

struct gpio_lookup gpio_230_1 =
{0, 1, MFSEL6, 18, NULL};

struct gpio_lookup gpio_233_1 =
{0, 1, MFSEL6, 17, NULL};

struct gpio_lookup gpio_234_1 =
{0, 1, MFSEL5, 28, NULL};

struct gpio_lookup gpio_235_1 =
{0, 1, MFSEL5, 28, NULL};


struct gpio_lookup gpioLUT[GPIO_NUM] =
{// gpio 0
{0, 1, MFSEL1, 30, &gpio_0_1 },
// gpio 1
{0, 1, MFSEL1, 30, &gpio_1_1 },
// gpio 2
{0, 1, MFSEL1, 30, &gpio_2_1 },
// gpio 3
{0, 1, MFSEL1, 30, &gpio_3_1 },
// gpio 4
{0, 1, MFSEL3, 14, &gpio_4_1 },
// gpio 5
{0, 1, MFSEL3, 14, &gpio_5_1 },
// gpio 6
{0, 1, MFSEL3, 14, &gpio_6_1 },
// gpio 7
{0, 1, MFSEL3, 14, &gpio_7_1 },
// gpio 8
{0, 1, FLOCKR1, 4, &gpio_8_1 },
// gpio 9
{0, 1, FLOCKR1, 8, &gpio_9_1 },
// gpio 10
{0, 1, MFSEL3, 18, &gpio_10_1 },
// gpio 11
{0, 1, MFSEL3, 18, &gpio_11_1 },
// gpio 12
{0, 1, MFSEL1, 24, &gpio_12_1 },
// gpio 13
{0, 1, MFSEL1, 24, &gpio_13_1 },
// gpio 14
{0, 1, MFSEL1, 24, &gpio_14_1 },
// gpio 15
{0, 1, MFSEL1, 24, &gpio_15_1 },
// gpio 16
{0, 1, FLOCKR1, 0, &gpio_16_1 },
// gpio 17
{0, 1, MFSEL3, 13, &gpio_17_1 },
// gpio 18
{0, 1, MFSEL3, 13, &gpio_18_1 },
// gpio 19
{0, 1, MFSEL3, 13, &gpio_19_1 },
// gpio 20
{0, 1, MFSEL2, 24, &gpio_20_1 },
// gpio 21
{0, 1, MFSEL2, 25, &gpio_21_1 },
// gpio 22
{0, 1, MFSEL2, 26, &gpio_22_1 },
// gpio 23
{0, 1, MFSEL2, 27, &gpio_23_1 },
// gpio 24
{0, 1, MFSEL2, 28, &gpio_24_1 },
// gpio 25
{0, 1, MFSEL2, 29, &gpio_25_1 },
// gpio 26
{0, 1, MFSEL1, 2, NULL},
// gpio 27
{0, 1, MFSEL1, 2, NULL},
// gpio 28
{0, 1, MFSEL1, 1, NULL},
// gpio 29
{0, 1, MFSEL1, 1, NULL},
// gpio 30
{0, 1, MFSEL1, 0, NULL},
// gpio 31
{0, 1, MFSEL1, 0, NULL},
// gpio 32
{0, 1, MFSEL1, 3, &gpio_32_1 },
// gpio 33
{0, 1, MFSEL6, 10, NULL},
// gpio 34
{0, 1, MFSEL6, 10, NULL},
// gpio 35
{1, 1, MFSEL5, 16, NULL},
// gpio 36
{1, 1, MFSEL5, 18, NULL},
// gpio 37
{0, 1, I2CSEGSEL, 12, &gpio_37_1 },
// gpio 38
{0, 1, I2CSEGSEL, 12, &gpio_38_1 },
// gpio 39
{0, 1, I2CSEGSEL, 11, &gpio_39_1 },
// gpio 40
{0, 1, I2CSEGSEL, 11, &gpio_40_1 },
// gpio 41
{0, 1, MFSEL1, 9, &gpio_41_1 },
// gpio 42
{0, 1, MFSEL1, 9, &gpio_42_1 },
// gpio 43
{0, 1, MFSEL1, 10, &gpio_43_1 },
// gpio 44
{0, 1, MFSEL1, 28, &gpio_44_1 },
// gpio 45
{0, 1, MFSEL1, 4, &gpio_45_1 },
// gpio 46
{0, 1, MFSEL1, 4, &gpio_46_1 },
// gpio 47
{0, 1, MFSEL1, 4, NULL},
// gpio 48
{0, 1, MFSEL1, 11, &gpio_48_1 },
// gpio 49
{0, 1, MFSEL1, 11, &gpio_49_1 },
// gpio 50
{0, 1, MFSEL1, 29, &gpio_50_1 },
// gpio 51
{0, 1, MFSEL1, 29, &gpio_51_1 },
// gpio 52
{0, 1, MFSEL1, 5, &gpio_52_1 },
// gpio 53
{0, 1, MFSEL1, 5, &gpio_53_1 },
// gpio 54
{0, 1, MFSEL1, 5, &gpio_54_1 },
// gpio 55
{0, 1, MFSEL1, 5, &gpio_55_1 },
// gpio 56
{0, 1, MFSEL1, 12, &gpio_56_1 },
// gpio 57
{0, 1, MFSEL1, 13, &gpio_57_1 },
// gpio 58
{0, 1, MFSEL1, 13, &gpio_58_1 },
// gpio 59
{0, 1, MFSEL2, 30, &gpio_59_1 },
// gpio 60
{0, 1, MFSEL2, 31, &gpio_60_1 },
// gpio 61
{0, 1, MFSEL1, 4, NULL},
// gpio 62
{0, 1, MFSEL1, 28, &gpio_62_1 },
// gpio 63
{0, 1, MFSEL1, 10, &gpio_63_1 },
// gpio 64
{0, 1, MFSEL2, 0, NULL},
// gpio 65
{0, 1, MFSEL2, 1, NULL},
// gpio 66
{0, 1, MFSEL2, 2, NULL},
// gpio 67
{0, 1, MFSEL2, 3, NULL},
// gpio 68
{0, 1, MFSEL2, 4, NULL},
// gpio 69
{0, 1, MFSEL2, 5, NULL},
// gpio 70
{0, 1, MFSEL2, 6, NULL},
// gpio 71
{0, 1, MFSEL2, 7, NULL},
// gpio 72
{0, 1, MFSEL2, 8, NULL},
// gpio 73
{0, 1, MFSEL2, 9, NULL},
// gpio 74
{0, 1, MFSEL2, 10, NULL},
// gpio 75
{0, 1, MFSEL2, 11, NULL},
// gpio 76
{0, 1, MFSEL2, 12, NULL},
// gpio 77
{0, 1, MFSEL2, 13, NULL},
// gpio 78
{0, 1, MFSEL2, 14, NULL},
// gpio 79
{0, 1, MFSEL2, 15, NULL},
// gpio 80
{0, 1, MFSEL2, 16, NULL},
// gpio 81
{0, 1, MFSEL2, 17, NULL},
// gpio 82
{0, 1, MFSEL2, 18, NULL},
// gpio 83
{0, 1, MFSEL2, 19, NULL},
// gpio 84
{0, 1, MFSEL1, 14, NULL},
// gpio 85
{0, 1, MFSEL1, 14, NULL},
// gpio 86
{0, 1, MFSEL1, 14, NULL},
// gpio 87
{0, 1, MFSEL1, 14, NULL},
// gpio 88
{0, 1, MFSEL1, 14, NULL},
// gpio 89
{0, 1, MFSEL1, 14, NULL},
// gpio 90
{0, 1, MFSEL1, 15, &gpio_90_1 },
// gpio 91
{0, 1, MFSEL1, 16, &gpio_91_1 },
// gpio 92
{0, 1, MFSEL1, 16, &gpio_92_1 },
// gpio 93
{0, 1, MFSEL1, 17, &gpio_93_1 },
// gpio 94
{0, 1, MFSEL1, 17, &gpio_94_1 },
// gpio 95
{0, 1, MFSEL1, 26, &gpio_95_1 },
// gpio 96
{0, 1, MFSEL6, 24, &gpio_96_1 },
// gpio 97
{0, 1, MFSEL6, 25, &gpio_97_1 },
// gpio 98
{0, 1, MFSEL5, 13, &gpio_98_1 },
// gpio 99
{0, 1, MFSEL5, 13, &gpio_99_1 },
// gpio 100
{0, 1, MFSEL5, 12, &gpio_100_1 },
// gpio 101
{0, 1, MFSEL5, 12, &gpio_101_1 },
// gpio 102
{0, 1, MFSEL7, 29, NULL},
// gpio 103
{0, 1, MFSEL7, 29, NULL},
// gpio 104
{0, 1, MFSEL7, 29, NULL},
// gpio 105
{0, 1, MFSEL7, 29, NULL},
// gpio 106
{0, 1, MFSEL3, 22, NULL},
// gpio 107
{0, 1, MFSEL3, 22, NULL},
// gpio 108
{0, 1, MFSEL4, 21, NULL},
// gpio 109
{0, 1, MFSEL4, 21, NULL},
// gpio 110
{0, 1, MFSEL4, 24, &gpio_110_1 },
// gpio 111
{0, 1, MFSEL4, 24, &gpio_111_1 },
// gpio 112
{0, 1, MFSEL4, 24, &gpio_112_1 },
// gpio 113
{0, 1, MFSEL4, 24, &gpio_113_1 },
// gpio 114
{0, 1, MFSEL1, 6, NULL},
// gpio 115
{0, 1, MFSEL1, 6, NULL},
// gpio 116
{0, 1, MFSEL1, 7, NULL},
// gpio 117
{0, 1, MFSEL1, 7, NULL},
// gpio 118
{0, 1, MFSEL1, 8, NULL},
// gpio 119
{0, 1, MFSEL1, 8, NULL},
// gpio 120
{0, 1, I2CSEGSEL, 9, NULL},
// gpio 121
{0, 1, I2CSEGSEL, 9, NULL},
// gpio 122
{0, 1, I2CSEGSEL, 8, NULL},
// gpio 123
{0, 1, I2CSEGSEL, 8, NULL},
// gpio 124
{0, 1, I2CSEGSEL, 6, &gpio_124_1 },
// gpio 125
{0, 1, I2CSEGSEL, 6, &gpio_125_1 },
// gpio 126
{0, 1, I2CSEGSEL, 5, &gpio_126_1 },
// gpio 127
{0, 1, I2CSEGSEL, 5, &gpio_127_1 },
// gpio 128
{0, 1, MFSEL4, 11, NULL},
// gpio 129
{0, 1, MFSEL4, 11, NULL},
// gpio 130
{0, 1, MFSEL4, 12, NULL},
// gpio 131
{0, 1, MFSEL4, 12, NULL},
// gpio 132
{0, 1, MFSEL4, 13, NULL},
// gpio 133
{0, 1, MFSEL4, 13, NULL},
// gpio 134
{0, 1, MFSEL4, 14, &gpio_134_1 },
// gpio 135
{0, 1, MFSEL4, 14, &gpio_135_1 },
// gpio 136
{0, 1, MFSEL5, 15, NULL},
// gpio 137
{0, 1, MFSEL5, 15, NULL},
// gpio 138
{0, 1, MFSEL5, 15, NULL},
// gpio 139
{0, 1, MFSEL5, 15, NULL},
// gpio 140
{0, 1, MFSEL5, 15, NULL},
// gpio 141
{0, 1, I2CSEGSEL, 27, NULL},
// gpio 142
{0, 1, I2CSEGSEL, 29, &gpio_142_1 },
// gpio 143
{0, 1, I2CSEGSEL, 29, &gpio_143_1 },
// gpio 144
{0, 1, MFSEL2, 20, NULL},
// gpio 145
{0, 1, MFSEL2, 21, NULL},
// gpio 146
{0, 1, MFSEL2, 22, NULL},
// gpio 147
{0, 1, MFSEL2, 23, NULL},
// gpio 148
{0, 1, MFSEL3, 11, NULL},
// gpio 149
{0, 1, MFSEL3, 11, NULL},
// gpio 150
{0, 1, MFSEL3, 11, NULL},
// gpio 151
{0, 1, MFSEL3, 11, NULL},
// gpio 152
{0, 1, MFSEL3, 10, NULL},
// gpio 153
{0, 1, FLOCKR1, 24, NULL},
// gpio 154
{0, 1, MFSEL3, 10, NULL},
// gpio 155
{0, 1, MFSEL3, 25, &gpio_155_1 },
// gpio 156
{0, 1, MFSEL3, 10, NULL},
// gpio 157
{0, 1, MFSEL3, 10, NULL},
// gpio 158
{0, 1, MFSEL3, 10, NULL},
// gpio 159
{0, 1, MFSEL3, 10, NULL},
// gpio 160
{0, 1, MFSEL1, 21, NULL},
// gpio 161
{0, 1, MFSEL1, 26, &gpio_161_1 },
// gpio 162
{0, 1, MFSEL3, 16, NULL},
// gpio 163
{0, 1, MFSEL1, 26, &gpio_163_1 },
// gpio 164
{0, 1, MFSEL1, 26, &gpio_164_1 },
// gpio 165
{0, 1, MFSEL1, 26, &gpio_165_1 },
// gpio 166
{0, 1, MFSEL1, 26, &gpio_166_1 },
// gpio 167
{0, 1, MFSEL1, 26, &gpio_167_1 },
// gpio 168
{0, 1, MFSEL1, 31, &gpio_168_1 },
// gpio 169
{0, 1, MFSEL3, 0, &gpio_169_1 },
// gpio 170
{0, 1, MFSEL1, 22, &gpio_170_1 },
// gpio 171
{0, 1, MFSEL3, 1, NULL},
// gpio 172
{0, 1, MFSEL3, 1, NULL},
// gpio 173
{0, 1, MFSEL3, 2, NULL},
// gpio 174
{0, 1, MFSEL3, 2, NULL},
// gpio 175
{0, 1, MFSEL3, 4, &gpio_175_1 },
// gpio 176
{0, 1, MFSEL3, 4, &gpio_176_1 },
// gpio 177
{0, 1, MFSEL3, 4, &gpio_177_1 },
// gpio 178
{0, 1, MFSEL3, 9, NULL},
// gpio 179
{0, 1, MFSEL3, 9, NULL},
// gpio 180
{0, 1, MFSEL3, 9, NULL},
// gpio 181
{0, 1, MFSEL3, 9, NULL},
// gpio 182
{0, 1, MFSEL3, 9, NULL},
// gpio 183
{1, 1, MFSEL6, 19, NULL},
// gpio 184
{1, 1, MFSEL6, 19, NULL},
// gpio 185
{1, 1, MFSEL6, 19, NULL},
// gpio 186
{1, 1, MFSEL6, 19, NULL},
// gpio 187
{1, 1, MFSEL7, 24, NULL},
// gpio 188
{1, 1, MFSEL7, 25, NULL},
// gpio 189
{1, 1, MFSEL7, 25, NULL},
// gpio 190
{0, 1, FLOCKR1, 20, NULL},
// gpio 191
{0, 1, MFSEL5, 3, &gpio_191_1 },
// gpio 192
{0, 1, MFSEL5, 3, &gpio_192_1 },
// gpio 193
{0, 1, MFSEL3, 9, NULL},
// gpio 194
{0, 1, I2CSEGSEL, 0, &gpio_194_1 },
// gpio 195
{0, 1, I2CSEGSEL, 0, &gpio_195_1 },
// gpio 196
{0, 1, I2CSEGSEL, 1, &gpio_196_1 },
// gpio 197
{0, 1, I2CSEGSEL, 22, &gpio_197_1 },
// gpio 198
{0, 1, I2CSEGSEL, 2, &gpio_198_1 },
// gpio 199
{0, 1, I2CSEGSEL, 2, &gpio_199_1 },
// gpio 200
{0, 1, MFSEL1, 14, NULL},
// gpio 201
{0, 1, MFSEL3, 9, NULL},
// gpio 202
{0, 1, I2CSEGSEL, 1, &gpio_202_1 },
// gpio 203
{0, 1, MFSEL3, 3, &gpio_203_1 },
// gpio 204
{DEF_NONE, 0, 0, 0, NULL},
// gpio 205
{DEF_NONE, 0, 0, 0, NULL},
// gpio 206
{DEF_NONE, 0, 0, 0, NULL},
// gpio 207
{DEF_NONE, 0, 0, 0, NULL},
// gpio 208
{0, 1, MFSEL4, 24, &gpio_208_1 },
// gpio 209
{0, 1, MFSEL4, 24, &gpio_209_1 },
// gpio 210
{0, 1, MFSEL4, 24, &gpio_210_1 },
// gpio 211
{0, 1, MFSEL4, 24, &gpio_211_1 },
// gpio 212
{0, 1, MFSEL4, 24, &gpio_212_1 },
// gpio 213
{0, 1, MFSEL4, 24, &gpio_213_1 },
// gpio 214
{0, 1, MFSEL4, 24, &gpio_214_1 },
// gpio 215
{0, 1, MFSEL4, 24, &gpio_215_1 },
// gpio 216
{0, 1, MFSEL4, 23, &gpio_216_1 },
// gpio 217
{0, 1, MFSEL4, 23, &gpio_217_1 },
// gpio 218
{0, 1, MFSEL3, 19, &gpio_218_1 },
// gpio 219
{0, 1, MFSEL3, 20, &gpio_219_1 },
// gpio 220
{0, 1, MFSEL3, 5, &gpio_220_1 },
// gpio 221
{0, 1, MFSEL3, 5, &gpio_221_1 },
// gpio 222
{0, 1, MFSEL3, 6, NULL},
// gpio 223
{0, 1, MFSEL3, 6, NULL},
// gpio 224
{0, 1, MFSEL4, 27, &gpio_224_1 },
// gpio 225
{0, 1, MFSEL4, 27, &gpio_225_1 },
// gpio 226
{0, 1, MFSEL4, 27, &gpio_226_1 },
// gpio 227
{0, 1, MFSEL4, 27, &gpio_227_1 },
// gpio 228
{0, 1, MFSEL4, 28, &gpio_228_1 },
// gpio 229
{0, 1, MFSEL4, 27, &gpio_229_1 },
// gpio 230
{0, 1, MFSEL4, 27, &gpio_230_1 },
// gpio 231
{0, 1, MFSEL4, 9, NULL},
// gpio 232
{DEF_NONE, 0, 0, 0, NULL},
// gpio 233
{0, 1, MFSEL5, 0, &gpio_233_1 },
// gpio 234
{0, 1, MFSEL6, 13, &gpio_234_1 },
// gpio 235
{0, 1, MFSEL6, 14, &gpio_235_1 },
// gpio 236
{DEF_NONE, 0, 0, 0, NULL},
// gpio 237
{DEF_NONE, 0, 0, 0, NULL},
// gpio 238
{DEF_NONE, 0, 0, 0, NULL},
// gpio 239
{DEF_NONE, 0, 0, 0, NULL},
// gpio 240
{0, 1, MFSEL5, 17, NULL},
// gpio 241
{0, 1, MFSEL5, 17, NULL},
// gpio 242
{0, 1, MFSEL5, 19, NULL},
// gpio 243
{0, 1, MFSEL5, 19, NULL},
// gpio 244
{0, 1, MFSEL5, 21, NULL},
// gpio 245
{0, 1, MFSEL5, 21, NULL},
// gpio 246
{0, 1, MFSEL5, 23, NULL},
// gpio 247
{0, 1, MFSEL5, 23, NULL},
// gpio 248
{DEF_NONE, 0, 0, 0, NULL},
// gpio 249
{DEF_NONE, 0, 0, 0, NULL},
// gpio 250
{DEF_NONE, 0, 0, 0, NULL},
// gpio 251
{DEF_NONE, 0, 0, 0, NULL},
// gpio 252
{DEF_NONE, 0, 0, 0, NULL},
// gpio 253
{DEF_NONE, 0, 0, 0, NULL},
// gpio 254
{DEF_NONE, 0, 0, 0, NULL},
// gpio 255
{DEF_NONE, 0, 0, 0, NULL}};
} // namespace nuvoton

} // namespace ipmi