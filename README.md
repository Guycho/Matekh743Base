# matek743mini

Project for the Matek 743 Mini based on the STM32H743 MCU.

## Overview

This repository contains the firmware for the matek743mini, implementing a custom Hardware Abstraction Layer (HAL) and leveraging FreeRTOS for multi-tasking.

## Key Features

- **MCU**: STM32H743
- **RTOS**: FreeRTOS
- **Custom HAL**: Provides a clean abstraction for:
    - ADC DMA Reading
    - Digital I/O
    - I2C and SPI Communication
    - PWM Output
    - Serial Port
- **Libraries**: Includes a PID controller implementation for stable control loops.

## Project Structure

- `common/`: Shared code, hardware abstraction, and libraries.
    - `common/hal/`: Hardware abstraction layer.
    - `common/lib/`: General purpose libraries (e.g., PID).
    - `common/Src/` & `common/Inc/`: Application logic and task definitions.
- `mcu/`: MCU-specific configurations, startup code, and build system (Makefile).

## Getting Started

### Prerequisites

- STM32CubeIDE or a compatible GCC toolchain for ARM.
- STM32CubeMX (for `.ioc` configuration).

### Building

Run the provided Makefile in the `mcu/` directory:

```bash
cd mcu
make
```

### Flashing

Use the provided flash script:

```bash
cd mcu
./flash.sh