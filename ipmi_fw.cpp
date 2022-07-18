#include "ipmi_fw.hpp"

#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ipmid/api.hpp>
#include <ipmid/utils.hpp>
#include <nlohmann/json.hpp>
#include <phosphor-logging/elog-errors.hpp>
#include <phosphor-logging/log.hpp>
#include <regex>
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
static constexpr auto MAX_DATA_BYTES = 240;

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
           ", r:" + std::to_string(fw_config.rd_len);
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
static constexpr auto FW_INFO_LENGTH = 10;
static constexpr auto PSU_SERVICE = "psu_update.service";
static constexpr auto SYSTEMD_BUSNAME = "org.freedesktop.systemd1";
static constexpr auto SYSTEMD_PATH = "/org/freedesktop/systemd1";
static constexpr auto SYSTEMD_INTERFACE = "org.freedesktop.systemd1.Manager";

int read_fw_info(std::string bus_id, uint16_t address, uint8_t image,
                 std::string& ver, uint8_t* active)
{
    std::string i2cBus = I2C_DEV + bus_id;
    std::vector<uint8_t> cmdWrite = {0xEF, 0x01, image};
    static std::vector<uint8_t> readBuf(FW_INFO_LENGTH);
    ipmi::Cc ret = ipmi::i2cWriteRead(i2cBus, static_cast<uint8_t>(address),
                                      cmdWrite, readBuf);
    if (ret != ipmi::ccSuccess)
    {
        return ret;
    }
    /*
    struct FW_INFO
    {
        uint8_t length;
        uint8_t active;
        uint8_t revision[];
    } __attribute__((packed));
    */
    ver = std::string(readBuf.begin() + 2, readBuf.end());
    *active = readBuf[1];

    return ret;
}

// TBD: wait for implement
ipmi::RspType<uint8_t> psuFwUpdate(std::string image)
{
    std::string msg = "psuFwUpdate: " + image;
    log<level::INFO>(msg.c_str());
    return ipmi::responseCmdFailFwUpdMode();
}

// TBD: wait for implement
ipmi::RspType<uint8_t> psuFwStatus(uint8_t region)
{
    log<level::INFO>("psuFwStatus");
    return ipmi::responseCmdFailFwUpdMode();
}

// TBD: wait for implement
ipmi::RspType<uint8_t> psuFwAbort(uint8_t region)
{
    log<level::INFO>("psuFwAbort");
    return ipmi::responseCmdFailFwUpdMode();
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

    // Read A image first
    ret = psu::read_fw_info(config.bus, config.address, 0xA, rev, &active);
    if (ret == 0 && active > 0)
    {
        ver = rev;
    }
    else
    {
        // If A image is not active, read B image
        ret = psu::read_fw_info(config.bus, config.address, 0xB, rev, &active);
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

ipmi::RspType<uint8_t> ipmiOemPsuFwUpdate(uint8_t region, uint8_t action,
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

// TODO: implement check bus and address is allowed
bool isCmdPmbusAllowed(uint8_t bus_id, uint8_t slaveAddr,
                       std::vector<uint8_t>& writeData)
{
    return true;
}

/** @brief Perform PMBUS read/write command with specific phase
 *
 *  @param isPrivateBus -to indicate private bus usage
 *  @param busId - bus id
 *  @param channelNum - channel number
 *  @param reserved - skip 1 bit
 *  @param slaveAddr - slave address
 *  @param readCount - number of bytes to be read, the maximum read count should
 * be 240 bytes.
 *  @param writeData - data to be written. This command should support 240 bytes
 * of write data.
 *
 *  @returns IPMI completion code plus response data
 *   - readData - i2c response data
 *
 * Note: additional Completion Code: 81h = Lost Arbitration 82h = Bus Error
 *                                   83h = NAK on Write 84h = Truncated Read
 */
ipmi::RspType<std::vector<uint8_t>>
    masterPhase(bool isPrivateBus, uint3_t busId, uint4_t channelNum,
                bool reserved, uint7_t slaveAddr, uint8_t phase,
                uint8_t readCount, std::vector<uint8_t> writeData)
{
    std::string msg =
        "bus:" + std::to_string(static_cast<uint8_t>(busId)) +
        ",address:" + std::to_string(static_cast<uint8_t>(slaveAddr)) +
        ",phase:" + std::to_string(phase) +
        ",read count:" + std::to_string(readCount);
    log<level::INFO>(msg.c_str());
    // normal check as master write read
    if (reserved)
    {
        return ipmi::responseInvalidFieldRequest();
    }
    if (readCount > MAX_DATA_BYTES)
    {
        log<level::ERR>(
            "Master phase write read command: Read count exceeds limit");
        return ipmi::responseParmOutOfRange();
    }
    const size_t writeCount = writeData.size();
    if (writeCount > MAX_DATA_BYTES)
    {
        log<level::ERR>(
            "Master phase write read command: write data exceeds limit");
        return ipmi::responseParmOutOfRange();
    }
    if (!readCount && !writeCount)
    {
        log<level::ERR>(
            "Master phase write read command: Read & write count are 0");
        return ipmi::responseInvalidFieldRequest();
    }

    // check phase
    if (phase != 0xff && phase >= 3)
    {
        log<level::ERR>("Master phase write read command: Invalid phase");
        return ipmi::responseInvalidFieldRequest();
    }

    // check is valid PMBUS
    if (!isCmdPmbusAllowed(static_cast<uint8_t>(busId),
                           static_cast<uint8_t>(slaveAddr), writeData))
    {
        log<level::ERR>("Master phase write read command: Command blocked",
                        entry("BUS=%d", static_cast<uint8_t>(busId)),
                        entry("ADDR=0x%x", static_cast<uint8_t>(slaveAddr)));
        return ipmi::responseInvalidFieldRequest();
    }

    // set selected phase
    ipmi::Cc ret;
    std::string i2cBus =
        "/dev/i2c-" + std::to_string(static_cast<uint8_t>(busId));
    static std::vector<uint8_t> phaseReadBuf = {};
    ret = ipmi::i2cWriteRead(i2cBus, static_cast<uint8_t>(slaveAddr),
                             std::vector<uint8_t>{phase}, phaseReadBuf);
    if (ret != ipmi::ccSuccess)
    {
        return ipmi::response(ret);
    }

    // execute command
    std::vector<uint8_t> readBuf(readCount);
    ret = ipmi::i2cWriteRead(i2cBus, static_cast<uint8_t>(slaveAddr), writeData,
                             readBuf);
    if (ret != ipmi::ccSuccess)
    {
        return ipmi::response(ret);
    }

    // set phase to 0xff
    ret = ipmi::i2cWriteRead(i2cBus, static_cast<uint8_t>(slaveAddr),
                             std::vector<uint8_t>{0xFF}, phaseReadBuf);
    if (ret != ipmi::ccSuccess)
    {
        return ipmi::response(ret);
    }

    return ipmi::responseSuccess(readBuf);
}

} // namespace psu

namespace cpld
{
static const std::string CPLD = "CPLD";
static const std::string SCM_CPLD = "DC-SCM CPLD";
static constexpr auto CPLD_BUF_MAX = 8;
static const std::vector<uint8_t> CPLD_VER_CMD = {0xC0, 0x0, 0x0, 0x0};
static const std::vector<uint8_t> SCM_CPLD_VER_CMD = {0xC0, 0x0, 0x0, 0x0};

int read_clpd_version(int i2cdev, FW_CONFIG cfg, std::string& ver)
{
    struct i2c_rdwr_ioctl_data i2c_rdwr;
    struct i2c_msg i2cmsg[2];
    uint8_t buf[CPLD_BUF_MAX];
    char hexstr[CPLD_BUF_MAX + 1];
    int ret;
    std::string msg;
    if (cfg.wr_len > CPLD_BUF_MAX || cfg.rd_len > CPLD_BUF_MAX ||
        cfg.wr_cmds.size() > CPLD_BUF_MAX)
    {
        log<level::ERR>("CPLD data out of buffer");
        return ipmi::ccParmOutOfRange;
    }
    if (cfg.rd_len > 4)
    {
        log<level::ERR>("We are now only support 4 byte version");
        return ipmi::ccParmOutOfRange;
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
    // ver = std::string(reinterpret_cast<const char*>(buf), cfg.rd_len);
    // ver = std::to_string(buf[0]);
    // uint32_t data = buf[0] << 24 | buf[1] << 16 | buf[2] << 8 | buf[3];
    // ver = std::to_string(data);
    for (size_t i = 0; i < cfg.rd_len; i++)
    {
        sprintf(hexstr + i * 2, "%02x", buf[i]);
    }
    ver = std::string(hexstr);

    return ipmi::ccSuccess;
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
        config.wr_len = 4;
        config.rd_len = 4;
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

// Level 2 mux may be a FPGA emulated bus, we need implement set channel
ipmi::Cc setMuxChannel(std::string i2cBus, uint8_t slaveAddr,
                       uint8_t channelNum)
{
    ipmi::Cc ret;
    static std::vector<uint8_t> muxReadBuf = {};
    uint8_t chControl = (uint8_t)1 << channelNum;
    ret = ipmi::i2cWriteRead(i2cBus, static_cast<uint8_t>(slaveAddr),
                             std::vector<uint8_t>{chControl}, muxReadBuf);
    if (ret != ipmi::ccSuccess)
    {
        std::string msg = "I2C set mux channel failed, bus:" + i2cBus +
                          ",addr:" + std::to_string(slaveAddr);
        log<level::ERR>(msg.c_str());
    }
    return ret;
}

#ifdef SEARCH_MUX_BUS
// Level 1 mux must define in DTS, and should be found as I2C bus
// We should use it to make sure I2C transction is atomic.
ipmi::Cc findMuxBus(uint8_t busId, uint8_t slaveAddr, uint8_t channelNum,
                    std::string& i2cBus)
{
    namespace fs = std::filesystem;
    // like /sys/class/i2c-dev/i2c-9/device
    std::string bus_path =
        "/sys/class/i2c-dev/i2c-" + std::to_string(busId) + "/device";
    // like 11-0072
    char mux_dev[8];
    std::string msg;
    sprintf(mux_dev, "%u-%04x", busId, slaveAddr);
    std::string mux_path = std::string(mux_dev);
    if (!fs::exists(bus_path) || !fs::exists(bus_path + "/" + mux_path))
    {
        msg = "Bus or mux not exists, bus:" + bus_path + ", mux:" + mux_path;
        log<level::ERR>(msg.c_str());
        return ipmi::ccInvalidFieldRequest;
    }
    std::string matchString = R"(i2c-\d+$)";
    std::regex search(matchString);
    std::smatch match;
    std::vector<std::string> foundBusses;
    for (auto p : std::filesystem::directory_iterator{bus_path})
    {
        std::string path = p.path().string();
        if (std::regex_search(path, match, search))
        {
            foundBusses.push_back(match[0]);
        }
    }
    std::sort(foundBusses.begin(), foundBusses.end());
#ifdef DEBUG
    msg = "";
    for (auto const& _bus : foundBusses)
    {
        msg += _bus + " ";
    }
    log<level::INFO>(msg.c_str());
#endif
    if (channelNum >= foundBusses.size())
    {
        log<level::ERR>("Channel number is larget than found");
        return ipmi::ccInvalidFieldRequest;
    }
    i2cBus = "/dev/" + foundBusses[channelNum];
    return ipmi::ccSuccess;
}
#endif

// Test command ipmitool raw 0x38 0x53 0x13 0xE0 0x0 0xFF 0x00 0xD4 0x08 0x0
// Read NVM-e status
ipmi::RspType<std::vector<uint8_t>>
    masterMuxWR(bool isPrivateBus, uint4_t busId, uint3_t reserved, bool resrv1,
                uint7_t muxSlaveAddr1, uint3_t channelNum1, uint5_t resrv2,
                bool secondMux, uint7_t muxSlaveAddr2, uint3_t channelNum2,
                uint5_t resrv3, bool resrv4, uint7_t slaveAddr,
                uint8_t readCount, std::vector<uint8_t> writeData)
{
    std::string msg =
        "bus:" + std::to_string(static_cast<uint8_t>(busId)) +
        ",address:" + std::to_string(static_cast<uint8_t>(slaveAddr)) +
        ",SA1:" + std::to_string(static_cast<uint8_t>(muxSlaveAddr1)) +
        ",CH1:" + std::to_string(static_cast<uint8_t>(channelNum1)) +
        ",SA2:" + std::to_string(static_cast<uint8_t>(muxSlaveAddr2)) +
        ",CH2:" + std::to_string(static_cast<uint8_t>(channelNum2)) +
        ",read count:" + std::to_string(readCount);
    log<level::INFO>(msg.c_str());
    // normal check as master write read
    uint32_t all_reserved = static_cast<uint8_t>(reserved) + resrv1 +
                            static_cast<uint8_t>(resrv2) +
                            static_cast<uint8_t>(resrv3) + resrv4;
    if (all_reserved || !isPrivateBus)
    {
        return ipmi::responseInvalidFieldRequest();
    }
    if (readCount > MAX_DATA_BYTES)
    {
        log<level::ERR>(
            "Master phase write read command: Read count exceeds limit");
        return ipmi::responseParmOutOfRange();
    }
    const size_t writeCount = writeData.size();
    if (writeCount > MAX_DATA_BYTES)
    {
        log<level::ERR>(
            "Master phase write read command: write data exceeds limit");
        return ipmi::responseParmOutOfRange();
    }
    if (!readCount && !writeCount)
    {
        log<level::ERR>(
            "Master phase write read command: Read & write count are 0");
        return ipmi::responseInvalidFieldRequest();
    }

    // find level 1 mux mapping bus
    ipmi::Cc ret;
    std::vector<uint8_t> readBuf(readCount);
    std::string i2cBus;
#ifdef SEARCH_MUX_BUS
    ret = findMuxBus(static_cast<uint8_t>(busId),
                     static_cast<uint8_t>(muxSlaveAddr1),
                     static_cast<uint8_t>(channelNum1), i2cBus);
    if (ret != ipmi::ccSuccess)
    {
        return ipmi::response(ret);
    }
#else
    i2cBus = I2C_DEV + std::to_string(static_cast<uint8_t>(busId));
    log<level::INFO>("set level 1 mux channel",
                     entry("CHANNEL=%d", static_cast<uint8_t>(channelNum1)));
    ret = setMuxChannel(i2cBus, static_cast<uint8_t>(muxSlaveAddr1),
                        static_cast<uint8_t>(channelNum1));
    if (ret != ipmi::ccSuccess)
    {
        return ipmi::response(ret);
    }
#endif
    // if level 2 mux set, set mux channel first
    if (secondMux == 0)
    {
        log<level::INFO>(
            "set level 2 mux channel",
            entry("CHANNEL=%d", static_cast<uint8_t>(channelNum2)));
        ret = setMuxChannel(i2cBus, static_cast<uint8_t>(muxSlaveAddr2),
                            static_cast<uint8_t>(channelNum2));
        if (ret != ipmi::ccSuccess)
        {
            return ipmi::response(ret);
        }
    }

    // execute command
    ret = ipmi::i2cWriteRead(i2cBus, static_cast<uint8_t>(slaveAddr), writeData,
                             readBuf);
    if (ret != ipmi::ccSuccess)
    {
        return ipmi::response(ret);
    }
    return ipmi::responseSuccess(readBuf);
}

} // namespace nuvoton

} // namespace ipmi