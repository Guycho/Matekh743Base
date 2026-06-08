#ifndef PWM_OUTPUT_HPP
#define PWM_OUTPUT_HPP

#include "../../../mcu/Core/Inc/main.h"
#include <cstdint>

#include "HardwareConfig.hpp"

/**
 * @brief Simple PWM output channel wrapper.
 *
 * Wraps a single TIM channel. Call start() once, then setDuty() or setRaw()
 * to change the pulse width at any time (lock-free register write).
 */
class PwmOutput {
public:
    PwmOutput(const PwmOutputConfig& config);

    bool start();
    void stop();

    void setDuty(float duty);
    void setRaw(uint32_t ccr);

    uint32_t period() const;
    uint32_t channel() const { return channel_; }

private:
    TIM_HandleTypeDef *htim_;
    uint32_t channel_;
};

#endif /* PWM_OUTPUT_HPP */