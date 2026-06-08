#include "SerialPort.hpp"
#include <cstring>
#include <algorithm>

SerialPort::SerialPort(const SerialPortConfig& config)
    : huart_(config.huart), bufferSize_(config.bufferSize)
{
    rxBuffer_ = new uint8_t[config.bufferSize];
    memset(rxBuffer_, 0, bufferSize_);
}

SerialPort::~SerialPort()
{
    delete[] rxBuffer_;
}

void SerialPort::write(const uint8_t* data, size_t len)
{
    // Use a reasonable timeout (100ms) instead of HAL_MAX_DELAY
    HAL_UART_Transmit(huart_, (uint8_t*)data, len, 100);
}

void SerialPort::write(const std::string& str)
{
    write(reinterpret_cast<const uint8_t*>(str.c_str()), str.length());
}

bool SerialPort::available() const
{
    return availableCount() > 0;
}

size_t SerialPort::availableCount() const
{
    // Calculate the current DMA write position (the head)
    // __HAL_DMA_GET_COUNTER returns the number of bytes remaining to be transferred
    size_t head = bufferSize_ - __HAL_DMA_GET_COUNTER(huart_->hdmarx);
    
    if (head >= readPtr_) {
        return head - readPtr_;
    } else {
        return (bufferSize_ - readPtr_) + head;
    }
}

int SerialPort::readByte()
{
    if (!available()) {
        return -1;
    }
    uint8_t byte = rxBuffer_[readPtr_];
    readPtr_ = (readPtr_ + 1) % bufferSize_;
    return byte;
}

size_t SerialPort::readString(char* buf, size_t maxLen)
{
    size_t count = 0;
    while (available() && count < maxLen - 1) {
        buf[count++] = static_cast<char>(readByte());
        if (buf[count-1] == '\n' || buf[count-1] == '\r') {
            break;
        }
    }
    buf[count] = '\0';
    return count;
}

void SerialPort::clear()
{
    size_t head = bufferSize_ - __HAL_DMA_GET_COUNTER(huart_->hdmarx);
    readPtr_ = head;
}

void SerialPort::updateRxPointer(uint16_t currentDmaPos)
{
    // This method is kept for compatibility, but logic is handled in availableCount()
    (void)currentDmaPos;
}

void SerialPort::poll()
{
    // Error recovery logic based on cp_hw/MavlinkChannel
    // If the UART state has returned to READY, it may indicate a DMA stop or error
    if (huart_->gState == HAL_UART_STATE_READY) {
        // Clear potential error flags
        __HAL_UART_CLEAR_OREFLAG(huart_);
        __HAL_UART_CLEAR_FLAG(huart_, UART_FLAG_NE);
        __HAL_UART_CLEAR_FLAG(huart_, UART_FLAG_FE);
        __HAL_UART_CLEAR_FLAG(huart_, UART_FLAG_PE);
        
        // Restart DMA receive
        HAL_UART_Receive_DMA(huart_, rxBuffer_, bufferSize_);
    }
}

void SerialPort::pushByte(uint8_t byte)
{
    // Not used in DMA mode, but kept for interface consistency
    (void)byte;
}