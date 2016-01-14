/******************************************************************************
 *
 *  stm32l15XxC_flash.ld - linker script for nutos on cortex-m3 STM32F15XxC
 *
 *****************************************************************************/

ENTRY(NutInit)
SEARCH_DIR(.)

MEMORY
{
    FLASH0 (rx) : ORIGIN = 0x08000000, LENGTH = 256K
    SRAM0 (rwx) : ORIGIN = 0x20000000, LENGTH = 32K
}

INCLUDE stm32f10x_flash.ld
