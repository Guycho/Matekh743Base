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
    constexpr uint8_t IMU_WHO_AM_I_REG = 0x75U;
    constexpr uint8_t ICM42688P_WHO_AM_I = 0x47U;
    constexpr uint8_t ICM42605_WHO_AM_I = 0x42U;
    constexpr uint8_t MPU6000_WHO_AM_I = 0x68U;
    constexpr uint8_t MPU6000_ALT_WHO_AM_I = 0x70U;
    constexpr uint8_t ICM20602_WHO_AM_I = 0x12U;
    constexpr uint8_t QMC5883L_ADDRESS = 0x0DU;
    constexpr uint8_t HMC5883L_LIS3MDL_ADDRESS = 0x1EU;
    constexpr uint8_t LIS3MDL_ALT_ADDRESS = 0x1CU;
    constexpr uint8_t QMC5883L_CHIP_ID = 0xFFU;
    constexpr uint8_t LIS3MDL_WHO_AM_I = 0x3DU;
    constexpr uint8_t QMC5883L_CHIP_ID_REG = 0x0DU;
    constexpr uint8_t HMC5883L_ID_REG = 0x0AU;
    constexpr uint8_t LIS3MDL_WHO_AM_I_REG = 0x0FU;

    enum class ImuChip {
        None,
        Icm426xx,
        Mpu6000,
        Icm20602,
    };

    struct ImuProbeResult {
        SpiDevice* device;
        const char* probeName;
        bool hasWhoAmI;
        uint8_t whoAmI;
        ImuChip chip;
        const char* chipName;
    };

    struct MagByteProbeResult {
        I2cDevice* device;
        const char* sensorName;
        uint8_t address;
        uint8_t registerAddress;
        bool isReady;
        bool hasValue;
        uint8_t value;
    };

    struct Hmc5883lProbeResult {
        I2cDevice* device;
        uint8_t address;
        bool isReady;
        bool hasId;
        uint8_t id[3];
    };

    // Buffer for ADC DMA - Allocated in a specific section to be compatible with D1 RAM
    // Matches the configuration in main.c
    __attribute__((section(".RAM_D1"), aligned(32)))
    static uint16_t gAdcBuffer[32] = {0};

    constexpr uint16_t makeI2cAddress(uint8_t address)
    {
        return static_cast<uint16_t>(address << 1U);
    }

    ImuChip getImuChip(uint8_t whoAmI)
    {
        if (whoAmI == ICM42688P_WHO_AM_I || whoAmI == ICM42605_WHO_AM_I) {
            return ImuChip::Icm426xx;
        }
        if (whoAmI == MPU6000_WHO_AM_I || whoAmI == MPU6000_ALT_WHO_AM_I) {
            return ImuChip::Mpu6000;
        }
        if (whoAmI == ICM20602_WHO_AM_I) {
            return ImuChip::Icm20602;
        }
        return ImuChip::None;
    }

    const char* getImuChipName(ImuChip chip, uint8_t whoAmI)
    {
        if (chip == ImuChip::Icm426xx && whoAmI == ICM42688P_WHO_AM_I) {
            return "ICM42688-P";
        }
        if (chip == ImuChip::Icm426xx && whoAmI == ICM42605_WHO_AM_I) {
            return "ICM42605";
        }
        if (chip == ImuChip::Mpu6000) {
            return "MPU6000";
        }
        if (chip == ImuChip::Icm20602) {
            return "ICM20602";
        }
        return "none";
    }

    ImuProbeResult probeImu(SpiDevice& spiDevice, const char* probeName)
    {
        uint8_t whoAmI = 0U;
        const bool hasWhoAmI = spiDevice.readRegister(IMU_WHO_AM_I_REG, &whoAmI, 1);
        const ImuChip chip = hasWhoAmI ? getImuChip(whoAmI) : ImuChip::None;
        std::printf("IMU probe %s whoami_ok=%d whoami=0x%02x\r\n",
                    probeName,
                    hasWhoAmI ? 1 : 0,
                    static_cast<unsigned int>(whoAmI));
        return {&spiDevice, probeName, hasWhoAmI, whoAmI, chip, getImuChipName(chip, whoAmI)};
    }

    MagByteProbeResult probeMagByte(I2cDevice& i2cDevice, const char* sensorName, uint8_t address, uint8_t registerAddress)
    {
        uint8_t value = 0U;
        const bool isReady = i2cDevice.isReady();
        const bool hasValue = i2cDevice.readRegister(registerAddress, &value, 1);
        std::printf("MAG probe %s addr=0x%02x ready=%d reg=0x%02x ok=%d value=0x%02x\r\n",
                    sensorName,
                    static_cast<unsigned int>(address),
                    isReady ? 1 : 0,
                    static_cast<unsigned int>(registerAddress),
                    hasValue ? 1 : 0,
                    static_cast<unsigned int>(value));
        return {&i2cDevice, sensorName, address, registerAddress, isReady, hasValue, value};
    }

    Hmc5883lProbeResult probeHmc5883l(I2cDevice& i2cDevice, uint8_t address)
    {
        uint8_t id[3] = {};
        const bool isReady = i2cDevice.isReady();
        const bool hasId = i2cDevice.readRegister(HMC5883L_ID_REG, id, sizeof(id));
        std::printf("MAG probe HMC5883L addr=0x%02x ready=%d ok=%d id=%c%c%c\r\n",
                    static_cast<unsigned int>(address),
                    isReady ? 1 : 0,
                    hasId ? 1 : 0,
                    hasId ? static_cast<char>(id[0]) : '.',
                    hasId ? static_cast<char>(id[1]) : '.',
                    hasId ? static_cast<char>(id[2]) : '.');
        return {&i2cDevice, address, isReady, hasId, {id[0], id[1], id[2]}};
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

void App::initializeDiscoveredImu(SpiDevice& spiDevice, const char* probeName, uint8_t whoAmI)
{
    const ImuChip chip = getImuChip(whoAmI);
    const char* chipName = getImuChipName(chip, whoAmI);
    if (chip == ImuChip::None) {
        std::printf("IMU init %s skipped whoami=0x%02x\r\n", probeName, static_cast<unsigned int>(whoAmI));
        return;
    }
    std::printf("IMU init %s %s start\r\n", probeName, chipName);
    if (chip == ImuChip::Icm426xx) {
        Icm426xx* icm426xx = new Icm426xx(spiDevice);
        const bool isInitialized = icm426xx->initialize();
        std::printf("IMU init %s %s done ok=%d\r\n", probeName, chipName, isInitialized ? 1 : 0);
        if (isInitialized && imuTaskContext_.icm426xxCount < 3U) {
            imuTaskContext_.icm426xxImus[imuTaskContext_.icm426xxCount] = icm426xx;
            ++imuTaskContext_.icm426xxCount;
            std::printf("Detected IMU: %s\r\n", icm426xx->getName());
            return;
        }
        delete icm426xx;
        return;
    }
    if (chip == ImuChip::Mpu6000) {
        Mpu6000* mpu6000 = new Mpu6000(spiDevice);
        const bool isInitialized = mpu6000->initialize();
        std::printf("IMU init %s %s done ok=%d\r\n", probeName, chipName, isInitialized ? 1 : 0);
        if (isInitialized && imuTaskContext_.mpu6000Count < 3U) {
            imuTaskContext_.mpu6000Imus[imuTaskContext_.mpu6000Count] = mpu6000;
            ++imuTaskContext_.mpu6000Count;
            std::printf("Detected IMU: %s\r\n", mpu6000->getName());
            return;
        }
        delete mpu6000;
        return;
    }
    Icm20602* icm20602 = new Icm20602(spiDevice);
    const bool isInitialized = icm20602->initialize();
    std::printf("IMU init %s %s done ok=%d\r\n", probeName, chipName, isInitialized ? 1 : 0);
    if (isInitialized && imuTaskContext_.icm20602Count < 3U) {
        imuTaskContext_.icm20602Imus[imuTaskContext_.icm20602Count] = icm20602;
        ++imuTaskContext_.icm20602Count;
        std::printf("Detected IMU: %s\r\n", icm20602->getName());
        return;
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

void App::probeMagnetometers()
{
    qmc5883lI2c_ = new I2cDevice({&hi2c1, makeI2cAddress(QMC5883L_ADDRESS)});
    const MagByteProbeResult qmcProbe = probeMagByte(*qmc5883lI2c_, "QMC5883L", QMC5883L_ADDRESS, QMC5883L_CHIP_ID_REG);
    hasQmc5883lProbe_ = qmcProbe.hasValue;
    qmc5883lChipId_ = qmcProbe.value;
    hmcLis3mdlI2c_ = new I2cDevice({&hi2c1, makeI2cAddress(HMC5883L_LIS3MDL_ADDRESS)});
    const Hmc5883lProbeResult hmcProbe = probeHmc5883l(*hmcLis3mdlI2c_, HMC5883L_LIS3MDL_ADDRESS);
    hasHmc5883lProbe_ = hmcProbe.hasId;
    hmc5883lId_[0] = hmcProbe.id[0];
    hmc5883lId_[1] = hmcProbe.id[1];
    hmc5883lId_[2] = hmcProbe.id[2];
    const MagByteProbeResult lisProbe = probeMagByte(*hmcLis3mdlI2c_, "LIS3MDL", HMC5883L_LIS3MDL_ADDRESS, LIS3MDL_WHO_AM_I_REG);
    hasLis3mdlProbe_ = lisProbe.hasValue;
    lis3mdlChipId_ = lisProbe.value;
    lis3mdlAltI2c_ = new I2cDevice({&hi2c1, makeI2cAddress(LIS3MDL_ALT_ADDRESS)});
    const MagByteProbeResult lisAltProbe = probeMagByte(*lis3mdlAltI2c_, "LIS3MDL", LIS3MDL_ALT_ADDRESS, LIS3MDL_WHO_AM_I_REG);
    hasLis3mdlAltProbe_ = lisAltProbe.hasValue;
    lis3mdlAltChipId_ = lisAltProbe.value;
}

void App::initializeDiscoveredMagnetometer()
{
    if (hasQmc5883lProbe_ && qmc5883lChipId_ == QMC5883L_CHIP_ID) {
        Qmc5883l* qmc5883l = new Qmc5883l(*qmc5883lI2c_);
        std::printf("MAG init QMC5883L addr=0x%02x start\r\n", static_cast<unsigned int>(QMC5883L_ADDRESS));
        const bool isInitialized = qmc5883l->initialize();
        std::printf("MAG init QMC5883L addr=0x%02x done ok=%d\r\n", static_cast<unsigned int>(QMC5883L_ADDRESS), isInitialized ? 1 : 0);
        if (isInitialized) {
            magTaskContext_.qmc5883l = qmc5883l;
            std::printf("Detected MAG: %s\r\n", qmc5883l->getName());
            delete hmcLis3mdlI2c_;
            delete lis3mdlAltI2c_;
            hmcLis3mdlI2c_ = nullptr;
            lis3mdlAltI2c_ = nullptr;
            return;
        }
        delete qmc5883l;
    }
    delete qmc5883lI2c_;
    qmc5883lI2c_ = nullptr;
    if (hasHmc5883lProbe_ && hmc5883lId_[0] == 'H' && hmc5883lId_[1] == '4' && hmc5883lId_[2] == '3') {
        Hmc5883l* hmc5883l = new Hmc5883l(*hmcLis3mdlI2c_);
        std::printf("MAG init HMC5883L addr=0x%02x start\r\n", static_cast<unsigned int>(HMC5883L_LIS3MDL_ADDRESS));
        const bool isInitialized = hmc5883l->initialize();
        std::printf("MAG init HMC5883L addr=0x%02x done ok=%d\r\n", static_cast<unsigned int>(HMC5883L_LIS3MDL_ADDRESS), isInitialized ? 1 : 0);
        if (isInitialized) {
            magTaskContext_.hmc5883l = hmc5883l;
            std::printf("Detected MAG: %s\r\n", hmc5883l->getName());
            delete lis3mdlAltI2c_;
            lis3mdlAltI2c_ = nullptr;
            return;
        }
        delete hmc5883l;
    }
    if (hasLis3mdlProbe_ && lis3mdlChipId_ == LIS3MDL_WHO_AM_I) {
        Lis3mdl* lis3mdl = new Lis3mdl(*hmcLis3mdlI2c_);
        std::printf("MAG init LIS3MDL addr=0x%02x start\r\n", static_cast<unsigned int>(HMC5883L_LIS3MDL_ADDRESS));
        const bool isInitialized = lis3mdl->initialize();
        std::printf("MAG init LIS3MDL addr=0x%02x done ok=%d\r\n", static_cast<unsigned int>(HMC5883L_LIS3MDL_ADDRESS), isInitialized ? 1 : 0);
        if (isInitialized) {
            magTaskContext_.lis3mdl = lis3mdl;
            std::printf("Detected MAG: %s\r\n", lis3mdl->getName());
            delete lis3mdlAltI2c_;
            lis3mdlAltI2c_ = nullptr;
            return;
        }
        delete lis3mdl;
    }
    delete hmcLis3mdlI2c_;
    hmcLis3mdlI2c_ = nullptr;
    if (hasLis3mdlAltProbe_ && lis3mdlAltChipId_ == LIS3MDL_WHO_AM_I) {
        Lis3mdl* lis3mdl = new Lis3mdl(*lis3mdlAltI2c_);
        std::printf("MAG init LIS3MDL addr=0x%02x start\r\n", static_cast<unsigned int>(LIS3MDL_ALT_ADDRESS));
        const bool isInitialized = lis3mdl->initialize();
        std::printf("MAG init LIS3MDL addr=0x%02x done ok=%d\r\n", static_cast<unsigned int>(LIS3MDL_ALT_ADDRESS), isInitialized ? 1 : 0);
        if (isInitialized) {
            magTaskContext_.lis3mdl = lis3mdl;
            std::printf("Detected MAG: %s\r\n", lis3mdl->getName());
            return;
        }
        delete lis3mdl;
    }
    delete lis3mdlAltI2c_;
    lis3mdlAltI2c_ = nullptr;
    std::printf("No external I2C magnetometer detected\r\n");
}

void App::configureSensorContext()
{
    imuTaskContext_.icm426xxCount = 0U;
    imuTaskContext_.mpu6000Count = 0U;
    imuTaskContext_.icm20602Count = 0U;
    const ImuProbeResult imuProbes[3] = {
        probeImu(*imu1Spi_, "IMU1/SPI1"),
        probeImu(*imu2Spi_, "IMU2/SPI1"),
        probeImu(*imu3Spi_, "IMU3/SPI4"),
    };
    probeMagnetometers();
    baroTaskContext_.barometer = detectBarometer();
    std::printf("%s\r\n", baroTaskContext_.barometer != nullptr ? "Detected BARO: DPS310" : "No DPS310 detected");
    for (const ImuProbeResult& imuProbe : imuProbes) {
        if (imuProbe.hasWhoAmI && imuProbe.chip != ImuChip::None) {
            initializeDiscoveredImu(*imuProbe.device, imuProbe.probeName, imuProbe.whoAmI);
        }
    }
    const size_t imuCount = imuTaskContext_.icm426xxCount + imuTaskContext_.mpu6000Count + imuTaskContext_.icm20602Count;
    if (imuCount == 0U) {
        std::printf("No SPI IMU detected\r\n");
    }
    initializeDiscoveredMagnetometer();
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
