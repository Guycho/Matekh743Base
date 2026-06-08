#include "SpiDevice.hpp"

SpiDevice::SpiDevice(const SpiDeviceConfig& config)
    : hspi_(config.hspi), csPort_(config.csPort), csPin_(config.csPin)
{
}

bool SpiDevice::transfer(const uint8_t* txData, uint8_t* rxData, size_t len)
{
    HAL_GPIO_WritePin(csPort_, csPin_, GPIO_PIN_RESET);
    
    HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(hspi_, txData, rxData, len, HAL_MAX_DELAY);
    
    HAL_GPIO_WritePin(csPort_, csPin_, GPIO_PIN_SET);
    return (status == HAL_OK);
}

bool SpiDevice::transmit(const uint8_t* data, size_t len)
{
    HAL_GPIO_WritePin(csPort_, csPin_, GPIO_PIN_RESET);
    
    HAL_StatusTypeDef status = HAL_SPI_Transmit(hspi_, (uint8_t*)data, len, HAL_MAX_DELAY);
    
    HAL_GPIO_WritePin(csPort_, csPin_, GPIO_PIN_SET);
    return (status == HAL_OK);
}

bool SpiDevice::receive(uint8_t* data, size_t len)
{
    HAL_GPIO_WritePin(csPort_, csPin_, GPIO_PIN_RESET);
    
    HAL_StatusTypeDef status = HAL_SPI_Receive(hspi_, data, len, HAL_MAX_DELAY);
    
    HAL_GPIO_WritePin(csPort_, csPin_, GPIO_PIN_SET);
    return (status == HAL_OK);
}
