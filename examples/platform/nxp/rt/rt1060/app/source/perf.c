/*
 * Copyright 2019-2021, 2023-2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "string.h"
#include "stdio.h"

#include "fsl_gpt.h"
#include "perf.h"

#if SLN_TRACE_CPU_USAGE
/* In this configuration, PERF_TIMER_GPT will overflow after ~10 hours and
 * has a resolution of 1 tick == 8.33us;
 * In case the MQS is used, this configuration should not be changed because
 * it is also used for the barge-in purposes. */
#define PERF_TIMER_GPT          GPT2
#define PERF_TIMER_GPT_FREQ_MHZ 24
#define PERF_TIMER_GPT_PS       200

/* Output of vTaskGetRunTimeStats requires ~40 bytes per task */
static char perfBuffer[30 * 40];

void PERF_InitTimer(void)
{
    gpt_config_t gpt;

    /* The timer is set with a PERF_TIMER_GPT_FREQ_MHZ clock freq. */
    CLOCK_SetMux(kCLOCK_PerclkMux, 1);
    CLOCK_SetDiv(kCLOCK_PerclkDiv, 0);

    GPT_GetDefaultConfig(&gpt);
    gpt.divider = PERF_TIMER_GPT_PS;
    GPT_Init(PERF_TIMER_GPT, &gpt);

    GPT_StartTimer(PERF_TIMER_GPT);
}

uint32_t PERF_GetTimer(void)
{
    return GPT_GetCurrentTimerCount(PERF_TIMER_GPT);
}

char *PERF_GetCPULoad(void)
{
    vTaskGetRunTimeStats(perfBuffer);
    return perfBuffer;
}
#endif /* SLN_TRACE_CPU_USAGE */

void PERF_PrintHeap(void)
{
    configPRINT_STRING("  min heap remaining: %d\r\n", xPortGetMinimumEverFreeHeapSize());
}

void PERF_PrintStack(TaskHandle_t task_id)
{
    configPRINT_STRING("  stack high water mark: %d\r\n", uxTaskGetStackHighWaterMark(task_id));
}

void PERF_PrintStacks(void)
{
    TaskStatus_t *pxTaskStatusArray;
    TaskStatus_t *pxTaskStatus;
    UBaseType_t uxArraySize;
    UBaseType_t taskId;
    uint32_t pos        = 0;
    char *pcWriteBuffer = NULL;

    uxArraySize = uxTaskGetNumberOfTasks();

    pxTaskStatusArray = pvPortMalloc(uxArraySize * sizeof(TaskStatus_t));
    if (pxTaskStatusArray != NULL)
    {
        /* Generate the (binary) data. */
        uxArraySize = uxTaskGetSystemState(pxTaskStatusArray, uxArraySize, NULL);

        pcWriteBuffer = pvPortMalloc(128);
        if (pcWriteBuffer != NULL)
        {
            configPRINT_STRING("\r\n\r\n");
            configPRINT_STRING("===========================================\r\n");
            configPRINT_STRING("|      TASK NAME      |  UNUSED STACK [B] |\r\n");
            configPRINT_STRING("===========================================\r\n");

            for (taskId = 0; taskId < uxArraySize; taskId++)
            {
                pxTaskStatus = &pxTaskStatusArray[taskId];

                pos = sprintf(pcWriteBuffer, "| %-*s", configMAX_TASK_NAME_LEN, pxTaskStatus->pcTaskName);

                /* One pxTaskStatus->usStackHighWaterMark unit means 4 bytes.  */
                pos += sprintf((pcWriteBuffer + pos), "|      %*u       |", 6,
                               (4 * (uint16_t)pxTaskStatus->usStackHighWaterMark));

                configPRINT_STRING("%s\r\n", pcWriteBuffer);

                /* Give serial port some time to dump logs */
                vTaskDelay(10);
            }
            configPRINT_STRING("===========================================\r\n\r\n");

            vPortFree(pcWriteBuffer);
            pcWriteBuffer = NULL;
        }
        else
        {
            configPRINT_STRING("%s: No memory for print buffer\r\n", __func__);
        }

        vPortFree(pxTaskStatusArray);
        pxTaskStatusArray = NULL;
    }
    else
    {
        configPRINT_STRING("%s: No memory for storing task status\r\n", __func__);
    }
}
