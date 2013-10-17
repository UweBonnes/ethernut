/******************************************************************************
 *
 *  stm32f401xC.ld - linker script for nutos on cortex-m3 STM32F401xC
 *
 *****************************************************************************/

ENTRY(NutInit)
SEARCH_DIR(.)

"bootloader_offset" = 0x00;

MEMORY
{
    FLASH0 (rx) : ORIGIN = 0x08000000, LENGTH = 256K
    SRAM0 (rwx) : ORIGIN = 0x20000000, LENGTH = 64K
}

INCLUDE stm32f10x_flash.ld
