/*
 * Copyright 2019-2022 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SLN_FLASH_FS_
#define _SLN_FLASH_FS_

/**
 * @brief Returns codes of the flash management functions
 */
typedef enum _sln_flash_fs_status
{
    SLN_FLASH_FS_OK         = 0x00,  /**< No errors */
    SLN_FLASH_FS_FAIL       = -0x01, /**< Fail for unknown reason */
    SLN_FLASH_FS_ENOLOCK    = -0x02, /**< Error file is locked */
    SLN_FLASH_FS_ERETRY     = -0x10, /**< Fail to lock the file */
    SLN_FLASH_FS_ENOMEM     = -0x20, /**< Fail to allocate memory */
    SLN_FLASH_FS_ENOMEM2    = -0x21, /**< Fail to allocate memory */
    SLN_FLASH_FS_ENOMEM3    = -0x22, /**< Fail to allocate memory */
    SLN_FLASH_FS_ENOMEM4    = -0x23, /**< Fail to allocate memory */
    SLN_FLASH_FS_EIO        = -0x30, /**< Fail to erase the sector */
    SLN_FLASH_FS_EINVAL     = -0x40, /**< Invalid input value */
    SLN_FLASH_FS_EINVAL2    = -0x41, /**< Invalid input value */
    SLN_FLASH_FS_EINVAL3    = -0x42, /**< Invalid input value */
    SLN_FLASH_FS_ENOENTRY   = -0x50, /**< No entry/directory found in the flash */
    SLN_FLASH_FS_ENOENTRY2  = -0x51, /**< No entry found in the flash */
    SLN_FLASH_FS_ENOENTRY3  = -0x52, /**< No entry found in the flash */
    SLN_FLASH_FS_EOVERFLOW  = -0x60, /**< The flash map is full */
    SLN_FLASH_FS_EOVERFLOW2 = -0x61, /**< The save file doesn't fit in the current sector */
    SLN_FLASH_FS_EENCRYPT   = -0x70, /**< Fail to encrypt or decrypt */
    SLN_FLASH_FS_EENCRYPT2  = -0x71, /**< Crc mismatch */
    SLN_FLASH_FS_CORRUPTED  = -0x72, /**< File system corrupted */
    SLN_FLASH_FS_FILEEXIST  = -0x73, /**< File already exists */
} sln_flash_fs_status_t;

/* Use the flash API with or without encryption */
#define SLN_FLASH_PLAIN     (false)
#define SLN_FLASH_ENCRYPTED (true)

typedef struct _sln_flash_fs_cbs
{
    void (*post_lock_cb)(void);         /*! Callback to be called after the lock takes place */
    void (*post_unlock_cb)(void);       /*! Callback to be called after the unlock takes place */
    void (*pre_sector_erase_cb)(void);  /*! Callback to be called before erasing a sector */
    void (*post_sector_erase_cb)(void); /*! Callback to be called after erasing a sector */
} sln_flash_fs_cbs_t;

#endif /* _SLN_FLASH_FS_ */
