#include "Magnetometers.hpp"

namespace {
int16_t makeInt16Be(uint8_t high, uint8_t low)
{
    return static_cast<int16_t>((static_cast<uint16_t>(high) << 8U) | low);
}

int16_t makeInt16Le(uint8_t low, uint8_t high)
{
    return makeInt16Be(high, low);
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

bool Qmc5883l::readSample(MagnetometerSample& sample)
{
    uint8_t data[6] = {};
    if (!device_.readRegister(0x00U, data, sizeof(data))) {
        return false;
    }
    sample.magnetic.x = makeInt16Le(data[0], data[1]);
    sample.magnetic.y = makeInt16Le(data[2], data[3]);
    sample.magnetic.z = makeInt16Le(data[4], data[5]);
    return true;
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

bool Hmc5883l::readSample(MagnetometerSample& sample)
{
    uint8_t data[6] = {};
    if (!device_.readRegister(0x03U, data, sizeof(data))) {
        return false;
    }
    sample.magnetic.x = makeInt16Be(data[0], data[1]);
    sample.magnetic.z = makeInt16Be(data[2], data[3]);
    sample.magnetic.y = makeInt16Be(data[4], data[5]);
    return true;
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

bool Lis3mdl::readSample(MagnetometerSample& sample)
{
    uint8_t data[6] = {};
    if (!device_.readRegister(0x28U | 0x80U, data, sizeof(data))) {
        return false;
    }
    sample.magnetic.x = makeInt16Le(data[0], data[1]);
    sample.magnetic.y = makeInt16Le(data[2], data[3]);
    sample.magnetic.z = makeInt16Le(data[4], data[5]);
    return true;
}
