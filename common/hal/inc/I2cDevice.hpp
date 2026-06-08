#ifndef I2C_DEVICE_HPP
#define I2C_DEVICE_HPP

#include "../../mcu/Core/Inc/main.h"
#include <cstdint>
#include <cstddef>

#include "HardwareConfig.hpp"

/**
 * @brief Wrapper for an I2C peripheral focused on register-based devices.
 */
class I2cDevice {
public:
    I2cDevice(const I2cDeviceConfig& config);

    /**
     * @brief Read a block of data from a specific register.
     * @return true if successful.
     */
    bool readRegister(uint16_t regAddr, uint8_t* data, size_t len);

    /**
     * @brief Write a block of data to a specific register.
     * @return true if successful.
     */
    bool writeRegister(uint16_t regAddr, const uint8_t* data, size_t len);

    /**
     * @brief Write a single byte to a specific register.
     */
    bool writeByte(uint16_t regAddr, uint8_t value);

private:
    I2C_HandleTypeDef *hi2c_;
    uint16_t devAddr_;
};

#endif /* I2C_DEVICE_HPP */