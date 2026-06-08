#include "mag_task.h"

#include <cstdio>

namespace {
constexpr uint32_t MAG_TASK_DELAY_MS = 200U;
}

void executeMagTask(void* argument)
{
    MagTaskContext* context = static_cast<MagTaskContext*>(argument);
    for (;;) {
        if (context != nullptr) {
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
        osDelay(MAG_TASK_DELAY_MS);
    }
}
