#include "imu_task.h"

#include <cstdio>

namespace {
constexpr uint32_t IMU_TASK_DELAY_MS = 10U;

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

void executeImuTask(void* argument)
{
    ImuTaskContext* context = static_cast<ImuTaskContext*>(argument);
    for (;;) {
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
        }
        osDelay(IMU_TASK_DELAY_MS);
    }
}
