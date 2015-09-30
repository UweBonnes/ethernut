/******************************************************************************
 *
 *  stm32l4XXxG.ld - linker script for nutos on cortex-m3 STM32L4XXxG
 *
 *****************************************************************************/

ENTRY(NutInit)
SEARCH_DIR(.)

"bootloader_offset" = 0x00;

MEMORY
{
    FLASH0 (rx) : ORIGIN = 0x08000000, LENGTH = 1024K
    SRAM0 (rwx) : ORIGIN = 0x20000000, LENGTH =   96K
    SRAM1 (rwx) : ORIGIN = 0x10000000, LENGTH =   32K
}

INCLUDE stm32f10x_flash.ld
