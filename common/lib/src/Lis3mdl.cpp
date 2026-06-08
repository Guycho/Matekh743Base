#include "Lis3mdl.hpp"

namespace {
int16_t makeInt16Le(uint8_t low, uint8_t high)
{
    return static_cast<int16_t>((static_cast<uint16_t>(high) << 8U) | low);
}
}

Lis3mdl::Lis3mdl(I2cDevice& device)
    : device_(device)
{
}

const char* Lis3mdl::getName() const
{
    return "LIS3MDL";
}

bool Lis3mdl::probe()
{
    uint8_t whoAmI = 0U;
    return device_.readRegister(0x0FU, &whoAmI, 1) && whoAmI == 0x3DU;
}

bool Lis3mdl::initialize()
{
    if (!probe()) {
        return false;
    }
    if (!device_.writeByte(0x20U, 0x70U)) {
        return false;
    }
    if (!device_.writeByte(0x21U, 0x00U)) {
        return false;
    }
    if (!device_.writeByte(0x22U, 0x00U)) {
        return false;
    }
    return device_.writeByte(0x23U, 0x0CU);
}

bool Lis3mdl::readSample(Lis3mdlSample& sample)
{
    uint8_t data[6] = {};
    if (!device_.readRegister(0x28U | 0x80U, data, sizeof(data))) {
        return false;
    }
    sample.magneticX = makeInt16Le(data[0], data[1]);
    sample.magneticY = makeInt16Le(data[2], data[3]);
    sample.magneticZ = makeInt16Le(data[4], data[5]);
    return true;
}
