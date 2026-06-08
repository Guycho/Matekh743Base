#ifndef APP_HPP
#define APP_HPP

#include "main.h"
#include "../hal/inc/AdcDmaReader.hpp"
#include "../hal/inc/PwmOutput.hpp"
#include "../hal/inc/DigitalOutput.hpp"

/**
 * @brief Main Application Orchestrator.
 * 
 * Handles the initialization of hardware wrappers and the spawning 
 * of high-level system tasks.
 */
class App {
public:
    App();

    /**
     * @brief Initializes hardware wrappers and shared data.
     * @return true if initialization was successful.
     */
    bool initialize();

    /**
     * @brief Spawns the system tasks and starts the application.
     */
    void run();

private:
    // Hardware Wrappers
    AdcDmaReader* adcReader_ = nullptr;
    PwmOutput* pwmS3_ = nullptr;
    PwmOutput* pwmS4_ = nullptr;
    
    // Digital Outputs
    DigitalOutput* digS5_ = nullptr;
    DigitalOutput* digS6_ = nullptr;
    DigitalOutput* digS7_ = nullptr;
    DigitalOutput* digS8_ = nullptr;

    // Configuration constants
    static constexpr size_t kAdcScanLength = 6;
    static constexpr float kAdcFullScaleVolts = 3.3f;
};

#endif /* APP_HPP */