#ifndef DPS310_HPP
#define DPS310_HPP

#include "I2cDevice.hpp"
#include <cstdint>

struct Dps310Sample {
    int32_t pressureRaw;
    int32_t temperatureRaw;
};

class Dps310 {
public:
    explicit Dps310(I2cDevice& device);
    bool probe();
    bool initialize();
    bool readPressureTemperature(Dps310Sample& sample);
private:
    I2cDevice& device_;
};

#endif /* DPS310_HPP */
