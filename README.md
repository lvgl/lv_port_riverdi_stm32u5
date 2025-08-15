**:rocket: This repository is available in the [LVGL Project Creator](https://lvgl.io/tools/project-creator), making it easy to create and customize a new project in just a few clicks.**

# LVGL ported to the 5-inch Riverdi STM32 Embedded Displays (STM32U599NJH6Q or STM32U5A9NJH6Q)

## Overview

STM32 Embedded 5” display is all-in-one complete and open-platform solution being able to independently handle the visual layer of devices with the need for high computing performance. The 5-inch STM32 Embedded Displays series are industrial-quality LCD-TFT solutions based on the STM32U599NJH6Q or STM32U5A9NJH6Q microcontroller. It has been designed in a way that allows to meet most of the hardware and programming challenges faced by engineers, including access to all interfaces.

## Buy

You can purchase the 5-inch Riverdi STM32 Embedded Displays from several sources:

- [Riverdi's website](https://riverdi.com/product/5-inch-lcd-display-capacitive-touch-panel-optical-bonding-uxtouch-stm32u5-rvt50hqsnwc00-b)
- [Mouser](https://www2.mouser.com/ProductDetail/Riverdi/RVT50HQSNWC00-B?qs=17ckDYBRdekaIIA5WqJAmw%3D%3D)
- [TME](https://www.tme.com/us/en-us/details/sm-rvt50hqsnwc00-b/intelligent-displays-modules/riverdi/) 
- [DigiKey](https://www.digikey.com/en/products/detail/riverdi/SM-RVT50HQSNWC00-B/22077604?s=N4IgTCBcDaIEoDUAqBWADACQIoGUByA6gMJpoC0AQiALoC%2BQA)

## Benchmark

The benchmark uses a screen-sized partial buffer and copies a rendered area to a frame buffer with DMA2D. No VSYNC is used, therfore some tearing is visible in some test cases. 

[![Riverdi STMU5-cover](https://github.com/lvgl/lv_port_riverdi_stm32u5/assets/7599318/589b9270-430e-426a-a2a8-185d9463e849)
](https://www.youtube.com/watch?v=aeDuthE5aA4)

## Specification

### CPU and memory

- **MCU** STM32U599NJH6Q/STM32U5A9NJH6Q (Cotex-M33, 160MHz)
- **RAM** 2.5MB
- **Flash** 4MB
- **GPU** Neo-Chrom (GPU2D), Chrom-Art (DMA2D), Chrom-GRC (GFXMMU)

### Display

- **Resolution** 800x480
- **Display size** 5.0”
- **Color depth** 24bit
- **Technology** IPS
- **Brightness** 850 nits
- **DPI** 188 px/inch
- **Touch pad** Projected Capacitive

### Others

- RS485, RS232
- Expansion connector (40 GPIOs to access 2x I2C, 1x UART, 1x USART, 1x SPI, 1x USB, 7x PWMs, 2x DACs, 2x ADCs)
- 2x CAN FD
- RiBUS connector
- USB
- Haptic feedback driver output (DRV2605L)

## Getting started
To be able to flash and debug your program you will need to purchase an SWD debug probe which supports the ARM Cortex-M7 core, e.g the STMicro ST-Link V2/V3 or the Segger J-Link.

### Hardware setup
- Connect a 6-48V power supply to the POWER header on the board using the supplied cable. The board draws about 0.28A at 9V.
- Connect a debug probe to the SWD header using the supplied cable.
  
### Software setup
- Install [STM32 CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html).

### Run the project
- Clone the project: `git clone --recursive https://github.com/lvgl/lv_port_riverdi_stm32u5`
- Open *STM32CubeIDE* and import project:` File => Open Projects from File System... => Directory => Select the "STM32CubeIde" folder => Finish`
- Build the project (for the best performance use *Release* configuration with *-O2* flag): `Project => Build Project`
- Click the ![image](https://github.com/lvgl/lv_port_riverdi_70-stm32h7/assets/7599318/ad1ba904-f917-4e0c-97b3-1c1ca12cf185) Run button to flash the project
    
### Debugging
- After building the project click the Debug button ![image](https://github.com/lvgl/lv_port_riverdi_70-stm32h7/assets/7599318/369e95fb-dbfb-44d8-9250-0a5f3f8bfc60) to flash the project. You will need to select the correct debug probe for the first run.

## Notes

This repository supports all configuration of 5-inch *Riverdi STM32 Embedded Displays*:

* [*RVT50HQSNWC00-B*](https://riverdi.com/product/5-inch-lcd-display-capacitive-touch-panel-optical-bonding-uxtouch-stm32u5-rvt50hqsnwc00-b)
* [*RVT50HQSNWC00*](https://riverdi.com/product/5-inch-lcd-display-capacitive-touch-panel-air-bonding-uxtouch-stm32u5-rvt50hqsnwc00)
* [*RVT50HQSFWCA0*](https://riverdi.com/product/5-inch-lcd-display-capacitive-touch-panel-air-bonding-atouch-frame-stm32u5-rvt50hqsfwca0)
* [*RVT50HQSNWCA0*](https://riverdi.com/product/5-inch-lcd-display-capacitive-touch-panel-air-bonding-atouch-stm32u5-rvt50hqsnwca0)
* [*RVT50HQSFWN00*](https://riverdi.com/product/5-inch-lcd-display-stm32u5-frame-rvt50hqsfwn00)
* [*RVT50HQSNWN00*](https://riverdi.com/product/5-inch-lcd-display-stm32u5-rvt50hqsnwn00)

## Contribution and Support

If you find any issues with the development board feel free to open an Issue in this repository. For LVGL related issues (features, bugs, etc) please use the main [lvgl repository](https://github.com/lvgl/lvgl).

If you found a bug and found a solution too please send a Pull request. If you are new to Pull requests refer to [Our Guide](https://docs.lvgl.io/master/CONTRIBUTING.html#pull-request) to learn the basics.
