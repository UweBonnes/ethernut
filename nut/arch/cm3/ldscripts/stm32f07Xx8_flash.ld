/******************************************************************************
 *
 *  stm32F051x8_flash.ld - linker script for nutos on cortex-m0 STM32F051x8
 *
 *****************************************************************************/

ENTRY(NutInit)
SEARCH_DIR(.)

 "bootloader_offset" = 0x00;

MEMORY
{
    FLASH0 (rx) : ORIGIN = 0x08000000, LENGTH = 64K
    SRAM0 (rwx) : ORIGIN = 0x20000000, LENGTH = 16K
}

INCLUDE stm32f10x_flash.ld
