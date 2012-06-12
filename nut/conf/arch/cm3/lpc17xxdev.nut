nutarch_cm3_lpc17xx_devices =
{
    -- ***********************************
    --
    -- LPC17xx Device Drivers
    --
    -- ***********************************

    --
    -- LPC17xx Debug UART
    --
    {
        name = "nutarch_cm3_lpc17xx_debug",
        brief = "UART Debug Output (LPC17xx)",
        requires = { "DEV_UART_LPC17xx" },
        provides = { "DEV_UART", "DEV_FILE", "DEV_WRITE" },
        options =
        {
            {
                macro = "DEBUG_USE_UART",
                brief = "Select Debug Channel",
                description = "Select the UART to use as debug output\n"..
                              "This is only to adapt the application examples and to tell them\n"..
                              "which USART to use as stdout. See next entry.",
                type = "enumerated",
                default = "DEV_UART0",
                choices = { "DEV_UART0", "DEV_UART1", "DEV_UART2", "DEV_UART3"},
                file = "include/cfg/uart.h",
--                sources = { "cm3/dev/nxp/lpc17xx_usart.c" }
            }
        }
    },

    {
        name = "nutarch_cm3_lpc1768_debug",
        brief = "UART Debug Output (LPC1768)",
        description = "Polling UART driver\n"..
                      "The debug output entry above uses the interrupt driven UART "..
                      "driver, which cannot be used in interrupt context."..
                      "But that's exactly what I need right now.\n\n"..
                      "I've currently no idea, which CPU variants this polling "..
                      "driver may support. For the time being this is limited "..
                      "to the LPC1768.",
        requires = { "HW_MCU_LPC1768" },
        provides = { "DEV_UART", "DEV_FILE", "DEV_WRITE" },
        sources = { "cm3/dev/nxp/lpc_debug.c", "cm3/dev/nxp/lpc176x_debug0.c" },
    },

    --
    -- LPC17xx RTC
    --
    {
        name = "nutarch_cm3_lpc17xx_rtc",
        brief = "LPC17xx RTC Driver",
        description = "LPC17xx RTC driver.",
        requires = { "HW_RTC_LPC17xx" },
        provides = { "DEV_RTC" },
        sources = { "cm3/dev/nxp/lpc17xx_rtc.c", "cm3/dev/nxp/ih_lpc17xx_rtc.c" },
    },
    --
    -- LPC17xx Watchdog Timer
    --
    {
        name = "nutarch_cm3_lpc17xx_wdt",
        brief = "LPC17xx Watchdog Timer",
        requires = { "HW_WDT_LPC17xx" },
        sources = { "cm3/dev/nxp/lpc17xx_wdt.c", "cm3/dev/nxp/ih_lpc17xx_wdt.c"}
    },
    --
    -- LPC17xx Flash Memory Controller
    --
    {
        name = "nutarch_cm3_lpc17xx_iap",
        brief = "LPC17xx In Application Flash Programming API (IAP)",
        description = "Routines for setup and programming LPC17x series internal FLASH.\n",
        requires = { "HW_FLASH_LPC17xx" },
        sources = { "cm3/dev/nxp/lpc17xx_iap.c" }
    },
    --
    -- LPC17xx General purpose DMA Controller
    --
    {
        name = "nutarch_cm3_lpc17xx_gpdma",
        brief = "LPC17xx General purpose DMA Controller API",
        description = "Routines for setup and programming LPC17x series GPDMA controller.\n",
        requires = { "HW_GPDMA_LPC17xx" },
        sources = { "cm3/dev/nxp/lpc17xx_gpdma.c" }
    },
}

