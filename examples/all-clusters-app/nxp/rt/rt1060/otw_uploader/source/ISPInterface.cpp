/*
 *  Copyright (c) 2021, The OpenThread Authors.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

#include "ISPInterface.h"
#include "ot-rcp-fw-placeholder.bin.h"

using namespace ::chip::DeviceLayer;

#ifndef ISP_UART_INSTANCE
#define ISP_UART_INSTANCE 3
#endif
#ifndef ISP_ENABLE_RX_RTS
#define ISP_ENABLE_RX_RTS 0
#endif
#ifndef ISP_ENABLE_TX_RTS
#define ISP_ENABLE_TX_RTS 0
#endif

#ifndef ISP_UART_BAUD_RATE
#define ISP_UART_BAUD_RATE 115200
#endif

#ifndef ISP_UART_CLOCK_RATE
#define ISP_UART_CLOCK_RATE BOARD_BT_UART_CLK_FREQ
#endif

#ifndef ISP_DEFAULT_RESET_DELAY_MS
#define ISP_DEFAULT_RESET_DELAY_MS 2000 ///< Default delay after R̅E̅S̅E̅T̅ assertion, in miliseconds.
#endif

#ifndef RESET_PIN_PORT
#define RESET_PIN_PORT 6
#endif

#ifndef RESET_PIN_NUM
#define RESET_PIN_NUM 2
#endif

#ifndef DIO5_PIN_PORT
#define DIO5_PIN_PORT 6
#endif

#ifndef DIO5_PIN_NUM
#define DIO5_PIN_NUM 26
#endif

#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U) && (ISP_ENABLE_TX_RTS > 0U) && \
     (ISP_ENABLE_RX_RTS > 0U))
#error "DMA on UART with flow control not required"
#endif


#define ISP_MALLOC pvPortMalloc
#define ISP_FREE vPortFree

static void Uart_RxCallBackStatic(void *                             pData,
                                  serial_manager_callback_message_t *message,
                                  serial_manager_status_t            status)
{
    /* notify the main loop that a RX buffer is available */
#if (defined(SERIAL_MANAGER_TASK_HANDLE_RX_AVAILABLE_NOTIFY) && (SERIAL_MANAGER_TASK_HANDLE_RX_AVAILABLE_NOTIFY > 0U))
#else
#endif
}

static void Uart_TxCallBackStatic(void *                             pData,
                                  serial_manager_callback_message_t *message,
                                  serial_manager_status_t            status)
{
    /* Close the write handle */
    SerialManager_CloseWriteHandle((serial_write_handle_t)pData);
    /* Free the buffer */
    ISP_FREE(pData);
}

namespace RT {

teStatus ISPInterface::runOTWUpdater()
{
    teStatus status = E_PRG_OK;

    Init();

    status = eBL_CheckCrc((uint8_t *)ot_rcp_fw_placeholder_bin, sizeof(ot_rcp_fw_placeholder_bin));
    if (status == E_PRG_ERROR)
    {
        status = eBL_ProgramFirmware((uint8_t *)ot_rcp_fw_placeholder_bin, sizeof(ot_rcp_fw_placeholder_bin), CRC_ENABLED);
    }

    GPIODeinit();

    return status;
}

ISPInterface ISPInterface::getInstance()
{
    static ISPInterface ispSingleton;

    return ispSingleton;
}

ISPInterface::ISPInterface()
{
    isInitialized = false;
}

ISPInterface::~ISPInterface(void)
{
}

void ISPInterface::printMessage(uint8_t *message, uint32_t length, message_direction_t direction)
{
    if (direction)
    {
        PRINTF("TX: ");
    }
    else
    {
        PRINTF("RX: ");
    }
    for (int i = 0; i < length; i++)
    {
        PRINTF("%X ", message[i]);
    }
    PRINTF("\r\n");
}

inline void ISPInterface::putBuffer(const void *data, uint32_t length, uint8_t **pu8Store)
{
    memcpy(*pu8Store, data, length);
    *pu8Store += length;
}

/** Byte swap a 16 bit value.
 * @param x value to swap
 * @return byte swapped value
 */
inline uint16_t ISPInterface::_bswap16(uint16_t x)
{
    return (((x & 0xff00) >> 8) | ((x & 0x00ff) << 8));
}

/** Byte swap a 32 bit value.
 * @param x value to swap
 * @return byte swapped value
 */
inline uint32_t ISPInterface::_bswap32(uint32_t x)
{
    return (((x & 0xff000000) >> 24) | ((x & 0x00ff0000) >>  8) |
            ((x & 0x0000ff00) <<  8) | ((x & 0x000000ff) << 24));
}

inline uint32_t ISPInterface::le32btoh(uint8_t *pu8Store)
{
   uint32_t u32Le;
   memcpy(&u32Le, pu8Store, sizeof(uint32_t));
   return le32toh(u32Le);
}

inline uint32_t ISPInterface::be32btoh(uint8_t *pu8Store)
{
   uint32_t u32Be;
   memcpy(&u32Be, pu8Store, sizeof(uint32_t));
   return be32toh(u32Be);
}

inline void ISPInterface::put32b(uint32_t u32Host, uint8_t **pu8Store)
{
    uint32_t u32Be = htobe32(u32Host);
    memcpy(*pu8Store, &u32Be, sizeof(uint32_t));
    *pu8Store += sizeof(uint32_t);
}

inline void ISPInterface::put16b(uint16_t u16Host, uint8_t **pu8Store)
{
    uint16_t u16Be = htobe16(u16Host);
    memcpy(*pu8Store, &u16Be, sizeof(uint16_t));
    *pu8Store += sizeof(uint16_t);
}

inline void ISPInterface::put8(uint8_t u8Host, uint8_t **pu8Store)
{
    **pu8Store = u8Host;
    *pu8Store += sizeof(uint8_t);
}

void ISPInterface::TriggerReset(void)
{
    hal_gpio_status_t status;

    // Set Reset pin to low level.
    status = HAL_GpioSetOutput((hal_gpio_handle_t)GpioResetHandle, 0);
    assert(status == kStatus_HAL_GpioSuccess);

    // Set DIO5 pin to low level.
    status = HAL_GpioSetOutput((hal_gpio_handle_t)GpioDIO5Handle, 0);
    assert(status == kStatus_HAL_GpioSuccess);

    OSA_TimeDelay(10);

    // Set Reset pin to high level.
    status = HAL_GpioSetOutput((hal_gpio_handle_t)GpioResetHandle, 1);
    assert(status == kStatus_HAL_GpioSuccess);

    OSA_TimeDelay(10);

    // Set DIO5 pin to low level.
    status = HAL_GpioSetOutput((hal_gpio_handle_t)GpioDIO5Handle, 1);
    assert(status == kStatus_HAL_GpioSuccess);
}

void ISPInterface::Init(void)
{
    hal_gpio_status_t     status;
    hal_gpio_pin_config_t resetPinConfig = {
        .direction = kHAL_GpioDirectionOut,
        .level     = 0U,
        .port      = RESET_PIN_PORT,
        .pin       = RESET_PIN_NUM,
    };
    hal_gpio_pin_config_t DIO5PinConfig = {
        .direction = kHAL_GpioDirectionOut,
        .level     = 0U,
        .port      = DIO5_PIN_PORT,
        .pin       = DIO5_PIN_NUM,
    };

    if (!isInitialized)
    {
        InitUart();

        mResetDelay = ISP_DEFAULT_RESET_DELAY_MS;

        /* Init the reset output gpio */
        status = HAL_GpioInit((hal_gpio_handle_t)GpioResetHandle, &resetPinConfig);
        assert(status == kStatus_HAL_GpioSuccess);

        status = HAL_GpioInit((hal_gpio_handle_t)GpioDIO5Handle, &DIO5PinConfig);
        assert(status == kStatus_HAL_GpioSuccess);

        /* Reset RCP chip. */
        TriggerReset();

        /* Waiting for the RCP chip starts up */
        OSA_TimeDelay(mResetDelay);
        isInitialized = true;
    }

    OT_UNUSED_VARIABLE(status);
}

void ISPInterface::GPIODeinit(void)
{
    HAL_GpioDeinit(GpioResetHandle);
    HAL_GpioDeinit(GpioDIO5Handle);
}

void ISPInterface::Deinit(void)
{
    HAL_GpioDeinit(GpioResetHandle);
    HAL_GpioDeinit(GpioDIO5Handle);
    DeinitUart();
}

teStatus ISPInterface::eBL_Reset()
{
    teBL_Response eResponse = E_BL_RESPONSE_OK;
    teBL_MessageType eRxType = E_BL_MSG_TYPE_NONE;

    eResponse = eBL_Request(E_BL_MSG_TYPE_RESET_REQUEST, 0, NULL, 0, NULL, &eRxType, NULL, NULL, "Reset");
    return eBL_CheckResponse(eResponse, eRxType, E_BL_MSG_TYPE_RESET_RESPONSE);
}

teStatus ISPInterface::eBL_Connect()
{
    teStatus eStatus = E_PRG_OK;
    uint32_t u32BaudRate = 0;
    uint32_t u32ChipId;
    uint32_t u32BootloaderVersion;

    uint8_t key[] = {17, 34, 51, 68, 85, 102, 119, 136, 17, 34, 51, 68, 85, 102, 119, 136};

    eStatus = eBL_Unlock(1, key, 16);

    if (eStatus == E_PRG_OK)
    {
        eStatus = eBL_ChipIdRead(&u32ChipId, &u32BootloaderVersion);
    }

    return eStatus;
}

teStatus ISPInterface::eBL_ChipIdRead(uint32_t *pu32ChipId, uint32_t *pu32BootloaderVersion)
{

    teBL_Response eResponse = E_BL_RESPONSE_OK;
    teBL_MessageType eRxType = E_BL_MSG_TYPE_NONE;
    uint16_t u16RxDataLen = 0;
    uint8_t au8Buffer[BOOTLOADER_MAX_MESSAGE_LENGTH];

    if (pu32ChipId == NULL)
    {
        return E_PRG_NULL_PARAMETER;
    }

    if (pu32BootloaderVersion == NULL)
    {
        return E_PRG_NULL_PARAMETER;
    }

    eResponse = eBL_Request(E_BL_MSG_TYPE_GET_CHIPID_REQUEST, 0, NULL, 0, NULL, &eRxType, &u16RxDataLen, au8Buffer, "Get Device Info");

    if ((u16RxDataLen != 4) && (u16RxDataLen != 8))
    {
        return E_PRG_COMMS_FAILED;
    }

    *pu32ChipId  = au8Buffer[0] << 24;
    *pu32ChipId |= au8Buffer[1] << 16;
    *pu32ChipId |= au8Buffer[2] << 8;
    *pu32ChipId |= au8Buffer[3] << 0;

    if (u16RxDataLen == 8)
    {
        // Bootloader version is included
        *pu32BootloaderVersion  = au8Buffer[4] << 24;
        *pu32BootloaderVersion |= au8Buffer[5] << 16;
        *pu32BootloaderVersion |= au8Buffer[6] << 8;
        *pu32BootloaderVersion |= au8Buffer[7] << 0;

    } else {
        *pu32BootloaderVersion = 0;
    }

    return eBL_CheckResponse(eResponse, eRxType, E_BL_MSG_TYPE_GET_CHIPID_RESPONSE);
}

teStatus ISPInterface::eBL_SetBaudrate(uint32_t u32Baudrate)
{

    teBL_Response eResponse = E_BL_RESPONSE_OK;
    teBL_MessageType eRxType = E_BL_MSG_TYPE_NONE;
    uint8_t au8Buffer[6];
    uint32_t u32Divisor;

    // Divide 1MHz clock by baudrate to get the divisor
    //9 for 115 K, 1 for 1M
    u32Divisor = (uint32_t)(1000000 / u32Baudrate);
    if ((1000000 % u32Baudrate) > (u32Baudrate/2))
    {
        u32Divisor++;
    }

    au8Buffer[0] = (uint8_t)u32Divisor;
    au8Buffer[1] = 0;
    au8Buffer[2] = 0;
    au8Buffer[3] = 0;
    au8Buffer[4] = 0;

    eResponse = eBL_Request(E_BL_MSG_TYPE_SET_BAUD_REQUEST, 5, au8Buffer, 0, NULL, &eRxType, NULL, NULL, "Set Baudrate");
    return eBL_CheckResponse(eResponse, eRxType, E_BL_MSG_TYPE_SET_BAUD_RESPONSE);
}

teStatus ISPInterface::eBL_MemBlank(uint8_t u8Index, uint32_t u32Address, uint32_t u32Length)
{
    uint8_t au8CmdBuffer[10];
    teBL_Response eResponse = E_BL_RESPONSE_OK;
    teBL_MessageType eRxType = E_BL_MSG_TYPE_NONE;
    uint16_t u16RxLength;
    uint8_t u8RxBuffer[BOOTLOADER_MAX_MESSAGE_LENGTH];

    au8CmdBuffer[0] = u8Index;

    au8CmdBuffer[1] = 0; /* Mode */

    au8CmdBuffer[2] = (uint8_t)(u32Address >> 0)  & 0xff;
    au8CmdBuffer[3] = (uint8_t)(u32Address >> 8)  & 0xff;
    au8CmdBuffer[4] = (uint8_t)(u32Address >> 16) & 0xff;
    au8CmdBuffer[5] = (uint8_t)(u32Address >> 24) & 0xff;

    au8CmdBuffer[6] = (uint8_t)(u32Length >> 0)  & 0xff;
    au8CmdBuffer[7] = (uint8_t)(u32Length >> 8)  & 0xff;
    au8CmdBuffer[8] = (uint8_t)(u32Length >> 16) & 0xff;
    au8CmdBuffer[9] = (uint8_t)(u32Length >> 24) & 0xff;

    eResponse = eBL_Request(TYPE_MEM_BLANK_CHECK_REQUEST, 10, au8CmdBuffer, 0, NULL, &eRxType, &u16RxLength, u8RxBuffer, "Check Blank Memory");

    return eBL_CheckResponse(eResponse, eRxType, TYPE_MEM_BLANK_CHECK_RESPONSE);
}

teStatus ISPInterface::eBL_MemClose(uint8_t u8Index)
{
    uint8_t au8CmdBuffer[18];
    teBL_Response eResponse = E_BL_RESPONSE_OK;
    teBL_MessageType eRxType = E_BL_MSG_TYPE_NONE;
    uint16_t u16RxLength;
    uint8_t u8RxBuffer[BOOTLOADER_MAX_MESSAGE_LENGTH];

    au8CmdBuffer[0] = u8Index;

    eResponse = eBL_Request(TYPE_MEM_CLOSE_REQUEST, 1, au8CmdBuffer, 0, NULL, &eRxType, &u16RxLength, u8RxBuffer, "Close Memory");

    return eBL_CheckResponse(eResponse, eRxType, TYPE_MEM_CLOSE_RESPONSE);
}

teStatus ISPInterface::eBL_MemWrite(uint8_t u8Index, uint32_t u32Address, uint32_t u32Length, uint8_t *pu8Buffer)
{
    uint8_t au8CmdBuffer[10];
    teBL_Response eResponse = E_BL_RESPONSE_OK;
    teBL_MessageType eRxType = E_BL_MSG_TYPE_NONE;

    if (pu8Buffer == NULL)
    {
        return E_PRG_BAD_PARAMETER;
    }

    au8CmdBuffer[0] = u8Index;

    au8CmdBuffer[1] = 0; /* Mode */

    au8CmdBuffer[2] = (uint8_t)(u32Address >> 0)  & 0xff;
    au8CmdBuffer[3] = (uint8_t)(u32Address >> 8)  & 0xff;
    au8CmdBuffer[4] = (uint8_t)(u32Address >> 16) & 0xff;
    au8CmdBuffer[5] = (uint8_t)(u32Address >> 24) & 0xff;

    au8CmdBuffer[6] = (uint8_t)(u32Length >> 0)  & 0xff;
    au8CmdBuffer[7] = (uint8_t)(u32Length >> 8)  & 0xff;
    au8CmdBuffer[8] = (uint8_t)(u32Length >> 16) & 0xff;
    au8CmdBuffer[9] = (uint8_t)(u32Length >> 24) & 0xff;

    eResponse = eBL_Request(TYPE_MEM_WRITE_REQUEST, 10, au8CmdBuffer, u32Length, pu8Buffer, &eRxType, NULL, NULL, "Write Memory");

    return eBL_CheckResponse(eResponse, eRxType, TYPE_MEM_WRITE_RESPONSE);
}

teStatus ISPInterface::eBL_WriteMultiple(uint8_t *pu8Data, uint32_t u32DataLength, uint32_t u32Offset, tsMemInfo *psMemInfo)
{
    teStatus eStatus;
    int n;
    uint32_t u32ChunkSize;

    char acOperationText[256];

    if ((u32Offset + u32DataLength) > psMemInfo->u32Size)
    {
        ChipLogError(DeviceLayer, "Image larger than memory!");
        return E_PRG_INCOMPATIBLE;
    }

    snprintf(acOperationText, sizeof(acOperationText), "Programming %s", psMemInfo->pcMemName);

    ChipLogProgress(DeviceLayer, "%s", acOperationText);

    for (n = 0; n < u32DataLength; n += u32ChunkSize)
    {
        if ((u32DataLength - n) > FLASH_SECTOR_SIZE)
        {
            u32ChunkSize = FLASH_SECTOR_SIZE;
        }
        else
        {
            u32ChunkSize = u32DataLength - n;
        }

        if ((eStatus = eBL_MemWrite(E_BL_FLASH_ID, psMemInfo->u32BaseAddress + u32Offset + n, u32ChunkSize, pu8Data + n)) != E_PRG_OK)
        {
            ChipLogError(DeviceLayer, "Writing memory at address 0x%08lu", psMemInfo->u32BaseAddress + u32Offset + n);
            return E_PRG_ERROR;
        }
    }

    ChipLogProgress(DeviceLayer, "Memory written successfully!");

    return E_PRG_OK;
}

teStatus ISPInterface::eBL_MemErase(uint8_t u8Index, uint32_t u32Address, uint32_t u32Length)
{
    uint8_t au8CmdBuffer[10];
    teBL_Response eResponse = E_BL_RESPONSE_OK;
    teBL_MessageType eRxType = E_BL_MSG_TYPE_NONE;
    uint16_t u16RxLength;
    uint8_t u8RxBuffer[BOOTLOADER_MAX_MESSAGE_LENGTH];

    au8CmdBuffer[0] = u8Index;

    au8CmdBuffer[1] = 0; /* Mode */

    au8CmdBuffer[2] = (uint8_t)(u32Address >> 0)  & 0xff;
    au8CmdBuffer[3] = (uint8_t)(u32Address >> 8)  & 0xff;
    au8CmdBuffer[4] = (uint8_t)(u32Address >> 16) & 0xff;
    au8CmdBuffer[5] = (uint8_t)(u32Address >> 24) & 0xff;

    au8CmdBuffer[6] = (uint8_t)(u32Length >> 0)  & 0xff;
    au8CmdBuffer[7] = (uint8_t)(u32Length >> 8)  & 0xff;
    au8CmdBuffer[8] = (uint8_t)(u32Length >> 16) & 0xff;
    au8CmdBuffer[9] = (uint8_t)(u32Length >> 24) & 0xff;

    eResponse = eBL_Request(TYPE_MEM_ERASE_REQUEST, 10, au8CmdBuffer, 0, NULL, &eRxType, &u16RxLength, u8RxBuffer, "Erase Memory");

    return eBL_CheckResponse(eResponse, eRxType, TYPE_MEM_ERASE_RESPONSE);
}

teStatus ISPInterface::eBL_MemInfo(uint8_t u8Index, tsMemInfo *psMemInfo)
{
    teStatus status;
    uint8_t au8CmdBuffer[18];
    teBL_Response eResponse = E_BL_RESPONSE_OK;
    teBL_MessageType eRxType = E_BL_MSG_TYPE_NONE;
    uint16_t u16RxLength;
    uint16_t u16NameLen;
    uint8_t u8RxBuffer[BOOTLOADER_MAX_MESSAGE_LENGTH];

    au8CmdBuffer[0] = u8Index;

    eResponse = eBL_Request(TYPE_MEM_GET_INFO_REQUEST, 1, au8CmdBuffer, 0, NULL, &eRxType, &u16RxLength, u8RxBuffer, "Get Memory Info");

    status = eBL_CheckResponse(eResponse, eRxType, TYPE_MEM_GET_INFO_RESPONSE);

    if (status == E_PRG_OK)
    {
        psMemInfo->u8Index = u8RxBuffer[0];
        psMemInfo->u32BaseAddress = le32btoh(&u8RxBuffer[1]);
        psMemInfo->u32Size = le32btoh(&u8RxBuffer[5]);
        psMemInfo->u32BlockSize = le32btoh(&u8RxBuffer[9]);
        /* Memory type - unused */
        psMemInfo->u8Access = u8RxBuffer[14];

        u16NameLen = u16RxLength - 15;

        if (u16NameLen > 0)
        {
            psMemInfo->pcMemName = (char *)malloc(u16NameLen + 1);
            memcpy(psMemInfo->pcMemName, &u8RxBuffer[15], u16NameLen);
            psMemInfo->pcMemName[u16NameLen] = '\0';
        }

        uint32_t u32Size = psMemInfo->u32Size;
        char pcSizeUnit[3] = "b";

        if (u32Size > 1024)
        {
            u32Size = psMemInfo->u32Size  / 1024;
            strcpy(pcSizeUnit, "Kb");
        }
        ChipLogProgress(DeviceLayer, "\r\n%d '%s'\r\nBase: 0x%lu\r\nLength: %3lu%s\r\nBlock: %3lub",
                psMemInfo->u8Index, psMemInfo->pcMemName, psMemInfo->u32BaseAddress, u32Size, pcSizeUnit, psMemInfo->u32BlockSize);
    }

    return status;
}

teStatus ISPInterface::eBL_MemRead(uint8_t u8Index, uint32_t u32Address, uint32_t *pu32Length, uint8_t *pu8Buffer)
{
    uint16_t u16RxDataLen = 0;
    uint8_t au8CmdBuffer[10];
    teBL_Response eResponse = E_BL_RESPONSE_OK;
    teBL_MessageType eRxType = E_BL_MSG_TYPE_NONE;

    if (pu8Buffer == NULL)
    {
        return E_PRG_BAD_PARAMETER;
    }

    au8CmdBuffer[0] = u8Index;

    au8CmdBuffer[1] = 0; /* Mode */

    au8CmdBuffer[2] = (uint8_t)(u32Address >> 0)  & 0xff;
    au8CmdBuffer[3] = (uint8_t)(u32Address >> 8)  & 0xff;
    au8CmdBuffer[4] = (uint8_t)(u32Address >> 16) & 0xff;
    au8CmdBuffer[5] = (uint8_t)(u32Address >> 24) & 0xff;

    au8CmdBuffer[6] = (uint8_t)(*pu32Length >> 0)  & 0xff;
    au8CmdBuffer[7] = (uint8_t)(*pu32Length >> 8)  & 0xff;
    au8CmdBuffer[8] = (uint8_t)(*pu32Length >> 16) & 0xff;
    au8CmdBuffer[9] = (uint8_t)(*pu32Length >> 24) & 0xff;

    eResponse = eBL_Request(TYPE_MEM_READ_REQUEST, 10, au8CmdBuffer, 0, NULL, &eRxType, &u16RxDataLen, pu8Buffer, "Read Memory");

    if (u16RxDataLen == 0)
    {
        return E_PRG_ERROR_READING;
    }

    *pu32Length = u16RxDataLen;

    return eBL_CheckResponse(eResponse, eRxType, TYPE_MEM_READ_RESPONSE);
}

teStatus ISPInterface::eBL_MemOpen(uint8_t u8Index, uint8_t u8AccessMode)
{
    uint8_t au8CmdBuffer[18];
    teBL_Response eResponse = E_BL_RESPONSE_OK;
    teBL_MessageType eRxType = E_BL_MSG_TYPE_NONE;
    uint16_t u16RxLength;
    uint8_t u8RxBuffer[BOOTLOADER_MAX_MESSAGE_LENGTH];

    au8CmdBuffer[0] = u8Index;
    au8CmdBuffer[1] = 0x0f;

    eResponse = eBL_Request(TYPE_MEM_OPEN_REQUEST, 2, au8CmdBuffer, 0, NULL, &eRxType, &u16RxLength, u8RxBuffer, "Open Memory");

    return eBL_CheckResponse(eResponse, eRxType, TYPE_MEM_OPEN_RESPONSE);
}

otError ISPInterface::Write(const uint8_t *aFrame, uint16_t aLength)
{
    serial_manager_status_t status;
    otError otResult = OT_ERROR_FAILED;
    serial_manager_status_t serialManagerStatus;
    uint8_t *pWriteHandleAndFrame = (uint8_t *)ISP_MALLOC(SERIAL_MANAGER_WRITE_HANDLE_SIZE + aLength);
    uint8_t *pNewFrameBuffer      = NULL;

    do
    {
        if (pWriteHandleAndFrame == NULL)
        {
            ChipLogError(DeviceLayer, "Frame alloc failure!");
            break;
        }
        pNewFrameBuffer = pWriteHandleAndFrame + SERIAL_MANAGER_WRITE_HANDLE_SIZE;
        memcpy(pNewFrameBuffer, aFrame, aLength);
        serialManagerStatus = SerialManager_OpenWriteHandle((serial_handle_t)SerialHandle,
                                                            (serial_write_handle_t)pWriteHandleAndFrame);
        if (serialManagerStatus != kStatus_SerialManager_Success)
            break;
        serialManagerStatus = SerialManager_InstallTxCallback((serial_write_handle_t)pWriteHandleAndFrame,
                                                              Uart_TxCallBackStatic, pWriteHandleAndFrame);
        if (serialManagerStatus != kStatus_SerialManager_Success)
            break;
        serialManagerStatus =
            SerialManager_WriteNonBlocking((serial_write_handle_t)pWriteHandleAndFrame, pNewFrameBuffer, aLength);
        if (serialManagerStatus != kStatus_SerialManager_Success)
            break;
        otResult = OT_ERROR_NONE;
    } while (0);

    if (otResult != OT_ERROR_NONE && pWriteHandleAndFrame != NULL)
    {
        status = SerialManager_CloseWriteHandle((serial_write_handle_t)pWriteHandleAndFrame);
        assert(status == kStatus_SerialManager_Success);
        ISP_FREE(pWriteHandleAndFrame);
    }

    return otResult;
}


teStatus ISPInterface::eBL_CheckResponse(teBL_Response eResponse, teBL_MessageType eRxType, teBL_MessageType eExpectedRxType)
{
    switch (eResponse)
    {
        case(E_BL_RESPONSE_OK):
            break;
        case (E_BL_RESPONSE_NOT_SUPPORTED):
            return E_PRG_UNSUPPORTED_OPERATION;
        case (E_BL_RESPONSE_WRITE_FAIL):
            return E_PRG_ERROR_WRITING;
        case (E_BL_RESPONSE_READ_FAIL):
            return E_PRG_ERROR_READING;
        default:
            return E_PRG_COMMS_FAILED;
    }

    if (eRxType != eExpectedRxType)
    {
        return E_PRG_COMMS_FAILED;
    }
    return E_PRG_OK;
}

teStatus ISPInterface::eBL_Unlock(uint8_t u8Mode, uint8_t *pu8Key, uint16_t u16KeyLen)
{
    teBL_Response eResponse = E_BL_RESPONSE_OK;
    teBL_MessageType eRxType = E_BL_MSG_TYPE_NONE;
    uint8_t au8Buffer[1];

    au8Buffer[0] = u8Mode;

    eResponse = eBL_Request(TYPE_UNLOCK_ISP_REQUEST, 1, au8Buffer, u16KeyLen, pu8Key, &eRxType, NULL, NULL, "Unlock");
    return eBL_CheckResponse(eResponse, eRxType, TYPE_UNLOCK_ISP_RESPONSE);
}

teStatus ISPInterface::eBL_CheckCrc(uint8_t *pu8Data, uint32_t u32DataLength)
{
    uint8_t received_crc[K32W0_FW_CRC_SIZE];
    uint32_t received_crc_size = sizeof(received_crc);
    uint32_t int_received_crc;
    uint32_t crc;
    uint32_t crc_offset = FLASH_SECTOR_SIZE - u32DataLength % FLASH_SECTOR_SIZE;
    teStatus status = E_PRG_OK;

    crc = eBL_CalculateCrc((uint8_t *)pu8Data, u32DataLength);

    status = eBL_RunReadMemoryProcedure(received_crc, &received_crc_size, u32DataLength + crc_offset);

    if (status == E_PRG_OK)
    {
        int_received_crc = be32btoh((uint8_t*)received_crc);

        if (int_received_crc != crc)
        {
            ChipLogProgress(DeviceLayer, "Apply FW update. Current FW and new FW differ!");
            status = E_PRG_ERROR;
        }
        else
        {
            ChipLogProgress(DeviceLayer, "Skip FW update. Current FW and new FW are the same!");
        }
    }

    return status;
}

uint32_t ISPInterface::eBL_CalculateCrc(uint8_t *msg, uint32_t length)
{
    hal_crc_config_t config = {
        .crcRefIn           = KHAL_CrcRefInput,
        .crcRefOut          = KHAL_CrcRefOutput,
        .crcByteOrder       = KHAL_CrcLSByteFirst,
        .crcSeed            = 0xFFFFFFFF,
        .crcPoly            = KHAL_CrcPolynomial_CRC_32,
        .crcXorOut          = 0xFFFFFFFF,
        .complementChecksum = false,
        .crcSize            = 4,
        .crcStartByte       = 0,
    };

    return HAL_CrcCompute(&config, msg, length);
}

teBL_Response ISPInterface::eBL_ReadMessage(teBL_MessageType *peType,
                            uint16_t *pu16Length,
                            uint8_t *pu8Data)
{
    int n;
    //teStatus eStatus;
    uint8_t au8Msg[BOOTLOADER_MAX_MESSAGE_LENGTH] = { E_BL_MSG_TYPE_NONE };
    uint16_t u16Length = 0;
    teBL_Response eResponse = E_BL_RESPONSE_OK;

    /* Get the length byte */
    u16Length = pu8Data[2];

    /* Message must have at least 3 bytes, maximum is implicit */
    if (u16Length < 3)
    {
        return E_BL_RESPONSE_NO_RESPONSE;
    }

    /* Get the rest of the message */
    memcpy(au8Msg, pu8Data, u16Length);

    /* Try and receive the rest of the message */
    if (u16Length >= BOOTLOADER_MAX_MESSAGE_LENGTH)
    {
        return E_BL_RESPONSE_BAD_STATE;
    }

    /* Calculate CRC of message */
    uint32_t crc;

    crc = eBL_CalculateCrc((uint8_t *)au8Msg, u16Length - 4);

    uint32_t received_crc = be32btoh((uint8_t*)(&au8Msg[u16Length - 4]));

    if (crc != received_crc)
    {
        return E_BL_RESPONSE_CRC_ERROR;
    }

    *peType = (teBL_MessageType)au8Msg[3]; //3
    eResponse = (teBL_Response)au8Msg[4]; //4
    if (pu16Length)
    {
        *pu16Length = u16Length - 9;

        if (pu8Data)
        {
            memcpy(pu8Data, &au8Msg[5], *pu16Length); //5
        }
    }

    return eResponse;
}

teStatus ISPInterface::eBL_WriteMessage(
                            teBL_MessageType eType,
                            uint16_t u8HeaderLength,
                            uint8_t *pu8Header,
                            uint16_t u16DataLength,
                            const uint8_t *pu8Data,
                            uint8_t **pu8RcvdData)
{
    int n;
    uint8_t *pu8NextHash = NULL;

    uint8_t au8Msg[BOOTLOADER_MAX_MESSAGE_LENGTH];
    uint8_t *pu8Msg = au8Msg;

    uint8_t u8Flags = 0;
    uint16_t u16Length;

    u16Length = u8HeaderLength + u16DataLength + 8;

    if (u16Length >= BOOTLOADER_MAX_MESSAGE_LENGTH)
    {
        return E_PRG_BAD_PARAMETER;
    }

    if (pu8NextHash != NULL)
    {
        u16Length += 32;
        u8Flags |= (1 << 2);
    }

    /* Message flags */
    put8(u8Flags, &pu8Msg);

    /* Message length */
    put16b(u16Length, &pu8Msg);

    /* Message type */
    put8((uint8_t)eType, &pu8Msg);


    if (u8HeaderLength > 0)
    {
        /* Message header */
        putBuffer(pu8Header, u8HeaderLength, &pu8Msg);
    }

    if (u16DataLength > 0)
    {
        /* Message payload */
        putBuffer(pu8Data, u16DataLength, &pu8Msg);
    }

    if (pu8NextHash != NULL)
    {
        for (n = 0; n < 32; n++)
        {
            *pu8Msg++ = n;
        }
    }

    /* Calculate CRC of message */
    uint32_t crc;

    crc = eBL_CalculateCrc((uint8_t *)au8Msg, (pu8Msg - au8Msg));

    put32b(crc, &pu8Msg);

    uint32_t length = pu8Msg - au8Msg;

    Write((uint8_t *)au8Msg, length);

    return E_PRG_OK;
}

uint32_t ISPInterface::TryRead(uint8_t *mUartRxBuffer, uint32_t size, uint64_t aTimeoutUs)
{
    uint32_t totalBytesRead = 0U;
    uint32_t bytesRead      = 0;
    uint32_t now   = otPlatAlarmMilliGetNow();
    uint32_t start = now;

    do
    {
        OSA_InterruptDisable();
        SerialManager_TryRead((serial_read_handle_t)SerialReadHandle, mUartRxBuffer, size,
                              &bytesRead);
        OSA_InterruptEnable();
        if (bytesRead != 0)
        {
            totalBytesRead += bytesRead;
        }
        OSA_TimeDelay(10);
        now = otPlatAlarmMilliGetNow();
    } while (totalBytesRead < size && ((now - start) < aTimeoutUs / 1000));

    return totalBytesRead;
}

teBL_Response ISPInterface::eBL_Request(
                        teBL_MessageType eTxType,
                        uint8_t u8HeaderLen,
                        uint8_t *pu8Header,
                        uint16_t u16TxLength,
                        uint8_t *pu8TxData,
                        teBL_MessageType *peRxType,
                        uint16_t *pu16RxLength,
                        uint8_t *pu8RxData,
                        const char* funcName)
{
    uint8_t *pu8LocalRcvdData;
    uint8_t au8Msg[BOOTLOADER_MAX_MESSAGE_LENGTH] = { 0 };
    uint8_t uartProgBuffer[MAX_RX_LARGE_SERIAL_BUFFER_SIZE];
    teBL_Response eResponse;
    uint32_t n;
    int error;

    /* Check data is not too long */
    if (u16TxLength > 65530)
    {
        return E_BL_RESPONSE_BAD_STATE;
    }

    ChipLogProgress(DeviceLayer, "Send %s to K32W0!", funcName);

    /* Send message and get response */

    if (eBL_WriteMessage(eTxType, u8HeaderLen, pu8Header, u16TxLength, pu8TxData, &pu8LocalRcvdData) != E_PRG_OK)
    {
        return E_BL_RESPONSE_BAD_STATE;
    }

    /* This delay is needed for the K32W0 to respond */
    OSA_TimeDelay(K32W0_RESPONSE_DELAY_MS);

    ChipLogProgress(DeviceLayer, "Recv %s response from K32W0!", funcName);

    memset(uartProgBuffer, 0, MAX_RX_LARGE_SERIAL_BUFFER_SIZE);

    n = TryRead(uartProgBuffer, ISP_LENGTH_READ, DEFAULT_READ_WAIT_MS);
    n += TryRead(uartProgBuffer + ISP_LENGTH_READ, uartProgBuffer[2] - ISP_LENGTH_READ, DEFAULT_READ_WAIT_MS);

    if (n != uartProgBuffer[2] || n <= 0)
    {
        return E_BL_RESPONSE_NO_RESPONSE;
    }

    eResponse = eBL_ReadMessage(peRxType, pu16RxLength, uartProgBuffer);
    if (pu8RxData)
    {
        memcpy(pu8RxData,uartProgBuffer,*pu16RxLength);
    }
    /* Get the response to the request */
    return eResponse;
    OT_UNUSED_VARIABLE(n);
}


teStatus ISPInterface::eBL_RunUnlockProcedure()
{
    /* default unlock key is 0x11223344556677881122334455667788 */
    uint8_t key[] = {17, 34, 51, 68, 85, 102, 119, 136, 17, 34, 51, 68, 85, 102, 119, 136};
    uint32_t u32ChipId;
    uint32_t u32BootloaderVersion;
    teStatus status = E_PRG_OK;

    /* Unlock/Initialization Procedure */
    status = eBL_Unlock(0, NULL, 0);

    if (status == E_PRG_OK)
    {
        status = eBL_ChipIdRead(&u32ChipId, &u32BootloaderVersion);
    }

    if (status == E_PRG_OK)
    {
        status = eBL_Unlock(1, key, 16);
    }

    return status;
}

teStatus ISPInterface::eBL_RunEraseAllProcedure()
{
    teStatus status = E_PRG_OK;

    /* Erase Memory Procedure*/
    status = eBL_MemOpen(E_BL_FLASH_ID, E_BL_ERASE_ALL | E_BL_BLANK_CHECK);

    if (status == E_PRG_OK)
    {
        status = eBL_MemErase(E_BL_FLASH_ID, 0x0, K32W061_MEMORY_SIZE);
    }

    if (status == E_PRG_OK)
    {
        status = eBL_MemBlank(E_BL_FLASH_ID, 0x0, K32W061_MEMORY_SIZE);
    }

    if (status == E_PRG_OK)
    {
        status = eBL_MemClose(E_BL_FLASH_ID);
    }
    else
    {
        eBL_MemClose(E_BL_FLASH_ID);
    }

    return status;
}

teStatus ISPInterface::eBL_RunProgramProcedure(uint8_t *pu8Data, uint32_t u32DataLength, uint32_t u32Offset)
{
    tsMemInfo psMemInfo;
    teStatus status = E_PRG_OK;

    /* Retrieve Memory Info */
    status = eBL_MemInfo(E_BL_FLASH_ID, &psMemInfo);

    /* Program Memory */
    if (status == E_PRG_OK)
    {
        status = eBL_MemOpen(E_BL_FLASH_ID, E_BL_WRITE);
    }

    if (status == E_PRG_OK)
    {
        status = eBL_WriteMultiple(pu8Data, u32DataLength, u32Offset, &psMemInfo);
    }

    if (status == E_PRG_OK)
    {
        status = eBL_MemClose(E_BL_FLASH_ID);
    }
    else
    {
       eBL_MemClose(E_BL_FLASH_ID);
    }

    return status;
}

teStatus ISPInterface::eBL_RunProgramProcedureCRC(uint8_t *pu8Data, uint32_t u32DataLength, uint32_t u32Offset)
{
    tsMemInfo psMemInfo;
    teStatus status = E_PRG_OK;

    /* Retrieve Memory Info */
    status = eBL_MemInfo(E_BL_FLASH_ID, &psMemInfo);

    /* Program Memory */
    if (status == E_PRG_OK)
    {
        status = eBL_MemOpen(E_BL_FLASH_ID, E_BL_WRITE);
    }

    if (status == E_PRG_OK)
    {
        status = eBL_WriteMultiple(pu8Data, u32DataLength, u32Offset, &psMemInfo);
    }

    if (status == E_PRG_OK)
    {
        uint8_t crcMsg[4];
        uint8_t *pcrcMsg = crcMsg;
        uint32_t crc_offset = FLASH_SECTOR_SIZE - u32DataLength%FLASH_SECTOR_SIZE;
        uint32_t crc = eBL_CalculateCrc((uint8_t *)pu8Data, u32DataLength);

        put32b(crc, &pcrcMsg);

        status = eBL_WriteMultiple(crcMsg, sizeof(uint32_t), u32Offset + u32DataLength + crc_offset, &psMemInfo);
    }

    if (status == E_PRG_OK)
    {
        status = eBL_MemClose(E_BL_FLASH_ID);
    }
    else
    {
       eBL_MemClose(E_BL_FLASH_ID);
    }

    return status;
}

teStatus ISPInterface::eBL_RunReadMemoryProcedure(uint8_t *readBuf, uint32_t *readSize, uint32_t offset)
{
    tsMemInfo psMemInfo;
    teStatus status = E_PRG_OK;

    status = eBL_RunUnlockProcedure();

    /* Retrieve Memory Info */
    if (status == E_PRG_OK)
    {
        status = eBL_MemInfo(E_BL_FLASH_ID, &psMemInfo);
    }

    /* Reading Memory */
    if (status == E_PRG_OK)
    {
        status = eBL_MemOpen(E_BL_FLASH_ID, E_BL_READ);
    }

    if (status == E_PRG_OK)
    {
        status = eBL_MemRead(E_BL_FLASH_ID, psMemInfo.u32BaseAddress + offset, readSize, readBuf);
    }

    if (status == E_PRG_OK)
    {
        status = eBL_MemClose(E_BL_FLASH_ID);
    }
    else
    {
       eBL_MemClose(E_BL_FLASH_ID);
    }

    return status;
}

teStatus ISPInterface::eBL_ProgramFirmware(uint8_t *pu8Data, uint32_t u32DataLength, crc_state_t crc_enabled)
{
    teStatus status = E_PRG_OK;

    if (u32DataLength == 0)
    {
        return status;
    }

    status = eBL_RunUnlockProcedure();

    if (status == E_PRG_OK)
    {
        status = eBL_RunEraseAllProcedure();
    }

    if (status == E_PRG_OK)
    {
        if (crc_enabled)
        {
            status = eBL_RunProgramProcedureCRC(pu8Data, u32DataLength, 0);
        }
        else
        {
            status = eBL_RunProgramProcedure(pu8Data, u32DataLength, 0);
        }
    }

    status = eBL_Reset();

    return status;
}


void ISPInterface::InitUart(void)
{
    serial_manager_status_t status;
#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
#if (defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && (FSL_FEATURE_SOC_DMAMUX_COUNT > 0U))
    dma_mux_configure_t dma_mux;
    dma_mux.dma_dmamux_configure.dma_mux_instance = 0;
    dma_mux.dma_dmamux_configure.rx_request       = kDmaRequestMuxLPUART8Rx;
    dma_mux.dma_dmamux_configure.tx_request       = kDmaRequestMuxLPUART8Tx;
#endif
#endif
#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
#if (defined(FSL_FEATURE_EDMA_HAS_CHANNEL_MUX) && (FSL_FEATURE_EDMA_HAS_CHANNEL_MUX > 0U))
    dma_channel_mux_configure_t dma_channel_mux;
    dma_channel_mux.dma_dmamux_configure.dma_mux_instance   = 0;
    dma_channel_mux.dma_dmamux_configure.dma_tx_channel_mux = kDmaRequestMuxLPUART8Rx;
    dma_channel_mux.dma_dmamux_configure.dma_rx_channel_mux = kDmaRequestMuxLPUART8Tx;
#endif
#endif
#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
    serial_port_uart_dma_config_t uartConfig =
#else
    serial_port_uart_config_t uartConfig =
#endif
    {.clockRate       = ISP_UART_CLOCK_RATE,
     .baudRate        = ISP_UART_BAUD_RATE,
     .parityMode      = kSerialManager_UartParityDisabled,
     .stopBitCount    = kSerialManager_UartOneStopBit,
     .enableRx        = 1,
     .enableTx        = 1,
     .enableRxRTS     = ISP_ENABLE_RX_RTS,
     .enableTxCTS     = ISP_ENABLE_TX_RTS,
     .instance        = ISP_UART_INSTANCE,
     .txFifoWatermark = 0,
     .rxFifoWatermark = 0,
#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
     .dma_instance = 0,
     .rx_channel   = LPUART_RX_DMA_CHANNEL,
     .tx_channel   = LPUART_TX_DMA_CHANNEL,
#if (defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && (FSL_FEATURE_SOC_DMAMUX_COUNT > 0U))
     .dma_mux_configure = &dma_mux,
#else
     .dma_mux_configure         = NULL,
#endif
#if (defined(FSL_FEATURE_EDMA_HAS_CHANNEL_MUX) && (FSL_FEATURE_EDMA_HAS_CHANNEL_MUX > 0U))
     .dma_channel_mux_configure = &dma_channel_mux,
#else
     .dma_channel_mux_configure = NULL,
#endif
#endif
    };

    serial_manager_config_t serialManagerConfig = {
        .ringBuffer     = s_ringBuffer,
        .ringBufferSize = sizeof(s_ringBuffer),
#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
        .type = kSerialPort_UartDma,
#else
        .type = kSerialPort_Uart,
#endif
        .blockType  = kSerialManager_NonBlocking,
        .portConfig = (serial_port_uart_config_t *)&uartConfig,
    };

    OSA_InterruptDisable();
    status = SerialManager_Init((serial_handle_t)SerialHandle, &serialManagerConfig);
    OSA_InterruptEnable();
    assert(status == kStatus_SerialManager_Success);
    status = SerialManager_OpenReadHandle((serial_handle_t)SerialHandle,
                                          (serial_read_handle_t)SerialReadHandle);
    assert(status == kStatus_SerialManager_Success);
    status = SerialManager_InstallRxCallback((serial_read_handle_t)SerialReadHandle, Uart_RxCallBackStatic,
                                             NULL);
    assert(status == kStatus_SerialManager_Success);
    OT_UNUSED_VARIABLE(status);
    OT_UNUSED_VARIABLE(status);
}

void ISPInterface::DeinitUart(void)
{
    serial_manager_status_t status;
    status = SerialManager_CloseReadHandle((serial_read_handle_t)SerialReadHandle);
    assert(status == kStatus_SerialManager_Success);
    OSA_InterruptDisable();
    status = SerialManager_Deinit((serial_handle_t)SerialHandle);
    OSA_InterruptEnable();
    assert(status == kStatus_SerialManager_Success);
    OT_UNUSED_VARIABLE(status);
}

} // namespace RT
