/*
** ###################################################################
**     Processors:          RW610EVA0IK
**                          RW610EVA0IMP
**                          RW610HNA0IK
**                          RW610HNA0IMP
**                          RW610UKA0IZ
**
**     Compiler:            GNU C Compiler
**     Reference manual:    RW61X User manual Rev. 0.95, June 2022
**     Version:             rev. 1.0, 2021-03-16
**     Build:               b220615
**
**     Abstract:
**         Linker file for the GNU C Compiler
**
**     Copyright 2016 Freescale Semiconductor, Inc.
**     Copyright 2016-2023 NXP
**     All rights reserved.
**
**     SPDX-License-Identifier: BSD-3-Clause
**
**     http:                 www.nxp.com
**     mail:                 support@nxp.com
**
** ###################################################################
*/

/**
 * @file
 *   Linker instructions for RW610
 */


/* Entry Point */
ENTRY(Reset_Handler)

HEAP_SIZE  = DEFINED(__heap_size__)  ? __heap_size__  : 0x0A00;
STACK_SIZE = DEFINED(__stack_size__) ? __stack_size__ : 0x0A00;
M_VECTOR_RAM_SIZE = DEFINED(__ram_vector_table__) ? 0x00000280 : 0;

/* -------------------------------------------------------------------------- */
/*                               FLEX SPI FLASH                               */
/* -------------------------------------------------------------------------- */

m_flash_start                       = 0x08000000;
m_flash_size                        = 0x00300000; /* 3MB */
m_flash_end                         = m_flash_start + m_flash_size - 1;
m_sector_size                       = 0x1000;
m_vector_table_size                 = 0x280;

m_flash_config_start                = 0x08000400;
m_flash_config_size                 = 0xC00;
m_flash_config_end                  = m_flash_config_start + m_flash_config_size - 1;

m_interrupts_start                  = m_flash_config_end + 1;
m_interrupts_end                    = m_interrupts_start + m_vector_table_size - 1;

/* FileSystem Configuration */
NV_STORAGE_END_ADDRESS              = m_flash_end;
NV_STORAGE_MAX_SECTORS              = DEFINED(gNVMSectorCountLink_d) ? gNVMSectorCountLink_d : 16;
NV_STORAGE_SECTOR_SIZE              = m_sector_size;
NV_STORAGE_SIZE                     = NV_STORAGE_SECTOR_SIZE * NV_STORAGE_MAX_SECTORS;
NV_STORAGE_START_ADDRESS            = NV_STORAGE_END_ADDRESS - NV_STORAGE_SIZE + 1;
NV_STORAGE_START_ADDRESS_OFFSET     = NV_STORAGE_START_ADDRESS - m_flash_start;

m_text_start                        = m_interrupts_end + 1;
m_text_end                          = NV_STORAGE_START_ADDRESS - 1;
m_text_size                         = m_text_end - m_text_start;


/* -------------------------------------------------------------------------- */
/*                                    RAM                                     */
/* -------------------------------------------------------------------------- */

m_data_start                        = 0x20000000;
m_data_size                         = 0x00130000;
m_data_end                          = 0x2012FFFF;

/* -------------------------------------------------------------------------- */
/*                              SMU1 - CPU3/CPU1                              */
/* -------------------------------------------------------------------------- */

m_sqram_cpu13_mbox_start            = 0x41380000;
m_sqram_cpu13_mbox_size             = 0x00000488;
m_sqram_cpu13_mbox_end              = m_sqram_cpu13_mbox_start + m_sqram_cpu13_mbox_size - 1;

m_sqram_cpu13_txq_start             = m_sqram_cpu13_mbox_end + 1;
m_sqram_cpu13_txq_size              = 0x1000;
m_sqram_cpu13_txq_end               = m_sqram_cpu13_txq_start + m_sqram_cpu13_txq_size - 1;

/* -------------------------------------------------------------------------- */
/*                              SMU2 - CPU3/CPU2                              */
/* -------------------------------------------------------------------------- */

m_sqram_cpu23_mbox_start            = 0x443C0000;
m_sqram_cpu23_mbox_size             = 0x00000488;
m_sqram_cpu23_mbox_end              = m_sqram_cpu23_mbox_start + m_sqram_cpu23_mbox_size - 1;

m_sqram_cpu23_txq_start             = m_sqram_cpu23_mbox_end + 1;
m_sqram_cpu23_txq_size              = 0x1080;
m_sqram_cpu23_txq_end               = m_sqram_cpu23_txq_start + m_sqram_cpu23_txq_size - 1;

m_sqram_cpu32_txq_start             = m_sqram_cpu23_txq_end + 1;
m_sqram_cpu32_txq_size              = 0x1080;
m_sqram_cpu32_txq_end               = m_sqram_cpu32_txq_start + m_sqram_cpu32_txq_size - 1;


/* Specify the memory areas */
MEMORY
{
  m_flash_config        (RX)  : ORIGIN = m_flash_config_start, LENGTH = m_flash_config_size
  m_interrupts          (RX)  : ORIGIN = m_interrupts_start, LENGTH = m_vector_table_size
  m_text                (RX)  : ORIGIN = m_text_start, LENGTH = m_text_size
  m_nvm_region          (RW)  : ORIGIN = NV_STORAGE_START_ADDRESS, LENGTH = NV_STORAGE_SIZE  
  m_data                (RW)  : ORIGIN = m_data_start, LENGTH = m_data_size
  m_sqram_cpu13_mbox    (RW)  : ORIGIN = m_sqram_cpu13_mbox_start, LENGTH = m_sqram_cpu13_mbox_size
  m_sqram_cpu13_txq     (RW)  : ORIGIN = m_sqram_cpu13_txq_start, LENGTH = m_sqram_cpu13_txq_size
  m_sqram_cpu23_mbox    (RW)  : ORIGIN = m_sqram_cpu23_mbox_start, LENGTH = m_sqram_cpu23_mbox_size
  m_sqram_cpu23_txq     (RW)  : ORIGIN = m_sqram_cpu23_txq_start, LENGTH = m_sqram_cpu23_txq_size
  m_sqram_cpu32_txq     (RW)  : ORIGIN = m_sqram_cpu32_txq_start, LENGTH = m_sqram_cpu32_txq_size
}


/* Define output sections */
SECTIONS
{
  .flash_config : ALIGN(4)
    {
       . = ALIGN(4);
      __FLASH_BASE = .;
      KEEP(* (.flash_conf))     /* flash config section */
      . = ALIGN(4);
    } > m_flash_config

  /* The startup code goes first into internal ram */
  .interrupts :
  {
    . = ALIGN(4);
    __VECTOR_TABLE = .;
    __Vectors = .;
    KEEP(*(.isr_vector))     /* Startup code */
    . = ALIGN(4);
  } > m_interrupts

  /* The program code and other data goes into internal ram */
  .text :
  {
    . = ALIGN(4);
    *(.rcs.*)    
    *(EXCLUDE_FILE(
        *port.c.obj
        *portasm.c.obj
        *queue.c.obj
        *timers.c.obj
        *tasks.c.obj
        *inet_chksum.c.obj
        *ip4.c.obj
        *pbuf.c.obj
        *sys_arch.c.obj
        *tcp_out.c.obj
        *tcp_in.c.obj
        *mlan_11n_rxreorder.c.obj
        *mlan_wmm.c.obj
        *wifi.c.obj
        *wifi-imu.c.obj
        *fsl_adapter_rfimu.c.obj
        *fsl_cache.c.obj
        *fsl_os_abstraction_free_rtos.c.obj
	/* Exclude flash and frequently executed functions from XIP */
        *fsl_adapter_flexspi_nor_flash.c.o
        *mflash_drv.c.o
        *fsl_flexspi.c.o
      ) .text*)              /* .text* sections (code) */
    /*(EXCLUDE_FILE(
        /* Exclude flash and frequently executed functions from XIP *
        *fsl_adapter_flexspi_nor_flash.c.o
        *mflash_drv.c.o
        *fsl_flexspi.c.o
    ).rodata*)               /* .rodata* sections (constants, strings, etc.) */
    *(.rodata .rodata.* .constdata .constdata.*)
    *(.glue_7)               /* glue arm to thumb code */
    *(.glue_7t)              /* glue thumb to arm code */
    *(.eh_frame)
    KEEP (*(.init))
    KEEP (*(.fini))
    . = ALIGN(4);
  } > m_text

  ._bt_settings_area :
  {
    . = ALIGN(4);
    _bt_settings_start = .;
    KEEP(*(SORT(._bt_settings.static.*)))
    _bt_settings_end = .;
    . = ALIGN(4);
  } > m_text

  ._settings_handler_static :
  {
    . = ALIGN(4);
    _settings_handler_static_list_start = .;
    KEEP(*(SORT(._settings_handler_static.static.*)))
    _settings_handler_static_list_end = .;
    . = ALIGN(4);
  } > m_text

  ._bt_conn_cb :
  {
    . = ALIGN(4);
    _bt_conn_cb_list_start = .;
    KEEP(*(SORT(._bt_conn_cb.static.*)))
    _bt_conn_cb_list_end = .;
    . = ALIGN(4);
  } > m_text

  ._bt_gatt_service_static :
  {
    . = ALIGN(4);
    _bt_gatt_service_static_list_start = .;
    KEEP(*(SORT(._bt_gatt_service_static.static.*)))
    _bt_gatt_service_static_list_end = .;
    . = ALIGN(4);
  } > m_text

  ._bt_l2cap_fixed_chan :
  {
    . = ALIGN(4);
    _bt_l2cap_fixed_chan_list_start = .;
    KEEP(*(SORT(._bt_l2cap_fixed_chan.static.*)))
    _bt_l2cap_fixed_chan_list_end = .;
    . = ALIGN(4);
  } > m_text

  ._bt_l2cap_br_fixed_chan :
  {
    . = ALIGN(4);
    _bt_l2cap_br_fixed_chan_list_start = .;
    KEEP(*(SORT(._bt_l2cap_br_fixed_chan.static.*)))
    _bt_l2cap_br_fixed_chan_list_end = .;
    . = ALIGN(4);
  } > m_text

  .ARM.extab :
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } > m_text

  .ARM :
  {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } > m_text

 .ctors :
  {
    __CTOR_LIST__ = .;
    /* gcc uses crtbegin.o to find the start of
       the constructors, so we make sure it is
       first.  Because this is a wildcard, it
       doesn't matter if the user does not
       actually link against crtbegin.o; the
       linker won't look for a file to match a
       wildcard.  The wildcard also means that it
       doesn't matter which directory crtbegin.o
       is in.  */
    KEEP (*crtbegin.o(.ctors))
    KEEP (*crtbegin?.o(.ctors))
    /* We don't want to include the .ctor section from
       from the crtend.o file until after the sorted ctors.
       The .ctor section from the crtend file contains the
       end of ctors marker and it must be last */
    KEEP (*(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
    __CTOR_END__ = .;
  } > m_text

  .dtors :
  {
    __DTOR_LIST__ = .;
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
    __DTOR_END__ = .;
  } > m_text

  .preinit_array :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } > m_text

  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } > m_text

  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } > m_text

  __etext = .;    /* define a global symbol at end of code */
  __DATA_ROM = .; /* Symbol is used by startup for data initialization */

  .interrupts_ram :
  {
    . = ALIGN(4);
    __VECTOR_RAM__ = .;
    __interrupts_ram_start__ = .; /* Create a global symbol at data start */
    *(.m_interrupts_ram)     /* This is a user defined section */
    . += M_VECTOR_RAM_SIZE;
    . = ALIGN(4);
    __interrupts_ram_end__ = .; /* Define a global symbol at data end */
  } > m_data

  __VECTOR_RAM = DEFINED(__ram_vector_table__) ? __VECTOR_RAM__ : ORIGIN(m_interrupts);
  __RAM_VECTOR_TABLE_SIZE_BYTES = DEFINED(__ram_vector_table__) ? (__interrupts_ram_end__ - __interrupts_ram_start__) : 0x0;

  .data : AT(__DATA_ROM)
  {
    . = ALIGN(4);
    __DATA_RAM = .;
    __data_start__ = .;      /* create a global symbol at data start */
    *(CodeQuickAccess)       /* CodeQuickAccess sections */
    *(DataQuickAccess)       /* DataQuickAccess sections */
    *(.data)                 /* .data sections */
    *(.data*)                /* .data* sections */
    *port.c.obj(.text*)
    *portasm.c.obj(.text*)
    *queue.c.obj(.text*)
    *timers.c.obj(.text*)
    *tasks.c.obj(.text*)
    *inet_chksum.c.obj(.text*)
    *ip4.c.obj(.text*)
    *pbuf.c.obj(.text*)
    *sys_arch.c.obj(.text*)
    *tcp_out.c.obj(.text*)
    *tcp_in.c.obj(.text*)
    *mlan_11n_rxreorder.c.obj(.text*)
    *mlan_wmm.c.obj(.text*)
    *wifi.c.obj(.text*)
    *wifi-imu.c.obj(.text*)
    *fsl_adapter_rfimu.c.obj(.text*)
    *fsl_cache.c.obj(.text*)
    *fsl_os_abstraction_free_rtos.c.obj(.text*)
    /* Explicit placement of flash and frequently executed functions in RAM */
    *fsl_adapter_flexspi_nor_flash.c.o(.text .text* .rodata .rodata*)
    *mflash_drv.c.o(.text* .text* .rodata .rodata*)
    *fsl_flexspi.c.o(.text .text* .rodata .rodata*)
    KEEP(*(.jcr*))
    . = ALIGN(4);
    _k_mem_slab_list_start = .;
    KEEP(*(SORT(._k_mem_slab*)))
    _k_mem_slab_list_end = .;
    . = ALIGN(4);
    _net_buf_pool_list = .;
    KEEP(*(SORT(._net_buf_pool*)))
    . = ALIGN(4);
    __data_end__ = .;        /* define a global symbol at data end */
  } > m_data

  __DATA_END = __DATA_ROM + (__data_end__ - __data_start__);
  
  text_end = ORIGIN(m_text) + LENGTH(m_text);

  ASSERT(__DATA_END <= text_end, "region m_text overflowed with text and data")

  _image_size = __DATA_END - __VECTOR_TABLE;

  /* Uninitialized data section */
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss section */
    . = ALIGN(4);
    __START_BSS = .;
    __bss_start__ = .;
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    __bss_end__ = .;
    __END_BSS = .;
  } > m_data

  .stack :
  {
    . = ALIGN(8);
    __StackLimit = .;
    . += STACK_SIZE;
    __StackTop = .;
  } > m_data

  /* Initializes stack on the end of block */
  /*__StackTop   = ORIGIN(m_data) + LENGTH(m_data);
  __StackLimit = __StackTop - STACK_SIZE;*/
  PROVIDE(__stack = __StackTop);

  .heap :
  {
    . = ALIGN(8);
    PROVIDE(end = .);
    __HeapBase = .;
    . += HEAP_SIZE;
    __HeapLimit = .;
    __heap_limit = .; /* Add for _sbrk */
    *(.heap)
  } > m_data

  __HEAP_end__ = m_data_end;

  .smu_cpu13_mbox (NOLOAD) :
  {
    . = ALIGN(4);
    *(.smu_cpu13_mbox)
    KEEP (*(.smu_cpu13_mbox))
    . = ALIGN(4);
  } > m_sqram_cpu13_mbox

  .smu_cpu31_txq (NOLOAD) :
  {
    . = ALIGN(4);
    *(.smu_cpu31_txq)
    KEEP (*(.smu_cpu31_txq))
    . = ALIGN(4);
  } > m_sqram_cpu13_txq

 .smu_cpu23_mbox (NOLOAD) :
  {
    . = ALIGN(4);
    SQRAM_CPU23_MBOX = .;
    *(.smu_cpu23_mbox)
    KEEP (*(.smu_cpu23_mbox))
    . = ALIGN(4);
  } > m_sqram_cpu23_mbox

  .smu_cpu32_txq (NOLOAD) :
  {
    . = ALIGN(4);
    *(.smu_cpu32_txq)
    KEEP (*(.smu_cpu32_txq))
    . = ALIGN(4);
  } > m_sqram_cpu32_txq

  .NVM :
  {
    FILL(0xFFFFFFFF);
    . = ORIGIN(m_nvm_region) + LENGTH(m_nvm_region) - 1;
    BYTE(0xFF);
  } > m_nvm_region

  .ARM.attributes 0 : { *(.ARM.attributes) }

  /*ASSERT(__StackLimit >= __HeapLimit, "region m_data overflowed with stack and heap")*/
}
