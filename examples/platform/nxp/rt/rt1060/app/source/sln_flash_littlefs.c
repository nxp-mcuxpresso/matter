/*
 * Copyright 2019-2022, 2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

#include "sln_flash.h"
#include "sln_flash_littlefs.h"
#include "lfs.h"
#include "sln_encrypt.h"
#include "fsl_os_abstraction.h"

#if defined(FSL_SDK_ENABLE_DRIVER_CACHE_CONTROL) && FSL_SDK_ENABLE_DRIVER_CACHE_CONTROL
#include "fsl_cache.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define ATTR_ENCRYPT 0x1

/*! @brief Structure to hold file meta data during operations */
typedef struct _file_encypt_info
{
    uint32_t dataEncLen;
    uint32_t dataPlainLen;
    bool useEncryption;
} file_encypt_info_t;

typedef struct _file_meta
{
    lfs_file_t file;
    struct lfs_file_config cfg;
    struct lfs_attr attrs[1];
    file_encypt_info_t encryptInfo;
} file_meta_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static int LFS_Mutex_Lock(const struct lfs_config *lfsc);
static int LFS_Mutex_Unlock(const struct lfs_config *lfsc);
static int LFS_FlashRead(
    const struct lfs_config *lfsc, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size);
static int LFS_FlashProg(
    const struct lfs_config *lfsc, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size);
static int LFS_FlashErase(const struct lfs_config *lfsc, lfs_block_t block);
static int LFS_FlashSync(const struct lfs_config *lfsc);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static OSA_MUTEX_HANDLE_DEFINE(mLfsMutexId);

static const sln_encrypt_ctx_t s_flashLittlefsEncCtx = {
    .key = {0x2c, 0x7d, 0x13, 0x18, 0x26, 0xb0, 0xd0, 0xaa, 0xab, 0xf7, 0x16, 0x88, 0x09, 0xcf, 0x4f, 0x3e},

    .iv = {0xef, 0xf0, 0xf9, 0xec, 0xfc, 0xf1, 0xf4, 0xf9, 0xf8, 0xf9, 0xfa, 0x02, 0xfc, 0xfd, 0xfe, 0xff},

    .keySize = 16};

static uint32_t s_ErasedBlocks[LFS_SECTORS / 32] = {0x0};
sln_littlefs_t s_LittlefsHandler        = {};

AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t s_CacheBuffer[LFS_CACHE_SIZE], 8);
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t s_ReadBuffer[LFS_CACHE_SIZE], 8);
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t s_WriteBuffer[LFS_CACHE_SIZE], 8);
// the lookahead vector has to be 64bit-aligned (8B) (see below)
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t s_LookaheadBuffer[LFS_LOOKAHEAD_BUF_SIZE], 8);

static sln_flash_fs_cbs_t s_flashLittlefsCbs = {NULL};

static const struct lfs_config s_LittlefsConfigDefault = {
    // block device driver context data
    .context = NULL,

    // block device operations
    .read  = LFS_FlashRead,
    .prog  = LFS_FlashProg,
    .erase = LFS_FlashErase,
    .sync  = LFS_FlashSync,
#ifdef LFS_THREADSAFE
    .lock   = LFS_Mutex_Lock,
    .unlock = LFS_Mutex_Unlock,
#endif
    // block device configuration
    .read_size        = FLASH_PAGE_SIZE,
    .read_buffer      = s_ReadBuffer,
    .prog_buffer      = s_WriteBuffer,
    .prog_size        = FLASH_PAGE_SIZE,
    .lookahead_size   = LFS_LOOKAHEAD_BUF_SIZE,
    .lookahead_buffer = s_LookaheadBuffer,

    .cache_size   = LFS_CACHE_SIZE,
    .block_size   = FLASH_SECTOR_SIZE,
    .block_count  = LFS_SECTORS,
    .block_cycles = 100,

};

/*******************************************************************************
 * Code
 ******************************************************************************/
static int _lock(SemaphoreHandle_t lock)
{
    if (lock == NULL)
    {
        return -1;
    }

    if (pdTRUE != xSemaphoreTake(lock, portMAX_DELAY))
    {
        return -1;
    }

    if (s_flashLittlefsCbs.post_lock_cb)
    {
        s_flashLittlefsCbs.post_lock_cb();
    }
    return 0;
}

static int _unlock(SemaphoreHandle_t lock)
{
    if (lock == NULL)
    {
        return -1;
    }

    if (pdTRUE != xSemaphoreGive(lock))
    {
        return -1;
    }

    if (s_flashLittlefsCbs.post_unlock_cb)
    {
        s_flashLittlefsCbs.post_unlock_cb();
    }
    return 0;
}

static bool _is_blockBitSet(uint32_t *blockBitfield, lfs_block_t block)
{
    return blockBitfield[block / 32] & (1U << (block % 32));
}

static void _set_blockBit(uint32_t *blockBitfield, lfs_block_t block)
{
    blockBitfield[block / 32] |= (1U << (block % 32));
}

static void _clear_blockBit(uint32_t *blockBitfield, lfs_block_t block)
{
    blockBitfield[block / 32] &= ~(1U << (block % 32));
}

static int _lfs_traverse_create_used_blocks(void *p, lfs_block_t block)
{
    uint32_t *usedBlocks = p;
    _set_blockBit(usedBlocks, block);
    return 0;
}

static bool _lfs_checkBlockEmpty(lfs_block_t block)
{
    const struct lfs_config lfsc = s_LittlefsConfigDefault;
    uint32_t src                 = (uint32_t)(LFS_BASE_ADDR + block * lfsc.block_size);

    for (int i = 0; i < FLASH_SECTOR_SIZE; i += sizeof(uint32_t))
    {
        uint32_t dst = 0;
        SLN_Read_Flash_At_Address((src + i), (uint8_t *)&dst, sizeof(uint32_t));
        /* Compare block with 0xFF to see if it is empty. Consider using defines */
        if (dst != 0xFFFFFFFF)
        {
            return false;
        }
    }
    return true;
}

static int LFS_Mutex_Lock(const struct lfs_config *lfsc)
{
    (void)OSA_MutexLock((osa_mutex_handle_t)mLfsMutexId, osaWaitForever_c);

    return LFS_ERR_OK;
}

static int LFS_Mutex_Unlock(const struct lfs_config *lfsc)
{
    (void)OSA_MutexUnlock((osa_mutex_handle_t)mLfsMutexId);

    return LFS_ERR_OK;
}

static int LFS_FlashRead(const struct lfs_config *lfsc, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size)
{
    uint32_t src;
    uint32_t *dst;

    src = (uint32_t)(LFS_BASE_ADDR + block * lfsc->block_size + off);
    dst = (uint32_t *)buffer;

    SLN_Read_Flash_At_Address(src, (uint8_t *)dst, size);

    if (((uint32_t)src & 0x03) || ((uint32_t)dst & 0x03) || (size & 0x03))
    {
        return LFS_ERR_IO; /* unaligned access */
    }

    return LFS_ERR_OK;
}

static int LFS_FlashProg(
    const struct lfs_config *lfsc, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size)
{
    status_t status;
    uint32_t prog_addr = LFS_BASE_ADDR + block * lfsc->block_size + off;

    for (uint32_t pos = 0; pos < size; pos += lfsc->prog_size)
    {
        status = SLN_Write_Flash_Page(prog_addr + pos, (void *)((uintptr_t)buffer + pos), lfsc->prog_size);
        if (status != kStatus_Success)
        {
            break;
        }
    }

    if (status == kStatus_Fail)
    {
        return LFS_ERR_CORRUPT;
    }
    else if (status != kStatus_Success)
    {
        return LFS_ERR_IO;
    }
    _clear_blockBit(s_ErasedBlocks, block);

    return LFS_ERR_OK;
}

static int LFS_FlashErase(const struct lfs_config *lfsc, lfs_block_t block)
{
    status_t status     = kStatus_Success;
    uint32_t erase_addr = LFS_BASE_ADDR + block * lfsc->block_size;

    if (_is_blockBitSet(s_ErasedBlocks, block))
    {
        /* Block is marked as erased */
        return LFS_ERR_OK;
    }

    if (!_lfs_checkBlockEmpty(block))
    {
        /* The block is not empty, needs erase */
        if (s_flashLittlefsCbs.pre_sector_erase_cb)
        {
            s_flashLittlefsCbs.pre_sector_erase_cb();
        }

        status = SLN_Erase_Sector(erase_addr);

        if (s_flashLittlefsCbs.post_sector_erase_cb)
        {
            s_flashLittlefsCbs.post_sector_erase_cb();
        }
    }

    if (status == kStatus_Fail)
    {
        return LFS_ERR_CORRUPT;
    }
    else if (status != kStatus_Success)
    {
        return LFS_ERR_IO;
    }

    _set_blockBit(s_ErasedBlocks, block);

    return LFS_ERR_OK;
}

static int LFS_FlashSync(const struct lfs_config *lfsc)
{
    return LFS_ERR_OK;
}

static void LFS_GetDefaultFileConfig(file_meta_t *file)
{
    file->encryptInfo.dataEncLen    = 0;
    file->encryptInfo.dataPlainLen  = 0;
    file->encryptInfo.useEncryption = false;
    file->attrs[0].buffer           = &file->encryptInfo;
    file->attrs[0].size             = sizeof(file->encryptInfo);
    file->attrs[0].type             = ATTR_ENCRYPT;

    file->cfg.buffer     = s_CacheBuffer;
    file->cfg.attrs      = file->attrs;
    file->cfg.attr_count = 1;
}

static int LFS_GetDefaultConfig(struct lfs_config *lfsc)
{
    *lfsc = s_LittlefsConfigDefault; /* copy pre-initialized lfs config structure */
    return 0;
}

static sln_flash_fs_status_t LFS_CheckBasePath(const char *name)
{
    sln_flash_fs_status_t ret = SLN_FLASH_FS_OK;
    uint8_t offset            = strlen(name);

    while ((name[offset] != '/') && (offset > 0))
    {
        offset--;
    }

    if (offset != 0)
    {
        /* We have a directory */
        char *basePath = (char *)pvPortMalloc(offset + 1);
        if (basePath != NULL)
        {
            lfs_dir_t dir;
            memset(&dir, 0, sizeof(lfs_dir_t));

            memset(basePath, 0, offset + 1);
            strncpy(basePath, name, offset);
            if (lfs_dir_open(&s_LittlefsHandler.lfs, &dir, basePath) != 0)
            {
                ret = SLN_FLASH_FS_ENOENTRY;
            }
            else
            {
                lfs_dir_close(&s_LittlefsHandler.lfs, &dir);
            }
            vPortFree(basePath);
        }
        else
        {
            ret = SLN_FLASH_FS_ENOMEM;
        }
    }

    return ret;
}

static sln_flash_fs_status_t LFS_SaveFileContent(file_meta_t *file_meta, uint8_t *dataIn, uint32_t dataLenIn)
{
    sln_flash_fs_status_t ret = SLN_FLASH_FS_OK;
    uint8_t *data             = NULL;
    uint32_t len              = 0;

    if (file_meta->encryptInfo.useEncryption)
    {
        int32_t status;
        len  = SLN_Encrypt_Get_Crypt_Length(dataLenIn);
        data = (uint8_t *)pvPortMalloc(len);
        if (data == NULL)
        {
            ret = SLN_FLASH_FS_ENOMEM3;
        }
        else
        {
            status = SLN_Encrypt_AES_CBC_PKCS7((sln_encrypt_ctx_t *)(&s_flashLittlefsEncCtx), dataIn, dataLenIn, data, len);
            if (status == SLN_ENCRYPT_STATUS_OK)
            {
                file_meta->encryptInfo.dataPlainLen = dataLenIn;
                file_meta->encryptInfo.dataEncLen   = len;
            }
            else
            {
                ret = SLN_FLASH_FS_EENCRYPT;
            }
        }
    }
    else
    {
        data = dataIn;
        len  = dataLenIn;
    }

    if (ret == SLN_FLASH_FS_OK)
    {
        int32_t littlefs_res = 0;

        /* This can fail TODO */
        littlefs_res = lfs_file_write(&s_LittlefsHandler.lfs, &file_meta->file, data, len);
        if (littlefs_res < 0)
        {
            ret = SLN_FLASH_FS_FAIL;
        }
    }

    if ((file_meta->encryptInfo.useEncryption) && (data != NULL))
    {
        vPortFree(data);
    }

    return ret;
}

static sln_flash_fs_status_t LFS_GetFileContent(file_meta_t *file_meta,
                                                uint32_t offset,
                                                uint8_t *dataOut,
                                                uint32_t *dataLenOut)
{
    sln_flash_fs_status_t ret = SLN_FLASH_FS_OK;
    uint8_t *data             = NULL;
    uint8_t *dataPlain        = NULL;
    size_t len                = 0;

    if (file_meta->encryptInfo.useEncryption)
    {
        /* read the whole file */
        len  = file_meta->encryptInfo.dataEncLen;
        data = (uint8_t *)pvPortMalloc(len);
        if (data == NULL)
        {
            ret = SLN_FLASH_FS_ENOMEM3;
        }
        else
        {
            dataPlain = (uint8_t *)pvPortMalloc(len);
            if (dataPlain == NULL)
            {
                vPortFree(data);
                data = NULL;
                ret  = SLN_FLASH_FS_ENOMEM3;
            }
        }
    }
    else
    {
        data = dataOut;
        len  = *dataLenOut;

        /* Move file pos to offset */
        if (offset != 0)
        {
            if (file_meta->file.ctz.size < offset)
            {
                ret = SLN_FLASH_FS_EINVAL3;
            }
            else
            {
                lfs_file_seek(&s_LittlefsHandler.lfs, &file_meta->file, offset, LFS_SEEK_SET);
            }
        }
    }

    if (ret == SLN_FLASH_FS_OK)
    {
        int32_t littlefs_res = 0;
        uint32_t realSize    = 0;
        do
        {
            /* Start reading from file */
            littlefs_res = lfs_file_read(&s_LittlefsHandler.lfs, &file_meta->file, (data + realSize), len);
            if (littlefs_res < 0)
            {
                ret = SLN_FLASH_FS_FAIL;
                break;
            }
            else if (littlefs_res == 0)
            {
                /* Nothing to read. Break */
                break;
            }

            realSize += littlefs_res;
            len -= littlefs_res;
        } while (len > 0);

        len = realSize;
    }

    if (ret == SLN_FLASH_FS_OK)
    {
        if (file_meta->encryptInfo.useEncryption)
        {
            int32_t status = SLN_Decrypt_AES_CBC_PKCS7((sln_encrypt_ctx_t *)(&s_flashLittlefsEncCtx), (const uint8_t *)data, len, dataPlain, &len);
            if (status == SLN_ENCRYPT_STATUS_OK)
            {
                if (len <= offset)
                {
                    ret = SLN_FLASH_FS_EINVAL3;
                }
                else
                {
                    len -= offset;

                    if (*dataLenOut > len)
                    {
                        *dataLenOut = len;
                    }

                    memcpy(dataOut, (uint8_t *)(dataPlain + offset), *dataLenOut);
                }
            }
            else
            {
                ret = SLN_FLASH_FS_EENCRYPT;
            }
        }
        else
        {
            *dataLenOut = len;
        }
    }

    if ((file_meta->encryptInfo.useEncryption) && (data != NULL))
    {
        vPortFree(data);
        vPortFree(dataPlain);
    }

    return ret;
}

sln_flash_fs_status_t SLN_FLASH_LITTLEFS_Init(uint8_t erase)
{
    sln_flash_fs_status_t ret = SLN_FLASH_FS_OK;
    int32_t littlefs_res      = 0;

    if (s_LittlefsHandler.lfsMounted == true)
    {
        return SLN_FLASH_FS_OK;
    }

    s_LittlefsHandler.lock = xSemaphoreCreateMutex();
    (void)OSA_MutexCreate(mLfsMutexId);

    if (s_LittlefsHandler.lock == NULL)
    {
        ret = SLN_FLASH_FS_ENOLOCK;
    }

    if (SLN_FLASH_FS_OK == ret)
    {
        LFS_GetDefaultConfig(&s_LittlefsHandler.cfg);
        if (erase)
        {
            lfs_format(&s_LittlefsHandler.lfs, &s_LittlefsHandler.cfg);
        }

        littlefs_res = lfs_mount(&s_LittlefsHandler.lfs, &s_LittlefsHandler.cfg);
        if (littlefs_res == 0)
        {
            s_LittlefsHandler.lfsMounted = 1;
        }
        else if (littlefs_res == LFS_ERR_CORRUPT)
        {
            lfs_format(&s_LittlefsHandler.lfs, &s_LittlefsHandler.cfg);
            littlefs_res = lfs_mount(&s_LittlefsHandler.lfs, &s_LittlefsHandler.cfg);
            if (littlefs_res == 0)
            {
                ret = SLN_FLASH_FS_OK;
            }
            else
            {
                ret = SLN_FLASH_FS_CORRUPTED;
            }
        }
        else
        {
            ret = SLN_FLASH_FS_FAIL;
        }

        if (SLN_FLASH_FS_OK != ret)
        {
            vSemaphoreDelete(s_LittlefsHandler.lock);
            s_LittlefsHandler.lock = NULL;
        }
        else
        {
            SLN_Encrypt_Init_Slot((sln_encrypt_ctx_t *)(&s_flashLittlefsEncCtx));
        }
    }

    return ret;
}

sln_flash_fs_status_t SLN_FLASH_LITTLEFS_Save(const char *name, uint8_t *data, uint32_t len)
{
    file_meta_t file_meta;
    sln_flash_fs_status_t ret = SLN_FLASH_FS_OK;
    int32_t littlefs_res      = 0;

    if ((name == NULL) || (data == NULL) || (len == 0))
    {
        return SLN_FLASH_FS_EINVAL;
    }

    if (_lock(s_LittlefsHandler.lock))
    {
        return SLN_FLASH_FS_ENOLOCK;
    }

    LFS_GetDefaultFileConfig(&file_meta);

    /* check if the dir exists */
    ret = LFS_CheckBasePath(name);

    if (ret == SLN_FLASH_FS_OK)
    {
        littlefs_res =
            lfs_file_opencfg(&s_LittlefsHandler.lfs, &file_meta.file, name, LFS_O_CREAT | LFS_O_WRONLY, &file_meta.cfg);

        if (littlefs_res < 0)
        {
            ret = SLN_FLASH_FS_FAIL;
        }
        else
        {
            lfs_getattr(&s_LittlefsHandler.lfs, name, ATTR_ENCRYPT, file_meta.attrs[0].buffer, file_meta.attrs[0].size);
        }
    }

    if (ret == SLN_FLASH_FS_OK)
    {
        ret = LFS_SaveFileContent(&file_meta, data, len);
        lfs_file_close(&s_LittlefsHandler.lfs, &file_meta.file);
    }

    _unlock(s_LittlefsHandler.lock);

    return ret;
}

sln_flash_fs_status_t SLN_FLASH_LITTLEFS_Append(const char *name, uint8_t *data, uint32_t len)
{
    file_meta_t file_meta;
    sln_flash_fs_status_t ret = SLN_FLASH_FS_OK;
    int32_t littlefs_res      = 0;

    if ((name == NULL) || (data == NULL) || (len == 0))
    {
        return SLN_FLASH_FS_EINVAL;
    }

    if (_lock(s_LittlefsHandler.lock))
    {
        return SLN_FLASH_FS_ENOLOCK;
    }

    LFS_GetDefaultFileConfig(&file_meta);

    /* check if the dir exists */
    ret = LFS_CheckBasePath(name);

    if (ret == SLN_FLASH_FS_OK)
    {
        littlefs_res = lfs_file_opencfg(&s_LittlefsHandler.lfs, &file_meta.file, name, LFS_O_APPEND | LFS_O_WRONLY,
                                        &file_meta.cfg);

        if (littlefs_res == LFS_ERR_NOENT)
        {
            ret = SLN_FLASH_FS_ENOENTRY2;
        }
        else if (littlefs_res < 0)
        {
            ret = SLN_FLASH_FS_FAIL;
        }
        else
        {
            lfs_getattr(&s_LittlefsHandler.lfs, name, ATTR_ENCRYPT, file_meta.attrs[0].buffer, file_meta.attrs[0].size);
            if (file_meta.encryptInfo.useEncryption)
            {
                /* Not supported for now */
                lfs_file_close(&s_LittlefsHandler.lfs, &file_meta.file);
                ret = SLN_FLASH_FS_FAIL;
            }
        }
    }

    if (ret == SLN_FLASH_FS_OK)
    {
        ret = LFS_SaveFileContent(&file_meta, data, len);
        lfs_file_close(&s_LittlefsHandler.lfs, &file_meta.file);
    }

    _unlock(s_LittlefsHandler.lock);

    return ret;
}

sln_flash_fs_status_t SLN_FLASH_LITTLEFS_Update(const char *name, uint8_t *data, uint32_t *len)
{
    file_meta_t file_meta;
    sln_flash_fs_status_t ret;
    int32_t littlefs_res;

    if ((name == NULL) || (data == NULL) || (len == NULL))
    {
        return SLN_FLASH_FS_EINVAL;
    }

    if (_lock(s_LittlefsHandler.lock))
    {
        return SLN_FLASH_FS_ENOLOCK;
    }

    LFS_GetDefaultFileConfig(&file_meta);

    /* check if the dir exists */
    ret = LFS_CheckBasePath(name);

    if (ret == SLN_FLASH_FS_OK)
    {
        littlefs_res = lfs_file_opencfg(&s_LittlefsHandler.lfs, &file_meta.file, name, LFS_O_RDWR, &file_meta.cfg);
        if (littlefs_res == LFS_ERR_NOENT)
        {
            ret = SLN_FLASH_FS_ENOENTRY2;
        }
        else if (littlefs_res < 0)
        {
            ret = SLN_FLASH_FS_FAIL;
        }
        else
        {
            lfs_getattr(&s_LittlefsHandler.lfs, name, ATTR_ENCRYPT, file_meta.attrs[0].buffer, file_meta.attrs[0].size);
        }
    }

    if (ret == SLN_FLASH_FS_OK)
    {
        ret = LFS_SaveFileContent(&file_meta, data, *len);
        lfs_file_close(&s_LittlefsHandler.lfs, &file_meta.file);
    }

    _unlock(s_LittlefsHandler.lock);

    return ret;
}

sln_flash_fs_status_t SLN_FLASH_LITTLEFS_Read(const char *name, uint8_t *data, uint32_t offset, uint32_t *len)
{
    file_meta_t file_meta;
    sln_flash_fs_status_t ret = SLN_FLASH_FS_OK;
    int32_t littlefs_res      = 0;

    if ((name == NULL) || (len == NULL))
    {
        return SLN_FLASH_FS_EINVAL;
    }

    if (_lock(s_LittlefsHandler.lock))
    {
        return SLN_FLASH_FS_ENOLOCK;
    }

    LFS_GetDefaultFileConfig(&file_meta);

    /* check if the dir exists */
    ret = LFS_CheckBasePath(name);

    if (ret == SLN_FLASH_FS_OK)
    {
        littlefs_res = lfs_file_opencfg(&s_LittlefsHandler.lfs, &file_meta.file, name, LFS_O_RDONLY, &file_meta.cfg);
        if (littlefs_res == LFS_ERR_NOENT)
        {
            ret = SLN_FLASH_FS_ENOENTRY2;
        }
        else if (littlefs_res < 0)
        {
            ret = SLN_FLASH_FS_FAIL;
        }
        else if ((littlefs_res == 0) && (data == NULL))
        {
            /* return ok. Set len file size */
            if (file_meta.encryptInfo.useEncryption)
            {
                *len = file_meta.encryptInfo.dataPlainLen;
            }
            else
            {
                *len = file_meta.file.ctz.size;
            }

            lfs_file_close(&s_LittlefsHandler.lfs, &file_meta.file);
            ret = SLN_FLASH_FS_OK;
            goto exit;
        }
        else if (file_meta.file.ctz.size == 0)
        {
            lfs_file_close(&s_LittlefsHandler.lfs, &file_meta.file);
            memset(data, 0, *len);
            *len = 0;
            ret = SLN_FLASH_FS_OK;
            goto exit;
        }
    }

    if (ret == SLN_FLASH_FS_OK)
    {
        /* Get Content from the file. If encrypt, /p data will contain decrypt info */
        ret = LFS_GetFileContent(&file_meta, offset, data, len);

        /* Move file pos back to begining */
        lfs_file_seek(&s_LittlefsHandler.lfs, &file_meta.file, 0, LFS_SEEK_SET);
        lfs_file_close(&s_LittlefsHandler.lfs, &file_meta.file);
    }

exit:
    _unlock(s_LittlefsHandler.lock);

    return ret;
}

sln_flash_fs_status_t SLN_FLASH_LITTLEFS_Erase(const char *name)
{
    sln_flash_fs_status_t ret = SLN_FLASH_FS_OK;
    int32_t littlefs_res      = 0;

    if (name == NULL)
    {
        return SLN_FLASH_FS_EINVAL;
    }

    if (_lock(s_LittlefsHandler.lock))
    {
        return SLN_FLASH_FS_ENOLOCK;
    }

    /* check if the dir exists */
    ret = LFS_CheckBasePath(name);

    if (ret == SLN_FLASH_FS_OK)
    {
        littlefs_res = lfs_remove(&s_LittlefsHandler.lfs, name);
        if (littlefs_res == LFS_ERR_NOENT)
        {
            ret = SLN_FLASH_FS_ENOENTRY2;
        }
        else if (littlefs_res != 0)
        {
            ret = SLN_FLASH_FS_FAIL;
        }
    }
    _unlock(s_LittlefsHandler.lock);

    return ret;
}

sln_flash_fs_status_t SLN_FLASH_LITTLEFS_Deinit(uint8_t erase)
{
    sln_flash_fs_status_t ret = SLN_FLASH_FS_OK;
    int32_t littlefs_res      = 0;

    if (_lock(s_LittlefsHandler.lock))
    {
        return SLN_FLASH_FS_ENOLOCK;
    }

    if (erase)
    {
        lfs_format(&s_LittlefsHandler.lfs, &s_LittlefsHandler.cfg);
    }

    littlefs_res = lfs_unmount(&s_LittlefsHandler.lfs);

    if (littlefs_res != 0)
    {
        ret = SLN_FLASH_FS_FAIL;
    }
    else
    {
        SLN_Encrypt_Deinit_Slot((sln_encrypt_ctx_t *)(&s_flashLittlefsEncCtx));
        s_LittlefsHandler.lfsMounted = false;
        vSemaphoreDelete(s_LittlefsHandler.lock);
        s_LittlefsHandler.lock = NULL;
    }

    return ret;
}

sln_flash_fs_status_t SLN_FLASH_LITTLEFS_SetCbs(sln_flash_fs_cbs_t *cbs)
{
    sln_flash_fs_status_t ret = SLN_FLASH_FS_OK;

    if (NULL == cbs)
    {
        return SLN_FLASH_FS_EINVAL;
    }

    if (_lock(s_LittlefsHandler.lock))
    {
        return SLN_FLASH_FS_ENOLOCK;
    }

    s_flashLittlefsCbs.pre_sector_erase_cb  = cbs->pre_sector_erase_cb;
    s_flashLittlefsCbs.post_sector_erase_cb = cbs->post_sector_erase_cb;
    s_flashLittlefsCbs.post_lock_cb         = cbs->post_lock_cb;
    s_flashLittlefsCbs.post_unlock_cb       = cbs->post_unlock_cb;

    _unlock(s_LittlefsHandler.lock);

    return ret;
}

sln_flash_fs_status_t SLN_FLASH_LITTLEFS_Mkdir(const char *name)
{
    sln_flash_fs_status_t ret = SLN_FLASH_FS_OK;
    int32_t littlefs_res      = 0;

    if (name == NULL)
    {
        return SLN_FLASH_FS_EINVAL;
    }

    if (_lock(s_LittlefsHandler.lock))
    {
        return SLN_FLASH_FS_ENOLOCK;
    }

    /* check if the dir exists */
    ret = LFS_CheckBasePath(name);

    if (ret == SLN_FLASH_FS_OK)
    {
        littlefs_res = lfs_mkdir(&s_LittlefsHandler.lfs, name);
        if (littlefs_res == LFS_ERR_EXIST)
        {
            ret = SLN_FLASH_FS_FILEEXIST;
        }
        else if (littlefs_res != 0)
        {
            ret = SLN_FLASH_FS_FAIL;
        }
    }

    _unlock(s_LittlefsHandler.lock);
    return ret;
}

sln_flash_fs_status_t SLN_FLASH_LITTLEFS_Mkfile(const char *name, bool encrypt)
{
    sln_flash_fs_status_t ret = SLN_FLASH_FS_OK;
    int32_t littlefs_res      = 0;
    file_meta_t file_meta;

    if (name == NULL)
    {
        return SLN_FLASH_FS_EINVAL;
    }

    if (_lock(s_LittlefsHandler.lock))
    {
        return SLN_FLASH_FS_ENOLOCK;
    }

    /* check if the dir exists */
    ret = LFS_CheckBasePath(name);

    if (ret == SLN_FLASH_FS_OK)
    {
        LFS_GetDefaultFileConfig(&file_meta);

        /* Try to read first */
        littlefs_res = lfs_file_opencfg(&s_LittlefsHandler.lfs, &file_meta.file, name, LFS_O_RDONLY, &file_meta.cfg);
        if (littlefs_res == LFS_ERR_NOENT)
        {
            /* No file found. Create it */
            littlefs_res = lfs_file_opencfg(&s_LittlefsHandler.lfs, &file_meta.file, name, LFS_O_CREAT, &file_meta.cfg);
            if (littlefs_res == 0)
            {
                file_meta.encryptInfo.useEncryption = encrypt;
                littlefs_res = lfs_setattr(&s_LittlefsHandler.lfs, name, ATTR_ENCRYPT, file_meta.attrs[0].buffer,
                                           file_meta.attrs[0].size);
            }
            else
            {
                ret = SLN_FLASH_FS_FAIL;
            }
        }
        else if (littlefs_res == 0)
        {
            ret = SLN_FLASH_FS_FILEEXIST;
        }
        else if (littlefs_res < 0)
        {
            ret = SLN_FLASH_FS_FAIL;
        }
    }

    if ((ret == SLN_FLASH_FS_OK) || (ret == SLN_FLASH_FS_FILEEXIST))
    {
        lfs_file_close(&s_LittlefsHandler.lfs, &file_meta.file);
    }

    _unlock(s_LittlefsHandler.lock);
    return ret;
}

sln_flash_fs_status_t SLN_FLASH_LITTLEFS_Format()
{
    sln_flash_fs_status_t ret = SLN_FLASH_FS_OK;
    int32_t littlefs_res      = 0;
    if (_lock(s_LittlefsHandler.lock))
    {
        return SLN_FLASH_FS_ENOLOCK;
    }

    littlefs_res = lfs_format(&s_LittlefsHandler.lfs, &s_LittlefsHandler.cfg);
    if (littlefs_res != 0)
    {
        ret = SLN_FLASH_FS_FAIL;
    }

    _unlock(s_LittlefsHandler.lock);
    return ret;
}

sln_flash_fs_status_t SLN_FLASH_LITTLEFS_Rename(const char *oldName, const char *newName)
{
    sln_flash_fs_status_t ret = SLN_FLASH_FS_OK;
    int32_t littlefs_res      = 0;
    file_meta_t file_meta;

    if ((oldName == NULL) || (newName == NULL))
    {
        return SLN_FLASH_FS_EINVAL;
    }

    if (_lock(s_LittlefsHandler.lock))
    {
        return SLN_FLASH_FS_ENOLOCK;
    }

    /* check if the dir exists */
    ret = LFS_CheckBasePath(oldName);
    if (ret == SLN_FLASH_FS_OK)
    {
        ret = LFS_CheckBasePath(newName);
    }

    if (ret == SLN_FLASH_FS_OK)
    {
        LFS_GetDefaultFileConfig(&file_meta);
        littlefs_res = lfs_file_opencfg(&s_LittlefsHandler.lfs, &file_meta.file, oldName, LFS_O_RDONLY, &file_meta.cfg);
        if (littlefs_res == LFS_ERR_NOENT)
        {
            /* Old file doesn't exist */
            ret = SLN_FLASH_FS_ENOENTRY2;
        }
        else if (littlefs_res < 0)
        {
            ret = SLN_FLASH_FS_FAIL;
        }
    }

    if (ret == SLN_FLASH_FS_OK)
    {
        lfs_file_close(&s_LittlefsHandler.lfs, &file_meta.file);
        littlefs_res = lfs_rename(&s_LittlefsHandler.lfs, oldName, newName);
        if (littlefs_res)
        {
            ret = SLN_FLASH_FS_FAIL;
        }
    }

    _unlock(s_LittlefsHandler.lock);
    return ret;
}

sln_flash_fs_status_t SLN_FLASH_LITTLEFS_Cleanup(uint32_t timeout_ms)
{
    sln_flash_fs_status_t ret             = SLN_FLASH_FS_OK;
    int32_t littlefs_res                  = 0;
    uint32_t usedBlocks[LFS_SECTORS / 32] = {0};
    uint32_t startTime, currentTime, emptyBlocks;
    if (_lock(s_LittlefsHandler.lock))
    {
        return SLN_FLASH_FS_ENOLOCK;
    }

    /* create used block list */
    littlefs_res = lfs_fs_traverse(&s_LittlefsHandler.lfs, _lfs_traverse_create_used_blocks, &usedBlocks);

    if (littlefs_res)
    {
        ret = SLN_FLASH_FS_FAIL;
    }

    if (ret == SLN_FLASH_FS_OK)
    {
        startTime   = portTICK_PERIOD_MS * xTaskGetTickCount();
        emptyBlocks = 0;
        /* find next block starting from free.i */
        for (int i = 0; i < LFS_SECTORS; i++)
        {
            currentTime = portTICK_PERIOD_MS * xTaskGetTickCount();
            /* Check timeout */
            if ((timeout_ms) && (currentTime >= (startTime + timeout_ms)))
            {
                break;
            }

            lfs_block_t block = i;

            /* take next unused marked block */
            if (!_is_blockBitSet(usedBlocks, block))
            {
                LFS_FlashErase(&s_LittlefsConfigDefault, block);
                emptyBlocks += 1;
            }
        }
    }
    _unlock(s_LittlefsHandler.lock);
    return ret;
}

int32_t SLN_FLASH_LITTLEFS_GetFileSystemSize(void)
{
    int32_t fsBlocks = 0;

    if (_lock(s_LittlefsHandler.lock))
    {
        return SLN_FLASH_FS_ENOLOCK;
    }

    fsBlocks = lfs_fs_size(&s_LittlefsHandler.lfs);
    _unlock(s_LittlefsHandler.lock);

    return fsBlocks * FLASH_SECTOR_SIZE;
}

