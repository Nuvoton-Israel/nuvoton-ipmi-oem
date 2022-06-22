#include "ipmi_fw.hpp"

#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>

#include <fstream>
#include <iostream>
#include <ipmid/api.hpp>
#include <nlohmann/json.hpp>
#include <optional>
#include <phosphor-logging/elog-errors.hpp>
#include <phosphor-logging/log.hpp>
#include <stdexcept>
#include <xyz/openbmc_project/Common/error.hpp>

namespace ipmi
{
namespace nuvoton
{
using namespace phosphor::logging;
using Json = nlohmann::json;
using InternalFailure =
    sdbusplus::xyz::openbmc_project::Common::Error::InternalFailure;

static constexpr auto psu_update = "/usr/bin/update_psu";
static constexpr auto PSUImagePath = "/var/wcs/home/";
// TODO: use entity config instead of define a new configuration
static constexpr auto FwConfig = "/usr/share/ipmi-providers/fw.json";
static constexpr auto FwBusType = "bus";
static constexpr auto FwAddressType = "address";
static const std::string space = " ";
// static std::string PSUBus = "7";
// static std::string PSUAddress = "0x58";
static constexpr auto I2C_DEV = "/dev/i2c-";

Json parseJSONConfig(const std::string& configFile)
{
    std::ifstream jsonFile(configFile);
    if (!jsonFile.is_open())
    {
        log<level::ERR>("Temperature readings JSON file not found");
        elog<InternalFailure>();
    }

    auto data = Json::parse(jsonFile, nullptr, false);
    if (data.is_discarded())
    {
        log<level::ERR>("Temperature readings JSON parser failure");
        elog<InternalFailure>();
    }

    return data;
}

typedef struct
{
    std::string bus;
    uint16_t address;
} FW_CONFIG;

int readFwConfig(std::string target, FW_CONFIG& fw_config)
{
    std::string bus, address, msg;
    try
    {
        Json data = parseJSONConfig(FwConfig);
        auto config = data.find(target);
        if (config != data.end())
        {
            bus = (*config).value(FwBusType, bus);
            address = (*config).value(FwAddressType, address);
            fw_config.address = (uint16_t)std::stoul(address, nullptr, 16);
            ;
            fw_config.bus = bus;
        }
    }
    catch (const std::exception& e)
    {
        log<level::ERR>(e.what());
        return -1;
    }
    msg = target + " bus: " + bus + ", address: " + address;
    log<level::INFO>(msg.c_str());
    return 0;
}

// TBD: wait for implement
ipmi::RspType<> psuFwUpdate(std::string image)
{
    std::string msg = "psuFwUpdate: " + image;
    log<level::INFO>(msg.c_str());
    /*if (readFwConfig() != 0)
    {
        log<level::ERR>("Cannot read PSU config to get bus and address");
        return ipmi::responseUnspecifiedError();
    }
    std::string cmd = psu_update + space + PSUBus + space + PSUAddress + space +
                      std::string(PSUImagePath) + image;
    // exec may cause timeout, use service to implement
    // ipmi_dbus_sendrecv: failed to send dbus message (Connection timed out)
    try
    {
        log<level::INFO>(cmd.c_str());
        std::string result = exec(cmd.c_str());
        log<level::INFO>(result.c_str());
    }
    catch (const std::exception& e)
    {
        log<level::ERR>(e.what());
        return ipmi::responseUnspecifiedError();
    }
    return ipmi::responseSuccess();*/
    return ipmi::responseCmdFailFwUpdMode();
}

// TBD: wait for implement
ipmi::RspType<> psuFwStatus(uint8_t region)
{
    log<level::INFO>("psuFwStatus");
    return ipmi::responseCmdFailFwUpdMode();
}

// TBD: wait for implement
ipmi::RspType<> psuFwAbort(uint8_t region)
{
    log<level::INFO>("psuFwAbort");
    return ipmi::responseCmdFailFwUpdMode();
}

ipmi::RspType<> ipmiOemPsuFwUpdate(uint8_t region, uint8_t action,
                                   std::string image)
{
    switch (action)
    {
        case as_int(FirmwareAction::ACTIVE):
            return psuFwUpdate(image);
        case as_int(FirmwareAction::STATUS):
            return psuFwStatus(region);
        case as_int(FirmwareAction::ABORT):
            return psuFwAbort(region);
        default:
            return ipmi::responseUnspecifiedError();
    }
}

std::string exec(const char* cmd)
{
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe)
        throw std::runtime_error("popen() failed!");
    try
    {
        while (fgets(buffer, sizeof buffer, pipe) != NULL)
        {
            result += buffer;
        }
    }
    catch (...)
    {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

int fw_open_i2c(std::string i2c_bus)
{
    std::string i2c_dev = I2C_DEV + i2c_bus;
    int i2cdev = open(i2c_dev.c_str(), O_RDWR);
    if (i2cdev < 0)
    {
        std::string msg = "Cannot open Fw I2C device: " + i2c_dev;
        log<level::ERR>(msg.c_str());
    }
    return i2cdev;
}

namespace psu
{
static constexpr auto FW_INFO_LENGTH = 11;
static constexpr auto PSU_SERVICE = "psu_update.service";
static constexpr auto SYSTEMD_BUSNAME = "org.freedesktop.systemd1";
static constexpr auto SYSTEMD_PATH = "/org/freedesktop/systemd1";
static constexpr auto SYSTEMD_INTERFACE = "org.freedesktop.systemd1.Manager";

struct FW_INFO
{
    uint8_t length;
    uint8_t active;
    char revision[]; // unit8, for eaiser convert to string
} __attribute__((packed));

int read_fw_info(int i2cdev, int address, uint8_t image, std::string& ver,
                 uint8_t* active)
{
    struct i2c_rdwr_ioctl_data i2c_rdwr;
    struct i2c_msg i2cmsg[2];
    uint8_t buf[32];
    int ret;
    std::string msg;

    buf[0] = 0xEF;
    buf[1] = 0x1;
    buf[2] = image;

    i2cmsg[0].addr = address;
    i2cmsg[0].flags = 0x00; // write
    i2cmsg[0].len = 3;
    i2cmsg[0].buf = buf;

    i2cmsg[1].addr = address;
    i2cmsg[1].flags = I2C_M_RD; // read
    i2cmsg[1].len = FW_INFO_LENGTH;
    i2cmsg[1].buf = buf;

    i2c_rdwr.msgs = i2cmsg;
    i2c_rdwr.nmsgs = 2;
    ret = ioctl(i2cdev, I2C_RDWR, &i2c_rdwr);
    if (ret < 0)
    {
        msg = "read_fw_info: i2c err ret =" + std::to_string(ret);
        log<level::ERR>(msg.c_str());
        return ret;
    }
    auto info = reinterpret_cast<FW_INFO*>(buf);
    ver = std::string(info->revision, info->length - 1);
    *active = info->active;

    return 0;
}

int getPsuVersionInfo(ipmi::Context::ptr& ctx, std::string& ver)
{
    std::string rev;
    uint8_t active;
    int ret = -1;
    FW_CONFIG config;

    // read PSU bus and address
    if (readFwConfig(std::string("PSU"), config) != 0)
        return ret;

    // get version from dbus first?

    // read PSU versions
    int i2cdev = fw_open_i2c(config.bus);
    if (i2cdev > 0)
    {
        // Read A image first
        ret = psu::read_fw_info(i2cdev, config.address, 0xA, rev, &active);
        if (ret == 0 && active > 0)
        {
            ver = rev;
        }
        else
        {
            // If A image is not active, read B image
            ret = psu::read_fw_info(i2cdev, config.address, 0xB, rev, &active);
            if (ret == 0 && active > 0)
            {
                ver = rev;
            }
            // somthing goes wrong, A/B inactive
            else
            {
                log<level::ERR>("Cannot get active image!");
                if (ret == 0)
                {
                    ret = -1;
                }
            }
        }
        close(i2cdev);
    }
    return ret;
}

void startflashPsu(sdbusplus::bus::bus& bus)
{
    auto method = bus.new_method_call(SYSTEMD_BUSNAME, SYSTEMD_PATH,
                                      SYSTEMD_INTERFACE, "StartUnit");
    method.append(PSU_SERVICE, "replace");
    try
    {
        auto reply = bus.call(method);
    }
    catch (const sdbusplus::exception::exception& e)
    {
        log<level::ERR>(e.what());
        elog<InternalFailure>();
    }
}
} // namespace psu

namespace cpld
{
static const std::string CPLD = "CPLD";
static const std::string SCM_CPLD = "DC-SCM CPLD";
static constexpr auto FW_INFO_LENGTH = 1;

// TBD, we have no CPLD spec yet
int read_clpd_version(int i2cdev, u_int16_t address, std::string& ver)
{
    struct i2c_rdwr_ioctl_data i2c_rdwr;
    struct i2c_msg i2cmsg[2];
    uint8_t buf[4];
    int ret;
    std::string msg;

    buf[0] = 0x0;

    i2cmsg[0].addr = address;
    i2cmsg[0].flags = 0x00; // write
    i2cmsg[0].len = 1;      // CPLD version cmd: 0x0
    i2cmsg[0].buf = buf;

    i2cmsg[1].addr = address;
    i2cmsg[1].flags = I2C_M_RD; // read
    i2cmsg[1].len = FW_INFO_LENGTH;
    i2cmsg[1].buf = buf;

    i2c_rdwr.msgs = i2cmsg;
    i2c_rdwr.nmsgs = 2;
    ret = ioctl(i2cdev, I2C_RDWR, &i2c_rdwr);
    if (ret < 0)
    {
        msg = "read_fw_info: i2c err ret =" + std::to_string(ret);
        log<level::ERR>(msg.c_str());
        return ret;
    }
    // cast uint8 to char to build string
    //ver = std::string(reinterpret_cast<const char*>(buf), FW_INFO_LENGTH);
    ver = std::to_string(buf[0]);

    return 0;
}

int getCpldVersionInfo(ipmi::Context::ptr& ctx, std::string& ver,
                       uint8_t fw_type)
{
    std::string cfg_typename;
    int ret = -1;
    FW_CONFIG config;
    if (fw_type == as_int(FirmwareType::CPLD))
        cfg_typename = CPLD;
    else
        cfg_typename = SCM_CPLD;

    if (readFwConfig(cfg_typename, config) != 0)
        return ret;

    int i2cdev = fw_open_i2c(config.bus);
    if (i2cdev > 0)
    {
        ret = read_clpd_version(i2cdev, config.address, ver);
        close(i2cdev);
    }
    return ret;
}

} // namespace cpld

} // namespace nuvoton

} // namespace ipmi