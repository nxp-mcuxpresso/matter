/*
 * Copyright 2019-2022, 2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SLN_FLASH_LITTLEFS_
#define _SLN_FLASH_LITTLEFS_

/*!
 * SLN Flash Management
 *
 * Sector Size: 256 KBytes
 * Page Size:  512 Bytes
 *
 * One Sector per File
 *
 * Sector Layout:
 *
 * Page #   | Usage
 * ---------|------------------
 *    0     | Sector Map
 *    1     | File {Header, Data}
 *    2     | File {Header, Data}
 *    ...   | ...
 *    255   | File {Header, Data}
 *
 *  New File entries are page aligned; some wasted space may occur
 *
 */

#include <stdint.h>
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "sln_flash_config.h"
#include "fica_definition.h"
#include "sln_flash_fs.h"
#include "lfs.h"

#define LFS_BASE_ADDR          (FICA_IMG_FILE_SYS_ADDR)
#define LFS_LAST_ADDR          (FICA_IMG_FILE_SYS_ADDR + FICA_FILE_SYS_SIZE)
#define LFS_SECTORS            (FICA_FILE_SYS_SIZE / FLASH_SECTOR_SIZE)
#define LFS_CACHE_SIZE         (FLASH_PAGE_SIZE)
#define LFS_LOOKAHEAD_BUF_SIZE (64)

typedef struct _sln_littlefs
{
    lfs_t lfs;
    SemaphoreHandle_t lock;
    struct lfs_config cfg;
    uint8_t lfsMounted;
} sln_littlefs_t;

extern sln_littlefs_t s_LittlefsHandler;

/*!
 * @brief Initialize flash management; initializes private memory and file lock
 *
 * @param erase Flag to indicate whether to erase all entries during initilialization
 *
 * @returns Status of initialization
 *
 */

sln_flash_fs_status_t SLN_FLASH_LITTLEFS_Init(uint8_t erase);

/*!
 * @brief Save to a named entry. If the files doesn't exit a new entry will be created with
 * default configuration noencryption
 *
 * @param name String name of entry/file to save data into
 * @param data Pointer to data to save to file
 * @param len Length in bytes to save
 *
 * @returns Status of save
 */
sln_flash_fs_status_t SLN_FLASH_LITTLEFS_Save(const char *name, uint8_t *data, uint32_t len);

/*!
 * @brief Append the data to an existing file. If no fail exist, an error will be returned.
 * Not working for encrypted files
 *
 * @param name String name of entry/file to save data into
 * @param data Pointer to data to save to file
 * @param len Length in bytes to save
 *
 * @returns Status of save
 */
sln_flash_fs_status_t SLN_FLASH_LITTLEFS_Append(const char *name, uint8_t *data, uint32_t len);

/*!
 * @brief Update a named entry [can only clear bits as per nature of Flash Memory]
 *
 * @param name String name of entry/file to update data
 * @param data Pointer to data to update to file
 * @param len Pointer for length in bytes to update, output back to user to know files size on NVM
 *
 * @returns Status of save [will fail with SLN_FLASH_FS_ENOENTRY2 if no entry]
 */

sln_flash_fs_status_t SLN_FLASH_LITTLEFS_Update(const char *name, uint8_t *data, uint32_t *len);

/*!
 * @brief Read from a named entry
 *
 * Usage:
 * @code
 *
 *      int32_t ret = SLN_FLASH_FS_OK;
 *      uint8_t *data = NULL;
 *      uint32_t len = 0;
 *
 *      ret = SLN_FLASH_LITTLEFS_Read("file.dat", NULL, 0, &len);
 *
 *      if (SLN_FLASH_FS_OK == ret)
 *      {
 *          // len is set to size in bytes of file data in Flash
 *          data = (uint8_t *)pvPortMalloc(len);
 *          if (NULL == data)
 *          {
 *              // Handle per application spec
 *          }
 *      }
 *      else
 *      {
 *          // Handle per application spec
 *      }
 *
 *      // Before calling len can be reduced to read less data,
 *      // but cannot be increased beyond size in file header (function will reduce len param value)
 *      ret = SLN_FLASH_FS_Read("file.dat", data, &len);
 *
 *      if (SLN_FLASH_FS_OK == ret)
 *      {
 *          // Data is read correctly
 *      }
 *      else
 *      {
 *          // Handle per application spec
 *      }
 *
 * @endcode
 *
 * @param name String name of entry/file to read from
 * @param data Pointer to data to copy data into. If NULL, len will be populated
 * with size of the file saved.
 * @param offset Read file starting with an offset
 * @param len Pointer for length in bytes to read
 *
 * @returns Status of read
 */

sln_flash_fs_status_t SLN_FLASH_LITTLEFS_Read(const char *name, uint8_t *data, uint32_t offset, uint32_t *len);

/*!
 * @brief Completely erase an entry [sector erase]
 *
 * @param name String name of entry/file to erase
 *
 * @returns Status of the erase
 */
sln_flash_fs_status_t SLN_FLASH_LITTLEFS_Erase(const char *name);

/*!
 * @brief De-initialize the file system
 *
 * @param *flashEntries Pointer to the list of entries to de-initialize
 * @param erase Flag to indicate whether on not to erase each entry/sector during de-init
 *
 * @returns Status of de-initialization
 */
sln_flash_fs_status_t SLN_FLASH_LITTLEFS_Deinit(uint8_t erase);

/*!
 * @brief Format the filesystem
 *
 * @returns Status of the format
 */
sln_flash_fs_status_t SLN_FLASH_LITTLEFS_Format();

/*!
 * @brief Set the flash fs callbacks
 *
 * @param *cbs Pointer to a sln_flash_fs_cbs_t structure
 *
 * @returns SLN_FLASH_FS_OK on success, an error code otherwise
 */
sln_flash_fs_status_t SLN_FLASH_LITTLEFS_SetCbs(sln_flash_fs_cbs_t *cbs);

/*!
 * @brief Make directory.
 *
 * @param name String name of the directory to create
 *
 * @returns Status of the mkdir
 */
sln_flash_fs_status_t SLN_FLASH_LITTLEFS_Mkdir(const char *name);

/*!
 * @brief Make file with attributes. For now only the encrypt attribute exist..
 *
 * @param name String name of the directory to create
 * @param encrypt If the file should be encrypted
 * @returns Status of the mkfile
 */
sln_flash_fs_status_t SLN_FLASH_LITTLEFS_Mkfile(const char *name, bool encrypt);

/*!
 * @brief Rename a file
 *
 * @param oldName String name of the old file
 * @param newName String name of the new file
 * @returns Status of the rename
 */
sln_flash_fs_status_t SLN_FLASH_LITTLEFS_Rename(const char *oldName, const char *newName);

/*!
 * @brief
 *
 * @param timeout_ms The time allowed to run the cleanup operation
 * @returns Status of the cleanup
 */
sln_flash_fs_status_t SLN_FLASH_LITTLEFS_Cleanup(uint32_t timeout_ms);

/*!
 * @brief Get filesystem size in bytes
 *
 * @returns Filesystem size or negative value on failure
 */
int32_t SLN_FLASH_LITTLEFS_GetFileSystemSize(void);





#endif /* _SLN_FLASH_LITTLEFS_ */
