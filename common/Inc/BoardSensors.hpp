#ifndef BOARD_SENSORS_HPP
#define BOARD_SENSORS_HPP

#include "AdcDmaReader.hpp"
#include "HardwareConfig.hpp"

/**
 * @brief Scaled readings from the board analog inputs.
 */
struct AnalogSensorReadings {
    float mainVoltage;
    float mainCurrentVoltage;
    float mainCurrentAmps;
    float auxVoltage;
    float auxCurrentVoltage;
    float auxCurrentAmps;
    float airspeedVoltage;
    float rssiVoltage;
    float rssiPercent;
};

/**
 * @brief Board-level ADC mapping and scaling for Matek H743-MINI analog inputs.
 */
class BoardSensors {
public:
    explicit BoardSensors(AdcDmaReader& adcReader);
    float getAdcVoltage(AdcChannel channel) const;
    AnalogSensorReadings readAnalogSensors() const;
private:
    AdcDmaReader& adcReader_;
};

#endif /* BOARD_SENSORS_HPP */
