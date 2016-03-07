/******************************************************************************
 *
 *  stm32f302xB_flash.ld - linker script for nutos on cortex-m3 STM32F302xB,
 *  STM32F373xB and  STM32F383xB
 *
 *****************************************************************************/

ENTRY(NutInit)
SEARCH_DIR(.)

"bootloader_offset" = 0x00;

MEMORY
{
    FLASH0 (rx) : ORIGIN = 0x08000000, LENGTH = 128K
    SRAM0 (rwx) : ORIGIN = 0x20000000, LENGTH =  24K
}

INCLUDE stm32f10x_flash.ld
