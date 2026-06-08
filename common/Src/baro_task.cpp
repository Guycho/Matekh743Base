#include "baro_task.h"

#include <cstdio>

namespace {
constexpr uint32_t BARO_TASK_DELAY_MS = 100U;
}

void executeBaroTask(void* argument)
{
    BaroTaskContext* context = static_cast<BaroTaskContext*>(argument);
    for (;;) {
        if (context != nullptr && context->barometer != nullptr) {
            Dps310Sample sample = {};
            if (context->barometer->readPressureTemperature(sample)) {
                std::printf("BARO dps310 pressureRaw=%ld tempRaw=%ld\r\n",
                            static_cast<long>(sample.pressureRaw),
                            static_cast<long>(sample.temperatureRaw));
            }
        }
        osDelay(BARO_TASK_DELAY_MS);
    }
}
