/*
 * Copyright 2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "binding_table.h"
#include "FreeRTOSConfig.h"

sln_flash_fs_status_t binding_table_flash_get(bindingStruct** arrayBindingStructs, int *bindingEntriesNumber)
{
    sln_flash_fs_status_t status             = SLN_FLASH_FS_OK;
    uint32_t len1                            = 0;
    uint32_t len2                            = 0;
    bindingStruct *arrayBindingStructsLocal  = NULL;

    if (arrayBindingStructs == NULL)
    {
        configPRINTF(("Failed receiving parameter arrayBindingStructs.\r\n"));
        status = SLN_FLASH_FS_FAIL;
    }
    else
    {
        status = sln_flash_fs_ops_read(BINDING_TABLE_SIZE_FILE_NAME, NULL, 0, &len2);
        if (status == SLN_FLASH_FS_OK)
        {
            status = sln_flash_fs_ops_read(BINDING_TABLE_SIZE_FILE_NAME, (uint8_t *)bindingEntriesNumber, 0, &len2);
        }

        if (*bindingEntriesNumber == 0)
        {
            configPRINTF(("No binded devices to read from flash.\r\n"));
        }
        else
        {
            arrayBindingStructsLocal = (bindingStruct *)pvPortMalloc(*bindingEntriesNumber * sizeof(bindingStruct));
            if (arrayBindingStructsLocal != NULL)
            {
                status = sln_flash_fs_ops_read(BINDING_TABLE_FILE_NAME, NULL, 0, &len1);
                if (status == SLN_FLASH_FS_OK)
                {
                    status = sln_flash_fs_ops_read(BINDING_TABLE_FILE_NAME, (uint8_t *)arrayBindingStructsLocal, 0, &len1);
                    *arrayBindingStructs = arrayBindingStructsLocal;
                }

                if (status != SLN_FLASH_FS_OK)
                {
                    configPRINTF(("Failed reading arrayBindingStructsLocal.\r\n"));
                }
            }
            else
            {
                configPRINTF(("Failed allocating arrayBindingStructsLocal.\r\n"));
                status = SLN_FLASH_FS_FAIL;
            }
        }
    }

    return status;
}

sln_flash_fs_status_t binding_table_flash_set(bindingStruct* arrayBindingStructs, int *bindingEntriesNumber, uint8_t size)
{
    sln_flash_fs_status_t status = SLN_FLASH_FS_OK;

    binding_table_flash_reset();

    status = sln_flash_fs_ops_save(BINDING_TABLE_FILE_NAME, (uint8_t *)arrayBindingStructs,
                                              size);

    status = sln_flash_fs_ops_save(BINDING_TABLE_SIZE_FILE_NAME, (uint8_t *)bindingEntriesNumber,
                                              4);
    if (status != SLN_FLASH_FS_OK)
    {
        configPRINTF(("Failed to save binding table entries in flash memory.\r\n"));
    }
    else
    {
        configPRINTF(("Updated binding table entries in flash memory.\r\n"));
    }

    return status;
}

sln_flash_fs_status_t binding_table_flash_reset(void)
{
    sln_flash_fs_status_t status = SLN_FLASH_FS_OK;

    status = sln_flash_fs_ops_erase(BINDING_TABLE_FILE_NAME);
    status = sln_flash_fs_ops_erase(BINDING_TABLE_SIZE_FILE_NAME);

    if (status != SLN_FLASH_FS_OK)
    {
        configPRINTF(("Failed to delete binding table entries from flash memory.\r\n\r\n"));
    }

    return status;
}

