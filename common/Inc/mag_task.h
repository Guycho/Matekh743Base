#ifndef MAG_TASK_H
#define MAG_TASK_H

#include "Hmc5883l.hpp"
#include "Lis3mdl.hpp"
#include "Qmc5883l.hpp"
#include "cmsis_os2.h"

struct MagTaskContext {
    Qmc5883l* qmc5883l;
    Hmc5883l* hmc5883l;
    Lis3mdl* lis3mdl;
};

void executeMagTask(void* argument);

constexpr osThreadAttr_t MAG_TASK_ATTR = {
    .name = "magTask",
    .attr_bits = 0,
    .cb_mem = nullptr,
    .cb_size = 0,
    .stack_mem = nullptr,
    .stack_size = 768 * 4,
    .priority = osPriorityNormal,
    .tz_module = 0,
    .reserved = 0,
};

#endif /* MAG_TASK_H */
