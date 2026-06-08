#include "DigitalOutput.hpp"

DigitalOutput::DigitalOutput(const DigitalOutputConfig& config)
    : port_(config.port), pin_(config.pin)
{
}

void DigitalOutput::set(bool state)
{
    HAL_GPIO_WritePin(port_, pin_, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void DigitalOutput::write(bool state)
{
    set(state);
}

void DigitalOutput::toggle()
{
    HAL_GPIO_TogglePin(port_, pin_);
}

bool DigitalOutput::read() const
{
    return HAL_GPIO_ReadPin(port_, pin_) == GPIO_PIN_SET;
}