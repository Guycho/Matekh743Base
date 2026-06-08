#include "Icm20602.hpp"

namespace {
constexpr uint8_t WHO_AM_I_REG = 0x75U;
constexpr uint8_t PWR_MGMT_1_REG = 0x6BU;
constexpr uint8_t CONFIG_REG = 0x1AU;
constexpr uint8_t GYRO_CONFIG_REG = 0x1BU;
constexpr uint8_t ACCEL_CONFIG_REG = 0x1CU;
constexpr uint8_t ACCEL_XOUT_H_REG = 0x3BU;
constexpr uint8_t ICM20602_WHO_AM_I = 0x12U;

int16_t makeInt16(uint8_t high, uint8_t low)
{
    return static_cast<int16_t>((static_cast<uint16_t>(high) << 8U) | low);
}
}

Icm20602::Icm20602(SpiDevice& device)
    : device_(device)
{
}

const char* Icm20602::getName() const
{
    return "ICM20602";
}

ImuKind Icm20602::getKind() const
{
    return ImuKind::Icm20602;
}

bool Icm20602::probe()
{
    uint8_t whoAmI = 0U;
    if (!device_.readRegister(WHO_AM_I_REG, &whoAmI, 1)) {
        return false;
    }
    return whoAmI == ICM20602_WHO_AM_I;
}

bool Icm20602::initialize()
{
    if (!probe()) {
        return false;
    }
    if (!device_.writeRegister(PWR_MGMT_1_REG, 0x80U)) {
        return false;
    }
    HAL_Delay(100);
    if (!device_.writeRegister(PWR_MGMT_1_REG, 0x01U)) {
        return false;
    }
    if (!device_.writeRegister(CONFIG_REG, 0x03U)) {
        return false;
    }
    if (!device_.writeRegister(GYRO_CONFIG_REG, 0x18U)) {
        return false;
    }
    return device_.writeRegister(ACCEL_CONFIG_REG, 0x10U);
}

bool Icm20602::readSample(ImuSample& sample)
{
    uint8_t data[14] = {};
    if (!device_.readRegister(ACCEL_XOUT_H_REG, data, sizeof(data))) {
        return false;
    }
    sample.accel.x = makeInt16(data[0], data[1]);
    sample.accel.y = makeInt16(data[2], data[3]);
    sample.accel.z = makeInt16(data[4], data[5]);
    sample.temperature = makeInt16(data[6], data[7]);
    sample.gyro.x = makeInt16(data[8], data[9]);
    sample.gyro.y = makeInt16(data[10], data[11]);
    sample.gyro.z = makeInt16(data[12], data[13]);
    return true;
}
