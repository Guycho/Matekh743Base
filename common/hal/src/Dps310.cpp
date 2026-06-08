#include "Dps310.hpp"

namespace {
constexpr uint8_t PRS_B2_REG = 0x00U;
constexpr uint8_t PRS_CFG_REG = 0x06U;
constexpr uint8_t TMP_CFG_REG = 0x07U;
constexpr uint8_t MEAS_CFG_REG = 0x08U;
constexpr uint8_t PRODUCT_ID_REG = 0x0DU;
constexpr uint8_t DPS310_PRODUCT_ID = 0x10U;

int32_t makeSigned24(uint8_t high, uint8_t middle, uint8_t low)
{
    uint32_t value = (static_cast<uint32_t>(high) << 16U) | (static_cast<uint32_t>(middle) << 8U) | low;
    if ((value & 0x00800000UL) != 0U) {
        value |= 0xFF000000UL;
    }
    return static_cast<int32_t>(value);
}
}

Dps310::Dps310(I2cDevice& device)
    : device_(device)
{
}

bool Dps310::probe()
{
    uint8_t productId = 0U;
    if (!device_.readRegister(PRODUCT_ID_REG, &productId, 1)) {
        return false;
    }
    return (productId & 0xF0U) == DPS310_PRODUCT_ID;
}

bool Dps310::initialize()
{
    if (!probe()) {
        return false;
    }
    if (!device_.writeByte(PRS_CFG_REG, 0x04U)) {
        return false;
    }
    if (!device_.writeByte(TMP_CFG_REG, 0x84U)) {
        return false;
    }
    return device_.writeByte(MEAS_CFG_REG, 0x07U);
}

bool Dps310::readPressureTemperature(BarometerSample& sample)
{
    uint8_t data[6] = {};
    if (!device_.readRegister(PRS_B2_REG, data, sizeof(data))) {
        return false;
    }
    sample.pressureRaw = makeSigned24(data[0], data[1], data[2]);
    sample.temperatureRaw = makeSigned24(data[3], data[4], data[5]);
    return true;
}
