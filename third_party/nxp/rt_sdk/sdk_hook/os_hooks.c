/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>

/* Provide calloc port for mbedTLS. */

/******************************************************************************/
/*************************** FreeRTOS ********************************************/
/******************************************************************************/
#if defined(USE_RTOS) && defined(SDK_OS_FREE_RTOS) && defined(MBEDTLS_FREERTOS_CALLOC_ALT)
#include "FreeRTOS.h"
#include "task.h"
#include <stdlib.h>

/*---------HEAP_4 calloc --------------------------------------------------*/
#if defined(configFRTOS_MEMORY_SCHEME) && (configFRTOS_MEMORY_SCHEME == 4)
void * pvPortCalloc(size_t num, size_t size)
{
    void * ptr;

    ptr = pvPortMalloc(num * size);
    if (!ptr)
    {
        extern void vApplicationMallocFailedHook(void);
        vApplicationMallocFailedHook();
    }
    else
    {
        memset(ptr, 0, num * size);
    }
    return ptr;
}
#else // HEAP_3
void * pvPortCalloc(size_t num, size_t size)
{
    void * pvReturn;

    vTaskSuspendAll();
    {
        pvReturn = calloc(num, size);
        traceMALLOC(pvReturn, size);
    }
    (void) xTaskResumeAll();

#if (configUSE_MALLOC_FAILED_HOOK == 1)
    {
        if (pvReturn == NULL)
        {
            extern void vApplicationMallocFailedHook(void);
            vApplicationMallocFailedHook();
        }
    }
#endif

    return pvReturn;
}
#endif // configFRTOS_MEMORY_SCHEME
#endif /* USE_RTOS && defined(SDK_OS_FREE_RTOS) && defined(MBEDTLS_FREERTOS_CALLOC_ALT) */