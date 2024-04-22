/*
 * Copyright 2018, 2020 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */



void HardFault_Handler(void)
{
    __asm("BKPT #0");
}

void MemManage_Handler(void)
{
    __asm("BKPT #1");
}

void BusFault_Handler(void)
{
    __asm("BKPT #2");
}

void UsageFault_Handler(void)
{
    __asm("BKPT #3");
}
