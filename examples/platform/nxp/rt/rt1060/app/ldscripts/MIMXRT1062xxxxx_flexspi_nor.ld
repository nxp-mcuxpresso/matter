/*
 *  Copyright (c) 2021, The OpenThread Authors.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file
 *   Linker instructions for RT1060
 */


/** If OTA is enabled : MCUBoot bootloader resides at the base of the flash and occupies 0x11000 (68kBytes) 
  slot 0 : main application partition (active app)
           slot0 (3MB) = header (4kB) + ivt (1kB) + BOARD_FLASH + trailer (4kB) + padding (4kB)
  slot 1 : OTA Update storage partition (candidate app)
           slot1 (3MB) = OTA_image (code is already wrapped with header & trailer)
**/

/************************ Flash Layout with OTA enabled *********************

             0x6000_0000    FLASH_START
    - - - +---------------+ - - - - - - - - - - - - - - - -
          |               |                 
          |    MCUBoot    |                 
   68k    |   bootloader  |                 
          |               |
    - - - +---------------+ - - - - - - - - - - - - - - - -
   4k     |  Header_slot0 |                            '
          +---------------+ m_interrupts_start         '
   4k     |  m_interrupts |                            '
          +---------------+ m_text_start               '
          |               |                            '
(3M-4*4k) |     BOARD_FLASH    |                            '  slot 0 (3M)
          |               |                            '
          +---------------+ m_text_end                 '
   4k     | Trailer_slot0 |                            '
          +---------------+                            '
   4k     |    padding    | (padding used by mcuboot)  '
    - - - +---------------+ - - - - - - - - - - - - - - - -
          |               | OTA_STORAGE_START_ADDRESS  '
          |               |                            '
          |               |                            '
          |               |                            '
   3M     |     Slot 1    |                            '  slot 1 (3M)
          |               |                            '
          |               |                            '
          |               |                            '
          |               | OTA_STORAGE_END_ADDRESS    '
    - - - +---------------+ - - - - - - - - - - - - - - - -
          +               +
          +               + 
    - - - +---------------+ - - - - - - - - - - - - - - - -
   4k     | FactoryData   |                             '
    - - - +---------------+ - - - - - - - - - - - - - - - -
          |               | NV_STORAGE_START_ADDRESS
          |   NVM_region  | 
          |               | NV_STORAGE_END_ADDRESS
    - - - +---------------+ - - - - - - - -
             0x6080_0000    FLASH_END

*****************************************************************************/


/* Entry Point */
ENTRY(ResetISR)

FLASH_START                  = 0x60000000;
FLASH_SIZE                   = 0x1000000; /* 16 MBytes */
FLASH_END                    = FLASH_START + FLASH_SIZE - 1;
FLASH_SECTOR_SIZE            = 0x1000;  /* 4k flash sector size */

/*** FileSystem Configuration ***/
/* OT/Matter/BLE  filesystem configuration*/
NV_STORAGE_END_ADDRESS       = FLASH_END;
NV_STORAGE_MAX_SECTORS       = DEFINED(gNVMSectorCountLink_d) ? gNVMSectorCountLink_d : 32;
NV_STORAGE_SECTOR_SIZE       = FLASH_SECTOR_SIZE;
NV_STORAGE_SIZE              = FLASH_SECTOR_SIZE * NV_STORAGE_MAX_SECTORS;
NV_STORAGE_START_ADDRESS     = NV_STORAGE_END_ADDRESS - NV_STORAGE_SIZE + 1;
NV_STORAGE_START_ADDRESS_OFFSET = NV_STORAGE_START_ADDRESS - FLASH_START;

/*** FactoryData space 1 sector is reserved ***/
__FACTORY_DATA_SIZE   = FLASH_SECTOR_SIZE;
__FACTORY_DATA_END    = NV_STORAGE_START_ADDRESS -1;
__FACTORY_DATA_START  = __FACTORY_DATA_END - __FACTORY_DATA_SIZE + 1;
__FACTORY_DATA_START_OFFSET = __FACTORY_DATA_START - FLASH_START;

/* Max number of sectors in a slot (mcuboot design) */
slot_max_sectors = 0x300; /* 3 MBytes */

/* If OTA is enabled, we must reserve enough space for mcuboot and the image header before m_interrupts */
/*mcu_boot_size                = DEFINED(__mcu_boot_size__)  ? __mcu_boot_size__  : 0;*/
mcuboot_header_start         = DEFINED(__mcu_boot_size__) ? FLASH_START + mcu_boot_size : 0;
m_flash_config_size          = FLASH_SECTOR_SIZE;
m_interrupts_start           = DEFINED(__mcu_boot_size__) ? mcuboot_header_start + FLASH_SECTOR_SIZE : FLASH_START + 2*FLASH_SECTOR_SIZE;
m_interrupts_size            = 0x00001000;

/* BOARD_FLASH starts after the header of the application image (4kB) + ivt (m_interrupts_size) */
m_text_start                 = m_interrupts_start + m_interrupts_size;
m_text_max_sectors           = slot_max_sectors - 4;   /* BOARD_FLASH = slot0 (3MB) - [header(4kB) + ivt(1kB) + trailer(4kB) + padding(4kB)] */
/* If mcuboot is used, allocate 3MB for text to accomodate OTA storage, else allocate memory up to FactoryData region */
m_text_size                  = DEFINED(__mcu_boot_size__) ? m_text_max_sectors * FLASH_SECTOR_SIZE : __FACTORY_DATA_START - m_text_start;
m_text_end                   = m_text_start + m_text_size - 1;

/* OTA Update Image section */
OTA_STORAGE_MAX_SECTORS      = slot_max_sectors; /* slot 1 must be equal to slot 0 */
OTA_STORAGE_START_ADDRESS    = mcuboot_header_start + slot_max_sectors * FLASH_SECTOR_SIZE;
OTA_STORAGE_SIZE             = FLASH_SECTOR_SIZE * OTA_STORAGE_MAX_SECTORS;
OTA_STORAGE_END_ADDRESS      = OTA_STORAGE_START_ADDRESS + OTA_STORAGE_SIZE - 1;

HEAP_SIZE  = DEFINED(__heap_size__)  ? __heap_size__  : 0;
STACK_SIZE = DEFINED(__stack_size__) ? __stack_size__ : 0x0800;
VECTOR_RAM_SIZE = DEFINED(__ram_vector_table__) ? 0x00000400 : 0;

ITCM_START = 0x00000000;
ITCM_SIZE = 0x00018000; /* 96kb */

DTCM_START = 0x20000000;
DTCM_SIZE = 0x00068000; /* 416kb */

RAM_NC_START = 0x20200000;
RAM_NC_SIZE = 0x00040000; /* 256kb */

RAM_C_START = 0x20240000;
RAM_C_SIZE = 0x00040000 ; /* 256kb */

/* Specify the memory areas */
MEMORY
{
  m_flash_config        (RX)  : ORIGIN = FLASH_START, LENGTH = m_flash_config_size
  m_ivt                 (RX)  : ORIGIN = FLASH_START + m_flash_config_size, LENGTH = 0x00001000
  m_interrupts          (RX)  : ORIGIN = m_interrupts_start, LENGTH = m_interrupts_size
  BOARD_FLASH                (RX)  : ORIGIN = m_text_start, LENGTH = m_text_size
  /* SRAM_ITC zone is used to store code for functions to be executed from RAM. Adding an offset
   * to prevent overwrite in case of using NULL pointers to reference data
  */
  SRAM_ITC              (RX)  : ORIGIN = ITCM_START, LENGTH = ITCM_SIZE /* ITCM 64 KB */
  NVM_region            (RW)  : ORIGIN = NV_STORAGE_START_ADDRESS, LENGTH = NV_STORAGE_SIZE
  SRAM_DTC              (RW)  : ORIGIN = DTCM_START, LENGTH = DTCM_SIZE /* DTCM 448 KB */
  SRAM_OC_NON_CACHEABLE              (RW)  : ORIGIN = RAM_NC_START, LENGTH = RAM_NC_SIZE /* OCRAM2 32 KB */
  SRAM_OC_CACHEABLE               (RW)  : ORIGIN = RAM_C_START, LENGTH = RAM_C_SIZE /* OCRAM2 480 KB */
}

  /* Define a symbol for the top of each memory region */
  __base_BOARD_FLASH = FLASH_START  ; /* BOARD_FLASH */  
  __base_Flash = FLASH_START ; /* Flash */  
  __top_BOARD_FLASH = FLASH_START + FLASH_SIZE ;
  __top_Flash = FLASH_START+ FLASH_SIZE ;

  __base_SRAM_DTC = DTCM_START  ; /* SRAM_DTC */  
  __base_RAM = DTCM_START ; /* RAM */  
  __top_SRAM_DTC = DTCM_START + DTCM_SIZE ; 
  __top_RAM = DTCM_START + DTCM_SIZE ; 
    
  __base_SRAM_ITC = ITCM_START  ; /* SRAM_ITC */  
  __base_RAM2 = ITCM_START ; /* RAM2 */  
  __top_SRAM_ITC = ITCM_START + ITCM_SIZE ; 
  __top_RAM2 = ITCM_START + ITCM_SIZE ; 

  __base_SRAM_OC_NON_CACHEABLE = RAM_NC_START  ; /* SRAM_OC_NON_CACHEABLE */  
  __base_RAM3 = RAM_NC_START ; /* RAM3 */  
  __top_SRAM_OC_NON_CACHEABLE = RAM_NC_START + RAM_NC_SIZE ;
  __top_RAM3 = RAM_NC_START + RAM_NC_SIZE ; 

  __base_SRAM_OC_CACHEABLE = RAM_C_START  ; /* SRAM_OC_CACHEABLE */  
  __base_RAM4 = RAM_C_START ; /* RAM4 */  
  __top_SRAM_OC_CACHEABLE = RAM_C_START + RAM_C_SIZE ; 
  __top_RAM4 = RAM_C_START + RAM_C_SIZE ; 
  
/* Define output sections */
SECTIONS
{
  __NCACHE_REGION_START = ORIGIN(SRAM_OC_NON_CACHEABLE);
  __NCACHE_REGION_SIZE  = LENGTH(SRAM_OC_NON_CACHEABLE);

  /* If OTA is enabled this section should not be loaded as mcuboot already includes it*/
  .flash_config :
  {
    . = ALIGN(4);
    __FLASH_BASE = .;
    __boot_hdr_start__ = .;
    KEEP(* (.boot_hdr.conf))     /* flash config section */
    . = ALIGN(4);
  } > m_flash_config

  ivt_begin = ORIGIN(m_flash_config) + LENGTH(m_flash_config);

  /* If OTA is enabled this section should not be loaded as mcuboot already includes it*/
  .ivt : AT(ivt_begin)
  {
    . = ALIGN(4);
	KEEP(* (.boot_hdr.ivt))           /* ivt section */
	KEEP(* (.boot_hdr.boot_data))     /* boot section */
	KEEP(* (.boot_hdr.dcd_data))      /* dcd section */
  __boot_hdr_end__ = .;
    . = ALIGN(4);
  } > m_ivt

  /* The startup code goes first into internal RAM */
  .interrupts :
  {
    __VECTOR_TABLE = .;
    __Vectors = .;
    . = ALIGN(4);
    KEEP(*(.isr_vector))     /* Startup code */
    . = ALIGN(4);
  } > m_interrupts

  /* The program code and other data goes into internal RAM */
  .text :
  {
      /* Global Section Table */
    . = ALIGN(4) ;
    __section_table_start = .;
    __data_section_table = .;
    LONG(LOADADDR(.data));
    LONG(    ADDR(.data));
    LONG(  SIZEOF(.data));
    LONG(LOADADDR(.data_SRAM_ITC));
    LONG(    ADDR(.data_SRAM_ITC));
    LONG(  SIZEOF(.data_SRAM_ITC));
    LONG(LOADADDR(.data_SRAM_OC_NON_CACHEABLE));
    LONG(    ADDR(.data_SRAM_OC_NON_CACHEABLE));
    LONG(  SIZEOF(.data_SRAM_OC_NON_CACHEABLE));
    __data_section_table_end = .;
    __bss_section_table = .;
    LONG(    ADDR(.bss));
    LONG(  SIZEOF(.bss));
    LONG(    ADDR(.bss_SRAM_ITC));
    LONG(  SIZEOF(.bss_SRAM_ITC));
    LONG(    ADDR(.bss_SRAM_DTC));
    LONG(  SIZEOF(.bss_SRAM_DTC));
    LONG(    ADDR(.bss_SRAM_OC_NON_CACHEABLE));
    LONG(  SIZEOF(.bss_SRAM_OC_NON_CACHEABLE));
    __bss_section_table_end = .;
    __section_table_end = . ;
    /* End of Global Section Table */

    *(.after_vectors*)
		
    . = ALIGN(4);
    *(EXCLUDE_FILE(
        /* Exclude flash and frequently executed functions from XIP */
        *fsl_adapter_flexspi_nor_flash.c.o
        *flexspi_nor_flash_ops.c.o 
        *sln_flash.c.o 
        *flexspi_nor_flash_ops.c.o 
        *fsl_flexspi.c.o 
        *sln_flash.c.o 
        *pdm_to_pcm_task.c.o 
        *audio_processing_task.c.o 
        *fsl_sai.c.o 
        *fsl_sai_edma.c.o 
        *fsl_edma.c.o 
        *sln_pdm_mic.c.o 
        *sln_i2s_mic.c.o 
        *sln_afe.c.o 
        *libvoiceseeker.a: 
        *libsln_dsp_toolbox.a:
        *mflash_drv.c.o
        *fsl_flexspi.c.o
        *queue.c.o
        *tcp_out.c.o
        *mlan_11n_rxreorder.c.o
        *lib_a-memcpy-stub.o
        *tcp_in.c.o
        *inet_chksum.c.o
        *timers.c.o
        *fsl_usdhc.c.o
        *port.c.o
        *wifi-sdio.c.o
        *fsl_sdio.c.o
        *pbuf.c.o
        *tasks.c.o
        *lib_a-memset.o
        *fsl_cache.c.o
        *wifi.c.o
      ) .text*)              /* .text* sections (code) */
    *(.rodata .rodata.* .constdata .constdata.*)
    *(.glue_7)               /* glue arm to thumb code */
    *(.glue_7t)              /* glue thumb to arm code */
    *(.eh_frame)
    KEEP (*(.init))
    KEEP (*(.fini))
    KEEP(*freertos*/sdk.tasks.c.o(.rodata*)) /* FreeRTOS Debug Config */
    . = ALIGN(4);
  } > BOARD_FLASH

  ._bt_settings_area :
  {
    . = ALIGN(4);
    _bt_settings_start = .;
    KEEP(*(SORT(._bt_settings.static.*)))
    _bt_settings_end = .;
    . = ALIGN(4);
  } > BOARD_FLASH

  ._settings_handler_static :
  {
    . = ALIGN(4);
    _settings_handler_static_list_start = .;
    KEEP(*(SORT(._settings_handler_static.static.*)))
    _settings_handler_static_list_end = .;
    . = ALIGN(4);
  } > BOARD_FLASH

  ._bt_conn_cb :
  {
    . = ALIGN(4);
    _bt_conn_cb_list_start = .;
    KEEP(*(SORT(._bt_conn_cb.static.*)))
    _bt_conn_cb_list_end = .;
    . = ALIGN(4);
  } > BOARD_FLASH

  ._bt_gatt_service_static :
  {
    . = ALIGN(4);
    _bt_gatt_service_static_list_start = .;
    KEEP(*(SORT(._bt_gatt_service_static.static.*)))
    _bt_gatt_service_static_list_end = .;
    . = ALIGN(4);
  } > BOARD_FLASH

  ._bt_l2cap_fixed_chan :
  {
    . = ALIGN(4);
    _bt_l2cap_fixed_chan_list_start = .;
    KEEP(*(SORT(._bt_l2cap_fixed_chan.static.*)))
    _bt_l2cap_fixed_chan_list_end = .;
    . = ALIGN(4);
  } > BOARD_FLASH

  ._bt_l2cap_br_fixed_chan :
  {
    . = ALIGN(4);
    _bt_l2cap_br_fixed_chan_list_start = .;
    KEEP(*(SORT(._bt_l2cap_br_fixed_chan.static.*)))
    _bt_l2cap_br_fixed_chan_list_end = .;
    . = ALIGN(4);
  } > BOARD_FLASH


  .ARM.extab :
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } > BOARD_FLASH

  .ARM :
  {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } > BOARD_FLASH

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
  } > BOARD_FLASH

  .dtors :
  {
    __DTOR_LIST__ = .;
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
    __DTOR_END__ = .;
  } > BOARD_FLASH

  .preinit_array :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } > BOARD_FLASH

  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } > BOARD_FLASH

  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } > BOARD_FLASH

  __etext = .;    /* define a global symbol at end of code */
  __DATA_ROM = .; /* Symbol is used by startup for data initialization */

  .interrupts_ram :
  {
    . = ALIGN(4);
    __VECTOR_RAM__ = .;
    __interrupts_ram_start__ = .; /* Create a global symbol at data start */
    *(.m_interrupts_ram)     /* This is a user defined section */
    . += VECTOR_RAM_SIZE;
    . = ALIGN(4);
    __interrupts_ram_end__ = .; /* Define a global symbol at data end */
  } > SRAM_OC_CACHEABLE

  __VECTOR_RAM = DEFINED(__ram_vector_table__) ? __VECTOR_RAM__ : ORIGIN(m_interrupts);
  __RAM_VECTOR_TABLE_SIZE_BYTES = DEFINED(__ram_vector_table__) ? (__interrupts_ram_end__ - __interrupts_ram_start__) : 0x0;

  .data : 
  {
    . = ALIGN(4);
    __DATA_RAM = .;
    __data_start__ = .;      /* create a global symbol at data start */
    *(m_usb_dma_init_data)
    *(.data)                 /* .data sections */
    *(.data*)                /* .data* sections */
    *(.wlan_data .wlan_data.*)
    KEEP(*(.jcr*))
    . = ALIGN(4);
    PROVIDE(__start_NVM_TABLE = .);
    KEEP(*(.NVM_TABLE));
    PROVIDE(__stop_NVM_TABLE = .);
    . = ALIGN(4);
    _net_buf_pool_list = .;
    KEEP(*(SORT(._net_buf_pool*)))
    . = ALIGN(4);
    __data_end__ = .;        /* define a global symbol at data end */
  } > SRAM_OC_CACHEABLE AT>BOARD_FLASH

  __ram_function_flash_start = __DATA_ROM + (__data_end__ - __data_start__); /* Symbol is used by startup for TCM data initialization */

  .data_SRAM_ITC : 
  {
    . = ALIGN(32);
    __ram_function_start__ = .;
    *(.vectorTableRam)
    *(CodeQuickAccess)
    *(.ramfunc.$SRAM_ITC)
    /* Explicit placement of flash and frequently executed functions in RAM */
    *flexspi_nor_flash_ops.c.o(.text*) 
    *sln_flash.c.o(.text*) 
    *fsl_adapter_flexspi_nor_flash.c.o(.text*)
    *pdm_to_pcm_task.c.o(.text*)
    *audio_processing_task.c.o(.text*)
    *libvoiceseeker.a:(.text*)
    *sln_flash.c.o(.text*)
    *flexspi_nor_flash_ops.c.o(.text*)
    *sln_afe.c.o(.text*)
    *sln_pdm_mic.c.o(.text*)
    *sln_i2s_mic.c.o(.text*)
    *fsl_sai.c.o(.text*)
    *fsl_sai_edma.c.o(.text*)
    *fsl_edma.c.o(.text*)
    *fsl_flexspi.c.o(.text*)
    *mflash_drv.c.o(.text*)
    *fsl_flexspi.c.o(.text*)
    *queue.c.o(.text*)
    *tcp_out.c.o(.text*)
    *mlan_11n_rxreorder.c.o(.text*)
    *lib_a-memcpy-stub.o(.text*)
    *tcp_in.c.o(.text*)
    *inet_chksum.c.o(.text*)
    *timers.c.o(.text*)
    *fsl_usdhc.c.o(.text*)
    *port.c.o(.text*)
    *wifi-sdio.c.o(.text*)
    *fsl_sdio.c.o(.text*)
    *pbuf.c.o(.text*)
    *tasks.c.o(.text*)
    *lib_a-memset.o(.text*)
    *fsl_cache.c.o(.text*)
    *wifi.c.o(.text*)
    . = ALIGN(128);
    __ram_function_end__ = .;
  } > SRAM_ITC AT>BOARD_FLASH

  /* BSS section for SRAM_ITC */
  .bss_SRAM_ITC (NOLOAD) : ALIGN(4)
  {
    PROVIDE(__start_bss_RAM2 = .) ;
    PROVIDE(__start_bss_SRAM_ITC = .) ;
    *(.bss.$RAM2)
    *(.bss.$SRAM_ITC)
    *(.bss.$RAM2.*)
    *(.bss.$SRAM_ITC.*)
    . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
    PROVIDE(__end_bss_RAM2 = .) ;
    PROVIDE(__end_bss_SRAM_ITC = .) ;
  } > SRAM_ITC AT> SRAM_ITC

    /* section for SRAM_DTC */
  .bss_SRAM_DTC (NOLOAD) : ALIGN(4)
  {
    PROVIDE(__start_bss_SRAM_DTC = .);
    *(.bss.$SRAM_DTC)
    *(.bss.$SRAM_DTC.*)
    . = ALIGN(4);
    PROVIDE(__end_bss_SRAM_DTC = .) ;
  } > SRAM_DTC AT> SRAM_DTC

  __NDATA_ROM = __ram_function_flash_start + (__ram_function_end__ - __ram_function_start__);
  .data_SRAM_OC_NON_CACHEABLE.init : AT(__NDATA_ROM)
  {
    __noncachedata_start__ = .;   /* create a global symbol at data_SRAM_OC_NON_CACHEABLE data start */
    *(NonCacheable.init)
    . = ALIGN(4);
    __noncachedata_init_end__ = .;   /* create a global symbol at initialized data_SRAM_OC_NON_CACHEABLE data end */
  } > SRAM_OC_NON_CACHEABLE
  . = __noncachedata_init_end__;
  .data_SRAM_OC_NON_CACHEABLE :
  {
    *(.ocram_non_cacheable_data)
    *(NonCacheable)
    *(.ramfunc.$RAM3)
    *(.ramfunc.$SRAM_OC_NON_CACHEABLE)
   
    . = ALIGN(4);
    __noncachedata_end__ = .;     /* define a global symbol at data_SRAM_OC_NON_CACHEABLE data end */
  } > SRAM_OC_NON_CACHEABLE AT>BOARD_FLASH

  .bss_SRAM_OC_NON_CACHEABLE (NOLOAD) : ALIGN(4)
  {
    PROVIDE(__start_bss_ncache = .);
    *(.ocram_non_cacheable_bss)
    *ccidec.o(.bss.*)
    *(.bss.$RAM3)
    *(.bss.$SRAM_OC_NON_CACHEABLE)
    *(.bss.$RAM3.*)
    *(.bss.$SRAM_OC_NON_CACHEABLE.*)
    . = ALIGN(4);
    PROVIDE(__end_bss_SRAM_ncache = .) ;
  } > SRAM_OC_NON_CACHEABLE AT> SRAM_OC_NON_CACHEABLE

  __DATA_END = __NDATA_ROM + (__noncachedata_init_end__ - __noncachedata_start__);
  text_end = ORIGIN(BOARD_FLASH) + LENGTH(BOARD_FLASH);
  ASSERT(__DATA_END <= text_end, "region BOARD_FLASH overflowed with text and data")

  /* Uninitialized data section */
  .bss (NOLOAD) :
  {
    /* This is used by the startup in order to initialize the .bss section */
    . = ALIGN(4);
    __START_BSS = .;
    __bss_start__ = .;
    *(m_usb_dma_noninit_data)
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    __bss_end__ = .;
    __END_BSS = .;
  } > SRAM_OC_CACHEABLE AT>SRAM_OC_CACHEABLE

  /* Reserve and place Heap within memory map */
  _HeapSize = 0x800;
  .heap (NOLOAD) :  ALIGN(4)
  {
    _pvHeapStart = .;
    . += _HeapSize;
    . = ALIGN(4);
    __HeapLimit = .;
    _pvHeapLimit = .;
  } > SRAM_DTC

   _StackSize = 0x800;
  /* Locate actual Stack in memory map */
  .stack ORIGIN(SRAM_OC_CACHEABLE) + LENGTH(SRAM_OC_CACHEABLE) - _StackSize - 0 (NOLOAD) :  ALIGN(4)
  {
    _vStackBase = .;
    . = ALIGN(4);
    __StackLimit = .;
    _vStackTop = . + _StackSize;
    __StackTop = .;
  } > SRAM_OC_CACHEABLE

  PROVIDE(__stack = __StackTop);

  /* Remove this section to keep the nvm section on writting the device */
  .NVM (NOLOAD) :
  {
    FILL(0xFFFFFFFF);
    . = ORIGIN(NVM_region) + LENGTH(NVM_region) - 1;
    BYTE(0xFF);
  } > NVM_region

  .ARM.attributes 0 : { *(.ARM.attributes) }

  ASSERT(__StackLimit >= __HeapLimit, "region m_data overflowed with stack and heap")

  ASSERT( (DEFINED(__mcu_boot_size__) ? (__mcu_boot_size__ % FLASH_SECTOR_SIZE) : 0) == 0, "mcuboot size is not aligned on flash sector size")
  
  ASSERT(__FACTORY_DATA_START >= OTA_STORAGE_END_ADDRESS, "OTA storage section overflowed")
  
  _image_start = LOADADDR(.text);
  _image_end = LOADADDR(.data) + SIZEOF(.data);
  _image_size = _image_end - _image_start;
}
