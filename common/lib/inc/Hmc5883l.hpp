#ifndef HMC5883L_HPP
#define HMC5883L_HPP

#include "I2cDevice.hpp"
#include <cstdint>

struct Hmc5883lSample {
    int16_t magneticX;
    int16_t magneticY;
    int16_t magneticZ;
};

class Hmc5883l {
public:
    explicit Hmc5883l(I2cDevice& device);
    const char* getName() const;
    bool probe();
    bool initialize();
    bool readSample(Hmc5883lSample& sample);
private:
    I2cDevice& device_;
};

#endif /* HMC5883L_HPP */
