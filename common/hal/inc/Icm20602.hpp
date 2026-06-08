#ifndef ICM20602_HPP
#define ICM20602_HPP

#include "SensorTypes.hpp"
#include "SpiDevice.hpp"

class Icm20602 : public ImuDevice {
public:
    explicit Icm20602(SpiDevice& device);
    const char* getName() const override;
    ImuKind getKind() const override;
    bool probe() override;
    bool initialize() override;
    bool readSample(ImuSample& sample) override;
private:
    SpiDevice& device_;
};

#endif /* ICM20602_HPP */
