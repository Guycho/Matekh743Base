#ifndef LIS3MDL_HPP
#define LIS3MDL_HPP

#include "I2cDevice.hpp"
#include <cstdint>

struct Lis3mdlSample {
    int16_t magneticX;
    int16_t magneticY;
    int16_t magneticZ;
};

class Lis3mdl {
public:
    explicit Lis3mdl(I2cDevice& device);
    const char* getName() const;
    bool probe();
    bool initialize();
    bool readSample(Lis3mdlSample& sample);
private:
    I2cDevice& device_;
};

#endif /* LIS3MDL_HPP */
