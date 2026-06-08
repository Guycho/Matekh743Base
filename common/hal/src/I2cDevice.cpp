#include "I2cDevice.hpp"

I2cDevice::I2cDevice(const I2cDeviceConfig& config)
    : hi2c_(config.hi2c), devAddr_(config.deviceAddress)
{
}

bool I2cDevice::readRegister(uint16_t regAddr, uint8_t* data, size_t len)
{
    // Use a specific timeout (10ms) as seen in cp_hw to prevent infinite hangs
    if (HAL_I2C_Mem_Read(hi2c_, devAddr_, regAddr, I2C_MEMADD_SIZE_8BIT, data, len, 10) == HAL_OK) {
        return true;
    }
    return false;
}

bool I2cDevice::writeRegister(uint16_t regAddr, const uint8_t* data, size_t len)
{
    // Use a specific timeout (10ms) as seen in cp_hw to prevent infinite hangs
    if (HAL_I2C_Mem_Write(hi2c_, devAddr_, regAddr, I2C_MEMADD_SIZE_8BIT, (uint8_t*)data, len, 10) == HAL_OK) {
        return true;
    }
    return false;
}

bool I2cDevice::writeByte(uint16_t regAddr, uint8_t value)
{
    return writeRegister(regAddr, &value, 1);
}