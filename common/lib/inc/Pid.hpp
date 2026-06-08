#ifndef PID_H
#define PID_H

/**
 * @brief Discrete positional PID with bounded integral accumulator and saturated output.
 *
 * Integral uses ∑(e·Δt). If @ref setIntegralAccumulatorLimit is set to a positive value, the
 * accumulator is clamped to ± that value before the I term (Ki·∑) is applied (basic anti-windup).
 */
class Pid {
public:
    Pid();

    void setGains(float kp, float ki, float kd);
    void setOutputLimits(float min, float max);
    /** @brief Maximum absolute value of ∫e dt; 0 disables accumulator clamping. */
    void setIntegralAccumulatorLimit(float maxAbs);

    void reset();

    /**
     * @brief One control step.
     * @param setpoint Process setpoint (same units as @p measurement).
     * @param measurement Feedback value.
     * @param deltaTimeSec Sample period in seconds (> 0).
     * @retval float Saturated control output.
     */
    float step(float setpoint, float measurement, float deltaTimeSec);

    /** @brief Last saturated output from @ref step (0 before first successful step). */
    float lastOutput() const { return lastOutput_; }

private:
    float kp_;
    float ki_;
    float kd_;
    float outMin_;
    float outMax_;
    float intAccumLimit_;
    float integral_;
    float prevError_;
    bool hasPrevError_;
    float lastOutput_;
};

#endif /* PID_H */
