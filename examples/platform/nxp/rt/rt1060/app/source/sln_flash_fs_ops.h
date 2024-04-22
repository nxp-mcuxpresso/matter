/*
 * Copyright 2019-2022, 2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SLN_FLASH_FS_OPS_
#define _SLN_FLASH_FS_OPS_

#include "sln_flash_fs.h"
#include "sln_flash_littlefs.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define sln_flash_fs_ops_init(erase)                      SLN_FLASH_LITTLEFS_Init(erase)
#define sln_flash_fs_ops_deinit(erase)                    SLN_FLASH_LITTLEFS_Deinit(erase)
#define sln_flash_fs_ops_erase(pFile)                     SLN_FLASH_LITTLEFS_Erase(pFile)
#define sln_flash_fs_ops_save(pFile, pData, len)          SLN_FLASH_LITTLEFS_Save(pFile, pData, len)
#define sln_flash_fs_ops_update(pFile, pData, pLen)       SLN_FLASH_LITTLEFS_Update(pFile, pData, pLen)
#define sln_flash_fs_ops_read(pFile, pData, offset, pLen) SLN_FLASH_LITTLEFS_Read(pFile, pData, offset, pLen)
#define sln_flash_fs_ops_getFileSystemSize()              SLN_FLASH_LITTLEFS_GetFileSystemSize()

/* Littlefs doesn't support readPtr. A file can be split accrosed several sectors not consecutive */
#define sln_flash_fs_ops_readPtr(pFile, ppData, pLen)

/* Set callbacks */
#define sln_flash_fs_ops_setcbs(cbs) SLN_FLASH_LITTLEFS_SetCbs(cbs)

/* Not used in bootloader */

#define sln_flash_fs_ops_mkdir(pDir)                SLN_FLASH_LITTLEFS_Mkdir(pDir)
#define sln_flash_fs_ops_mkfile(pFile, encrypt)     SLN_FLASH_LITTLEFS_Mkfile(pFile, encrypt)
#define sln_flash_fs_ops_rename(pOldFile, pNewFile) SLN_FLASH_LITTLEFS_Rename(pOldFile, pNewFile)

/*******************************************************************************
 * API
 ******************************************************************************/

#endif /* _SLN_FLASH_FS_OPS_ */
