#ifndef IMU_TASK_H
#define IMU_TASK_H

#include "Icm20602.hpp"
#include "Icm426xx.hpp"
#include "Mpu6000.hpp"
#include "cmsis_os2.h"
#include <cstddef>

struct ImuTaskContext {
    Icm426xx* icm426xxImus[3];
    size_t icm426xxCount;
    Mpu6000* mpu6000Imus[3];
    size_t mpu6000Count;
    Icm20602* icm20602Imus[3];
    size_t icm20602Count;
};

void executeImuTask(void* argument);

constexpr osThreadAttr_t IMU_TASK_ATTR = {
    .name = "imuTask",
    .attr_bits = 0,
    .cb_mem = nullptr,
    .cb_size = 0,
    .stack_mem = nullptr,
    .stack_size = 1024 * 4,
    .priority = osPriorityAboveNormal,
    .tz_module = 0,
    .reserved = 0,
};

#endif /* IMU_TASK_H */
