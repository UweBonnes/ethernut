/******************************************************************************
 *
 *  stm32f303xC_flash.ld - linker script for nutos on STM32F303xC/313xC
 *
 *****************************************************************************/

ENTRY(NutInit)
SEARCH_DIR(.)

"bootloader_offset" = 0x00;

MEMORY
{
    FLASH0 (rx) : ORIGIN = 0x08000000, LENGTH = 256K
    SRAM0 (rwx) : ORIGIN = 0x20000000, LENGTH =  40K
    CCM   (rwx) : ORIGIN = 0x10000000, LENGTH =   8K
}

INCLUDE stm32f10x_flash.ld
