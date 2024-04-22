/*
 * Copyright 2019-2022 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2009-2021 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "sln_flash.h"
#include "sln_flash_config.h"
#include "sln_flash_ops.h"
#include "fsl_flexspi.h"

void SLN_ram_memset(void *dst, uint8_t data, size_t len)
{
    uint8_t *ptr = (uint8_t *)dst;

    while (len)
    {
        *ptr++ = data;
        len--;
    }
}

void SLN_ram_memcpy(void *dst, void *src, size_t len)
{
    uint8_t *ptrDst = (uint8_t *)dst;
    uint8_t *ptrSrc = (uint8_t *)src;

    while (len)
    {
        *ptrDst++ = *ptrSrc++;
        len--;
    }
}

uint32_t SLN_ram_disable_irq(void)
{
    uint32_t regPrimask = 0;

    // Get primask
    __ASM volatile("MRS %0, primask" : "=r"(regPrimask)::"memory");

    // Disable irq
    __ASM volatile("cpsid i" : : : "memory");

    return regPrimask;
}

void SLN_ram_enable_irq(uint32_t priMask)
{
    __ASM volatile("MSR primask, %0" : : "r"(priMask) : "memory");
}

void SLN_ram_disable_d_cache(void)
{
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
    register uint32_t ccsidr;
    register uint32_t sets;
    register uint32_t ways;

    SCB->CSSELR = 0U; /*(0U << 1U) | 0U;*/ /* Level 1 data cache */
    __ASM volatile("dsb 0xF" ::: "memory");

    SCB->CCR &= ~(uint32_t)SCB_CCR_DC_Msk; /* disable D-Cache */
    __ASM volatile("dsb 0xF" ::: "memory");

    ccsidr = SCB->CCSIDR;

    /* clean & invalidate D-Cache */
    sets = (uint32_t)(CCSIDR_SETS(ccsidr));
    do
    {
        ways = (uint32_t)(CCSIDR_WAYS(ccsidr));
        do
        {
            SCB->DCCISW = (((sets << SCB_DCCISW_SET_Pos) & SCB_DCCISW_SET_Msk) |
                           ((ways << SCB_DCCISW_WAY_Pos) & SCB_DCCISW_WAY_Msk));
#if defined(__CC_ARM)
            __schedule_barrier();
#endif
        } while (ways-- != 0U);
    } while (sets-- != 0U);

    __ASM volatile("dsb 0xF" ::: "memory");
    __ASM volatile("isb 0xF" ::: "memory");
#endif
}

void SLN_ram_enable_d_cache(void)
{
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
    uint32_t ccsidr;
    uint32_t sets;
    uint32_t ways;

    SCB->CSSELR = 0U; /*(0U << 1U) | 0U;*/ /* Level 1 data cache */
    __ASM volatile("dsb 0xF" ::: "memory");

    ccsidr = SCB->CCSIDR;

    /* invalidate D-Cache */
    sets = (uint32_t)(CCSIDR_SETS(ccsidr));
    do
    {
        ways = (uint32_t)(CCSIDR_WAYS(ccsidr));
        do
        {
            SCB->DCISW =
                (((sets << SCB_DCISW_SET_Pos) & SCB_DCISW_SET_Msk) | ((ways << SCB_DCISW_WAY_Pos) & SCB_DCISW_WAY_Msk));
#if defined(__CC_ARM)
            __schedule_barrier();
#endif
        } while (ways-- != 0U);
    } while (sets-- != 0U);
    __ASM volatile("dsb 0xF" ::: "memory");

    SCB->CCR |= (uint32_t)SCB_CCR_DC_Msk; /* enable D-Cache */

    __ASM volatile("dsb 0xF" ::: "memory");
    __ASM volatile("isb 0xF" ::: "memory");
#endif
}

void SLN_Flash_Init(void)
{
    uint32_t irqState;

    irqState = SLN_ram_disable_irq();

    SLN_ram_disable_d_cache();

    sln_flash_ops_init(BOARD_FLEXSPI);

    /* Do software reset. */
    FLEXSPI_SoftwareReset(BOARD_FLEXSPI);

    SLN_ram_enable_d_cache();

    SLN_ram_enable_irq(irqState);
    /* Flush pipeline to allow pending interrupts take place
     * before starting next loop */
    __ASM volatile("isb 0xF" ::: "memory");
}

status_t SLN_Write_Flash_Page(uint32_t address, uint8_t *data, uint32_t len)
{
    status_t status = 0;
    uint32_t irqState;

    irqState = SLN_ram_disable_irq();

    SLN_ram_disable_d_cache();

    /* Setup page size write buffer */
    uint8_t tempPage[FLASH_PAGE_SIZE];

    SLN_ram_memset(tempPage, 0xFF, FLASH_PAGE_SIZE);

    SLN_ram_memcpy(tempPage, data, len);

    /* Program page. */
    status = sln_flash_ops_page_program(BOARD_FLEXSPI, address, (void *)tempPage);

    SLN_ram_enable_d_cache();

    SLN_ram_enable_irq(irqState);
    /* Flush pipeline to allow pending interrupts take place
     * before starting next loop */
    __ASM volatile("isb 0xF" ::: "memory");

    return status;
}

static status_t SLN_Write_Flash_Page2(uint32_t address, uint8_t *data, uint32_t len)
{
    status_t status = 0;

    /* Setup page size write buffer */
    uint8_t tempPage[FLASH_PAGE_SIZE];

    SLN_ram_memset(tempPage, 0xFF, FLASH_PAGE_SIZE);

    SLN_ram_memcpy(tempPage, data, len);

    /*Program page. */
    status = sln_flash_ops_page_program(BOARD_FLEXSPI, address, (void *)tempPage);

    return status;
}

static status_t SLN_Erase_Sector2(uint32_t address)
{
    status_t status = 0;

    /* Erase sectors. */
    status = sln_flash_ops_erase_sector(BOARD_FLEXSPI, address);

    /* Do software reset. */
    FLEXSPI_SoftwareReset(BOARD_FLEXSPI);

    return status;
}

int32_t SLN_Write_Sector(uint32_t address, uint8_t *buf, uint32_t len)
{
    int32_t status     = kStatus_Success;
    uint32_t pageCount = 0;
    uint32_t pageMod   = 0;
    uint32_t toCopy    = 0;
    uint32_t irqState;

    irqState = SLN_ram_disable_irq();

    SLN_ram_disable_d_cache();

    status = SLN_Erase_Sector2(address);

    if (kStatus_Success == status)
    {
        // Adjust total write length to fit in sector
        len = (FLASH_SECTOR_SIZE < len) ? FLASH_SECTOR_SIZE : len;

        pageCount = len / FLASH_PAGE_SIZE;
        pageMod   = len % FLASH_PAGE_SIZE;

        if (pageMod)
        {
            pageCount++;
        }

        do
        {
            // How much should we copy? SLN_Write_Flash_Page will fill end of page with ones.
            toCopy = (len > FLASH_PAGE_SIZE) ? FLASH_PAGE_SIZE : len;

            // Write a page worth of data to NVM
            status = SLN_Write_Flash_Page2(address, (uint8_t *)buf, toCopy);

            if (kStatus_Success != status)
            {
                break;
            }

            address += FLASH_PAGE_SIZE; // Increment the destination NVM address
            buf += FLASH_PAGE_SIZE;     // Increment the source RAM address
            len -= FLASH_PAGE_SIZE;     // Decrement the total length
        } while (--pageCount);
    }

    SLN_ram_enable_d_cache();

    SLN_ram_enable_irq(irqState);
    /* Flush pipeline to allow pending interrupts take place
     * before starting next loop */
    __ASM volatile("isb 0xF" ::: "memory");

    return status;
}

status_t SLN_Erase_Sector(uint32_t address)
{
    status_t status = 0;
    uint32_t irqState;

    irqState = SLN_ram_disable_irq();

    SLN_ram_disable_d_cache();

    /* Erase sectors. */
    status = sln_flash_ops_erase_sector(BOARD_FLEXSPI, address);

    /* Do software reset. */
    FLEXSPI_SoftwareReset(BOARD_FLEXSPI);

    SLN_ram_enable_d_cache();

    SLN_ram_enable_irq(irqState);
    /* Flush pipeline to allow pending interrupts take place
     * before starting next loop */
    __ASM volatile("isb 0xF" ::: "memory");

    return status;
}

#if defined(ERASE_BLOCK_SUPPORT)
status_t SLN_Erase_Block(uint32_t address)
{
    status_t status = 0;
    uint32_t irqState;

    irqState = SLN_ram_disable_irq();

    SLN_ram_disable_d_cache();

    /* Erase sectors. */
    status = flexspi_nor_flash_erase_block(BOARD_FLEXSPI, address);

    /* Do software reset. */
    FLEXSPI_SoftwareReset(BOARD_FLEXSPI);

    SLN_ram_enable_d_cache();

    SLN_ram_enable_irq(irqState);
    /* Flush pipeline to allow pending interrupts take place
     * before starting next loop */
    __ASM volatile("isb 0xF" ::: "memory");

    return status;
}
#endif /* ERASE_BLOCK_SUPPORT */

/* NOTE: SLN_Erase_Sector must be called prior to writing pages in a sector
 *       Afterwards, multiple pages can be written in that sector
 * NOTE: This function must always be used for writing full write pages (512 bytes) */
status_t SLN_Write_Flash_At_Address(uint32_t address, uint8_t *data)
{
    status_t status = 0;
    uint32_t irqState;

    irqState = SLN_ram_disable_irq();

    SLN_ram_disable_d_cache();

    /* Do software reset. */
    FLEXSPI_SoftwareReset(BOARD_FLEXSPI);

    /*Program page. */
    status = sln_flash_ops_page_program(BOARD_FLEXSPI, address, (void *)data);

    SLN_ram_enable_d_cache();

    SLN_ram_enable_irq(irqState);
    /* Flush pipeline to allow pending interrupts take place
     * before starting next loop */
    __ASM volatile("isb 0xF" ::: "memory");

    return status;
}

status_t SLN_Read_Flash_At_Address(uint32_t address, uint8_t *data, uint32_t size)
{
    SLN_ram_memcpy(data, (void *)(FLEXSPI_AMBA_BASE + address), size);

    return kStatus_Success;
}

uint32_t SLN_Flash_Get_Read_Address(uint32_t address)
{
    return FLEXSPI_AMBA_BASE + address;
}
