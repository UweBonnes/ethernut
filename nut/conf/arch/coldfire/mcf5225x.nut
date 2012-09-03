nutarch_m68k_coldfire_mcf5225x =
{
   	--
    -- MCU Family
    --
    {
        name = "nutarch_m68k_coldfire_mcf5225x_family",
        brief = "Family",
        provides = {
	        	"HW_UART_COLDFIRE",
	        	"HW_PIT_COLDFIRE",
	        	"HW_CWD_COLDFIRE",
	        	"HW_I2C_COLDFIRE",
		},
        options =
        {
            {
                macro = "MCU_MCF5225X",
                brief = "MCF5225X",
                description = "MCF5225X Coldfire Family",
                type = "integer",
                default = 1,
                file = "include/cfg/arch.h"
            },
            {
                macro = "PIT0",
                type = "integer",
                default = 1,
                provides = { "HW_PIT0" },
                file = "include/cfg/peripherals.h"
            },
            {
                macro = "PIT1",
                type = "integer",
                default = 1,
                provides = { "HW_PIT1" },
                file = "include/cfg/peripherals.h"
            },
            {
                macro = "UART0",
                type = "integer",
                default = 1,
                provides = { "HW_UART0" },
                file = "include/cfg/peripherals.h"
            },
            {
                macro = "UART1",
                type = "integer",
                default = 1,
                provides = { "HW_UART1" },
                file = "include/cfg/peripherals.h"
            },
            {
                macro = "UART2",
                type = "integer",
                default = 1,
                provides = { "HW_UART2" },
                file = "include/cfg/peripherals.h"
            },
            {
                macro = "I2C0",
                type = "integer",
                default = 1,
                provides = { "HW_I2C0" },
                file = "include/cfg/peripherals.h"
            },
            {
                macro = "I2C1",
                type = "integer",
                default = 1,
                provides = { "HW_I2C1" },
                file = "include/cfg/peripherals.h"
            },
        }
    },
    
    --
    -- Runtime Initialization
    --
    {
        name = "nutarch_m68k_coldfire_mcf5225x_init",
        brief = "Initialization",
		description = "Family dependant parts of startup code for MCF5225X family CPUs:\n"..
		              "  - Vector table\n"..
		              "  - Memories\n"..
		              "  - CPU\n"..
		              "  - Peripherals",
        sources = {	"m68k/coldfire/init/crt_mcf5225x.S", 
        			"m68k/coldfire/init/crt_mcf5225x_c.c" },
        targets = { "m68k/coldfire/init/crt_mcf5225x.o" },
        requires = { "TOOL_CC_M68K", "TOOL_GCC"},
    },    
    
    --
    -- Interrupt Handler
    --
    {
        name = "nutarch_m68k_coldfire_mcf5225x_ihndlr",
        brief = "Interrupt Handler",
        description = "Peripheral interrupt handlers for MCF5225X family.",
        provides = { "DEV_IRQ_MCF5225X" },
        sources = { "m68k/coldfire/dev/mcf5225x/ih/ih_common.c",
        			"m68k/coldfire/dev/mcf5225x/ih/ih_pit.c",
        			"m68k/coldfire/dev/mcf5225x/ih/ih_cwd.c",
        			"m68k/coldfire/dev/mcf5225x/ih/ih_i2c.c"}
    },

    --
    -- System Timer Hardware
    --
    {
        name = "nutarch_m68k_coldfire_mcf5225x_ostimer",
        brief = "System Timer",
        requires = { "HW_PIT_COLDFIRE", "HW_PIT0", "DEV_IRQ_MCF5225X" },
        provides = { "NUT_OSTIMER_DEV" },
        sources = { "m68k/coldfire/dev/mcf5225x/ostimer.c" },
    },
    --
    -- Reset Controller
    --
    {
        name = "nutarch_m68k_coldfire_mcf5225x_reset",
        brief = "Reset Controller",
        provides = { "DEV_MCU_RESET" },
        sources = { "m68k/coldfire/dev/mcf5225x/reset.c" },
    },
    --
    -- Core Watchdog
    --
    {
        name = "nutarch_m68k_coldfire_mcf5225x_cwd",
        brief = "Core Watchdog",
        requires = {"HW_CWD_COLDFIRE", "DEV_IRQ_MCF5225X" },
        sources = { "m68k/coldfire/dev/mcf5225x/cwd.c" }
    },
}
