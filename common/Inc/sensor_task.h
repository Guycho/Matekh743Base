#ifndef SENSOR_TASK_H
#define SENSOR_TASK_H

#include "AdcDmaReader.hpp"
#include "Dps310.hpp"
#include "Hmc5883l.hpp"
#include "Icm20602.hpp"
#include "Icm426xx.hpp"
#include "Lis3mdl.hpp"
#include "Mpu6000.hpp"
#include "Qmc5883l.hpp"
#include "cmsis_os2.h"
#include <cstddef>

struct SensorTaskContext {
    AdcDmaReader* adcReader;
    Icm426xx* icm426xxImus[3];
    size_t icm426xxCount;
    Mpu6000* mpu6000Imus[3];
    size_t mpu6000Count;
    Icm20602* icm20602Imus[3];
    size_t icm20602Count;
    Dps310* barometer;
    Qmc5883l* qmc5883l;
    Hmc5883l* hmc5883l;
    Lis3mdl* lis3mdl;
};

void executeSensorTask(void* argument);

constexpr osThreadAttr_t SENSOR_TASK_ATTR = {
    .name = "sensorTask",
    .attr_bits = 0,
    .cb_mem = nullptr,
    .cb_size = 0,
    .stack_mem = nullptr,
    .stack_size = 1024 * 4,
    .priority = osPriorityNormal,
    .tz_module = 0,
    .reserved = 0,
};

#endif /* SENSOR_TASK_H */
