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

#define LENGTH_8BIT               8
#define LENGTH_16BIT              16
#define LENGTH_32BIT              32
#define LENGTH_64BIT              64

#define SUCCESS                   0x0

#define GPIO0                     0xf0010000
#define GPIO1                     0xf0011000
#define GPIO2                     0xf0012000
#define GPIO3                     0xf0013000
#define GPIO4                     0xf0014000
#define GPIO5                     0xf0015000
#define GPIO6                     0xf0016000
#define GPIO7                     0xf0017000

#define GPIO_OFFSET               0x1000
#define GPIO_NUM_IN_GROUP         32

#define GP_DOUT_OFFSET            0xc
#define GP_OE_OFFSET              0x10

#define GP_DIN_OFFSET             0x4
#define GP_IEM_OFFSET             0x58

#define GP_DIR_OUTPUT             0x1
#define GP_DIR_INPUT              0x0
#define GP_LEVEL_HIGH             0x1
#define GP_LEVEL_LOW              0x1

#define PDID                      0xf0800000

#define PWRON                     0xf0800004

#define SPSWC                     0xf0800038

#define MFSEL1                    0xf080000C
#define MFSEL2                    0xf0800010
#define MFSEL3                    0xf0800064
#define MFSEL4                    0xf08000B0

#define INTCR2                    0xf0800060

#define FLOCKR1                   0xf0800074

#define USB_BASE                  0xf0830000
#define USB_CTL_START             0
#define USB_CTL_END               9
#define USB_OFFSET                0x1000
#define USB_DEVICE_ADDR           0x154

#define ROM_CODE_STATUS           0xf084bffc
#define ST_ROM_USE_IMAGE_SPI0_CS0_OFFSET_0     0x21
#define ST_ROM_USE_IMAGE_SPI0_CS0_OFFSET_80000 0x22
#define ST_ROM_USE_IMAGE_SPI0_CS1_OFFSET_0     0x23
#define FLASH_0                   0
#define FLASH_1                   1

#define GEN_MASK                  0xff000000
#define SPSWC_MASK                0x00000007
#define BIT_MASK                  0x00000001
