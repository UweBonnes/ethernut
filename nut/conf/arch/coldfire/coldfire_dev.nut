nutarch_m68k_coldfire_devices =
{
    --
    -- UART Debug Output
    --
    {
        name = "nutarch_m68k_coldfire_devices_debug",
        brief = "UART Debug Output",
        description = "Polling UART driver.",
        requires = { "NUT_OSTIMER_DEV", "HW_UART"},
        provides = { "DEV_UART", "DEV_FILE", "DEV_WRITE" },
        sources = { "m68k/coldfire/dev/common/debug_uart.c"},
        options =
        {
            {
                macro = "DEBUG_USE_UART",
                brief = "Select Debug Channel",
                description = "Select the UART to use as debug output\n",
                type = "enumerated",
                default = "DEV_UART0",
                choices = { "DEV_UART0", "DEV_UART1", "DEV_UART2"},
                file = "include/cfg/uart.h",
            }
        }
    },
    
    --
    -- Default Exception Handler
    --
    {
        name = "nutarch_m68k_coldfire_devices_ih_default",
        brief = "Default Exception Handler",
        description = "Default Exception Handler is used as default interrupt handler for common interrupts.\n"..
                      "Is is usefull for spurious interrupts catching and analysing.\n\n"..
                      "This handler handles only CPU interrupts common for all coldfire CPUs. "..
                      "Family dependant interrupts are processed by the family Default Exception Handler",
        sources = { "m68k/coldfire/dev/common/ih/ih_default.c"},
    },
}
