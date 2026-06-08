#include "sensor_task.h"

#include <cstdio>

namespace {
constexpr uint32_t SENSOR_TASK_DELAY_MS = 100U;
constexpr uint32_t SLOW_LOG_DIVIDER = 10U;

int32_t toMilli(float value)
{
    return static_cast<int32_t>(value * 1000.0f);
}

float readAdcVoltage(AdcDmaReader& adcReader, AdcChannel channel)
{
    return adcReader.getVoltage(static_cast<size_t>(channel));
}

void logIcm426xx(Icm426xx& imu)
{
    Icm426xxSample sample = {};
    if (imu.readSample(sample)) {
        std::printf("IMU %s ax=%d ay=%d az=%d gx=%d gy=%d gz=%d\r\n",
                    imu.getName(),
                    sample.accelX,
                    sample.accelY,
                    sample.accelZ,
                    sample.gyroX,
                    sample.gyroY,
                    sample.gyroZ);
    }
}

void logMpu6000(Mpu6000& imu)
{
    Mpu6000Sample sample = {};
    if (imu.readSample(sample)) {
        std::printf("IMU %s ax=%d ay=%d az=%d gx=%d gy=%d gz=%d\r\n",
                    imu.getName(),
                    sample.accelX,
                    sample.accelY,
                    sample.accelZ,
                    sample.gyroX,
                    sample.gyroY,
                    sample.gyroZ);
    }
}

void logIcm20602(Icm20602& imu)
{
    Icm20602Sample sample = {};
    if (imu.readSample(sample)) {
        std::printf("IMU %s ax=%d ay=%d az=%d gx=%d gy=%d gz=%d\r\n",
                    imu.getName(),
                    sample.accelX,
                    sample.accelY,
                    sample.accelZ,
                    sample.gyroX,
                    sample.gyroY,
                    sample.gyroZ);
    }
}
}

void executeSensorTask(void* argument)
{
    SensorTaskContext* context = static_cast<SensorTaskContext*>(argument);
    uint32_t loopCount = 0U;
    for (;;) {
        if (context != nullptr && context->adcReader != nullptr && (loopCount % SLOW_LOG_DIVIDER) == 0U) {
            const float mainVoltage = readAdcVoltage(*context->adcReader, AdcChannel::MainVoltage) * board_config::MAIN_VOLTAGE_MULTIPLIER;
            const float auxVoltage = readAdcVoltage(*context->adcReader, AdcChannel::AuxVoltage) * board_config::AUX_VOLTAGE_MULTIPLIER;
            const float mainCurrentVoltage = readAdcVoltage(*context->adcReader, AdcChannel::MainCurrent);
            const float auxCurrentVoltage = readAdcVoltage(*context->adcReader, AdcChannel::AuxCurrent);
            const float airspeedVoltage = readAdcVoltage(*context->adcReader, AdcChannel::Airspeed) * board_config::AIRSPEED_VOLTAGE_MULTIPLIER;
            const float rssiVoltage = readAdcVoltage(*context->adcReader, AdcChannel::Rssi);
            std::printf("ADC vbat=%ldmV vb2=%ldmV curr=%ldmV cu2=%ldmV air=%ldmV rssi=%ldmV\r\n",
                        static_cast<long>(toMilli(mainVoltage)),
                        static_cast<long>(toMilli(auxVoltage)),
                        static_cast<long>(toMilli(mainCurrentVoltage)),
                        static_cast<long>(toMilli(auxCurrentVoltage)),
                        static_cast<long>(toMilli(airspeedVoltage)),
                        static_cast<long>(toMilli(rssiVoltage)));
        }
        if (context != nullptr) {
            for (size_t i = 0; i < context->icm426xxCount; ++i) {
                if (context->icm426xxImus[i] != nullptr) {
                    logIcm426xx(*context->icm426xxImus[i]);
                }
            }
            for (size_t i = 0; i < context->mpu6000Count; ++i) {
                if (context->mpu6000Imus[i] != nullptr) {
                    logMpu6000(*context->mpu6000Imus[i]);
                }
            }
            for (size_t i = 0; i < context->icm20602Count; ++i) {
                if (context->icm20602Imus[i] != nullptr) {
                    logIcm20602(*context->icm20602Imus[i]);
                }
            }
            if (context->barometer != nullptr && (loopCount % SLOW_LOG_DIVIDER) == 0U) {
                Dps310Sample barometerSample = {};
                if (context->barometer->readPressureTemperature(barometerSample)) {
                    std::printf("BARO dps310 pressureRaw=%ld tempRaw=%ld\r\n",
                                static_cast<long>(barometerSample.pressureRaw),
                                static_cast<long>(barometerSample.temperatureRaw));
                }
            }
            if ((loopCount % SLOW_LOG_DIVIDER) == 0U) {
                Qmc5883lSample qmcSample = {};
                if (context->qmc5883l != nullptr && context->qmc5883l->readSample(qmcSample)) {
                    std::printf("MAG %s x=%d y=%d z=%d\r\n",
                                context->qmc5883l->getName(),
                                qmcSample.magneticX,
                                qmcSample.magneticY,
                                qmcSample.magneticZ);
                }
                Hmc5883lSample hmcSample = {};
                if (context->hmc5883l != nullptr && context->hmc5883l->readSample(hmcSample)) {
                    std::printf("MAG %s x=%d y=%d z=%d\r\n",
                                context->hmc5883l->getName(),
                                hmcSample.magneticX,
                                hmcSample.magneticY,
                                hmcSample.magneticZ);
                }
                Lis3mdlSample lisSample = {};
                if (context->lis3mdl != nullptr && context->lis3mdl->readSample(lisSample)) {
                    std::printf("MAG %s x=%d y=%d z=%d\r\n",
                                context->lis3mdl->getName(),
                                lisSample.magneticX,
                                lisSample.magneticY,
                                lisSample.magneticZ);
                }
            }
        }
        ++loopCount;
        osDelay(SENSOR_TASK_DELAY_MS);
    }
}
