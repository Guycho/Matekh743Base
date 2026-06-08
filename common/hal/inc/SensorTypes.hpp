#ifndef SENSOR_TYPES_HPP
#define SENSOR_TYPES_HPP

#include <cstdint>

struct Vector3Int16 {
    int16_t x;
    int16_t y;
    int16_t z;
};

struct ImuSample {
    Vector3Int16 accel;
    Vector3Int16 gyro;
    int16_t temperature;
};

struct BarometerSample {
    int32_t pressureRaw;
    int32_t temperatureRaw;
};

struct MagnetometerSample {
    Vector3Int16 magnetic;
};

enum class ImuKind {
    Unknown,
    Icm42688p,
    Icm42605,
    Mpu6000,
    Icm20602,
};

class ImuDevice {
public:
    virtual ~ImuDevice() = default;
    virtual const char* getName() const = 0;
    virtual ImuKind getKind() const = 0;
    virtual bool probe() = 0;
    virtual bool initialize() = 0;
    virtual bool readSample(ImuSample& sample) = 0;
};

class MagnetometerDevice {
public:
    virtual ~MagnetometerDevice() = default;
    virtual const char* getName() const = 0;
    virtual bool probe() = 0;
    virtual bool initialize() = 0;
    virtual bool readSample(MagnetometerSample& sample) = 0;
};

#endif /* SENSOR_TYPES_HPP */
