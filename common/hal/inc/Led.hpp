#ifndef LED_HPP
#define LED_HPP

#include "DigitalOutput.hpp"

#include "HardwareConfig.hpp"

/**
 * @brief Logical LED wrapper over a GPIO output.
 */
class Led {
public:
    /**
     * @brief Creates a LED on a GPIO output pin.
     * @param config Configuration for the LED.
     */
    Led(const LedConfig& config);
    void turnOn();
    void turnOff();
    void set(bool isOn);
    void toggle();
    bool isOn() const;

private:
    bool getPhysicalState(bool isOn) const;
    DigitalOutput output_;
    bool isActiveLow_;
};

#endif /* LED_HPP */
