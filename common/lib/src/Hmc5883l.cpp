#include "Hmc5883l.hpp"

namespace {
int16_t makeInt16Be(uint8_t high, uint8_t low)
{
    return static_cast<int16_t>((static_cast<uint16_t>(high) << 8U) | low);
}
}

Hmc5883l::Hmc5883l(I2cDevice& device)
    : device_(device)
{
}

const char* Hmc5883l::getName() const
{
    return "HMC5883L";
}

bool Hmc5883l::probe()
{
    uint8_t id[3] = {};
    return device_.readRegister(0x0AU, id, sizeof(id)) && id[0] == 'H' && id[1] == '4' && id[2] == '3';
}

bool Hmc5883l::initialize()
{
    if (!probe()) {
        return false;
    }
    if (!device_.writeByte(0x00U, 0x70U)) {
        return false;
    }
    if (!device_.writeByte(0x01U, 0x20U)) {
        return false;
    }
    return device_.writeByte(0x02U, 0x00U);
}

bool Hmc5883l::readSample(Hmc5883lSample& sample)
{
    uint8_t data[6] = {};
    if (!device_.readRegister(0x03U, data, sizeof(data))) {
        return false;
    }
    sample.magneticX = makeInt16Be(data[0], data[1]);
    sample.magneticZ = makeInt16Be(data[2], data[3]);
    sample.magneticY = makeInt16Be(data[4], data[5]);
    return true;
}
