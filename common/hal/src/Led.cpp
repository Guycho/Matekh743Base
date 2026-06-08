#include "Led.hpp"

Led::Led(const LedConfig& config)
    : output_{ {config.port, config.pin} }, isActiveLow_(config.activeWhenLow)
{
    turnOff();
}

void Led::turnOn()
{
    set(true);
}

void Led::turnOff()
{
    set(false);
}

void Led::set(bool isOn)
{
    output_.set(getPhysicalState(isOn));
}

void Led::toggle()
{
    output_.toggle();
}

bool Led::isOn() const
{
    const bool isPhysicalOn = output_.read();
    return isActiveLow_ ? !isPhysicalOn : isPhysicalOn;
}

bool Led::getPhysicalState(bool isOn) const
{
    return isActiveLow_ ? !isOn : isOn;
}
