#include "AdcDmaReader.hpp"

AdcDmaReader::AdcDmaReader(const AdcDmaReaderConfig& config)
    : hadc_(config.hadc), scanLength_(config.scanLength), dmaBuffer_(config.dmaBuffer), vRef_(config.vRef)
{
}

bool AdcDmaReader::start()
{
    // In circular mode, HAL_ADC_Start_DMA is called once.
    return HAL_ADC_Start_DMA(hadc_, (uint32_t*)dmaBuffer_, scanLength_) == HAL_OK;
}

float AdcDmaReader::getVoltage(size_t channelIndex) const
{
    if (channelIndex >= scanLength_) {
        return 0.0f;
    }
    uint16_t raw = dmaBuffer_[channelIndex];
    // Convert raw value (16-bit ADC) to voltage
    // Assuming 0 to 65535 for 16-bit resolution
    return (static_cast<float>(raw) / 65535.0f) * vRef_;
}

uint16_t AdcDmaReader::getRaw(size_t channelIndex) const
{
    if (channelIndex >= scanLength_) {
        return 0;
    }
    return dmaBuffer_[channelIndex];
}