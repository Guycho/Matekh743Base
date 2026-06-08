#ifndef ICM20602_HPP
#define ICM20602_HPP

#include "SpiDevice.hpp"
#include <cstdint>

struct Icm20602Sample {
    int16_t accelX;
    int16_t accelY;
    int16_t accelZ;
    int16_t gyroX;
    int16_t gyroY;
    int16_t gyroZ;
    int16_t temperature;
};

class Icm20602 {
public:
    explicit Icm20602(SpiDevice& device);
    const char* getName() const;
    bool probe();
    bool initialize();
    bool readSample(Icm20602Sample& sample);
private:
    SpiDevice& device_;
};

#endif /* ICM20602_HPP */
