#include "BoardSensors.hpp"

namespace {
float clampPercent(float value)
{
    if (value < 0.0f) {
        return 0.0f;
    }
    if (value > 100.0f) {
        return 100.0f;
    }
    return value;
}
}

BoardSensors::BoardSensors(AdcDmaReader& adcReader)
    : adcReader_(adcReader)
{
}

float BoardSensors::getAdcVoltage(AdcChannel channel) const
{
    return adcReader_.getVoltage(static_cast<size_t>(channel));
}

AnalogSensorReadings BoardSensors::readAnalogSensors() const
{
    const float mainCurrentVoltage = getAdcVoltage(AdcChannel::MainCurrent);
    const float auxCurrentVoltage = getAdcVoltage(AdcChannel::AuxCurrent);
    const float rssiVoltage = getAdcVoltage(AdcChannel::Rssi);
    AnalogSensorReadings readings = {};
    readings.mainVoltage = getAdcVoltage(AdcChannel::MainVoltage) * board_config::MAIN_VOLTAGE_MULTIPLIER;
    readings.mainCurrentVoltage = mainCurrentVoltage;
    readings.mainCurrentAmps = mainCurrentVoltage * board_config::MAIN_CURRENT_AMPS_PER_VOLT;
    readings.auxVoltage = getAdcVoltage(AdcChannel::AuxVoltage) * board_config::AUX_VOLTAGE_MULTIPLIER;
    readings.auxCurrentVoltage = auxCurrentVoltage;
    readings.auxCurrentAmps = auxCurrentVoltage * board_config::AUX_CURRENT_AMPS_PER_VOLT;
    readings.airspeedVoltage = getAdcVoltage(AdcChannel::Airspeed) * board_config::AIRSPEED_VOLTAGE_MULTIPLIER;
    readings.rssiVoltage = rssiVoltage;
    readings.rssiPercent = clampPercent((rssiVoltage / board_config::RSSI_MAX_VOLTS) * 100.0f);
    return readings;
}
