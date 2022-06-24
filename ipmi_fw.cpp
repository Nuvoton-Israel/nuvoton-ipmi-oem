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

static constexpr auto PSUImagePath = "/var/wcs/home/";
// TODO: use entity config instead of define a new configuration
static constexpr auto FwConfig = "/usr/share/ipmi-providers/fw.json";
static constexpr auto FwBusType = "bus";
static constexpr auto FwAddressType = "address";
static constexpr auto FwWriteLength = "write_length";
static constexpr auto FwReadLength = "read_lngth";
static constexpr auto FwCommand = "command";
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
    uint8_t wr_len;
    uint8_t rd_len;
    std::vector<uint8_t> wr_cmds;
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
            fw_config.bus = bus;
            fw_config.wr_len = (*config).value(FwWriteLength, fw_config.wr_len);
            fw_config.rd_len = (*config).value(FwReadLength, fw_config.rd_len);
            auto cmd = (*config).find(FwCommand);
            if (cmd != (*config).end())
            {
                fw_config.wr_cmds = (*cmd).get<std::vector<uint8_t>>();
            }
        }
    }
    catch (const std::exception& e)
    {
        log<level::ERR>(e.what());
        return -1;
    }
    msg = target + " bus: " + bus + ", address: " + address;
    msg += ", w:" + std::to_string(fw_config.wr_len) +
           ",r:" + std::to_string(fw_config.rd_len);
    log<level::INFO>(msg.c_str());
    msg = "cmd";
    for (auto data = fw_config.wr_cmds.begin(); data != fw_config.wr_cmds.end();
         data++)
    {
        msg += " " + std::to_string(*data);
    }
    log<level::INFO>(msg.c_str());
    return 0;
}

// TBD: wait for implement
ipmi::RspType<> psuFwUpdate(std::string image)
{
    std::string msg = "psuFwUpdate: " + image;
    log<level::INFO>(msg.c_str());
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
static constexpr auto CPLD_BUF_MAX = 8;
static const std::vector<uint8_t> CPLD_VER_CMD = {0xC0, 0x0, 0x0, 0x0};
static const std::vector<uint8_t> SCM_CPLD_VER_CMD = {0x0};

int read_clpd_version(int i2cdev, FW_CONFIG cfg, std::string& ver)
{
    struct i2c_rdwr_ioctl_data i2c_rdwr;
    struct i2c_msg i2cmsg[2];
    uint8_t buf[CPLD_BUF_MAX];
    int ret;
    std::string msg;
    if (cfg.wr_len > CPLD_BUF_MAX || cfg.rd_len > CPLD_BUF_MAX ||
        cfg.wr_cmds.size() > CPLD_BUF_MAX)
    {
        log<level::ERR>("CPLD data out of buffer");
        return -1;
    }

    // set up CPLD version command
    std::copy(cfg.wr_cmds.begin(), cfg.wr_cmds.end(), buf);

    i2cmsg[0].addr = cfg.address;
    i2cmsg[0].flags = 0x00; // write
    i2cmsg[0].len = cfg.wr_len;
    i2cmsg[0].buf = buf;

    i2cmsg[1].addr = cfg.address;
    i2cmsg[1].flags = I2C_M_RD; // read
    i2cmsg[1].len = cfg.rd_len;
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
    ver = std::string(reinterpret_cast<const char*>(buf), cfg.rd_len);
    // ver = std::to_string(buf[0]);

    return 0;
}

int getCpldVersionInfo(ipmi::Context::ptr& ctx, std::string& ver,
                       uint8_t fw_type)
{
    std::string cfg_typename;
    int ret = -1;
    FW_CONFIG config;
    if (fw_type == as_int(FirmwareType::CPLD))
    {
        cfg_typename = CPLD;
        config.wr_cmds = CPLD_VER_CMD;
        config.wr_len = 4;
        config.rd_len = 4;
    }
    else if (fw_type == as_int(FirmwareType::SCM_CPLD))
    {
        cfg_typename = SCM_CPLD;
        config.wr_cmds = SCM_CPLD_VER_CMD;
        config.wr_len = 1;
        config.rd_len = 1;
    }
    else
        return ret;
    if (readFwConfig(cfg_typename, config) != 0)
        return ret;

    int i2cdev = fw_open_i2c(config.bus);
    if (i2cdev > 0)
    {
        ret = read_clpd_version(i2cdev, config, ver);
        close(i2cdev);
    }
    return ret;
}

} // namespace cpld

} // namespace nuvoton

} // namespace ipmi