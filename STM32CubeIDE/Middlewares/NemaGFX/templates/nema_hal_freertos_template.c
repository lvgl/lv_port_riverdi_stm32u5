/**
  ******************************************************************************
  * @file    nema_hal.c
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

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <platform_pool_config.h>

#include <nema_sys_defs.h>
#include <nema_core.h>

#include <assert.h>
#include <string.h>

/********* PLATFORM SPECIFIC HEADER FILES *********/
#include "stm32u5xx_hal.h"
#include "stm32u5xx_hal_gpu2d.h"
/**************************************************/

#if defined(NEMA_USE_TSI_MALLOC) || defined(NEMA_MULTI_MEM_POOLS)
#include "tsi_malloc.h"
#endif /* NEMA_USE_TSI_MALLOC */

#if defined(NEMA_USE_RTOS)
#include <cmsis_os.h>
#endif /* NEMA_USE_RTOS */

#define RING_SIZE 1024

static nema_ringbuffer_t ring_buffer_str;
volatile static int last_cl_id = -1;
GPU2D_HandleTypeDef hgpu2d = { 0 };

#if (defined(NEMA_MULTI_PROCESS) || defined(NEMA_MULTI_THREAD))
osMutexId  nema_mutexes_ids[MUTEX_MAX + 1]; // Mutexes IDs
osMutexDef (MUTEX_RB);
osMutexDef (MUTEX_MALLOC);
osMutexDef (MUTEX_FLUSH);
#endif /* NEMA_MULTI_PROCESS | NEMA_MULTI_THREAD */

#if defined(NEMA_WAIT_IRQ_BINARY_SEMAPHORE)
osSemaphoreDef (nema_irq_sem);     // Declare semaphore
osSemaphoreId  (nema_irq_sem_id);  // Semaphore ID
#elif defined(NEMA_WAIT_IRQ_TASK_NOTIFY) /* !NEMA_WAIT_IRQ_TASK_NOTIFY */
osThreadId nemaThreadId; // id for the NemaGFX running thread
#endif /* NEMA_WAIT_IRQ_BINARY_SEMAPHORE */

#if (USE_HAL_GPU2D_REGISTER_CALLBACKS == 1)
static void GPU2D_CommandListCpltCallback(GPU2D_HandleTypeDef *hgpu2d, uint32_t CmdListID)
#else /* USE_HAL_GPU2D_REGISTER_CALLBACKS = 0 */
void HAL_GPU2D_CommandListCpltCallback(GPU2D_HandleTypeDef *hgpu2d, uint32_t CmdListID)
#endif /* USE_HAL_GPU2D_REGISTER_CALLBACKS = 1 */
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hgpu2d);

    last_cl_id = CmdListID;
#if defined(NEMA_WAIT_IRQ_BINARY_SEMAPHORE)
    osSemaphoreRelease (nema_irq_sem_id); // Return a token back to a semaphore
#elif defined(NEMA_WAIT_IRQ_TASK_NOTIFY) /* !NEMA_WAIT_IRQ_TASK_NOTIFY */
    osSignalSet(nemaThreadId, 0);         // Send signals to the created thread
#endif /* NEMA_WAIT_IRQ_BINARY_SEMAPHORE */
}

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
    assert(nema_mutexes_ids[MUTEX_RB]);
    nema_mutexes_ids[MUTEX_MALLOC] = osMutexCreate(osMutex(MUTEX_MALLOC));
    assert(nema_mutexes_ids[MUTEX_MALLOC]);
    nema_mutexes_ids[MUTEX_FLUSH] = osMutexCreate(osMutex(MUTEX_FLUSH));
    assert(nema_mutexes_ids[MUTEX_FLUSH]);
#endif /* NEMA_MULTI_PROCESS | NEMA_MULTI_THREAD */

#if defined(NEMA_WAIT_IRQ_BINARY_SEMAPHORE)
    /* Create IRQ semaphore */
    nema_irq_sem_id = osSemaphoreCreate(osSemaphore(nema_irq_sem), 1);  // Create semaphore with 1 token
    assert(nema_irq_sem_id);
#elif defined(NEMA_WAIT_IRQ_TASK_NOTIFY) /* !NEMA_WAIT_IRQ_TASK_NOTIFY */
    nemaThreadId = osThreadGetId();
    assert(nemaThreadId);
#endif /* NEMA_WAIT_IRQ_BINARY_SEMAPHORE */

#if defined(NEMA_MULTI_MEM_POOLS)
    /* ------------------------------------------------------------------------------ */
    /* Initialize Graphics Mem pools                                                  */
    /* ------------------------------------------------------------------------------ */
    for (int pool_id=0; pool_id < NEMA_MULTI_MEM_POOLS_CNT; pool_id++)
    {
        tsi_malloc_init_pool(pool_id, (void*)memPools[pool_id].base_addr, memPools[pool_id].base_addr, memPools[pool_id].mem_size, 1);
    }
#endif /* NEMA_MULTI_MEM_POOLS */

    // Allocate ring_buffer memory
    ring_buffer_str.bo = nema_buffer_create(RING_SIZE);
    assert(ring_buffer_str.bo.base_virt);

    (void)nema_buffer_map(&ring_buffer_str.bo);

    // Initialize Ring Buffer
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
#if defined(NEMA_WAIT_IRQ_BINARY_SEMAPHORE)
    osSemaphoreWait (nema_irq_sem_id, osWaitForever);  // Wait indefinitely for a free semaphore
#elif defined(NEMA_WAIT_IRQ_TASK_NOTIFY) /* !NEMA_WAIT_IRQ_TASK_NOTIFY */
    osSignalWait (0, osWaitForever); // Wait indefinitely for a task notification
#endif /* NEMA_WAIT_IRQ_BINARY_SEMAPHORE */

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

    memset(&bo, 0, sizeof(bo));

    nema_mutex_lock(MUTEX_MALLOC);

    bo.base_virt = nema_host_malloc(size);
    assert(bo.base_virt);

    bo.base_phys = (uint32_t)bo.base_virt;
    bo.size      = size;
    bo.size      = -1; /* see nema_buffer_destroy */

    nema_mutex_unlock(MUTEX_MALLOC);

    return bo;
}

nema_buffer_t nema_buffer_create_pool(int pool, int size)
{
#ifdef NEMA_MULTI_MEM_POOLS
    nema_buffer_t bo;

    assert(pool < NEMA_MULTI_MEM_POOLS_CNT);

    memset(&bo, 0, sizeof(bo));

    nema_mutex_lock(MUTEX_MALLOC); /* tsi_malloc isn't thread-safe */

    bo.base_virt = tsi_malloc_pool(pool, size);
    assert(bo.base_virt);

    bo.base_phys = (uintptr_t)bo.base_virt;
    bo.size      = size;

    nema_mutex_unlock(MUTEX_MALLOC);

    return bo;
#else
    return nema_buffer_create(size);
#endif
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
    nema_mutex_lock(MUTEX_MALLOC);

#if defined(NEMA_MULTI_MEM_POOLS)
    if (bo->fd != -1)
    {
        tsi_free(bo->base_virt);
    }
    else
#endif
    {
        nema_host_free(bo->base_virt);
    }

    nema_mutex_unlock(MUTEX_MALLOC);
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
    void *ptr = NULL;

    ptr = pvPortMalloc(size);

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
