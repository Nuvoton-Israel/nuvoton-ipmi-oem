#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

#include <ipmid/api-types.hpp>
#include <phosphor-logging/log.hpp>

#ifdef SOC_NPCM8XX
#include "gpio_oem_npcm8xx.hpp"
#else
#include "gpio_oem.hpp"
#endif

namespace ipmi
{
namespace nuvoton
{
using namespace phosphor::logging;

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

void show_multifn_select(uint32_t reg, uint32_t value,uint8_t maskBit)
{
    std::string msg;
    char r_value[32];
    switch (reg)
    {
    case MFSEL1:
        msg = "MFSEL1";
        break;
    case MFSEL2:
        msg = "MFSEL2";
        break;
    case MFSEL3:
        msg = "MFSEL3";
        break;
    case MFSEL4:
        msg = "MFSEL4";
        break;
#ifdef SOC_NPCM8XX
    case MFSEL5:
        msg = "MFSEL5";
        break;
    case MFSEL6:
        msg = "MFSEL6";
        break;
    case MFSEL7:
        msg = "MFSEL7";
        break;
    case I2CSEGSEL:
        msg = "I2CSEGSEL";
        break;
#endif
    default:
        msg = "ERROR";
        break;
    }
    sprintf(r_value, "0x%08X, bit: 0x%X", value, maskBit);
    msg += ", reg val:" + std::string(r_value);
    log<level::DEBUG>(msg.c_str());
}

bool isGP (uint8_t pinNum)
{
    uint8_t loop;
    uint8_t mask = 0;
    bool isGpio = false, ret;
    uint64_t read_result = devmem_read( (gpioLUT[pinNum].reg), LENGTH_32BIT, ret);
    std::string msg = std::to_string(pinNum);

    if (!ret)
    {
        return false;
    }

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
                show_multifn_select(gl.reg, (uint32_t)read_result, gl.maskBit);
                break;
            }

            next = gl.next;
        }
    }

    if (!isGpio)
    {
        msg = "pin function is set, not GPIO, " + msg;
        show_multifn_select(gpioLUT[pinNum].reg, (uint32_t)read_result, gpioLUT[pinNum].maskBit);
    }
    else
    {
        if (gpioLUT[pinNum].refValue)
            msg = "GPIO function is set, " + msg;
        else
            msg = "pin function is not set, " + msg;
    }
    log<level::DEBUG>(msg.c_str());

    return isGpio;
}

bool dumpGP (uint8_t pinNum, uint8_t &direction, uint8_t &level)
{
    bool ret;
    uint64_t read_result = devmem_read( (GPIO0 + GPIO_OFFSET*(pinNum/GPIO_NUM_IN_GROUP) + GP_OE_OFFSET), LENGTH_32BIT, ret);

    if (!ret)
    {
        log<level::DEBUG>("devmem_read failed, check OE");
        return false;
    }

    uint8_t offSet = pinNum % GPIO_NUM_IN_GROUP;

    // check if it's output-enabled
    direction = ( ((uint32_t)read_result) & (BIT_MASK<<offSet) )>>offSet;

    if (direction)
    {
        read_result = devmem_read( (GPIO0 + GPIO_OFFSET*(pinNum/GPIO_NUM_IN_GROUP) + GP_DOUT_OFFSET), LENGTH_32BIT, ret);

        if (!ret)
        {
            log<level::DEBUG>("devmem_read failed, check DOUT");
            return false;
        }

        level = ( ((uint32_t)read_result) & (BIT_MASK<<offSet) )>>offSet;

        return true;
    }

    // check if it's input-enabled
    read_result = devmem_read( (GPIO0 + GPIO_OFFSET*(pinNum/GPIO_NUM_IN_GROUP) + GP_IEM_OFFSET), LENGTH_32BIT, ret);

    if (!ret)
    {
        log<level::DEBUG>("devmem_read failed, check IEM");
        return false;
    }

    direction = ( ((uint32_t)read_result) & (BIT_MASK<<offSet) )>>offSet;

    if (direction)
    {
        direction = GP_DIR_INPUT;
        read_result = devmem_read( (GPIO0 + GPIO_OFFSET*(pinNum/GPIO_NUM_IN_GROUP) + GP_DIN_OFFSET), LENGTH_32BIT, ret);

        if (!ret)
        {
            log<level::DEBUG>("devmem_read failed, check DIN");
            return false;
        }

        level = ( ((uint32_t)read_result) & (BIT_MASK<<offSet) )>>offSet;
        return true;
    }

    std::string msg = "not OE/IEM! pin may not set to GPIO, " + std::to_string(pinNum);
    log<level::DEBUG>(msg.c_str());
    return false;
}

ipmi::RspType<uint8_t, //direction
              uint8_t  //high/low
              >
ipmiOEMGetGpioStatus (uint8_t pinNum)
{
    uint8_t direction = 0;
    uint8_t level = 0;
    std::string msg = std::to_string(pinNum);

    if (GPIO_NUM <= pinNum)
    {
        msg = "Invalid pin number:" + msg;
        log<level::DEBUG>(msg.c_str());
        return ipmi::responseParmOutOfRange();
    }

    if (DEF_NONE == gpioLUT[pinNum].refValue)
    {
        msg = "Invalid pin value, pin:" + msg;
        log<level::DEBUG>(msg.c_str());
        return ipmi::responseResponseError();
    }

    if (DEF_GPIO == gpioLUT[pinNum].refValue)
    {
        if(!dumpGP(pinNum, direction, level))
        {
            msg = "Dump pin value fail: " + msg;
            log<level::DEBUG>(msg.c_str());
            return ipmi::responseResponseError();
        }

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

    msg = "fallback case, should never get in, " + msg;
    log<level::DEBUG>(msg.c_str());
    return ipmi::responseResponseError();
}

} // namespace nuvoton

} // namespace ipmi