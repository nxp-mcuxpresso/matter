/*
 * Copyright 2022 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OSA_MISC_H
#define OSA_MISC_H

/*!
 * @file    osa_misc.h
 * @brief   Contains miscellaneous function prototypes for libosa.
 */

/*!
 * @ingroup libosa
 * @brief   Exit thread
 * @details Causes the calling thread to exit with a return status.  On Linux
 *          this will cause the full process to terminate so it should be used
 *          with caution.  On MQX it will cause the OS to exit completely and
 *          sit in a tight loop with interrupts disabled.
 * @param   status An integer status to be posted to the OS on exit
 */
void osa_exit(int status);

/*!
 * @brief Misc Virtual Functions - Table of functions called by the API.
 */
struct OsaMiscFunctions {
    void (*exit) (int status);
};

extern struct OsaMiscFunctions misc_vtable;

#endif

/*! @} */

