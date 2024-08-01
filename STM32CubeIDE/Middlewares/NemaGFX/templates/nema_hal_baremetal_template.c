/**
  ******************************************************************************
  * @file    nema_hal_baremetal_template.c
  * @author  GPM Application Team
  * @brief   NemaGFX Interfaces and Platform Specific APIs with no OS support.
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

#include <stdlib.h>

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
/* #define NEMA_CACHED_MEMORY */     /* Using cached memory */
/**************************************************/

#define RING_SIZE 1024          /* Ring Buffer Size in byte */

static nema_ringbuffer_t ring_buffer_str = {{0}};
volatile static int last_cl_id = -1;
GPU2D_HandleTypeDef hgpu2d = { 0 };

#if (USE_HAL_GPU2D_REGISTER_CALLBACKS == 1)
static void GPU2D_CommandListCpltCallback(GPU2D_HandleTypeDef *hgpu2d, uint32_t CmdListID)
#else /* USE_HAL_GPU2D_REGISTER_CALLBACKS = 0 */
void HAL_GPU2D_CommandListCpltCallback(GPU2D_HandleTypeDef *hgpu2d, uint32_t CmdListID)
#endif /* USE_HAL_GPU2D_REGISTER_CALLBACKS = 1 */
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hgpu2d);

    last_cl_id = CmdListID;
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

    /* Allocate ring_buffer memory */
    ring_buffer_str.bo = nema_buffer_create(RING_SIZE);
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
    return 0;
}

//DMA2_Stream0_IRQHandler;

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

    bo.base_virt = malloc(size);
    bo.base_phys = (uint32_t)bo.base_virt;
    bo.size      = size;
    bo.fd        = 0;

    return bo;
}

nema_buffer_t nema_buffer_create_pool(int pool, int size)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(pool);
    return nema_buffer_create(size);
}

void* nema_buffer_map(nema_buffer_t *bo)
{
    return bo->base_virt;
}

void nema_buffer_unmap(nema_buffer_t *bo)
{
    UNUSED(bo);
}

void nema_buffer_destroy(nema_buffer_t *bo)
{
    free(bo->base_virt);
}

uintptr_t nema_buffer_phys(nema_buffer_t *bo)
{
    return bo->base_phys;
}

void nema_buffer_flush(nema_buffer_t * bo)
{
#if defined(NEMA_CACHED_MEMORY)
    SCB_CleanInvalidateDCache_by_Addr((uint32_t *)bo->base_virt, bo->size);
#else /* !NEMA_CACHED_MEMORY */
    UNUSED(bo);
#endif /* NEMA_CACHED_MEMORY */
}

void nema_host_free(void *ptr)
{
    if (ptr)
    {
        free(ptr);
    }
}

void* nema_host_malloc(unsigned size)
{
    return malloc(size);
}

int nema_mutex_lock(int mutex_id)
{
    UNUSED(mutex_id);
    return 0;
}

int nema_mutex_unlock(int mutex_id)
{
    UNUSED(mutex_id);
    return 0;
}


void platform_disable_cache(void)
{

}

void platform_invalidate_cache(void)
{
	extern DCACHE_HandleTypeDef hdcache1;
	extern DCACHE_HandleTypeDef hdcache2;

	HAL_DCACHE_Invalidate(&hdcache1);
	HAL_DCACHE_Invalidate(&hdcache2);
}
