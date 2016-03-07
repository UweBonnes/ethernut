/******************************************************************************
 *
 *  stm32F042x6_flash.ld - linker script for nutos on cortex-m0 STM32F042x6
 *
 *****************************************************************************/

ENTRY(NutInit)
SEARCH_DIR(.)

 "bootloader_offset" = 0x00;

MEMORY
{
    FLASH0 (rx) : ORIGIN = 0x08000000, LENGTH = 32K
    SRAM0 (rwx) : ORIGIN = 0x20000000, LENGTH =  6K
}

INCLUDE stm32f10x_flash.ld
