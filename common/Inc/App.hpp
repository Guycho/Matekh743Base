#ifndef APP_HPP
#define APP_HPP

#include "main.h"
#include "Dps310.hpp"
#include "Hmc5883l.hpp"
#include "Icm20602.hpp"
#include "Icm426xx.hpp"
#include "I2cDevice.hpp"
#include "Lis3mdl.hpp"
#include "Mpu6000.hpp"
#include "Qmc5883l.hpp"
#include "SpiDevice.hpp"
#include "../hal/inc/AdcDmaReader.hpp"
#include "../hal/inc/PwmOutput.hpp"
#include "../hal/inc/DigitalOutput.hpp"
#include "baro_task.h"
#include "imu_task.h"
#include "mag_task.h"

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
    void detectImu(SpiDevice& spiDevice);
    Dps310* detectBarometer();
    void detectMagnetometer();
    void configureSensorContext();

    // Hardware Wrappers
    AdcDmaReader* adcReader_ = nullptr;
    PwmOutput* pwmS3_ = nullptr;
    PwmOutput* pwmS4_ = nullptr;
    SpiDevice* imu1Spi_ = nullptr;
    SpiDevice* imu2Spi_ = nullptr;
    SpiDevice* imu3Spi_ = nullptr;
    I2cDevice* dps310I2c_ = nullptr;
    I2cDevice* qmc5883lI2c_ = nullptr;
    I2cDevice* hmcLis3mdlI2c_ = nullptr;
    ImuTaskContext imuTaskContext_ = {};
    BaroTaskContext baroTaskContext_ = {};
    MagTaskContext magTaskContext_ = {};
    
    // Digital Outputs
    DigitalOutput* digS5_ = nullptr;
    DigitalOutput* digS6_ = nullptr;
    DigitalOutput* digS7_ = nullptr;
    DigitalOutput* digS8_ = nullptr;

    // Configuration constants
    static constexpr size_t kAdcScanLength = board_config::ADC_SCAN_LENGTH;
    static constexpr float kAdcFullScaleVolts = board_config::ADC_REFERENCE_VOLTS;
};

#endif /* APP_HPP */