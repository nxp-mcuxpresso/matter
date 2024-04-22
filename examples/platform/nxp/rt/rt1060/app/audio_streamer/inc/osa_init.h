/*
 * Copyright 2022 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OSA_INIT_H
#define OSA_INIT_H

/*!
 * @file    osa_init.h
 * @brief   Contains initialization function prototypes for libosa.
 */

/*!
 * @ingroup libosa
 * @brief   Initialize the OSA library
 * @details Some parts of the OSA need to be initialized before first use.
 *          This function performs the necessary initialization and
 *          allocations.
 * @retval  ERRCODE_NO_ERROR  Function succeeded
 * @retval  ERRCODE_GENERAL_ERROR   Error initializing OSA.  Memory allocations
 *                                  failed.
*/
int osa_init(void);

#endif

