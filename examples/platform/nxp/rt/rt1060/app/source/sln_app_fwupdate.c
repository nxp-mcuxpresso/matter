/*
 * Copyright 2019-2021 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "sln_app_fwupdate.h"
#include "fsl_common.h"
#include "sln_flash.h"
#include "sln_flash_config.h"
#include "fica_definition.h"
#include "cJSON.h"

#define FWUPDATE_MESSAGETYPE_START (2)

/*!
 * @brief Clear the bit from FICA header in order to indicate a fwupdate
 */
static int32_t FICA_clr_bits(uint32_t bit)
{
    uint8_t current_bit;
    uint32_t config;
    int32_t ret = -1;
    uint8_t tempPage[FLASH_PAGE_SIZE];

    ret = SLN_Read_Flash_At_Address(FICA_START_ADDR, tempPage, FLASH_PAGE_SIZE);
    if (ret != 0)
    {
        return -1;
    }
    config      = *(uint32_t *)(tempPage + FICA_OFFSET_ICA_COMM);
    current_bit = ((config & bit) > 0); // 0 or 1

    /* Check if the bit is already 0.
     * The bit must be set to 0 on active due to flash restrictions
     * We can flip a bit to 0 from 1 without a sector erase
     */
    if (current_bit == 0)
    {
        return 0;
    }

    config &= ~bit;
    *(uint32_t *)(tempPage + FICA_OFFSET_ICA_COMM) = config;

    ret = SLN_Write_Flash_At_Address(FICA_START_ADDR, tempPage);
    if (ret != 0)
    {
        return -1;
    }

    return 0;
}

int32_t FWUpdate_set_SLN_OTA(void)
{
    /* set update and method at the same time to avoid one failling */
    return FICA_clr_bits(FICA_COMM_FWUPDATE_BIT | FICA_COMM_FWUPDATE_METHOD_BIT);
}

int32_t FWUpdate_set_SLN_OTW(void)
{
    return FICA_clr_bits(FICA_COMM_FWUPDATE_BIT);
}

/*!
 * @brief Receive a buffer, check if the buffer received is an ota start command
 */
fwupdate_check_status_t FWUpdate_check_start_command(uint8_t *buffer)
{
    fwupdate_check_status_t status = FWUPDATE_OK;
    cJSON *json                    = NULL;
    cJSON *messageType             = NULL;

    if (buffer == NULL)
    {
        return FWUPDATE_JSON_ERROR;
    }
    json = cJSON_Parse((const char *)buffer);
    if (!json)
    {
        status = FWUPDATE_JSON_ERROR;
    }
    if (FWUPDATE_OK == status)
    {
        /* Check if it contains messagetype JSON Object */
        messageType = cJSON_GetObjectItemCaseSensitive(json, "messageType");
        if (!cJSON_IsNumber(messageType))
        {
            status = FWUPDATE_JSON_ERROR;
        }
        else
        {
            /* Check if messagetype is the one used for start fwupdate command  */
            if (messageType->valueint != FWUPDATE_MESSAGETYPE_START)
            {
                status = FWUPDATE_WRONG_MESSAGETYPE;
            }
        }
    }

    /* Free heap memory allocated with cJSON_Parse() */
    cJSON_Delete(json);

    return status;
}
