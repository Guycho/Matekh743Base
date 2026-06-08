#include "SpiDevice.hpp"

SpiDevice::SpiDevice(const SpiDeviceConfig& config)
    : hspi_(config.hspi), csPort_(config.csPort), csPin_(config.csPin)
{
    HAL_GPIO_WritePin(csPort_, csPin_, GPIO_PIN_SET);
}

bool SpiDevice::transfer(const uint8_t* txData, uint8_t* rxData, size_t len)
{
    return transfer(txData, rxData, len, DEFAULT_TIMEOUT_MS);
}

bool SpiDevice::transfer(const uint8_t* txData, uint8_t* rxData, size_t len, uint32_t timeoutMs)
{
    HAL_GPIO_WritePin(csPort_, csPin_, GPIO_PIN_RESET);
    HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(hspi_, txData, rxData, len, timeoutMs);
    HAL_GPIO_WritePin(csPort_, csPin_, GPIO_PIN_SET);
    return (status == HAL_OK);
}

bool SpiDevice::transmit(const uint8_t* data, size_t len)
{
    return transmit(data, len, DEFAULT_TIMEOUT_MS);
}

bool SpiDevice::transmit(const uint8_t* data, size_t len, uint32_t timeoutMs)
{
    HAL_GPIO_WritePin(csPort_, csPin_, GPIO_PIN_RESET);
    HAL_StatusTypeDef status = HAL_SPI_Transmit(hspi_, (uint8_t*)data, len, timeoutMs);
    HAL_GPIO_WritePin(csPort_, csPin_, GPIO_PIN_SET);
    return (status == HAL_OK);
}

bool SpiDevice::receive(uint8_t* data, size_t len)
{
    return receive(data, len, DEFAULT_TIMEOUT_MS);
}

bool SpiDevice::receive(uint8_t* data, size_t len, uint32_t timeoutMs)
{
    HAL_GPIO_WritePin(csPort_, csPin_, GPIO_PIN_RESET);
    HAL_StatusTypeDef status = HAL_SPI_Receive(hspi_, data, len, timeoutMs);
    HAL_GPIO_WritePin(csPort_, csPin_, GPIO_PIN_SET);
    return (status == HAL_OK);
}

bool SpiDevice::readRegister(uint8_t regAddr, uint8_t* data, size_t len, uint8_t readMask)
{
    if (len == 0 || len > (MAX_REGISTER_TRANSFER_LENGTH - 1U)) {
        return false;
    }
    uint8_t txBuffer[MAX_REGISTER_TRANSFER_LENGTH] = {};
    uint8_t rxBuffer[MAX_REGISTER_TRANSFER_LENGTH] = {};
    txBuffer[0] = regAddr | readMask;
    if (!transfer(txBuffer, rxBuffer, len + 1U, DEFAULT_TIMEOUT_MS)) {
        return false;
    }
    for (size_t i = 0; i < len; ++i) {
        data[i] = rxBuffer[i + 1U];
    }
    return true;
}

bool SpiDevice::writeRegister(uint8_t regAddr, uint8_t value)
{
    const uint8_t txBuffer[2] = {regAddr, value};
    return transmit(txBuffer, sizeof(txBuffer), DEFAULT_TIMEOUT_MS);
}
