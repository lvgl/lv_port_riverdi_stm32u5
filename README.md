# LVGL port for the 5-inch Riverdi STM32 Embedded Displays (STM32U599NJH6Q or STM32U5A9NJH6Q)

This repository contains the __Light and Versatile Graphics Library (LVGL)__ port for the __5-inch Riverdi STM32 Embedded Displays__ with *STM32U599NJH6Q or STM32U5A9NJH6Q* MCUs. It's ready-to-use project template which you can easily import into *STM32CubeIDE* and *STM32CubeMX* (to reconfigure selected peripherals). This project uses latest stable LVGL release (v8.3.9). For more info about LVGL (docs, API, tutorials) please visit [*https://lvgl.io/*](https://lvgl.io/) and [*https://github.com/lvgl/lvgl*](https://github.com/lvgl/lvgl)

![img1](/riverdi/lv_riverdi_stm32u5/Docs/img_1.png)

This repository supports all configuration of 5-inch *Riverdi STM32 Embedded Displays*:

* [*RVT50HQSNWC00-B*](https://riverdi.com/product/5-inch-lcd-display-capacitive-touch-panel-optical-bonding-uxtouch-stm32u5-rvt50hqsnwc00-b)
* [*RVT50HQSNWC00*](https://riverdi.com/product/5-inch-lcd-display-capacitive-touch-panel-air-bonding-uxtouch-stm32u5-rvt50hqsnwc00)
* [*RVT50HQSFWCA0*](https://riverdi.com/product/5-inch-lcd-display-capacitive-touch-panel-air-bonding-atouch-frame-stm32u5-rvt50hqsfwca0)
* [*RVT50HQSNWCA0*](https://riverdi.com/product/5-inch-lcd-display-capacitive-touch-panel-air-bonding-atouch-stm32u5-rvt50hqsnwca0)
* [*RVT50HQSFWN00*](https://riverdi.com/product/5-inch-lcd-display-stm32u5-frame-rvt50hqsfwn00)
* [*RVT50HQSNWN00*](https://riverdi.com/product/5-inch-lcd-display-stm32u5-rvt50hqsnwn00)

For LVGL project templates for 7-inch and 10.1-inch *Riverdi STM32 Embedded Displays*, please visit:

* [*riverdi-70-stm32h7-lvgl*](https://github.com/riverdi/riverdi-70-stm32h7-lvgl)
* [*riverdi-101-stm32h7-lvgl*](https://github.com/riverdi/riverdi-101-stm32h7-lvgl)

## What is Riverdi STM32 Embedded Display?

[*Riverdi’s Display STM32 Embedded*](https://riverdi.com/product-category/stm32-embedded-displays/?pr=26938) product series are a comprehensive solution for a variety of applications. With high resolution, high brightness, and a host of features, these displays are designed to meet the most demanding requirements.

Riverdi offers a variety of STM32 touch screen displays, including a brand new 5-inch STM32 display modules in its top-tier embedded screen solutions. With the STM32 7-inch LCD and 10.1-inch STM32 TFT display already on the market, the Riverdi LCD Display STM32 Embedded series is the all-in-one HMI solution well-suited for the most demanding applications.

The STM32 Embedded Displays are not just a product, they are an ecosystem based on the industrial-quality LCD-TFT displays with high-brightness, high-resolution, and industrial touchscreen, the performance and interfaces offered by the STM32 microcontrollers, and the comprehensive technical support with free software and libraries.

## 5-inch Riverdi STM32 Embedded Display

Main features of the 5-inch __Riverdi STM32 Embedded Display__:

* 5-inch TFT display
* 800x480px resolution
* High brightness 850cd/m2
* Full viewing angles (IPS)
* STM32U599NJH6Q or STM32U5A9NJH6Q in Mass Production
* All STM32U5 interfaces
* Optical bonding
* Industrial projected capacitive touch screen
* Black decorative cover glass
* High quality
* 4MB Flash
* 2.5MB RAM
* TouchGFX and LVGL direct support
* Power Supply: 6V-48V

![img2](/riverdi/lv_riverdi_stm32u5/Docs/img_2.png)

* __RS485__
* __Expansion connector__ - the board offers additional and independent GPIOs over a 40pin, 1.27mm female socket. It provides direct access to the below GPIOs of MCU STM32U599NJH6Q, that makes it possible to be easily extended by an addon board for specific application (2x I2C, UART, USART, SPI, USB, 7x PWMs, 1x DAC, 2x ADCs)
* __Fully independent CAN FD__ - applicable in industrial and automotive area
* __USB__ - can be configured to both Host and Device
* __Micro-SD card slot__ - another external storage. Supporting uSD card type: SD, SDHC, SDXC cards
* __Fast-programming SWD port__ - SWD connector allows to program STM32 and QSPI with customer’s applications. Riverdi developed the ST-LINK programming cable that is included in the STM32 Embedded Display sample package (single packing)
* __RiBUS__ - in some applications there might be a need to connect a second display in one device. With the STM32 Embedded Display line it is very easy as these displays are equipped with Master RiBUS connector – universal interface to Riverdi’s intelligent displays. In this way, the second display does nots need an external host controller and 2 independent displays can be controlled by one STM32

## Riverdi STM32 Embedded Display unboxing

Have a look what you will get when you will order a sample of STM32 Embedded Display:

[![img3](/riverdi/lv_riverdi_stm32u5/Docs/img_3.png)](https://www.youtube.com/watch?v=UwMa3z_OOmQ)

## How to import this project to STM32CubeIDE:

[1] Clone this respository (do not forget about *--recursive* flag!):
```
git clone --recursive https://github.com/riverdi/riverdi-50-stm32u5-lvgl.git
```
[2] Open *STM32CubeIDE* and import project:
```
File => Open Projects from File System... => Directory => Select the "riverdi-50-stm32u5-lvgl/STM32CubeIde" folder => Finish
```
[3] Build the project (for the best performance, please use *Release* configuration with *-Ofast* flag):
```
Project => Build all
```
[4] Upload the firmware to the *Riverdi STM32 Embedded Display*

## TODO

- performance improvement!
