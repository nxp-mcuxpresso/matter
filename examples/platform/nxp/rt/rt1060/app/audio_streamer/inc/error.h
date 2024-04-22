/*
 * Copyright 2022 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __ERROR_H_
#define __ERROR_H_

/*!
 * @file    error.h
 * @brief   Contains common error codes and definitions.
 */

/*! General error/warning codes to be used by any module. */
#define ERRCODE_NO_ERROR 0x0
#define ERRCODE_TRY_AGAIN 0x1
#define ERRCODE_INVALID_ARGUMENT  0x02
#define ERRCODE_OUT_OF_MEMORY  0x03
#define ERRCODE_OUT_OF_SPACE  0x04
#define ERRCODE_GENERAL_ERROR  0x05
#define ERRCODE_DEADLOCK  0x06
#define ERRCODE_NOT_SUPPORTED  0x07
#define ERRCODE_BUSY  0x08
#define ERRCODE_PERMISSION_DENIED  0x09
#define ERRCODE_TIMED_OUT  0x0A
#define ERRCODE_SYSTEM_LIMIT  0x0B
#define ERRCODE_PARAMETER_EXISTS  0x0C
#define ERRCODE_END_OF_DIRECTORY  0x0D
#define ERRCODE_NOT_FOUND  0x0E
#define ERRCODE_UNKNOWN  0x0F
#define ERRCODE_INTERNAL  0x10
#define ERRCODE_OVERFLOW  0x11
#define ERRCODE_MSG_FAILURE 0x12
#define ERRCODE_THREAD_FAILURE 0x13
#define ERRCODE_MUTEX_FAILURE 0x14
#define ERRCODE_DEVICE_ELEMENT_EXISTS (0x15)
#define ERRCODE_NOT_SEEKABLE 0x16

#endif

