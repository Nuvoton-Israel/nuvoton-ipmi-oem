#pragma once
#include <ipmid/api.hpp>

namespace ipmi
{
namespace nuvoton
{

enum class FirmwareType : uint8_t
{
    BIOS = 0,
    CPLD = 1,
    BMC = 2,
    PSU = 3,
    SCM_CPLD = 7,
};

enum class FirmwareTarget : uint8_t
{
    PSU = 0x11,
};

enum class FirmwareAction : uint8_t
{
    ACTIVE = 1,
    ABORT = 2,
    STATUS = 3,
};

template <typename Enumeration>
constexpr auto as_int(Enumeration const value) ->
    typename std::underlying_type<Enumeration>::type
{
    return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}

int fw_open_i2c(std::string i2c_bus);
std::string exec(const char* cmd);

namespace psu
{
int read_fw_info(int i2cdev, int address, uint8_t image, std::string& ver,
                 uint8_t* active);
int getPsuVersionInfo(ipmi::Context::ptr& ctx, std::string& ver);
void startflashPsu(sdbusplus::bus::bus& bus);
void subscribeToSystemdSignals(sdbusplus::bus::bus& bus);
void unsubscribeFromSystemdSignals(sdbusplus::bus::bus& bus);
ipmi::RspType<uint8_t> ipmiOemPsuFwUpdate(uint8_t region, uint8_t action,
                                          std::string image);
ipmi::RspType<std::vector<uint8_t>>
    masterPhase(bool isPrivateBus, uint3_t busId, uint4_t channelNum,
                bool reserved, uint7_t slaveAddr, uint8_t phase,
                uint8_t readCount, std::vector<uint8_t> writeData);
} // namespace psu

namespace cpld
{
int getCpldVersionInfo(ipmi::Context::ptr& ctx, std::string& ver,
                       uint8_t fw_type);
} // namespace cpld

ipmi::RspType<std::vector<uint8_t>>
    masterMuxWR(bool isPrivateBus, uint4_t busId, uint3_t reserved, bool resrv1,
                uint7_t muxSlaveAddr1, uint3_t channelNum1, uint5_t resrv2,
                bool secondMux, uint7_t muxSlaveAddr2, uint3_t channelNum2,
                uint5_t resrv3, bool resrv4, uint7_t slaveAddr,
                uint8_t readCount, std::vector<uint8_t> writeData);

} // namespace nuvoton

} // namespace ipmi