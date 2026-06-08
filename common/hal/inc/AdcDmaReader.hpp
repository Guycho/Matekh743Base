#ifndef ADC_DMA_READER_HPP
#define ADC_DMA_READER_HPP

#include "../../../mcu/Core/Inc/main.h"
#include <cstdint>
#include <vector>

#include "HardwareConfig.hpp"

/**
 * @brief Multi-channel ADC reader using DMA in circular mode.
 */
class AdcDmaReader {
public:
    static constexpr size_t MAX_CHANNELS = 32;

    AdcDmaReader(const AdcDmaReaderConfig& config);

    /**
     * @brief Start the ADC DMA conversion.
     * @return true if started successfully.
     */
    bool start();

    /**
     * @brief Get the voltage for a specific channel index.
     * @param channelIndex The index in the DMA buffer (0 to scanLength-1).
     * @return The voltage in Volts.
     */
    float getVoltage(size_t channelIndex) const;

    /**
     * @brief Get the raw value for a specific channel index.
     * @param channelIndex The index in the DMA buffer.
     * @return The raw ADC count.
     * @ uma a raw value for a specific channel index.
     */
    uint16_t getRaw(size_t channelIndex) const;

private:
    ADC_HandleTypeDef *hadc_;
    size_t scanLength_;
    uint16_t *dmaBuffer_;
    float vRef_;
};

#endif /* ADC_DMA_READER_HPP */