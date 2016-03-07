/******************************************************************************
 *
 *  stm32f7XXxG_flash.ld - linker script for nutos on cortex-m3 STM32F745 and
 *  STM32F746 with 1 MiB Flash
 *
 *****************************************************************************/

ENTRY(NutInit)
SEARCH_DIR(.)

"bootloader_offset" = 0x00;


/* Forget about different memories for now*/
MEMORY
{
    FLASH0 (rx) : ORIGIN = 0x08000000, LENGTH = 1024K
    SRAM0 (rwx) : ORIGIN = 0x20000000, LENGTH =  320K
}

INCLUDE stm32f10x_flash.ld
