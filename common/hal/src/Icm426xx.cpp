#include "Icm426xx.hpp"

namespace {
constexpr uint8_t WHO_AM_I_REG = 0x75U;
constexpr uint8_t PWR_MGMT0_REG = 0x4EU;
constexpr uint8_t GYRO_CONFIG0_REG = 0x4FU;
constexpr uint8_t ACCEL_CONFIG0_REG = 0x50U;
constexpr uint8_t ACCEL_DATA_X1_REG = 0x1FU;
constexpr uint8_t ICM42688P_WHO_AM_I = 0x47U;
constexpr uint8_t ICM42605_WHO_AM_I = 0x42U;
constexpr uint8_t ENABLE_ACCEL_GYRO_LOW_NOISE = 0x0FU;
constexpr uint8_t TWO_KHZ_ODR_2000DPS = 0x06U;
constexpr uint8_t TWO_KHZ_ODR_16G = 0x06U;

int16_t makeInt16(uint8_t high, uint8_t low)
{
    return static_cast<int16_t>((static_cast<uint16_t>(high) << 8U) | low);
}
}

Icm426xx::Icm426xx(SpiDevice& device)
    : device_(device), kind_(ImuKind::Unknown), whoAmI_(0U)
{
}

const char* Icm426xx::getName() const
{
    if (kind_ == ImuKind::Icm42688p) {
        return "ICM42688-P";
    }
    if (kind_ == ImuKind::Icm42605) {
        return "ICM42605";
    }
    return "ICM426xx";
}

ImuKind Icm426xx::getKind() const
{
    return kind_;
}

bool Icm426xx::probe()
{
    if (!device_.readRegister(WHO_AM_I_REG, &whoAmI_, 1)) {
        return false;
    }
    if (whoAmI_ == ICM42688P_WHO_AM_I) {
        kind_ = ImuKind::Icm42688p;
        return true;
    }
    if (whoAmI_ == ICM42605_WHO_AM_I) {
        kind_ = ImuKind::Icm42605;
        return true;
    }
    kind_ = ImuKind::Unknown;
    return false;
}

bool Icm426xx::initialize()
{
    if (kind_ == ImuKind::Unknown && !probe()) {
        return false;
    }
    if (!device_.writeRegister(PWR_MGMT0_REG, ENABLE_ACCEL_GYRO_LOW_NOISE)) {
        return false;
    }
    HAL_Delay(10);
    if (!device_.writeRegister(GYRO_CONFIG0_REG, TWO_KHZ_ODR_2000DPS)) {
        return false;
    }
    return device_.writeRegister(ACCEL_CONFIG0_REG, TWO_KHZ_ODR_16G);
}

bool Icm426xx::readSample(ImuSample& sample)
{
    uint8_t data[12] = {};
    if (!device_.readRegister(ACCEL_DATA_X1_REG, data, sizeof(data))) {
        return false;
    }
    sample.accel.x = makeInt16(data[0], data[1]);
    sample.accel.y = makeInt16(data[2], data[3]);
    sample.accel.z = makeInt16(data[4], data[5]);
    sample.gyro.x = makeInt16(data[6], data[7]);
    sample.gyro.y = makeInt16(data[8], data[9]);
    sample.gyro.z = makeInt16(data[10], data[11]);
    sample.temperature = 0;
    return true;
}
