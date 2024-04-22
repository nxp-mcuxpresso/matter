/*
 * Copyright 2021 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

.section .rodata
.align 4

.global en_model_begin

en_model_begin:
.incbin "./en/oob_demo_en_pack_WithMapID.bin"
en_model_end:
