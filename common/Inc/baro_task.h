#ifndef BARO_TASK_H
#define BARO_TASK_H

#include "Dps310.hpp"
#include "cmsis_os2.h"

struct BaroTaskContext {
    Dps310* barometer;
};

void executeBaroTask(void* argument);

constexpr osThreadAttr_t BARO_TASK_ATTR = {
    .name = "baroTask",
    .attr_bits = 0,
    .cb_mem = nullptr,
    .cb_size = 0,
    .stack_mem = nullptr,
    .stack_size = 768 * 4,
    .priority = osPriorityNormal,
    .tz_module = 0,
    .reserved = 0,
};

#endif /* BARO_TASK_H */
