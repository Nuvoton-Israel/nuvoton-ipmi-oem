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

#define PDID                      0xf0800000

#define PWRON                     0xf0800004

#define SPSWC                     0xf0800038

#define MFSEL1                    0xf080000C
#define MFSEL2                    0xf0800010
#define MFSEL3                    0xf0800064
#define MFSEL4                    0xf08000B0

#define INTCR2                    0xf0800060

#define USB_BASE                  0xf0830000
#define USB_CTL_START             0
#define USB_CTL_END               9
#define USB_OFFSET                0x1000
#define USB_DEVICE_ADDR           0x154

#define GEN_MASK                  0xff000000
#define SPSWC_MASK                0x00000007
#define BIT_MASK                  0x00000001
