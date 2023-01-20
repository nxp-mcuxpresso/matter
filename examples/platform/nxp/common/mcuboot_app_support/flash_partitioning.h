/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FLASH_PARTITIONING_H_
#define _FLASH_PARTITIONING_H_

/* Flash base address */
#define BOOT_FLASH_BASE     0x60000000
/* Start address of the primary application partition (Active App) */
#define BOOT_FLASH_ACT_APP  0x60011000
/* Start address of the secondary application partition (Candidate App) */
#define BOOT_FLASH_CAND_APP 0x60311000

#endif
