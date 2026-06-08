#ifndef DIGITAL_OUTPUT_HPP
#define DIGITAL_OUTPUT_HPP

#include "../../../mcu/Core/Inc/main.h"
#include <cstdint>

#include "HardwareConfig.hpp"

/**
 * @brief Simple wrapper for a GPIO output pin.
 */
class DigitalOutput {
public:
    DigitalOutput(const DigitalOutputConfig& config);

    void set(bool state);
    void write(bool state);
    void toggle();
    bool read() const;

private:
    GPIO_TypeDef* port_;
    uint16_t pin_;
};

#endif /* DIGITAL_OUTPUT_HPP */