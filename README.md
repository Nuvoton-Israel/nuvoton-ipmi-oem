# nuvoton-ipmi-oem

Nuvoton IPMI OEM support library is intended to provide Nuvoton-specific IPMI command handlers for OpenBMC.
It is compiled as a shared library and intended to provide implementation for non-standard OEM extensions.

**Source URL**
* [https://github.com/Nuvoton-Israel/nuvoton-ipmi-oem](https://github.com/Nuvoton-Israel/nuvoton-ipmi-oem)

## Building

Please include the recipe **nuvoton-ipmi-oem** in the packagegroups and make the image again.

## Usage

|Name          | Format  | Return                                                                                                   | Note |
|:-------------:|:-------: |:--------------------------------------------------------------------------------------------------------: | :-------:     |
| Get BIOS post code      | ipmitool raw 0x32 0x73 | 1 byte is returned.  |   |
| Get firmware version    | ipmitool raw 0x38 0x0b ${fw_type} | 2 byte are returned. <br>The first is major, the second is minor. | The major/minor byte are returned in decimal format. <br>fw_type:<br> 00h - BIOS<br>01h - CPLD<br>02h - BMC<br>03h - PSU ||


