/**
  ******************************************************************************
  * @file    platform_pool_config.h
  * @author  GPM Application Team
  * @brief   Header file of NemaGFX Memory Pools Definitions for STM32 Platforms.
  *          This file provides definition for memory pools (ids, sizes and
  *          addresses) definitions being used by the NemaGFX TSi Allocator.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PLATFORM_POOL_CONFIG_H__
#define PLATFORM_POOL_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ----------------------------------------------------------------------------- */
/* Configuration Flags                                                           */
/* ----------------------------------------------------------------------------- */
#define NEMA_USE_TSI_MALLOC    /* Use TSi Memory Allocator                       */
#define NEMA_MULTI_MEM_POOLS   /* Use Memory Pools - require NEMA_USE_TSI_MALLOC */
/* ----------------------------------------------------------------------------- */

typedef struct {
    uint32_t base_addr;
    uint32_t mem_size;
} nema_mem_pool_desc_t;

/* ----------------------------------------------------------------------------- */
/* Graphics Memory Mapping                                                       */
/* ----------------------------------------------------------------------------- */
/* NEMA_MULTI_MEM_POOLS_CNT pools must be equal or less than 8                   */
#define NEMA_MULTI_MEM_POOLS_CNT	1

/* Example Memory pool definition for STM32U599x devices */

/* Pool 0 */
#define NEMA_MEM_POOL_CL          0
#define GFX_MEM_CL_SIZE           (2 * 832 * 1024)
#define GFX_MEM_CL_BASEADDR       0x200D0000

static const nema_mem_pool_desc_t memPools[NEMA_MULTI_MEM_POOLS_CNT] =
{
    {GFX_MEM_CL_BASEADDR, GFX_MEM_CL_SIZE} /* Pool 0 */
};

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* PLATFORM_POOL_CONFIG_H__ */
