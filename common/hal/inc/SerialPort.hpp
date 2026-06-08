#ifndef SERIAL_PORT_HPP
#define SERIAL_PORT_HPP

#include "../../mcu/Core/Inc/main.h"
#include <cstdint>
#include <string>
#include <vector>

#include "HardwareConfig.hpp"

/**
 * @brief Wrapper for UART communication using DMA.
 * 
 * This class provides a ring-buffer interface for asynchronous RX 
 * and a simple interface for TX.
 */
class SerialPort {
public:
    SerialPort(const SerialPortConfig& config);
    ~SerialPort();

    // TX Methods
    void write(const uint8_t* data, size_t len);
    void write(const std::string& str);
    
    // RX Methods
    bool available() const;
    size_t availableCount() const;
    int readByte();
    size_t readString(char* buf, size_t maxLen);
    void clear();

    // DMA Bridge
    // This should be called from HAL_UART_RxCpltCallback and HAL_UART_RxHalfCpltCallback
    void updateRxPointer(uint16_t currentDmaPos);

    // Maintenance
    void poll();

private:
    UART_HandleTypeDef *huart_;
    uint8_t *rxBuffer_;
    size_t bufferSize_;
    
    volatile size_t readPtr_ = 0;
    volatile size_t writePtr_ = 0;

    void pushByte(uint8_t byte);
};

#endif /* SERIAL_PORT_HPP */