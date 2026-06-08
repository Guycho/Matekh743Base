#include "Pid.hpp"

namespace {
float clampFloat(float value, float minValue, float maxValue)
{
    if (value < minValue) {
        return minValue;
    }
    if (value > maxValue) {
        return maxValue;
    }
    return value;
}
} // namespace

Pid::Pid()
    : kp_(0.0f),
      ki_(0.0f),
      kd_(0.0f),
      outMin_(-1.0f),
      outMax_(1.0f),
      intAccumLimit_(0.0f),
      integral_(0.0f),
      prevError_(0.0f),
      hasPrevError_(false),
      lastOutput_(0.0f)
{
}

void Pid::setGains(float kp, float ki, float kd)
{
    kp_ = kp;
    ki_ = ki;
    kd_ = kd;
}

void Pid::setOutputLimits(float min, float max)
{
    outMin_ = min;
    outMax_ = max;
}

void Pid::setIntegralAccumulatorLimit(float maxAbs)
{
    intAccumLimit_ = maxAbs;
}

void Pid::reset()
{
    integral_ = 0.0f;
    prevError_ = 0.0f;
    hasPrevError_ = false;
    lastOutput_ = 0.0f;
}

float Pid::step(float setpoint, float measurement, float deltaTimeSec)
{
    if (deltaTimeSec <= 0.0f) {
        return lastOutput_;
    }
    const float error = setpoint - measurement;
    const float pTerm = kp_ * error;
    integral_ += error * deltaTimeSec;
    if (intAccumLimit_ > 0.0f) {
        integral_ = clampFloat(integral_, -intAccumLimit_, intAccumLimit_);
    }
    const float iTerm = ki_ * integral_;
    float dTerm = 0.0f;
    if (hasPrevError_) {
        dTerm = kd_ * (error - prevError_) / deltaTimeSec;
    }
    prevError_ = error;
    hasPrevError_ = true;
    float output = pTerm + iTerm + dTerm;
    output = clampFloat(output, outMin_, outMax_);
    lastOutput_ = output;
    return output;
}
