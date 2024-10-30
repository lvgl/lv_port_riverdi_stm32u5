
  @verbatim
  ******************************************************************************
  *
  *   COPYRIGHT (C) 2020 STMicroelectronics
  *
  * @file    readme.txt
  * @author  MCD Application Team
  * @brief   This file describes the content of the "templates" directory
  ******************************************************************************
  *
  * This file is modified  by STMicroelectronics as specified in the
  * original license agreement and conditions
  * as issued by Think Silicon S.A. Confidential Proprietary
  *
  ******************************************************************************
  @endverbatim

This file contains template files that provide some required implementations for
NemaGFX third party library.

nema_hal_baremetal_template.c
-------------------------
Implements of the NemaGFX Interfaces and Platform Specific APIs with no OS support.
This file implements the strict minimum required to ensure NemaGFX behaving without RTOS.
This file need to be copied at user level and renamed to "nema_hal.c"

nema_hal_cmsis_os_template.c
-------------------------
Implements of the NemaGFX Interfaces and Platform Specific APIs with CMSIS OS support.
This file implements the strict minimum required to ensure NemaGFX behaving with CMSIS OS.
More options can be managed by enabling/disabling below flags :
  - WAIT_IRQ_BINARY_SEMAPHORE or WAIT_IRQ_TASK_NOTIFY : Use either Semaphore or Task
    Notification for IRQ waiting
  - NEMA_CACHED_MEMORY        : Enable cache coherency management (Clean and Invalidate)
  - NEMA_MULTI_PROCESS        : Protect Mult-Process concurrent access to the Nema HAL 
  - NEMA_MULTI_THREAD         : Protect Mult-Threads concurrent access to the Nema HAL 
This file need to be copied at user level and renamed to "nema_hal.c"

nema_hal_freertos_template.c
-------------------------
Implements of the NemaGFX Interfaces and Platform Specific APIs with FreeRTOS support.
This file implements the strict minimum required to ensure NemaGFX behaving with FreeRTOS.
More options can be managed by enabling/disabling below flags :
  - WAIT_IRQ_BINARY_SEMAPHORE or WAIT_IRQ_TASK_NOTIFY : Use either Semaphore or Task
    Notification for IRQ waiting
  - NEMA_CACHED_MEMORY        : Enable cache coherency management (Clean and Invalidate)
  - NEMA_MULTI_PROCESS        : Protect Mult-Process concurrent access to the Nema HAL 
  - NEMA_MULTI_THREAD         : Protect Mult-Threads concurrent access to the Nema HAL 
This file need to be copied at user level and renamed to "nema_hal.c"

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
