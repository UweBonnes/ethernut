/******************************************************************************
 *
 *  stm32f303xB_flash.ld - linker script for nutos on cortex-m3 STM32F303xB
 *
 *****************************************************************************/

ENTRY(NutInit)
SEARCH_DIR(.)

"bootloader_offset" = 0x00;

MEMORY
{
    FLASH0 (rx) : ORIGIN = 0x08000000, LENGTH = 128K
    SRAM0 (rwx) : ORIGIN = 0x20000000, LENGTH =  32K
    CCM   (rwx) : ORIGIN = 0x10000000, LENGTH =   8K
}

INCLUDE stm32f10x_flash.ld
