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
| Set PID manual mode for all zones | 0x32| 0x90| [enabled]| - | Enabled set 0x01 to enter manaul mode; 0x0 to exit manual mode for all PID zones|
| Get PID manual mode |0x32| 0x89| - |[mode]| 0:automatic mode, 1: manual mode|
| Set PWM |0x32| 0x91| [pwm_id] [value] | [set_value]| Set specific pwm with value in range: 0x0 ~ 0x64|
| Read PWM |0x32| 0x92| [pwm_id] |[pwm_value]| Read specific pwm value|
| Get BIOS post code| 0x32 | 0x73 | [previous] | [post code]| Previous:0, get current POST code; previous:1, get previous POST code|
| Get firmware version | 0x38 | 0x0b | [fw_type] | [bytes length] [version] | fw_type:<br> 00h - BIOS<br>01h - CPLD<br>02h - BMC<br>03h - PSU<br/>Return data: version string presented as ASCII hex, CPLD will return direct version bytes. |
| Get GPIO status| 0x30 | 0xE1 | [pin_number] | [direction] [value]| Return valid GPIO pin status, direction: 1=output, 0=input|
| Master Phase Write Read |0x38| 0x54| [bus_id] [address] [phase] [read_count] [command] |Bytes read from the specified slave address.| Sending PMBUS commands to selected phase. The bus_id, address, read_count, command is defined as Master write-read command, please refer IPMI specification section 22.11. |

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
root@scm-npcm845:~# ipmitool raw 0x32 0x73 0x0
 31 01 0c 92 93 96 b7 0c 00 90 92 91 01 0c 00 d2
 50 01 0c 00 00 14 15 50 01 0c 00 98 99 01 0c 00
 23 26 01 0c 08 d0 0c bb be 0c 00 94 95 b4 b7 0c
 00 c1 a7 a8 b5 d6 4f c8 01 0c 03 51 52 55 56 02
 0c c9 53 54 59 5a 57 58 ca c4 cd fd b0 b2 b3 bb
 e9 ea eb ec ed e9 ea eb ec ed ee ee 00 bc ff 04
 11 e9 ea eb ec ed e9 ea eb ec ed ee ee e9 ea eb
 ec ed e9 ea eb ec ed e9 ea eb ec ed e9 ea eb ec
 ed ee ee ee ee d1 15 50 e0 10 01 4f 41 43 40 47
 42 44 48 14 15 14 15 14 15 14 15 14 15 14 15 14
 15 14 15 14 15 14 15 14 15 14 15 14 15 14 15 14
 15 11 12 13 30 31 3e 3f 00 10 11 20 21 01 54 54
 3e 3f 21 14 94 00 22 50 14 15 14 15 14 15 14 15
 14 15 14 15 14 15 14 15 14 15 14 15 14 15 14 15
 51 e1 94 90 91 e0 54 52 53 54 33 32 3b 4f 60 61

```

### Get firmware version
```bash
# BMC version is 2.12.0-dev-1361-gb4e63f50d-dirty
root@evb-npcm845:~# ipmitool raw 0x38 0x0b 0x02
 32 2e 31 31 2e 30 2d 31 30 32 2d 67 63 31 34 35
 33 30 37 63 64 2d 64 69 72 74 79

# BIOS version is C2195.0.BS.1A06.GN.3
root@evb-npcm845:~# ipmitool raw 0x38 0x0b 0x00
 43 32 31 39 35 2e 30 2e 42 53 2e 31 41 30 36 2e
 47 4e 2e 33

# PSU version is 14133800
root@scm-npcm845:~# ipmitool raw 0x38 0x0b 0x03
 31 34 31 33 31 38 30 30

# CPLD version
root@scm-npcm845:~# ipmitool raw 0x38 0xb 0x1
 30 30 30 31 30 65 30 34

# SCM CPLD version
root@scm-npcm845:~# ipmitool raw 0x38 0xb 0x7
 62 30 37 30 30 63 30 34
```

### Get GPIO status
```bash
root@scm-npcm845:~# ipmitool raw 0x30 0xE1 233
 00 00
```

### Master Phase Write Read
```bash
# Enter phase 0 then get PSU version with image A
root@scm-npcm845:~# ipmitool raw 0x38 0x54 0xF 0xB0 0x00 0x0A 0xEF 0x01 0x0A
 09 01 31 34 31 33 31 38 30 30
```

## OEM version configuration
For more flexible, we use a JSON configuration file to set the information for PSU, and CPLD
I2C bus and address data.
```json
{
  "PSU":
  {
    "bus":"7",
    "address":"0x58"
  },
  "CPLD":
  {
    "bus":"5",
    "address":"0x40",
    "write_length":4,
    "read_lngth":4,
    "//":"[0xc0, 0x0, 0x0, 0x0]",
    "command":[
      192,
      0,
      0,
      0
    ]
  },
  "DC-SCM CPLD":
  {
    "bus":"15",
    "address":"0x60",
    "write_length":1,
    "read_lngth":1,
    "//":"[0xc0, 0x0, 0x0, 0x0]",
    "command":[
      192,
      0,
      0,
      0
    ]
  }
}
```
