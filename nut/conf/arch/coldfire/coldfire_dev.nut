nutarch_m68k_coldfire_devices =
{
    --
    -- Default Exception Handler
    --
    {
        name = "nutarch_m68k_coldfire_devices_ih_default",
        brief = "Default Exception Handler",
        description = "Default Exception Handler is used as default interrupt handler for common interrupts.\n"..
                      "Is is usefull for spurious interrupts catching and analysing.",
        sources = { "m68k/coldfire/dev/common/ih/ih_default.c"},
    },
    
    --
    -- GPIO Interface
    --
    {
        name = "nutarch_m68k_coldfire_mcf5225x_gpio",
        brief = "GPIO",
        description = "Generic port I/O API.",
        requires = { "HW_GPIO_COLDFIRE" },
        sources = { "m68k/coldfire/dev/common/mcf5_gpio.c"}
    },
    
    --
    -- UART Debug Output
    --
    {
        name = "nutarch_m68k_coldfire_devices_debug",
        brief = "UART Debug Output",
        description = "Polling UART driver.",
        requires = { "HW_UART_COLDFIRE" },
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
    -- I2C Bus Default
    --
    {
        name = "nutarch_m68k_coldfire_devices_i2c",
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
        name = "nutarch_m68k_coldfire_devices_i2c0",
        brief = "I2C Bus 0 Controller",
        description = "Coldfire hardware I2C support.",
        requires = { "HW_I2C_COLDFIRE", "HW_I2C0" },
        provides = { "DEV_TWI" },
        sources = { "m68k/coldfire/dev/common/mcf5_i2c.c",
                    "m68k/coldfire/dev/common/mcf5_i2c0.c" },
        options =
        {
            {
                macro = "I2CBUS0_SCL_PIN",
                brief = "I2C0 SCL Pin selection",
                type = "enumerated",
                description =
                    function() 
                        if c_is_provided("HW_MCU_MCF5225X") then return mcf5225x_i2c0_scl_pins_descr; end;
                    end,
                choices = 
                    function() 
                        if c_is_provided("HW_MCU_MCF5225X") then return mcf5225x_i2c0_scl_pins; end;
                    end,
                file = "include/cfg/twi.h"
            },
            {
                macro = "I2CBUS0_SDA_PIN",
                brief = "I2C0 SDA Pin selection",
                type = "enumerated",
                description =
                    function() 
                        if c_is_provided("HW_MCU_MCF5225X") then return mcf5225x_i2c0_sda_pins_descr; end;
                    end,
                choices = 
                    function() 
                        if c_is_provided("HW_MCU_MCF5225X") then return mcf5225x_i2c0_sda_pins; end;
                    end,
                file = "include/cfg/twi.h"
            },
            {
                macro = "I2CBUS0_DEFAULT_SPEED",
                brief = "Default Speed",
                description = "Default speed for this bus. Different speeds can be set by software.\n",
                default = "100",
                file = "include/cfg/twi.h"
            },
        },
    },
    
    --
    -- I2C Bus 1 Configuration
    --
    {
        name = "nutarch_m68k_coldfire_devices_i2c1",
        brief = "I2C Bus 1 Controller",
        description = "Coldfire hardware I2C support.",
        requires = { "HW_I2C_COLDFIRE", "HW_I2C1" },
        provides = { "DEV_TWI" },
        sources = { "m68k/coldfire/dev/common/mcf5_i2c.c",
                    "m68k/coldfire/dev/common/mcf5_i2c1.c" },
        options =
        {
            {
                macro = "I2CBUS1_SCL_PIN",
                brief = "I2C1 SCL Pin selection",
                type = "enumerated",
                description =
                    function() 
                        if c_is_provided("HW_MCU_MCF5225X") then return mcf5225x_i2c1_scl_pins_descr; end;
                    end,
                choices = 
                    function() 
                        if c_is_provided("HW_MCU_MCF5225X") then return mcf5225x_i2c1_scl_pins; end;
                    end,
                file = "include/cfg/twi.h"
            },
            {
                macro = "I2CBUS1_SDA_PIN",
                brief = "I2C1 SDA Pin selection",
                type = "enumerated",
                description =
                    function() 
                        if c_is_provided("HW_MCU_MCF5225X") then return mcf5225x_i2c1_sda_pins_descr; end;
                    end,
                choices = 
                    function() 
                        if c_is_provided("HW_MCU_MCF5225X") then return mcf5225x_i2c1_sda_pins; end;
                    end,
                file = "include/cfg/twi.h"
            },
            {
                macro = "I2CBUS1_DEFAULT_SPEED",
                brief = "Default Speed",
                description = "Default speed for this bus. Different speeds can be set by software.\n",
                default = "100",
                file = "include/cfg/twi.h"
            },
        },
    },
}
