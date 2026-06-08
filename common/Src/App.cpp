#include "../Inc/App.hpp"
#include "led_task.h"
#include <new>
#include <vector>

extern "C" {
    extern ADC_HandleTypeDef hadc1;
    extern TIM_HandleTypeDef htim2;
    extern TIM_HandleTypeDef htim3;
}

namespace {
    // Buffer for ADC DMA - Allocated in a specific section to be compatible with D1 RAM
    // Matches the configuration in main.c
    __attribute__((section(".RAM_D1"), aligned(32)))
    static uint16_t gAdcBuffer[32] = {0};
}

App::App() : adcReader_(nullptr), pwmS3_(nullptr), pwmS4_(nullptr) 
{
}

bool App::initialize() 
{
    // Initialize ADC Wrapper
    adcReader_ = new AdcDmaReader({&hadc1, kAdcScanLength, gAdcBuffer, kAdcFullScaleVolts});
    
    // Initialize PWM Wrappers
    // S3 is TIM2 CH1, S4 is TIM2 CH2
    pwmS3_ = new PwmOutput({&htim2, TIM_CHANNEL_1});
    pwmS4_ = new PwmOutput({&htim2, TIM_CHANNEL_2});

    // Initialize Digital Outputs
    digS5_ = new DigitalOutput({GPIOA, S5_Pin});
    digS6_ = new DigitalOutput({GPIOA, S6_Pin});
    digS7_ = new DigitalOutput({GPIOD, S7_Pin});
    digS8_ = new DigitalOutput({GPIOD, S8_Pin});

    if (!adcReader_->start()) {
        return false;
    }

    return true;
}

void App::run() 
{
    osThreadNew(executeLedTask, nullptr, &LED_TASK_ATTR);
    osThreadTerminate(osThreadGetId());
}

// Helper functions to bridge C and C++
extern "C" void app_main_task(void *argument) {
    (void)argument;
    static App app;
    if (!app.initialize()) {
        // Error handling
        while(1);
    }
    app.run();
}
