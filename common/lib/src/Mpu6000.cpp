#include "Mpu6000.hpp"

namespace {
constexpr uint8_t WHO_AM_I_REG = 0x75U;
constexpr uint8_t PWR_MGMT_1_REG = 0x6BU;
constexpr uint8_t CONFIG_REG = 0x1AU;
constexpr uint8_t GYRO_CONFIG_REG = 0x1BU;
constexpr uint8_t ACCEL_CONFIG_REG = 0x1CU;
constexpr uint8_t ACCEL_XOUT_H_REG = 0x3BU;
constexpr uint8_t MPU6000_WHO_AM_I = 0x68U;
constexpr uint8_t MPU6000_ALT_WHO_AM_I = 0x70U;

int16_t makeInt16(uint8_t high, uint8_t low)
{
    return static_cast<int16_t>((static_cast<uint16_t>(high) << 8U) | low);
}
}

Mpu6000::Mpu6000(SpiDevice& device)
    : device_(device)
{
}

const char* Mpu6000::getName() const
{
    return "MPU6000";
}

bool Mpu6000::probe()
{
    uint8_t whoAmI = 0U;
    if (!device_.readRegister(WHO_AM_I_REG, &whoAmI, 1)) {
        return false;
    }
    return whoAmI == MPU6000_WHO_AM_I || whoAmI == MPU6000_ALT_WHO_AM_I;
}

bool Mpu6000::initialize()
{
    if (!probe()) {
        return false;
    }
    if (!device_.writeRegister(PWR_MGMT_1_REG, 0x80U)) {
        return false;
    }
    HAL_Delay(100);
    if (!device_.writeRegister(PWR_MGMT_1_REG, 0x03U)) {
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

bool Mpu6000::readSample(Mpu6000Sample& sample)
{
    uint8_t data[14] = {};
    if (!device_.readRegister(ACCEL_XOUT_H_REG, data, sizeof(data))) {
        return false;
    }
    sample.accelX = makeInt16(data[0], data[1]);
    sample.accelY = makeInt16(data[2], data[3]);
    sample.accelZ = makeInt16(data[4], data[5]);
    sample.temperature = makeInt16(data[6], data[7]);
    sample.gyroX = makeInt16(data[8], data[9]);
    sample.gyroY = makeInt16(data[10], data[11]);
    sample.gyroZ = makeInt16(data[12], data[13]);
    return true;
}
