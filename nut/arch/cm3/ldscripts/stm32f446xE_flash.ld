/******************************************************************************
 *
 *  stm32f411xE.ld - linker script for nutos on cortex-m4 STM32F411xE
 *
 *****************************************************************************/

ENTRY(NutInit)
SEARCH_DIR(.)

"bootloader_offset" = 0x00;


MEMORY
{
    FLASH0 (rx) : ORIGIN = 0x08000000, LENGTH = 512K
    SRAM0 (rwx) : ORIGIN = 0x20000000, LENGTH = 112K
    DMA   (rwx) : ORIGIN = 0x2001c000, LENGTH =  16K
}

INCLUDE stm32f10x_flash.ld
