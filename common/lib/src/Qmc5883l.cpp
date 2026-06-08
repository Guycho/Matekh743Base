#include "Qmc5883l.hpp"

namespace {
int16_t makeInt16Le(uint8_t low, uint8_t high)
{
    return static_cast<int16_t>((static_cast<uint16_t>(high) << 8U) | low);
}
}

Qmc5883l::Qmc5883l(I2cDevice& device)
    : device_(device)
{
}

const char* Qmc5883l::getName() const
{
    return "QMC5883L";
}

bool Qmc5883l::probe()
{
    uint8_t chipId = 0U;
    return device_.readRegister(0x0DU, &chipId, 1) && chipId == 0xFFU;
}

bool Qmc5883l::initialize()
{
    if (!probe()) {
        return false;
    }
    if (!device_.writeByte(0x0BU, 0x01U)) {
        return false;
    }
    return device_.writeByte(0x09U, 0x1DU);
}

bool Qmc5883l::readSample(Qmc5883lSample& sample)
{
    uint8_t data[6] = {};
    if (!device_.readRegister(0x00U, data, sizeof(data))) {
        return false;
    }
    sample.magneticX = makeInt16Le(data[0], data[1]);
    sample.magneticY = makeInt16Le(data[2], data[3]);
    sample.magneticZ = makeInt16Le(data[4], data[5]);
    return true;
}
