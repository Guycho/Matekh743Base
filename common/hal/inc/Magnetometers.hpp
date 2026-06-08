#ifndef MAGNETOMETERS_HPP
#define MAGNETOMETERS_HPP

#include "I2cDevice.hpp"
#include "SensorTypes.hpp"

class Qmc5883l : public MagnetometerDevice {
public:
    explicit Qmc5883l(I2cDevice& device);
    const char* getName() const override;
    bool probe() override;
    bool initialize() override;
    bool readSample(MagnetometerSample& sample) override;
private:
    I2cDevice& device_;
};

class Hmc5883l : public MagnetometerDevice {
public:
    explicit Hmc5883l(I2cDevice& device);
    const char* getName() const override;
    bool probe() override;
    bool initialize() override;
    bool readSample(MagnetometerSample& sample) override;
private:
    I2cDevice& device_;
};

class Lis3mdl : public MagnetometerDevice {
public:
    explicit Lis3mdl(I2cDevice& device);
    const char* getName() const override;
    bool probe() override;
    bool initialize() override;
    bool readSample(MagnetometerSample& sample) override;
private:
    I2cDevice& device_;
};

#endif /* MAGNETOMETERS_HPP */
