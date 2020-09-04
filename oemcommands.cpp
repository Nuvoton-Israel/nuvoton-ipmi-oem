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

#include "xyz/openbmc_project/Common/error.hpp"

#include <systemd/sd-journal.h>


#include <array>
#include <boost/container/flat_map.hpp>
#include <boost/process/child.hpp>
#include <boost/process/io.hpp>
#include <filesystem>
#include <iostream>
#include <ipmid/api.hpp>
#include <ipmid/utils.hpp>
#include <nlohmann/json.hpp>
#include <phosphor-logging/log.hpp>

#include "nuvoton_defs.hpp"
#include "oemcommands.hpp"

uint64_t devmem_read(off_t target, unsigned width, bool &ret)
{
    void *map_base, *virt_addr;
    uint64_t read_result;
    unsigned page_size, mapped_size, offset_in_page;
    int fd;

    ret = true;
    fd = open("/dev/mem", (O_RDONLY | O_SYNC));
    if(-1 == fd)
    {
        ret = false;
        return 0;
    }
    mapped_size = page_size = getpagesize();
    offset_in_page = (unsigned)target & (page_size - 1);
    if (offset_in_page + width > page_size) {
        mapped_size *= 2;
    }
    map_base = mmap(NULL, mapped_size, PROT_READ, MAP_SHARED, fd, target & ~(off_t)(page_size - 1));
    if (MAP_FAILED == map_base)
    {
        ret = false;
        close(fd);
        return 0;
    }

    virt_addr = (char*)map_base + offset_in_page;

    switch (width) {
      case 8:
        read_result = *(volatile uint8_t*)virt_addr;
        break;
      case 16:
        read_result = *(volatile uint16_t*)virt_addr;
        break;
      case 32:
        read_result = *(volatile uint32_t*)virt_addr;
        break;
      case 64:
        read_result = *(volatile uint64_t*)virt_addr;
        break;
      default:
        read_result = 0;
        ret = false;
    }

    if (-1 == munmap(map_base, mapped_size))
    {
        ret = false;
        close(fd);
        return 0;
    }
    close(fd);
    return read_result;
}

ipmi::RspType<uint8_t> ipmiOEMGetImageLocationStatus (void)
{
    bool ret;
    uint64_t read_result = devmem_read(ROM_CODE_STATUS, LENGTH_32BIT, ret);
    if (ret)
    {
        switch ((uint8_t)read_result)
        {
            case ST_ROM_USE_IMAGE_SPI0_CS0_OFFSET_0:
            case ST_ROM_USE_IMAGE_SPI0_CS0_OFFSET_80000:
                return ipmi::responseSuccess(FLASH_0);
            case ST_ROM_USE_IMAGE_SPI0_CS1_OFFSET_0:
                return ipmi::responseSuccess(FLASH_1);
            default:
                return ipmi::responseResponseError();
        }
    }
    else
        return ipmi::responseResponseError();
}

ipmi::RspType<uint8_t,
              uint8_t
             >
ipmiOEMGetBmcRebootReason (void)
{
    bool ret;
    uint32_t mask = GEN_MASK;
    uint32_t mask2 = BIT_MASK;
    uint64_t read_result = devmem_read(INTCR2, LENGTH_32BIT, ret);
    return (ret)? ipmi::responseSuccess(
                      (((uint32_t)read_result & (mask) )>>24),
                      (((uint32_t)read_result & (mask2<<23) )>>16)
                        ): ipmi::responseResponseError();
}

bool isGP (uint8_t pinNum)
{
    uint8_t loop;
    uint8_t mask = 0;
    bool isGpio = false, ret;
    uint64_t read_result = devmem_read( (gpioLUT[pinNum].reg), LENGTH_32BIT, ret);

    if (!ret)
        return false;

    // make the bit mask. The bit number might be greater than 1.
    for(loop = 0; loop < gpioLUT[pinNum].maskBit; loop++)
    {
        mask += (BIT_MASK << loop);
    }

    // check if the pin's value in MFSELx matches refValue
    // If so, it means that the pin is or might be configured as a gpio.
    if(gpioLUT[pinNum].refValue == ( ((uint32_t)read_result) &
                                     (mask<<gpioLUT[pinNum].offSet) )>>gpioLUT[pinNum].offSet )
    {
        isGpio = true;

        // if the pin needs to check the second MFSELx or the third MFSELx
        // configuration in the look up table
        struct gpio_lookup *next = gpioLUT[pinNum].next;

        while(NULL != next)
        {
            struct gpio_lookup gl = *next;
            read_result = devmem_read( gl.reg, LENGTH_32BIT, ret);

            if (!ret)
            {
                isGpio = false;
                break;
            }

            mask = 0;

            for(loop = 0; loop < gl.maskBit; loop++)
            {
                mask += (BIT_MASK << loop);
            }

            if(gl.refValue == ( ( ((uint32_t)read_result) &
                                             (mask<<gl.offSet) )>>gl.offSet  )  )
                isGpio = true;
            else
            {
                isGpio = false;
                break;
            }

            next = gl.next;
        }
    }

    return isGpio;
}

bool dumpGP (uint8_t pinNum, uint8_t &direction, uint8_t &level)
{
    bool ret;
    uint64_t read_result = devmem_read( (GPIO0 + GPIO_OFFSET*(pinNum/GPIO_NUM_IN_GROUP) + GP_OE_OFFSET), LENGTH_32BIT, ret);


    if (!ret)
        return false;

    uint8_t offSet = pinNum % GPIO_NUM_IN_GROUP;

    // check if it's output-enabled
    direction = ( ((uint32_t)read_result) & (BIT_MASK<<offSet) )>>offSet;

    if (direction)
    {
        read_result = devmem_read( (GPIO0 + GPIO_OFFSET*(pinNum/GPIO_NUM_IN_GROUP) + GP_DOUT_OFFSET), LENGTH_32BIT, ret);

        if (!ret)
            return false;

        level = ( ((uint32_t)read_result) & (BIT_MASK<<offSet) )>>offSet;

        return true;
    }

    // check if it's input-enabled
    read_result = devmem_read( (GPIO0 + GPIO_OFFSET*(pinNum/GPIO_NUM_IN_GROUP) + GP_IEM_OFFSET), LENGTH_32BIT, ret);

    if (!ret)
        return false;

    direction = ( ((uint32_t)read_result) & (BIT_MASK<<offSet) )>>offSet;

    if (direction)
    {
        direction = GP_DIR_INPUT;
        read_result = devmem_read( (GPIO0 + GPIO_OFFSET*(pinNum/GPIO_NUM_IN_GROUP) + GP_DIN_OFFSET), LENGTH_32BIT, ret);

        if (!ret)
            return false;

        level = ( ((uint32_t)read_result) & (BIT_MASK<<offSet) )>>offSet;
        return true;
    }

    return false;
}

ipmi::RspType<uint8_t, //direction
              uint8_t  //high/low
              >
ipmiOEMGetGpioStatus (uint8_t pinNum)
{
    uint8_t direction = 0;
    uint8_t level = 0;

    if ( (0 > pinNum) || (GPIO_NUM <= pinNum))
    {
        return ipmi::responseResponseError();
    }

    if (DEF_NONE == gpioLUT[pinNum].refValue)
    {
        return ipmi::responseResponseError();
    }

    if (DEF_GPIO == gpioLUT[pinNum].refValue)
    {
        if(!dumpGP(pinNum, direction, level))
            return ipmi::responseResponseError();

        return ipmi::responseSuccess(direction, level);
    }
    else
    {
        // Check if the pin is used as a gpio or something else.
        // If it's a gpio, then the direction and level attributes of that
        // pin is retrieved.
        if( isGP(pinNum) && dumpGP(pinNum, direction, level) )
            return ipmi::responseSuccess(direction, level);

        return ipmi::responseResponseError();
    }

    return ipmi::responseResponseError();
}

ipmi::RspType<uint8_t> ipmiOEMGetUsbDeviceStatus (uint8_t id)
{
    bool ret;
    uint32_t mask = GEN_MASK;
    uint32_t reg = USB_BASE;

    if( (USB_CTL_START > id) || (USB_CTL_END < id) )
    {
        return ipmi::responseInvalidFieldRequest();
    }

    uint64_t read_result = devmem_read( (reg + id*USB_OFFSET + USB_DEVICE_ADDR), LENGTH_32BIT, ret);

    // The USBADR starts from the 25th bit.
    uint8_t valid = static_cast<uint8_t> ( ((uint32_t)read_result & mask) >> 25);

    return (valid)? ipmi::responseSuccess(SUCCESS) : ipmi::responseResponseError();
}

ipmi::RspType<uint8_t> ipmiOEMGetUartMode (void)
{
    bool ret;
    uint32_t mask = SPSWC_MASK;
    uint64_t read_result = devmem_read(SPSWC, LENGTH_32BIT, ret);
    return (ret)? ipmi::responseSuccess(
                       static_cast<uint8_t> (((uint32_t)read_result & mask))
                                       ): ipmi::responseResponseError();
}

ipmi::RspType<uint8_t,
              uint8_t,
			        uint8_t,
			        uint8_t>
ipmiOEMGetStrapPinStatus(void)
{
    bool ret;
    uint32_t mask = GEN_MASK;
    uint64_t read_result = devmem_read(PWRON, LENGTH_32BIT, ret);
    return (ret)? ipmi::responseSuccess( static_cast<uint8_t> (((uint32_t)read_result & mask)>>24),
                                         static_cast<uint8_t> (((uint32_t)read_result & (mask>>8))>>16),
                       static_cast<uint8_t> (((uint32_t)read_result & (mask>>16))>>8),
                       static_cast<uint8_t> (((uint32_t)read_result & (mask>>24)))
                                       ): ipmi::responseResponseError();
}


namespace ipmi
{
static void registerOEMFunctions() __attribute__((constructor));

static void registerOEMFunctions(void)
{
    phosphor::logging::log<phosphor::logging::level::INFO>(
        "Registering Nuvoton IPMI OEM commands");

    // <Get Strap Pin Status command>
    registerHandler(prioOemBase, nuvoton::netFnGeneral,
                      nuvoton::general::cmdGetStrapPinStatus, Privilege::Callback,
                      ipmiOEMGetStrapPinStatus);

    // <Get UART mode command>
    registerHandler(prioOemBase, nuvoton::netFnGeneral,
                      nuvoton::general::cmdGetUartMode, Privilege::Callback,
                      ipmiOEMGetUartMode);

    // <Get USB device status command>
    registerHandler(prioOemBase, nuvoton::netFnGeneral,
                      nuvoton::general::cmdGetUsbDeviceStatus, Privilege::Callback,
                      ipmiOEMGetUsbDeviceStatus);

    // <Get GPIO status command>
    registerHandler(prioOemBase, nuvoton::netFnGeneral,
                      nuvoton::general::cmdGetGpioStatus, Privilege::Callback,
                      ipmiOEMGetGpioStatus);

    // <Get BMC reboot reason command>
    registerHandler(prioOemBase, nuvoton::netFnGeneral,
                      nuvoton::general::cmdGetBmcRebootReason, Privilege::Callback,
                      ipmiOEMGetBmcRebootReason);

    // <Get image location status command>
    registerHandler(prioOemBase, nuvoton::netFnGeneral,
                      nuvoton::general::cmdGetImageLocationStatus, Privilege::Callback,
                      ipmiOEMGetImageLocationStatus);

}

} // namespace ipmi
