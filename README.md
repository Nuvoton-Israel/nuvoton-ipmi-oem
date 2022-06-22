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
| Set PID manual mode for all zones | 0x32| 0x90| [enabled]| - | enabled set 0x01 to enter manaul mode; 0x0 to exit manual mode for all PID zones|
| Get PID manual mode |0x32| 0x89| - |[mode]| 0:automatic mode, 1: manual mode|
| Set PWM |0x32| 0x91| [pwm_id] [value] | [set_value]| set specific pwm with value in range: 0x0 ~ 0x64|
| Read PWM |0x32| 0x92| [pwm_id] |[pwm_value]| read specific pwm value|
| Get BIOS post code| 0x34 | 0x73 | - | [post code]||
| Get firmware version | 0x34 | 0x0b | [fw_type] | [bytes length] [version] | fw_type:<br> 00h - BIOS<br>01h - CPLD<br>02h - BMC<br>03h - PSU<br/>Return data: version string presented as ASCII hex |
| Get GPIO status| 0x30 | 0xE1 | [pin_number] | [direction] [value]| return valid GPIO pin status, direction: 1=output, 0=input|

## Exmples
### Manual change PWM value
```bash
# enter manual mode to avoid PID control change pwm value
$ ipmitool raw 0x32 0x90 0x1

# set pwm 4 value 0x64
$ ipmitool raw 0x32 0x91 0x3 0x64
 64
# get pwm value from pwm 4
$ ipmitool raw 0x32 0x92 0x3
 64
# exit manual mode
$ ipmitool raw 0x32 0x90 0x0
# check we are in automatic mode
$ ipmitool raw 0x32 0x89
 00
```

### Get BIOS post code
```bash
root@evb-npcm845:~# ipmitool raw 0x34 0x73
 00

```

### Get firmware version
```bash
# BMC version is 2.12.0-dev-1361-gb4e63f50d-dirty
root@evb-npcm845:~# ipmitool raw 0x34 0x0b 0x02
 32 2e 31 31 2e 30 2d 31 30 32 2d 67 63 31 34 35
 33 30 37 63 64 2d 64 69 72 74 79
# BIOS version is C2195.0.BS.1A06.GN.3
root@evb-npcm845:~# ipmitool raw 0x34 0x0b 0x00
 43 32 31 39 35 2e 30 2e 42 53 2e 31 41 30 36 2e
 47 4e 2e 33
 # PSU version is 14133800
 root@scm-npcm845:~# ipmitool raw 0x34 0x0b 0x03
 31 34 31 33 31 38 30 30
```

# Get GPIO status
```bash
root@evb-npcm845:~# ipmitool raw 0x30 0xE1 91
 01 00
```
