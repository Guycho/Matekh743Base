#ifndef SENSOR_TASK_H
#define SENSOR_TASK_H

#include "BoardSensors.hpp"
#include "Dps310.hpp"
#include "Magnetometers.hpp"
#include "SensorTypes.hpp"
#include "cmsis_os2.h"
#include <cstddef>

struct SensorTaskContext {
    BoardSensors* boardSensors;
    ImuDevice* imus[3];
    size_t imuCount;
    Dps310* barometer;
    MagnetometerDevice* magnetometer;
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
