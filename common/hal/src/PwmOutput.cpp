#include "../inc/PwmOutput.hpp"

PwmOutput::PwmOutput(const PwmOutputConfig& config)
    : htim_(config.htim), channel_(config.channel)
{
}

bool PwmOutput::start()
{
    return HAL_TIM_PWM_Start(htim_, channel_) == HAL_OK;
}

void PwmOutput::stop()
{
    HAL_TIM_PWM_Stop(htim_, channel_);
}

void PwmOutput::setDuty(float duty)
{
    if (duty < 0.0f) duty = 0.0f;
    if (duty > 1.0f) duty = 1.0f;
    setRaw(static_cast<uint32_t>(duty * static_cast<float>(period())));
}

void PwmOutput::setRaw(uint32_t value)
{
    __HAL_TIM_SET_COMPARE(htim_, channel_, value);
}

uint32_t PwmOutput::period() const
{
    return __HAL_TIM_GET_AUTORELOAD(htim_);
}