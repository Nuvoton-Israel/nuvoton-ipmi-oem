# nuvoton-ipmi-oem

Nuvoton IPMI OEM support library is intended to provide Nuvoton-specific IPMI command handlers for OpenBMC.
It is compiled as a shared library and intended to provide implementation for non-standard OEM extensions.

**Source URL**
* [https://github.com/Nuvoton-Israel/nuvoton-ipmi-oem](https://github.com/Nuvoton-Israel/nuvoton-ipmi-oem)

## Building

Please include the recipe **nuvoton-ipmi-oem** in the packagegroups and make the image again.

## Usage


|Name  | NetFn | Commmand | Requst Data [byte]|  Return Data| Description |
|------|---|---|---------|-----------|----------------|
| Set PID manual mode for all zones | 0x34| 0x90| [enabled]| - | enabled set 0x01 to enter manaul mode; 0x0 to exit manual mode for all PID zones|
| Set PWM |0x34| 0x91| [pwm_id] [value] | [set_value]| set specific pwm with value 0x0 ~ 0xff|
| Read PWM |0x34| 0x92| [pwm_id] |[pwm_value]| read specific pwm value|
| Get BIOS post code| 0x34 | 0x73 | - | [post code]||
| Get firmware version | 0x34 | 0x0b | [fw_type] | [major] [minor] | fw_type:<br> 00h - BIOS<br>01h - CPLD<br>02h - BMC<br>03h - PSU |

## Exmples
### Manual change PWM value
```bash
# enter manual mode to avoid PID control change pwm value
$ ipmitool raw 0x34 0x90 0x1

# set pwm 4 value 0x64
$ ipmitool raw 0x34 0x91 0x3 0x64
 64
# get pwm value from pwm 4
$ ipmitool raw 0x34 0x92 0x3
 64
# exit manual mode
$ ipmitool raw 0x34 0x90 0x0
```

### Get BIOS post code
```bash
root@evb-npcm845:~# ipmitool raw 0x34 0x73
 00

```

### Get firmware version
```bash
# BMC version is 2.10
root@evb-npcm845:~# ipmitool raw 0x34 0x0b 0x02
 02 0a
```