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

#ifndef ISP_COMMANDS_CLASS_
#define ISP_COMMANDS_CLASS_

#include "board.h"
#include "fsl_os_abstraction.h"
#include "fsl_adapter_gpio.h"
#include "fsl_component_serial_manager.h"
#include <platform/CHIPDeviceLayer.h>
#include <openthread/platform/alarm-milli.h>
#include "fsl_adapter_crc.h"

#define BOOTLOADER_MAX_MESSAGE_LENGTH       1024
#define LARGE_APDU_PAYLOAD_SIZE             1600
#define MAX_RX_LARGE_SERIAL_BUFFER_SIZE     (33+LARGE_APDU_PAYLOAD_SIZE+8)
#define FLASH_SECTOR_SIZE                   512
#define K32W061_MEMORY_SIZE                 0x9DE00
#define K32W0_RESPONSE_DELAY_MS             50
#define ISP_LENGTH_READ                     3
#define DEFAULT_READ_WAIT_MS                1000
#define K32W0_FW_CRC_SIZE                   4

/*! @brief Define the port interrupt number for the board switches */
#define BOARD_DIO5_GPIO GPIO6
#define BOARD_DIO5_GPIO_PIN (11U)

#define BOARD_RESET_GPIO GPIO6
#define BOARD_RESET_GPIO_PIN (17U)

/* we are little-endian */
# define htole16(x) (x)
# define htobe16(x) _bswap16(x)
# define le16toh(x) (x)
# define be16toh(x) _bswap16(x)
# define htole32(x) (x)
# define htobe32(x) _bswap32(x)
# define le32toh(x) (x)
# define be32toh(x) _bswap32(x)

typedef enum
{
    RX,
    TX,
} message_direction_t;

typedef enum
{
    CRC_DISABLED,
    CRC_ENABLED,
} crc_state_t;

typedef enum
{
    E_BL_MSG_TYPE_NONE                                  = 0x0,
    E_BL_MSG_TYPE_RESET_REQUEST                         = 0x14,
    E_BL_MSG_TYPE_RESET_RESPONSE                        = 0x15,

    E_BL_MSG_TYPE_RAM_RUN_REQUEST                       = 0x21,
    E_BL_MSG_TYPE_RAM_RUN_RESPONSE                      = 0x22,

    E_BL_MSG_TYPE_SET_BAUD_REQUEST                      = 0x27,
    E_BL_MSG_TYPE_SET_BAUD_RESPONSE                     = 0x28,

    E_BL_MSG_TYPE_GET_CHIPID_REQUEST                    = 0x32,
    E_BL_MSG_TYPE_GET_CHIPID_RESPONSE                   = 0x33,

    TYPE_MEM_OPEN_REQUEST = 0x40,
    TYPE_MEM_OPEN_RESPONSE,
    TYPE_MEM_ERASE_REQUEST,
    TYPE_MEM_ERASE_RESPONSE,
    TYPE_MEM_BLANK_CHECK_REQUEST,
    TYPE_MEM_BLANK_CHECK_RESPONSE,
    TYPE_MEM_READ_REQUEST,
    TYPE_MEM_READ_RESPONSE,
    TYPE_MEM_WRITE_REQUEST,
    TYPE_MEM_WRITE_RESPONSE,
    TYPE_MEM_CLOSE_REQUEST,
    TYPE_MEM_CLOSE_RESPONSE,
    TYPE_MEM_GET_INFO_REQUEST,
    TYPE_MEM_GET_INFO_RESPONSE,

    TYPE_UNLOCK_ISP_REQUEST,
    TYPE_UNLOCK_ISP_RESPONSE,

    TYPE_START_AUTHENTICATION_REQUEST,
    TYPE_START_AUTHENTICATION_RESPONSE,

    TYPE_USE_CERTIFICATE_REQUEST,
    TYPE_USE_CERTIFICATE_RESPONSE,

    TYPE_SET_ENCRYPTION_REQUEST,
    TYPE_SET_ENCRYPTION_RESPONSE,

}teBL_MessageType;

typedef enum
{
    E_BL_RESPONSE_OK                                    = 0x00,
    E_BL_RESPONSE_MEMORY_INVALID_MODE                   = 0xef,
    E_BL_RESPONSE_NOT_SUPPORTED                         = 0xff,
    E_BL_RESPONSE_WRITE_FAIL                            = 0xfe,
    E_BL_RESPONSE_INVALID_RESPONSE                      = 0xfd,
    E_BL_RESPONSE_CRC_ERROR                             = 0xfc,
    E_BL_RESPONSE_ASSERT_FAIL                           = 0xfb,
    E_BL_RESPONSE_USER_INTERRUPT                        = 0xfa,
    E_BL_RESPONSE_READ_FAIL                             = 0xf9,
    E_BL_RESPONSE_TST_ERROR                             = 0xf8,
    E_BL_RESPONSE_AUTH_ERROR                            = 0xf7,
    E_BL_RESPONSE_NO_RESPONSE                           = 0xf6,
    E_BL_RESPONSE_MEMORY_INVALID                        = 0xf5,
    E_BL_RESPONSE_MEMORY_NOT_SUPPORTED                  = 0xf4,
    E_BL_RESPONSE_MEMORY_ACCESS_INVALID                 = 0xf3,
    E_BL_RESPONSE_MEMORY_OUT_OF_RANGE                   = 0xf2,
    E_BL_RESPONSE_MEMORY_TOO_LONG                       = 0xf1,
    E_BL_RESPONSE_BAD_STATE                             = 0xf0,
}teBL_Response;

typedef enum
{
    E_BL_FLASH_ID,
    E_BL_PSECT_ID,
    E_BL_pFlash_ID,
    E_BL_Config_ID,
    E_BL_EFUSE_ID,
    E_BL_ROM_ID,
    E_BL_RAM0_ID,
    E_BL_RAM1_ID,
}teBL_MemoryID;

typedef enum
{
    E_BL_ROM_TYPE,
    E_BL_FLASH_TYPE,
    E_BL_RAM_TYPE,
    E_BL_EFUSE_TYPE,
}teBL_MemoryType;

typedef enum
{
    E_BL_READ_ID,
    E_BL_WRITE_ID,
    E_BL_ERASE_ID,
    E_BL_ERASE_ALL_ID,
    E_BL_BLANK_CHECK_ID
}teBL_MemoryAccessID;

typedef enum
{
    E_BL_READ = 1 << E_BL_READ_ID,
    E_BL_WRITE = 1 << E_BL_WRITE_ID,
    E_BL_ERASE = 1 << E_BL_ERASE_ID,
    E_BL_ERASE_ALL = 1 << E_BL_ERASE_ALL_ID,
    E_BL_BLANK_CHECK = 1 << E_BL_BLANK_CHECK_ID
}teBL_MemoryAccess;

typedef enum
{
    E_PRG_OK,
    E_PRG_ERROR,
    E_PRG_OUT_OF_MEMORY,
    E_PRG_ERROR_WRITING,
    E_PRG_ERROR_READING,
    E_PRG_FAILED_TO_OPEN_FILE,
    E_PRG_BAD_PARAMETER,
    E_PRG_NULL_PARAMETER,
    E_PRG_INCOMPATIBLE,
    E_PRG_INVALID_FILE,
    E_PRG_UNSUPPORED_CHIP,
    E_PRG_ABORTED,
    E_PRG_VERIFICATION_FAILED,
    E_PRG_INVALID_TRANSPORT,
    E_PRG_COMMS_FAILED,
    E_PRG_UNSUPPORTED_OPERATION,
    E_PRG_FLASH_DEVICE_UNAVAILABLE,
} teStatus;

typedef struct __tsMemInfo
{
    char*       pcMemName;                      /**< Human readable name of the flash */
    uint32_t    u32BaseAddress;
    uint32_t    u32Size;
    uint32_t    u32BlockSize;
    uint8_t     u8Index;
    uint8_t     u8ManufacturerID;               /**< Flash manufacturer ID */
    uint8_t     u8DeviceID;                     /**< Flash device ID */
    uint8_t     u8ChipSelect;                   /**< Which SPI chip select for this flash */
    uint8_t     u8Access;
    struct __tsMemInfo *psNext;
} tsMemInfo;

namespace RT {

/**
 * This class defines an ISP interface to the Radio Co-processor (RCP).
 *
 */
class ISPInterface
{
    enum
    {
        /* Ring buffer size should be >= to the RCP max TX buffer size value which is 2048 */
        kMaxRingBufferSize = 1024,

    };

    SERIAL_MANAGER_HANDLE_DEFINE(SerialHandle);
    SERIAL_MANAGER_READ_HANDLE_DEFINE(SerialReadHandle);

    GPIO_HANDLE_DEFINE(GpioResetHandle);
    GPIO_HANDLE_DEFINE(GpioDIO5Handle);

    bool isInitialized;
    uint32_t mResetDelay;
    uint8_t  s_ringBuffer[kMaxRingBufferSize];

    ISPInterface(void);
    void     TriggerReset(void);
    void     InitUart(void);
    uint32_t TryRead(uint8_t *mUartRxBuffer, uint32_t size, uint64_t aTimeoutUs);
    otError  Write(const uint8_t *aFrame, uint16_t aLength);

    void printMessage(uint8_t *message, uint32_t length, message_direction_t direction);
    inline void putBuffer(const void *data, uint32_t length, uint8_t **pu8Store);
    inline uint16_t _bswap16(uint16_t x);
    inline uint32_t _bswap32(uint32_t x);
    inline uint32_t le32btoh(uint8_t *pu8Store);
    inline uint32_t be32btoh(uint8_t *pu8Store);
    inline void put32b(uint32_t u32Host, uint8_t **pu8Store);
    inline void put16b(uint16_t u16Host, uint8_t **pu8Store);
    inline void put8(uint8_t u8Host, uint8_t **pu8Store);
    uint32_t eBL_CalculateCrc(uint8_t *msg, uint32_t length);

    teStatus eBL_SetBaudrate(uint32_t u32Baudrate);
    teStatus eBL_ChipIdRead(uint32_t *pu32ChipId, uint32_t *pu32BootloaderVersion);
    teStatus eBL_MemErase(uint8_t u8Index, uint32_t u32Address, uint32_t u32Length);
    teStatus eBL_MemBlank(uint8_t u8Index, uint32_t u32Address, uint32_t u32Length);
    teStatus eBL_MemClose(uint8_t u8Index);
    teStatus eBL_MemOpen(uint8_t u8Index, uint8_t u8AccessMode);
    teStatus eBL_MemRead(uint8_t u8Index, uint32_t u32Address, uint32_t *pu32Length, uint8_t *pu8Buffer);
    teStatus eBL_MemInfo(uint8_t u8Index, tsMemInfo *psMemInfo);
    teStatus eBL_MemWrite(uint8_t u8Index, uint32_t u32Address, uint32_t u32Length, uint8_t *pu8Buffer);
    teStatus eBL_WriteMultiple(uint8_t *pu8Data, uint32_t u32DataLength, uint32_t u32Offset, tsMemInfo *psMemInfo);
    teStatus eBL_Connect();
    teStatus eBL_CheckResponse(teBL_Response eResponse, teBL_MessageType eRxType, teBL_MessageType eExpectedRxType);
    teStatus eBL_Unlock(uint8_t u8Mode, uint8_t *pu8Key, uint16_t u16KeyLen);
    teBL_Response eBL_ReadMessage(teBL_MessageType *peType, uint16_t *pu16Length, uint8_t *pu8Data);
    teStatus eBL_WriteMessage(teBL_MessageType eType, uint16_t u8HeaderLength, uint8_t *pu8Header, uint16_t u16DataLength, const uint8_t *pu8Data, uint8_t **pu8RcvdData);
    teBL_Response eBL_Request(teBL_MessageType eTxType, uint8_t u8HeaderLen,
                uint8_t *pu8Header, uint16_t u16TxLength, uint8_t *pu8TxData, teBL_MessageType *peRxType, uint16_t *pu16RxLength, uint8_t *pu8RxData, const char *funcName);

public:

    static ISPInterface getInstance();

    ~ISPInterface(void);

    void Init(void);
    void Deinit(void);
    void DeinitUart(void);
    void GPIODeinit(void);

    teStatus runOTWUpdater();
    teStatus eBL_CheckCrc(uint8_t *pu8Data, uint32_t u32DataLength);
    teStatus eBL_ProgramFirmware(uint8_t *pu8Data, uint32_t u32DataLength, crc_state_t crc_enabled);
    teStatus eBL_RunUnlockProcedure();
    teStatus eBL_RunEraseAllProcedure();
    teStatus eBL_RunProgramProcedure(uint8_t *pu8Data, uint32_t u32DataLength, uint32_t u32Offset);
    teStatus eBL_RunProgramProcedureCRC(uint8_t *pu8Data, uint32_t u32DataLength, uint32_t u32Offset);
    teStatus eBL_RunReadMemoryProcedure(uint8_t *readBuf, uint32_t *readSize, uint32_t offset);
    teStatus eBL_Reset();
};

} // namespace RT

#endif // ISP_COMMANDS_CLASS_
