#ifndef DIGITAL_INPUT_H
#define DIGITAL_INPUT_H

#include "main.h"
#include <cstdint>

#include "HardwareConfig.hpp"

class DigitalInput {
public:
    DigitalInput(const DigitalInputConfig& config);

    bool read() const;
    operator bool() const { return read(); }

    GPIO_TypeDef *port() const { return port_; }
    uint16_t pin() const { return pin_; }

private:
    GPIO_TypeDef *port_;
    uint16_t pin_;
    bool activeLow_;
};

#endif /* DIGITAL_INPUT_H */
