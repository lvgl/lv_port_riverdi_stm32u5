/**
  ******************************************************************************
  * @file    nema_hal_cmsis_os_template.c
  * @author  GPM Application Team
  * @brief   NemaGFX Interfaces and Platform Specific APIs with CMSIS OS support.
  *          This file provides NemaGFX functions to manage the following
  *          functionalities of the NemaGFX library:
  *           + Initialization and de-initialization functions
  *           + GPU2D Registers access operation functions
  *           + Buffer Creation, Destruction, MMAP and Flusing functions
  *           + Interrupt management functions
  *           + Memory management functions
  *           + Locking/Unlocking resources functions
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020-2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

#include "nema_core.h"
#include "nema_sys_defs.h"
#include <cmsis_os.h>

/********* PLATFORM SPECIFIC HEADER FILES *********/
#if defined(STM32V7R7xx) || defined(STM32V7R5xx) || defined(STM32V7S7xx) || defined(STM32V7S5xx)
#include "stm32v7xx_hal.h"
#include "stm32v7xx_hal_gpu2d.h"
#elif defined(STM32U599xx) || defined(STM32U5A9xx)
#include "stm32u5xx_hal.h"
#include "stm32u5xx_hal_gpu2d.h"
#else
#error "Unsupported Platform"
#endif /* STM32V7R7xx | STM32V7R5xx | STM32V7S7xx | STM32V7S5xx */
/**************************************************/

/********* APPLICATION SPECIFIC OPTIONS *********/
#define WAIT_IRQ_BINARY_SEMAPHORE    /* Using Semaphore instead of Task Notification - Default */
/* #define WAIT_IRQ_TASK_NOTIFY */   /* Using Task Notification instead of Semaphore           */
/* #define NEMA_CACHED_MEMORY */     /* Using cached memory                                    */
/* #define NEMA_MULTI_PROCESS */     /* Multi-process access lock/unlock                       */
/* #define NEMA_MULTI_THREAD */      /* Multi-threading access lock/unlock                     */
/************************************************/

#define RING_SIZE 1024          /* Ring Buffer Size in byte */

static nema_ringbuffer_t ring_buffer_str = {{0}};
volatile static int last_cl_id = -1;
GPU2D_HandleTypeDef hgpu2d = { 0 };

#if (defined(NEMA_MULTI_PROCESS) || defined(NEMA_MULTI_THREAD))
osMutexId  nema_mutexes_ids[MUTEX_MAX + 1]; /* Mutexes IDs */
osMutexDef (MUTEX_RB);
osMutexDef (MUTEX_FLUSH);
#endif /* NEMA_MULTI_PROCESS | NEMA_MULTI_THREAD */

#if defined(WAIT_IRQ_BINARY_SEMAPHORE )
osSemaphoreDef (nema_irq_sem);      /* Declare semaphore */
osSemaphoreId  (nema_irq_sem_id);   /* Semaphore ID      */
#elif defined(WAIT_IRQ_TASK_NOTIFY) /* !WAIT_IRQ_TASK_NOTIFY */
osThreadId nemaThreadId;            /* id for the NemaGFX running thread */
#endif /* WAIT_IRQ_BINARY_SEMAPHORE */

#if (USE_HAL_GPU2D_REGISTER_CALLBACKS == 1)
static void GPU2D_CommandListCpltCallback(GPU2D_HandleTypeDef *hgpu2d, uint32_t CmdListID)
#else /* USE_HAL_GPU2D_REGISTER_CALLBACKS = 0 */
void HAL_GPU2D_CommandListCpltCallback(GPU2D_HandleTypeDef *hgpu2d, uint32_t CmdListID)
#endif /* USE_HAL_GPU2D_REGISTER_CALLBACKS = 1 */
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hgpu2d);

    last_cl_id = CmdListID;

#if defined(WAIT_IRQ_BINARY_SEMAPHORE )
    osSemaphoreRelease (nema_irq_sem_id); /* Return a token back to a semaphore */
#elif defined(WAIT_IRQ_TASK_NOTIFY) /* !WAIT_IRQ_TASK_NOTIFY */
    osSignalSet (nemaThreadId, 0);         /* Send signals to the created thread */
#endif /* WAIT_IRQ_BINARY_SEMAPHORE */
}

// ----------------------------------------------------
int32_t nema_sys_init(void)
{
    /* Initialize GPU2D IP */
    hgpu2d.Instance = GPU2D;
    HAL_GPU2D_Init(&hgpu2d);

#if (USE_HAL_GPU2D_REGISTER_CALLBACKS == 1)
    /* Register Command List Comlete Callback */
    HAL_GPU2D_RegisterCommandListCpltCallback(&hgpu2d, GPU2D_CommandListCpltCallback);
#endif /* USE_HAL_GPU2D_REGISTER_CALLBACKS = 1 */

#if (defined(NEMA_MULTI_PROCESS) || defined(NEMA_MULTI_THREAD))
    /* Create Lock ressource */
    nema_mutexes_ids[MUTEX_RB] = osMutexCreate(osMutex(MUTEX_RB));
    configASSERT(nema_mutexes_ids[MUTEX_RB]);
    nema_mutexes_ids[MUTEX_FLUSH] = osMutexCreate(osMutex(MUTEX_FLUSH));
    configASSERT(nema_mutexes_ids[MUTEX_FLUSH]);
#endif /* NEMA_MULTI_PROCESS | NEMA_MULTI_THREAD */

#if defined(WAIT_IRQ_BINARY_SEMAPHORE )
    /* Create semaphore with 1 token */
    nema_irq_sem_id = osSemaphoreCreate(osSemaphore(nema_irq_sem), 1);
    configASSERT(nema_irq_sem_id);
#elif defined(WAIT_IRQ_TASK_NOTIFY) /* !WAIT_IRQ_TASK_NOTIFY */
    nemaThreadId = osThreadGetId();
    configASSERT(nemaThreadId);
#endif /* WAIT_IRQ_BINARY_SEMAPHORE */

    /* Allocate ring_buffer memory */
    ring_buffer_str.bo = nema_buffer_create(RING_SIZE);
    configASSERT(ring_buffer_str.bo.base_virt);
    (void)nema_buffer_map(&ring_buffer_str.bo);

    /* Initialize Ring Buffer */
    int ret = nema_rb_init(&ring_buffer_str, 1);
    if (ret < 0)
    {
        return ret;
    }

    /* Reset last_cl_id counter */
    last_cl_id = 0;

    return 0;
}

int nema_wait_irq(void)
{
#if defined(WAIT_IRQ_BINARY_SEMAPHORE)
    /* Wait indefinitely for a free semaphore */
    osSemaphoreWait (nema_irq_sem_id, osWaitForever);
#elif defined(WAIT_IRQ_TASK_NOTIFY) /* !WAIT_IRQ_TASK_NOTIFY */
    /* Wait indefinitely for a task notification */
    osSignalWait(0, osWaitForever);
#endif /* WAIT_IRQ_BINARY_SEMAPHORE */

    return 0;
}

int nema_wait_irq_cl(int cl_id)
{
    while (last_cl_id < cl_id)
    {
        (void)nema_wait_irq();
    }

    return 0;
}

int nema_wait_irq_brk(int brk_id)
{
    while (nema_reg_read(GPU2D_BREAKPOINT) == 0U)
    {
        (void)nema_wait_irq();
    }

    return 0;
}

uint32_t nema_reg_read(uint32_t reg)
{
    return HAL_GPU2D_ReadRegister(&hgpu2d, reg);
}

void nema_reg_write(uint32_t reg, uint32_t value)
{
    HAL_GPU2D_WriteRegister(&hgpu2d, reg, value);
}

nema_buffer_t nema_buffer_create(int size)
{
    nema_buffer_t bo;

    bo.base_virt = pvPortMalloc(size);
    assert(bo.base_virt);

    bo.base_phys = (uint32_t)bo.base_virt;
    bo.size      = size;
    bo.fd        = 0;

    return bo;
}

nema_buffer_t nema_buffer_create_pool(int pool, int size)
{
    UNUSED(pool);
    return nema_buffer_create(size);
}

void *nema_buffer_map(nema_buffer_t *bo)
{
    return bo->base_virt;
}

void nema_buffer_unmap(nema_buffer_t *bo)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(bo);
}

void nema_buffer_destroy(nema_buffer_t *bo)
{
    vPortFree(bo->base_virt);
}

uintptr_t nema_buffer_phys(nema_buffer_t *bo)
{
    return bo->base_phys;
}

void nema_buffer_flush(nema_buffer_t * bo)
{
#if defined(NEMA_CACHED_MEMORY)
    nema_mutex_lock(MUTEX_FLUSH);
    SCB_CleanInvalidateDCache_by_Addr((uint32_t *)bo->base_virt, bo->size);
    nema_mutex_unlock(MUTEX_FLUSH);
#else /* !NEMA_CACHED_MEMORY */
    UNUSED(bo);
#endif /* NEMA_CACHED_MEMORY */
}

void nema_host_free(void *ptr)
{
    if (ptr)
    {
        vPortFree(ptr);
    }
}

void* nema_host_malloc(unsigned size)
{
    void *ptr = pvPortMalloc(size);
    assert(ptr);
    return ptr;
}

int nema_mutex_lock(int mutex_id)
{
    int ret = 0;

#if (defined(NEMA_MULTI_PROCESS) || defined(NEMA_MULTI_THREAD))
    if ((mutex_id >= 0) && (mutex_id <= MUTEX_MAX) && (NULL != nema_mutexes_ids[mutex_id]))
    {
        ret = osMutexWait(nema_mutexes_ids[mutex_id], osWaitForever) == osOK ? 0 : -1;
    }
#else /* !NEMA_MULTI_PROCESS & !NEMA_MULTI_THREAD */
    UNUSED(mutex_id);
#endif /* NEMA_MULTI_PROCESS | NEMA_MULTI_THREAD */

    return ret;
}

int nema_mutex_unlock(int mutex_id)
{
    int ret = 0;

#if (defined(NEMA_MULTI_PROCESS) || defined(NEMA_MULTI_THREAD))
    if ((mutex_id >= 0) && (mutex_id <= MUTEX_MAX) && (NULL != nema_mutexes_ids[mutex_id]))
    {
        ret = osMutexRelease(nema_mutexes_ids[mutex_id]) == osOK ? 0 : -1;
    }
#else /* !NEMA_MULTI_PROCESS & !NEMA_MULTI_THREAD */
    UNUSED(mutex_id);
#endif /* NEMA_MULTI_PROCESS | NEMA_MULTI_THREAD */

    return ret;
}
