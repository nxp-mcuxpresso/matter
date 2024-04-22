/*
 * Copyright 2019-2021, 2023 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __PERF_H__
#define __PERF_H__

#include "FreeRTOS.h"
#include "task.h"

#if defined(__cplusplus)
extern "C" {
#endif /*_cplusplus*/

#if SLN_TRACE_CPU_USAGE
/**
 * @brief Returns a pointer to the string containing the CPU load info
 */
char *PERF_GetCPULoad(void);
#endif /* SLN_TRACE_CPU_USAGE */

/**
 * @brief print minimum heap remaining
 */
void PERF_PrintHeap(void);

/**
 * @brief print the statistics of memory tasks stack consuming
 */
void PERF_PrintStacks(void);

#if defined(__cplusplus)
}
#endif /*_cplusplus*/

#endif
