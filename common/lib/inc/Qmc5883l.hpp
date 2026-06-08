#ifndef QMC5883L_HPP
#define QMC5883L_HPP

#include "I2cDevice.hpp"
#include <cstdint>

struct Qmc5883lSample {
    int16_t magneticX;
    int16_t magneticY;
    int16_t magneticZ;
};

class Qmc5883l {
public:
    explicit Qmc5883l(I2cDevice& device);
    const char* getName() const;
    bool probe();
    bool initialize();
    bool readSample(Qmc5883lSample& sample);
private:
    I2cDevice& device_;
};

#endif /* QMC5883L_HPP */
