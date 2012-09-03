nutarch_m68k_coldfire_devices =
{
    --
    -- UART Debug Output
    --
    {
        name = "nutarch_m68k_coldfire_devices_debug",
        brief = "UART Debug Output",
        description = "Polling UART driver.",
        requires = { "NUT_OSTIMER_DEV", "HW_UART_COLDFIRE"},
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
    
    --
    -- I2C Bus Default
    --
    {
        name = "nutarch_m68k_coldfire_i2c",
        brief = "I2C Bus Default",
        description = "Choose which I2C bus will be used as a default.\n"..
                      "TwiXxx() Macros will be reconfigured to use this bus.\n"..
                      "This is for compatibility with old Nut/OS.",
        requires = { "DEV_TWI" },
        options =
        {
            {
                macro = "I2CBUS0_AS_DEFAULT",
                brief = "I2C Bus 0 Controller",
                description = "TwiXxx() Macros will be reconfigured to use I2C Bus 0 Controller.",
                exclusivity = { "I2CBUS0_AS_DEFAULT", "I2CBUS1_AS_DEFAULT" },
                flavor = "booldata",
                file = "include/cfg/twi.h"
            },
            {
                macro = "I2CBUS1_AS_DEFAULT",
                brief = "I2C Bus 1 Controller",
                description = "TwiXxx() Macros will be reconfigured to use I2C Bus 1 Controller.",
                exclusivity = { "I2CBUS0_AS_DEFAULT", "I2CBUS1_AS_DEFAULT" },
                flavor = "booldata",
                file = "include/cfg/twi.h"
            },
        },
    },
    
    --
    -- I2C Bus 0 Configuration
    --
    {
        name = "nutarch_m68k_coldfire_i2c0",
        brief = "I2C Bus 0 Controller",
        description = "Coldfire hardware I2C support.",
        requires = { "HW_I2C_COLDFIRE", "HW_I2C0" },
        provides = { "DEV_TWI" },
        sources = { "m68k/coldfire/dev/common/mcf5_i2c.c",
                    "m68k/coldfire/dev/common/mcf5_i2c0.c" },
        options =
        {
            {
                macro = "I2CBUS0_DEFAULT_SPEED",
                brief = "Default Speed",
                description = "Default speed for this bus. Different speeds can be set by software.\n",
                default = "100",
                file = "include/cfg/twi.h"
            },
            {
                macro = "I2CBUS0_PINSET_MCF5225X",
                brief = "MCF5225X Pinset",
                description = "Please choose which pins are used for I2C Bus 0\n"..
                              "\nPIN_SET1:\n "..
                              "\tSCL: PAS0 (I2C_SCL0)\n "..
                              "\tSDA: PAS1 (I2C_SDA0)\n "..
                              "\nPIN_SET2:\n "..
                              "\tSCL: PQS2 (QSPI_CLK)\n "..
                              "\tSDA: PQS3 (QSPI_CS0)\n "..
                              "",
                requires = { "HW_MCU_MCF5225X" },
                type = "enumerated",
                choices = function() return GetPinset2() end,
                file = "include/cfg/twi.h"
            },
         },
    },
    
    --
    -- I2C Bus 1 Configuration
    --
    {
        name = "nutarch_m68k_coldfire_i2c1",
        brief = "I2C Bus 1 Controller",
        description = "Coldfire hardware I2C support.",
        requires = { "HW_I2C_COLDFIRE", "HW_I2C1" },
        provides = { "DEV_TWI" },
        sources = { "m68k/coldfire/dev/common/mcf5_i2c.c",
                    "m68k/coldfire/dev/common/mcf5_i2c1.c" },
        options =
        {
            {
                macro = "I2CBUS1_DEFAULT_SPEED",
                brief = "Default Speed",
                description = "Default speed for this bus. Different speeds can be set by software.\n",
                default = "100",
                file = "include/cfg/twi.h"
            },
            {
                macro = "I2CBUS1_PINSET_MCF5225X",
                brief = "MCF5225X Pinset",
                description = "Please choose which pins are used for I2C Bus 1\n"..
                              "\nPIN_SET1:\n "..
                              "\tSCL: PQS0 (QSPI_DOUT)\n "..
                              "\tSDA: PQS1 (QSPI_DIN)\n "..
                              "\nPIN_SET2:\n "..
                              "\tSCL: PUB0 (UTXD1)\n "..
                              "\tSDA: PUB1 (URXD1)\n "..
                              "\nPIN_SET3:\n "..
                              "\tSCL: PUC3 (UCTS2)\n "..
                              "\tSDA: PUC2 (URTS2)\n "..
                              "\nPIN_SET4:\n "..
                              "\tSCL: PTH3 (FB_D5)\n "..
                              "\tSDA: PTH2 (FB_D4)\n "..
                              "",
                requires = { "HW_MCU_MCF5225X" },
                type = "enumerated",
                choices = function() return GetPinset4() end,
                file = "include/cfg/twi.h"
            },
        },
    },
}

--
-- Retrieve Family Specific Pinsets
--
function GetPinset2()
    return { "PIN_SET1", "PIN_SET2" }
end

function GetPinset3()
    return { "PIN_SET1", "PIN_SET2", "PIN_SET3" }
end

function GetPinset4()
    return { "PIN_SET1", "PIN_SET2", "PIN_SET3", "PIN_SET4" }
end
