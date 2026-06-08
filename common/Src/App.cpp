#include "../Inc/App.hpp"
#include "baro_task.h"
#include "imu_task.h"
#include "led_task.h"
#include "mag_task.h"
#include <cstdio>
#include <new>
#include <vector>

extern "C" {
    extern ADC_HandleTypeDef hadc1;
    extern I2C_HandleTypeDef hi2c1;
    extern I2C_HandleTypeDef hi2c2;
    extern SPI_HandleTypeDef hspi1;
    extern SPI_HandleTypeDef hspi4;
    extern TIM_HandleTypeDef htim2;
    extern TIM_HandleTypeDef htim3;
}

namespace {
    // Buffer for ADC DMA - Allocated in a specific section to be compatible with D1 RAM
    // Matches the configuration in main.c
    __attribute__((section(".RAM_D1"), aligned(32)))
    static uint16_t gAdcBuffer[32] = {0};

    constexpr uint16_t makeI2cAddress(uint8_t address)
    {
        return static_cast<uint16_t>(address << 1U);
    }
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

    imu1Spi_ = new SpiDevice({&hspi1, IMU1_CS_GPIO_Port, IMU1_CS_Pin});
    imu2Spi_ = new SpiDevice({&hspi1, IMU2_CS_GPIO_Port, IMU2_CS_Pin});
    imu3Spi_ = new SpiDevice({&hspi4, IMU3_CS_GPIO_Port, IMU3_CS_Pin});

    if (!adcReader_->start()) {
        return false;
    }

    configureSensorContext();
    return true;
}

void App::run() 
{
    osThreadNew(executeLedTask, nullptr, &LED_TASK_ATTR);
    const size_t imuCount = imuTaskContext_.icm426xxCount + imuTaskContext_.mpu6000Count + imuTaskContext_.icm20602Count;
    if (imuCount > 0U) {
        osThreadNew(executeImuTask, &imuTaskContext_, &IMU_TASK_ATTR);
    }
    if (baroTaskContext_.barometer != nullptr) {
        osThreadNew(executeBaroTask, &baroTaskContext_, &BARO_TASK_ATTR);
    }
    if (magTaskContext_.qmc5883l != nullptr || magTaskContext_.hmc5883l != nullptr || magTaskContext_.lis3mdl != nullptr) {
        osThreadNew(executeMagTask, &magTaskContext_, &MAG_TASK_ATTR);
    }
    osThreadTerminate(osThreadGetId());
}

void App::detectImu(SpiDevice& spiDevice)
{
    Icm426xx* icm426xx = new Icm426xx(spiDevice);
    if (icm426xx->probe() && icm426xx->initialize()) {
        if (imuTaskContext_.icm426xxCount < 3U) {
            imuTaskContext_.icm426xxImus[imuTaskContext_.icm426xxCount] = icm426xx;
            ++imuTaskContext_.icm426xxCount;
            std::printf("Detected IMU: %s\r\n", icm426xx->getName());
            return;
        }
    }
    delete icm426xx;
    Mpu6000* mpu6000 = new Mpu6000(spiDevice);
    if (mpu6000->probe() && mpu6000->initialize()) {
        if (imuTaskContext_.mpu6000Count < 3U) {
            imuTaskContext_.mpu6000Imus[imuTaskContext_.mpu6000Count] = mpu6000;
            ++imuTaskContext_.mpu6000Count;
            std::printf("Detected IMU: %s\r\n", mpu6000->getName());
            return;
        }
    }
    delete mpu6000;
    Icm20602* icm20602 = new Icm20602(spiDevice);
    if (icm20602->probe() && icm20602->initialize()) {
        if (imuTaskContext_.icm20602Count < 3U) {
            imuTaskContext_.icm20602Imus[imuTaskContext_.icm20602Count] = icm20602;
            ++imuTaskContext_.icm20602Count;
            std::printf("Detected IMU: %s\r\n", icm20602->getName());
            return;
        }
    }
    delete icm20602;
}

Dps310* App::detectBarometer()
{
    dps310I2c_ = new I2cDevice({&hi2c2, makeI2cAddress(0x76U)});
    Dps310* barometer = new Dps310(*dps310I2c_);
    if (barometer->probe() && barometer->initialize()) {
        return barometer;
    }
    delete barometer;
    delete dps310I2c_;
    dps310I2c_ = new I2cDevice({&hi2c2, makeI2cAddress(0x77U)});
    barometer = new Dps310(*dps310I2c_);
    if (barometer->probe() && barometer->initialize()) {
        return barometer;
    }
    delete barometer;
    delete dps310I2c_;
    dps310I2c_ = nullptr;
    return nullptr;
}

void App::detectMagnetometer()
{
    qmc5883lI2c_ = new I2cDevice({&hi2c1, makeI2cAddress(0x0DU)});
    Qmc5883l* qmc5883l = new Qmc5883l(*qmc5883lI2c_);
    if (qmc5883l->probe() && qmc5883l->initialize()) {
        magTaskContext_.qmc5883l = qmc5883l;
        std::printf("Detected MAG: %s\r\n", qmc5883l->getName());
        return;
    }
    delete qmc5883l;
    delete qmc5883lI2c_;
    qmc5883lI2c_ = nullptr;
    hmcLis3mdlI2c_ = new I2cDevice({&hi2c1, makeI2cAddress(0x1EU)});
    Hmc5883l* hmc5883l = new Hmc5883l(*hmcLis3mdlI2c_);
    if (hmc5883l->probe() && hmc5883l->initialize()) {
        magTaskContext_.hmc5883l = hmc5883l;
        std::printf("Detected MAG: %s\r\n", hmc5883l->getName());
        return;
    }
    delete hmc5883l;
    Lis3mdl* lis3mdl = new Lis3mdl(*hmcLis3mdlI2c_);
    if (lis3mdl->probe() && lis3mdl->initialize()) {
        magTaskContext_.lis3mdl = lis3mdl;
        std::printf("Detected MAG: %s\r\n", lis3mdl->getName());
        return;
    }
    delete lis3mdl;
    delete hmcLis3mdlI2c_;
    hmcLis3mdlI2c_ = nullptr;
    std::printf("No external I2C magnetometer detected\r\n");
}

void App::configureSensorContext()
{
    imuTaskContext_.icm426xxCount = 0U;
    imuTaskContext_.mpu6000Count = 0U;
    imuTaskContext_.icm20602Count = 0U;
    detectImu(*imu1Spi_);
    detectImu(*imu2Spi_);
    detectImu(*imu3Spi_);
    const size_t imuCount = imuTaskContext_.icm426xxCount + imuTaskContext_.mpu6000Count + imuTaskContext_.icm20602Count;
    if (imuCount == 0U) {
        std::printf("No SPI IMU detected\r\n");
    }
    baroTaskContext_.barometer = detectBarometer();
    std::printf("%s\r\n", baroTaskContext_.barometer != nullptr ? "Detected BARO: DPS310" : "No DPS310 detected");
    detectMagnetometer();
}

// Helper functions to bridge C and C++
extern "C" void app_main_task(void *argument) {
    (void)argument;
    static App app;
    if (!app.initialize()) {
        // Error handling
        HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
        while(1);
    }
    app.run();
}
