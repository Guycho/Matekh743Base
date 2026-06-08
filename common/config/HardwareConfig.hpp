#ifndef HARDWARE_CONFIG_HPP
#define HARDWARE_CONFIG_HPP

#include "../../../mcu/Core/Inc/main.h"
#include <cstdint>
#include <cstddef>

enum class AdcChannel : size_t {
    MainVoltage = 0,
    MainCurrent = 1,
    AuxVoltage = 2,
    AuxCurrent = 3,
    Airspeed = 4,
    Rssi = 5,
};

namespace board_config {
    constexpr size_t ADC_SCAN_LENGTH = 6;
    constexpr float ADC_REFERENCE_VOLTS = 3.3f;
    constexpr float MAIN_VOLTAGE_MULTIPLIER = 11.0f;
    constexpr float AUX_VOLTAGE_MULTIPLIER = 21.0f;
    constexpr float MAIN_CURRENT_AMPS_PER_VOLT = 40.0f;
    constexpr float AUX_CURRENT_AMPS_PER_VOLT = 40.0f;
    constexpr float AIRSPEED_VOLTAGE_MULTIPLIER = 2.0f;
    constexpr float RSSI_MAX_VOLTS = 3.3f;
}

/**
 * @brief Configuration for a GPIO output pin.
 */
struct DigitalOutputConfig {
    GPIO_TypeDef* port;
    uint16_t pin;
};

/**
 * @brief Configuration for a logical LED.
 */
struct LedConfig {
    GPIO_TypeDef* port;
    uint16_t pin;
    bool activeWhenLow = false;
};

/**
 * @brief Configuration for a GPIO input pin.
 */
struct DigitalInputConfig {
    GPIO_TypeDef *port;
    uint16_t pin;
    bool activeLow = false;
};

/**
 * @brief Configuration for an ADC DMA reader.
 */
struct AdcDmaReaderConfig {
    ADC_HandleTypeDef *hadc;
    size_t scanLength;
    uint16_t *dmaBuffer;
    float vRef;
};

/**
 * @brief Configuration for a PWM output channel.
 */
struct PwmOutputConfig {
    TIM_HandleTypeDef *htim;
    uint32_t channel;
};

/**
 * @brief Configuration for a UART serial port.
 */
struct SerialPortConfig {
    UART_HandleTypeDef *huart;
    size_t bufferSize = 256;
};

/**
 * @brief Configuration for an SPI device.
 */
struct SpiDeviceConfig {
    SPI_HandleTypeDef *hspi;
    GPIO_TypeDef* csPort;
    uint16_t csPin;
};

/**
 * @brief Configuration for an I2C device.
 */
struct I2cDeviceConfig {
    I2C_HandleTypeDef *hi2c;
    uint16_t deviceAddress;
};

#endif /* HARDWARE_CONFIG_HPP */