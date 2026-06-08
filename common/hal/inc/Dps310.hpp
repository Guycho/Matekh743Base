#ifndef DPS310_HPP
#define DPS310_HPP

#include "I2cDevice.hpp"
#include "SensorTypes.hpp"

class Dps310 {
public:
    explicit Dps310(I2cDevice& device);
    bool probe();
    bool initialize();
    bool readPressureTemperature(BarometerSample& sample);
private:
    I2cDevice& device_;
};

#endif /* DPS310_HPP */
