/*
 * Copyright 2018-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if ENABLE_SHELL

#ifndef __SLN_SHELL_H_
#define __SLN_SHELL_H_

#include "fsl_common.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "fsl_shell.h"
#include "fsl_str.h"
#include "fica_definition.h"

#include "clock_config.h"
#include "pin_mux.h"

/* FreeRTOS kernel includes */
#include "FreeRTOS.h"
#include "task.h"

#include "fsl_component_serial_manager.h"
#include "fsl_component_serial_port_usb.h"
#include "usb.h"
#include "usb_device.h"
#include "usb_device_class.h"
#include "usb_device_descriptor.h"
#include "usb_phy.h"

#include "event_groups.h"
#include "fsl_debug_console.h"
#include "usb_device_config.h"

#define APP_VECTOR_TABLE_APP_A (FLEXSPI_AMBA_BASE + FICA_IMG_APP_A_ADDR)
#define APP_VECTOR_TABLE_APP_B (FLEXSPI_AMBA_BASE + FICA_IMG_APP_B_ADDR)
#define JSON_FILEPATH_APPA     "AppA"
#define JSON_FILEPATH_APPB     "AppB"
#define JSON_FILEPATH_APPUNK   "UnknownApp"

#define CONTROLLER_ID kSerialManager_UsbControllerEhci0

/* Offset to the 30th MB + (1 sector) of Flash, having 256KB sectors */
#define USB_LOGGING_FLAG_FLASH_OFFSET 0xF200
#define USB_LOGGING_FLAG_ADDRESS      (USB_LOGGING_FLAG_FLASH_OFFSET * FLASH_PAGE_SIZE)

/* Offset to the 30th MB + (2 sectors) of Flash, having 256KB sectors */
#define HARDFAULT_LOGS_HISTORY_FLASH_OFFSET 0xF400
#define HARDFAULT_LOGS_HISTORY_ADDRESS      (HARDFAULT_LOGS_HISTORY_FLASH_OFFSET * FLASH_PAGE_SIZE)

/* Offset to the 30th MB + (3 sectors) of Flash, having 256KB sectors */
#define HARDFAULT_LOGS_HISTORY_SIZE_FLASH_OFFSET 0xF600
#define HARDFAULT_LOGS_HISTORY_SIZE_ADDRESS      (HARDFAULT_LOGS_HISTORY_SIZE_FLASH_OFFSET * FLASH_PAGE_SIZE)

#define MAX_LOG_HISTORY 20

/* Limited to 24 events */
typedef enum _shell_event
{
    RESET_EVENT          = (1 << 0U),
    VERSION_EVT          = (1 << 1U),
#if ENABLE_USB_AUDIO_DUMP
    AUDIO_DUMP_TEST_EVT  = (1 << 2U),
#endif /* ENABLE_USB_AUDIO_DUMP */
#if SLN_TRACE_CPU_USAGE
    TRACE_CPU_USAGE_EVT  = (1 << 3U),
#endif /* SLN_TRACE_CPU_USAGE */
    FACTORY_RESET_EVT    = (1 << 4U),
    PRINT_COMMANDS_EVT   = (1 << 5U),
    CHANGE_DEMO_EVT      = (1 << 6U),
#if ENABLE_VIT_ASR || ENABLE_DSMT_ASR
    CHANGE_LANG_EVT      = (1 << 7U),
#endif /* ENABLE_VIT_ASR || ENABLE_DSMT_ASR */
#if ENABLE_AMPLIFIER
    SET_VOLUME_EVT       = (1 << 8U),
#endif /* ENABLE_AMPLIFIER */
    ASR_MODE_EVT         = (1 << 9U),
    SET_TIMEOUT_EVT      = (1 << 10U),
    MICS_EVT             = (1 << 11U),
#if ENABLE_WIFI
    WIFI_EVT             = (1 << 12U),
#endif /* ENABLE_WIFI */
#if ENABLE_DSMT_ASR
    PRINT_CMD_RES_EVT    = (1 << 13U),
#endif /* ENABLE_DSMT_ASR */
    OTW_UPDATE_EVT       = (1 << 14U),
    CHANGE_BANK_EVT      = (1 << 15U),
#if ENABLE_AMPLIFIER
    PLAY_PROMPT_EVT      = (1 << 16U),
#endif /* ENABLE_AMPLIFIER */
    HEAP_VIEW_EVT        = (1 << 17U),
    STACKS_VIEW_EVT      = (1 << 18U),
    MEM_VIEW_EVT         = (1 << 19U),
#if ENABLE_AEC
    AEC_MODE_EVT         = (1 << 20U),
#endif /* ENABLE_AEC */
} shell_event_t;

typedef struct __shell_heap_trace
{
    uint8_t enable : 1;
    uint16_t threshold;
} shell_heap_trace_t;

typedef struct log_history_node
{
    void *log;
    void *next;
} log_history_node_t;

/**
 * @brief Main function called from shellTask. Initializes USB, SerialManager and SHELL.
 *
 * @return                        0 on success execution, 1 otherwise
 */
int sln_shell_init(void);

/**
 * @brief The task implements a basic SHELL function, supporting a limited number of commands
 *        and also having a history feature.
 *
 * @param arg[in]                 Pointer to an argument, in accordance with TaskFunction_t
 * @return                        Void.
 */
void sln_shell_task(void *arg);

int log_shell_printf(const char *formatString, ...);
status_t log_history_flash_save(void);

/*
 * @brief Shell callback for FreeRTOS heap trace on pvPortMalloc
 *
 * @param ptr Heap address that is allocated
 * @param size Size of allocation in bytes
 */
void sln_shell_trace_malloc(void *ptr, size_t size);

/*
 * @brief Shell callback for FreeRTOS heap trace on vPortFree
 *
 * @param ptr Heap address that is de-allocated
 * @param size Size of de-allocation in bytes
 */
void sln_shell_trace_free(void *ptr, size_t size);

#endif /* __SLN_SHELL_H */

#endif /* ENABLE_SHELL */
