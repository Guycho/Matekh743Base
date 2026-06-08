#ifndef MPU6000_HPP
#define MPU6000_HPP

#include "SpiDevice.hpp"
#include <cstdint>

struct Mpu6000Sample {
    int16_t accelX;
    int16_t accelY;
    int16_t accelZ;
    int16_t gyroX;
    int16_t gyroY;
    int16_t gyroZ;
    int16_t temperature;
};

class Mpu6000 {
public:
    explicit Mpu6000(SpiDevice& device);
    const char* getName() const;
    bool probe();
    bool initialize();
    bool readSample(Mpu6000Sample& sample);
private:
    SpiDevice& device_;
};

#endif /* MPU6000_HPP */
