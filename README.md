---
layout: post
title: Riverdi 5-inch STM32 Embedded Display - board certification review
author: "kisvegabor"
cover: /assets/cover_cert_gold.webp
---
# LVGL port for the 5-inch Riverdi STM32 Embedded Displays (STM32U599NJH6Q or STM32U5A9NJH6Q)
This repository contains the __Light and Versatile Graphics Library (LVGL)__ port for the __5-inch Riverdi STM32 Embedded Displays__ with *STM32U599NJH6Q or STM32U5A9NJH6Q* MCUs. It's ready-to-use project template which you can easily import into *STM32CubeIDE* and *STM32CubeMX* (to reconfigure selected peripherals). This project uses the master LVGL release (at the time or writing v9.1.1). It is based on the [LVGL example](https://github.com/riverdi/riverdi-50-stm32u5-lvgl) provided by Riverdi, but this repository is maintained by LVGL.

For more info about LVGL (docs, API, tutorials) please visit [*https://lvgl.io/*](https://lvgl.io/) and [*https://github.com/lvgl/lvgl*](https://github.com/lvgl/lvgl)

![img1](/Docs/img_1.png)

This repository supports all configuration of 5-inch *Riverdi STM32 Embedded Displays*:

* [*RVT50HQSNWC00-B*](https://riverdi.com/product/5-inch-lcd-display-capacitive-touch-panel-optical-bonding-uxtouch-stm32u5-rvt50hqsnwc00-b)
* [*RVT50HQSNWC00*](https://riverdi.com/product/5-inch-lcd-display-capacitive-touch-panel-air-bonding-uxtouch-stm32u5-rvt50hqsnwc00)
* [*RVT50HQSFWCA0*](https://riverdi.com/product/5-inch-lcd-display-capacitive-touch-panel-air-bonding-atouch-frame-stm32u5-rvt50hqsfwca0)
* [*RVT50HQSNWCA0*](https://riverdi.com/product/5-inch-lcd-display-capacitive-touch-panel-air-bonding-atouch-stm32u5-rvt50hqsnwca0)
* [*RVT50HQSFWN00*](https://riverdi.com/product/5-inch-lcd-display-stm32u5-frame-rvt50hqsfwn00)
* [*RVT50HQSNWN00*](https://riverdi.com/product/5-inch-lcd-display-stm32u5-rvt50hqsnwn00)

For LVGL project templates for 7-inch and 10.1-inch *Riverdi STM32 Embedded Displays*, please visit:

* [*riverdi-70-stm32h7-lvgl*](https://github.com/lvgl/lv_port_riverdi_70-stm32h7)
* [*riverdi-101-stm32h7-lvgl*](https://github.com/lvgl/lv_port_riverdi_101-stm32h7)

## How to import this project to STM32CubeIDE:

[1] Clone this respository (do not forget about *--recursive* flag!):
```
git clone --recursive https://github.com/lvgl/lv_port_riverdi_stm32u5
```
[2] Open *STM32CubeIDE* and import project:
```
File => Open Projects from File System... => Directory => Select the "riverdi-stm32u5/STM32CubeIde" folder => Finish
```
[3] Build the project (for the best performance, please use *Release* configuration with *-Ofast* flag):
```
Project => Build all
```
[4] Upload the firmware to the *Riverdi STM32 Embedded Display*

## What is Riverdi STM32 Embedded Display?

[*Riverdi’s Display STM32 Embedded*](https://riverdi.com/product-category/stm32-embedded-displays/?pr=26938) product series are a comprehensive solution for a variety of applications. With high resolution, high brightness, and a host of features, these displays are designed to meet the most demanding requirements.

Riverdi offers a variety of STM32 touch screen displays, including a brand new 5-inch STM32 display modules in its top-tier embedded screen solutions. With the STM32 7-inch LCD and 10.1-inch STM32 TFT display already on the market, the Riverdi LCD Display STM32 Embedded series is the all-in-one HMI solution well-suited for the most demanding applications.

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

## Specifications

### CPU and memory

- **MCU** STM32U599NJH6Q/STM32U5A9NJH6Q
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

## LVGL Certified Board

<img src="https://lvgl.io/assets/images/cert_pro.png" alt="Professional LVGL certificate for Riverdi 5-inch STM32 Embedded Display">

The 5-inch Riverdi STM32 Embedded Display board earned Professional LVGL Board Certificate, which means that users can use it to create amazing user interfaces without worrying about performance and quality.

<iframe width="560" height="315" src="https://www.youtube.com/embed/3sdUdsvtNio" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen></iframe>

## Performance

As you can see from the video it's clear that you won't have any issues with performance. 

### Frame rate (FPS)

We have measured a stable 33 FPS with 90-96% CPU load. While the frame rate is very good, the CPU use is a bit on the high side (it should be noted that the current software example does not utilize the GPU built into the STM32U599). Nevertheless, the animations were fluid, without any noticable hickups.

### Memory

The 2.5MB internal memory allows to use full size frame buffers. This improves the performance considerably, as it is not necessary to copy data from the display buffer into the frame buffer. The internal RAM is also significantly faster then the external RAM.

Although this board is not equipped with external RAM or flash the 2.5 MB internal RAM and the 4 MB internal flash should be enough for most of the UI applications.

## Quality

The board is very compact and it feels robust. It also looks professional. The display is flat, and it can be mounted without any additional holes in the housing using the attached double-sided adhesive tape. The connectors are robust, 1.25mm Molex type. They were designed to attach to external connectors, which is a good choice for an industrial product, as it does not limit the design.

### Display

This board comes with an IPS display so it has very good viewing angles. The display is also exceptionally bright.

![Viewing angles of the STM32 Embedded 5.0" display](/assets/cert_riverdi_STM32_embedded_5/display.webp)

### Touchpad

The display can be ordered with either an industrial grade projective capacitive touch panel or without touch panel. During our evaluation the touchpad was quite sensitive and accurate. We haven't found any issues with it.

### Robustness

This board was created with the definite purpose of being mounted into an end product even in a harsh environment.


## Conclusion

Riverdi's 5.0" STM32 Embedded display is an outstanding product that perfectly aligns with our Professional certification standards. To sum it up, this display offers high brightness, exceptional visibility, and vibrant colors. It features an industrial touch panel, all impeccably assembled for stability. At its core, it boasts the power and modernity of an STM32U599 MCU.

On the software front, it utilizes ST's well-established and mature STM32CubeIDE.

If you're in search of a dependable solution that seamlessly combines stability and performance, you should definitely explore the Riverdi STM32 Embedded 5.0" display.

## Buy now

You can purchase the 5-inch Riverdi STM32 Embedded Displays from several sources:

- [Riverdi's website](https://riverdi.com/product/5-inch-lcd-display-capacitive-touch-panel-optical-bonding-uxtouch-stm32u5-rvt50hqsnwc00-b)
- [Mouser](https://www2.mouser.com/ProductDetail/Riverdi/RVT50HQSNWC00-B?qs=17ckDYBRdekaIIA5WqJAmw%3D%3D)
- [TME](https://www.tme.com/us/en-us/details/sm-rvt50hqsnwc00-b/intelligent-displays-modules/riverdi/) 
- [DigiKey](https://www.digikey.pl/pl/products/detail/riverdi/SM-RVT50HQSNWC00-B/22077604?s=N4IgTCBcDaIEoDUAqBWADACQIoGUByA6gMJpoC0AQiALoC%2BQA)
