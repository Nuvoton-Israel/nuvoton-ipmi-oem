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
} // namespace psu

namespace cpld
{
int getCpldVersionInfo(ipmi::Context::ptr& ctx, std::string& ver,
                       uint8_t fw_type);
} // namespace cpld

} // namespace nuvoton

} // namespace ipmi