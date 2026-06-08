#ifndef LED_TASK_H
#define LED_TASK_H

#include "cmsis_os2.h"

/**
 * @brief GPIO LED task entry point.
 */
void executeLedTask(void* argument);

constexpr osThreadAttr_t LED_TASK_ATTR = {
    .name = "ledTask",
    .attr_bits = 0,
    .cb_mem = nullptr,
    .cb_size = 0,
    .stack_mem = nullptr,
    .stack_size = 512 * 4,
    .priority = osPriorityNormal,
    .tz_module = 0,
    .reserved = 0,
};

#endif /* LED_TASK_H */
