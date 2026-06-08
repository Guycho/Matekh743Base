#include "DigitalInput.hpp"

DigitalInput::DigitalInput(const DigitalInputConfig& config)
    : port_(config.port), pin_(config.pin), activeLow_(config.activeLow)
{
}

bool DigitalInput::read() const
{
    bool raw = HAL_GPIO_ReadPin(port_, pin_) == GPIO_PIN_SET;
    return activeLow_ ? !raw : raw;
}
