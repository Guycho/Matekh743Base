#ifndef MPU6000_HPP
#define MPU6000_HPP

#include "SensorTypes.hpp"
#include "SpiDevice.hpp"

class Mpu6000 : public ImuDevice {
public:
    explicit Mpu6000(SpiDevice& device);
    const char* getName() const override;
    ImuKind getKind() const override;
    bool probe() override;
    bool initialize() override;
    bool readSample(ImuSample& sample) override;
private:
    SpiDevice& device_;
};

#endif /* MPU6000_HPP */
