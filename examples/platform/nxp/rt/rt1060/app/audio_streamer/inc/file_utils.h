/*
 * Copyright 2022 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef FILE_UTILS_H
#define FILE_UTILS_H

/*!
 * @file    file_utils.h
 * @brief   This file provides file manipulation functions.
 */

#include "osa_common.h"

/*!
 * @ingroup libcommon
 * @brief file_getsize
 * @details Function to get the device file size.
 * @param fd       File handle
 * @returns size of file in bytes
 */
uint32_t file_getsize(void *fd);

/*!
 * @ingroup libcommon
 * @brief   file_exists
 * @details Returns true if the given file exists on disk and false if not
 * @param   filename Name and path of the file to test for existence
 * @returns true or false
 */
bool file_exists(const char *filename);

/*!
 * @ingroup libcommon
 * @brief   dir_exists
 * @details Returns true if the given directory exists on disk and false if not
 * @param   dirname Name and path of the directory to test for existence
 * @returns true or false
 */
bool dir_exists(const char *dirname);

#endif

