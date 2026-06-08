#include "led_task.h"

#include "Led.hpp"
#include "main.h"

#include <cstdio>

namespace {
constexpr uint32_t kBlinkDelayMs = 500U;
constexpr bool kLedIsActiveLow = true;
}  // namespace

void executeLedTask(void* argument) {

    (void)argument;
    Led blueLed({LED_BLUE_GPIO_Port, LED_BLUE_Pin, kLedIsActiveLow});
    Led greenLed({LED_GREEN_GPIO_Port, LED_GREEN_Pin, kLedIsActiveLow});
    blueLed.turnOn();
    greenLed.turnOff();
    std::printf("LED blue=%s green=%s\r\n", blueLed.isOn() ? "on" : "off", greenLed.isOn() ? "on" : "off");
    for (;;) {
        blueLed.toggle();
        greenLed.toggle();
        std::printf("LED blue=%s green=%s\r\n", blueLed.isOn() ? "on" : "off", greenLed.isOn() ? "on" : "off");
        osDelay(kBlinkDelayMs);
    }
}
