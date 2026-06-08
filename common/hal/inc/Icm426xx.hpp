#ifndef ICM426XX_HPP
#define ICM426XX_HPP

#include "SensorTypes.hpp"
#include "SpiDevice.hpp"

class Icm426xx : public ImuDevice {
public:
    explicit Icm426xx(SpiDevice& device);
    const char* getName() const override;
    ImuKind getKind() const override;
    bool probe() override;
    bool initialize() override;
    bool readSample(ImuSample& sample) override;
private:
    SpiDevice& device_;
    ImuKind kind_;
    uint8_t whoAmI_;
};

#endif /* ICM426XX_HPP */
