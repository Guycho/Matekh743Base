#ifndef SPI_DEVICE_HPP
#define SPI_DEVICE_HPP

#include "../../../mcu/Core/Inc/main.h"
#include <cstdint>
#include <cstddef>

#include "HardwareConfig.hpp"

/**
 * @brief Wrapper for an SPI peripheral with integrated Chip Select management.
 */
class SpiDevice {
public:
    SpiDevice(const SpiDeviceConfig& config);

    /**
     * @brief Perform a full-duplex SPI transfer.
     * @return true if successful.
     */
    bool transfer(const uint8_t* txData, uint8_t* rxData, size_t len);
    bool transfer(const uint8_t* txData, uint8_t* rxData, size_t len, uint32_t timeoutMs);

    /**
     * @brief Send data without reading.
     */
    bool transmit(const uint8_t* data, size_t len);
    bool transmit(const uint8_t* data, size_t len, uint32_t timeoutMs);

    /**
     * @brief Receive data without sending.
     */
    bool receive(uint8_t* data, size_t len);
    bool receive(uint8_t* data, size_t len, uint32_t timeoutMs);

    bool readRegister(uint8_t regAddr, uint8_t* data, size_t len, uint8_t readMask = 0x80U);
    bool writeRegister(uint8_t regAddr, uint8_t value);

private:
    static constexpr size_t MAX_REGISTER_TRANSFER_LENGTH = 32;
    static constexpr uint32_t DEFAULT_TIMEOUT_MS = 10;
    SPI_HandleTypeDef *hspi_;
    GPIO_TypeDef* csPort_;
    uint16_t csPin_;
};

#endif /* SPI_DEVICE_HPP */