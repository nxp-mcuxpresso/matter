/*
 * Copyright 2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef BINDING_TABLE_H_
#define BINDING_TABLE_H_

#include "sln_flash_ops.h"
#include "fsl_common.h"
#include "sln_flash.h"
#include "stdint.h"
#include "sln_flash_fs_ops.h"

#define BINDING_TABLE_FILE_NAME "binding_table.dat"
#define BINDING_TABLE_SIZE_FILE_NAME "binding_table_size.dat"

struct bindingHueSaturationData
{
    uint8_t hue;
    uint8_t saturation;
};

typedef enum
{
    kBindingBrightnessActionSet = 0,
    kBindingBrightnessActionIncrease,
    kBindingBrightnessActionDecrease
} bindingBrightnessAction_t;

struct bindingBrightnessData
{
    bindingBrightnessAction_t action;
    uint8_t level;
};

typedef struct {
    char deviceName[32];
    int entryId;
} bindingStruct;

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/**
 * @brief Reads binding table entries from flash memory
 *
 * @param arrayBindingStructs Pointer to an array of bindingStruct elements that represent the current binded devices
 * @param bindingEntriesNumber The current number of entries in the arrayBindingStructs
 */
sln_flash_fs_status_t binding_table_flash_get(bindingStruct** arrayBindingStructs, int *bindingEntriesNumber);

/**
 * @brief Writes binding table entries in flash memory
 *
 * @param arrayBindingStructs    Pointer to an array of bindingStruct elements that represent the current binded devices
 * @param bindingEntriesNumber   The current number of entries in the arrayBindingStructs
 * @param size                   The size of arrayBindingStructs, based on the number of elements and the size of bindingStruct
 */
sln_flash_fs_status_t binding_table_flash_set(bindingStruct* arrayBindingStructs, int *bindingEntriesNumber, uint8_t size);

/**
 * @brief Resets binding table entries stored in flash memory
 *
 * @return 0 on success, flash status otherwise
 */
sln_flash_fs_status_t binding_table_flash_reset(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* BINDING_TABLE_H_ */

