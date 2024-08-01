Different silicon revisions of the STM32U599 have different NemaGFX IP hardware. This different software libraries are mapped to the silicon revision according to the list below:

STM32U599 (Device ID 0x481) Revision ID Rev B -> use libs from "\Middlewares\Third_Party\NemaGFX\lib\core\cortex_m33" 
STM32U599 (Device ID 0x481) Revision ID Rev C -> use libs from "\Middlewares\Third_Party\NemaGFX\lib\core\cortex_m33_revC"

Revision information can be obtained directly from the package information (Refer to the device datasheet for how to identify this code on different types of package). Alternatively connect the board to the STM32CubeProgrammer and read out the target information, which also includes the Revision ID.