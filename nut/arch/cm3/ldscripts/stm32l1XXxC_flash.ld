/******************************************************************************
 *
 *  stm32l1XXxE_flash.ld - linker script for nutos on cortex-m3 STM32F1XXxE
 *
 *****************************************************************************/

ENTRY(NutInit)
SEARCH_DIR(.)

 "bootloader_offset" = 0x00;

MEMORY
{
    FLASH0 (rx) : ORIGIN = 0x08000000, LENGTH = 512K
    SRAM0 (rwx) : ORIGIN = 0x20000000, LENGTH = 80K
}

INCLUDE stm32f10x_flash.ld
