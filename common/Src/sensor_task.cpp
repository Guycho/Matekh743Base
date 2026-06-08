#include "sensor_task.h"

#include <cstdio>

namespace {
constexpr uint32_t SENSOR_TASK_DELAY_MS = 100U;
constexpr uint32_t SLOW_LOG_DIVIDER = 10U;

int32_t toMilli(float value)
{
    return static_cast<int32_t>(value * 1000.0f);
}
}

void executeSensorTask(void* argument)
{
    SensorTaskContext* context = static_cast<SensorTaskContext*>(argument);
    uint32_t loopCount = 0U;
    for (;;) {
        if (context != nullptr && context->boardSensors != nullptr && (loopCount % SLOW_LOG_DIVIDER) == 0U) {
            const AnalogSensorReadings analog = context->boardSensors->readAnalogSensors();
            std::printf("ADC vbat=%ldmV vb2=%ldmV curr=%ldmV cu2=%ldmV air=%ldmV rssi=%ldmV\r\n",
                        static_cast<long>(toMilli(analog.mainVoltage)),
                        static_cast<long>(toMilli(analog.auxVoltage)),
                        static_cast<long>(toMilli(analog.mainCurrentVoltage)),
                        static_cast<long>(toMilli(analog.auxCurrentVoltage)),
                        static_cast<long>(toMilli(analog.airspeedVoltage)),
                        static_cast<long>(toMilli(analog.rssiVoltage)));
        }
        if (context != nullptr) {
            for (size_t i = 0; i < context->imuCount; ++i) {
                ImuSample sample = {};
                if (context->imus[i] != nullptr && context->imus[i]->readSample(sample)) {
                    std::printf("IMU %s ax=%d ay=%d az=%d gx=%d gy=%d gz=%d\r\n",
                                context->imus[i]->getName(),
                                sample.accel.x,
                                sample.accel.y,
                                sample.accel.z,
                                sample.gyro.x,
                                sample.gyro.y,
                                sample.gyro.z);
                }
            }
            if (context->barometer != nullptr && (loopCount % SLOW_LOG_DIVIDER) == 0U) {
                BarometerSample barometerSample = {};
                if (context->barometer->readPressureTemperature(barometerSample)) {
                    std::printf("BARO dps310 pressureRaw=%ld tempRaw=%ld\r\n",
                                static_cast<long>(barometerSample.pressureRaw),
                                static_cast<long>(barometerSample.temperatureRaw));
                }
            }
            if (context->magnetometer != nullptr && (loopCount % SLOW_LOG_DIVIDER) == 0U) {
                MagnetometerSample magnetometerSample = {};
                if (context->magnetometer->readSample(magnetometerSample)) {
                    std::printf("MAG %s x=%d y=%d z=%d\r\n",
                                context->magnetometer->getName(),
                                magnetometerSample.magnetic.x,
                                magnetometerSample.magnetic.y,
                                magnetometerSample.magnetic.z);
                }
            }
        }
        ++loopCount;
        osDelay(SENSOR_TASK_DELAY_MS);
    }
}
