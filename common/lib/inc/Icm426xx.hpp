#ifndef ICM426XX_HPP
#define ICM426XX_HPP

#include "SpiDevice.hpp"
#include <cstdint>

enum class Icm426xxKind {
    Unknown,
    Icm42688p,
    Icm42605,
};

struct Icm426xxSample {
    int16_t accelX;
    int16_t accelY;
    int16_t accelZ;
    int16_t gyroX;
    int16_t gyroY;
    int16_t gyroZ;
    int16_t temperature;
};

class Icm426xx {
public:
    explicit Icm426xx(SpiDevice& device);
    const char* getName() const;
    Icm426xxKind getKind() const;
    bool probe();
    bool initialize();
    bool readSample(Icm426xxSample& sample);
private:
    SpiDevice& device_;
    Icm426xxKind kind_;
    uint8_t whoAmI_;
};

#endif /* ICM426XX_HPP */
