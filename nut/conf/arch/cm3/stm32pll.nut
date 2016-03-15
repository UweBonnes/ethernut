    --
    -- STM32 PLL Configuration
    --
--
-- Retrieve maximun system clock description
--
function GetSysClockFrequencyDesc()
    if c_is_provided("HW_MCU_STM32L0") then
        return "System clock target frequency with SYSCLK_PLL selected.\n\n"..
               "With SYSCLK_FREQ undefined, setup will try to reach the\n"..
               "highest possible value.\n\n"..
               "If in doubt, leave undefined!\n\n"..
               "With SYSCLK_FREQ given, User must set usefull values for:\n"..
               " - PLLCLK_MULT\t: Pll multiplier factor.\n"..
               " - PLLCLK_DIV\t\t: PLL VCO output division factor.\n\n"..
               "Value (MHz)\t\tRange1\n"..
               "PLL input (range)\t2 .. 24\n"..
               "PLL output(range)\t2 .. 32\n"..
               "SYSCLK_FREQ\t\t0 .. 32"
    end
    if c_is_provided("HW_MCU_STM32L1") then
        return "System clock target frequency with SYSCLK_PLL selected.\n"..
               "With SYSCLK_FREQ undefined, setup will try to reach the\n"..
               "\thighest possible value.\n\n"..
               "If in doubt, leave undefined!\n\n"..
               "With SYSCLK_FREQ given, User must set usefull values for:\n"..
               " - PLLCLK_MULT\t: Pll multiplier factor.\n"..
               " - PLLCLK_DIV\t\t: PLL VCO output division factor.\n\n"..
               "Value (MHz)\t\tRange1\n"..
               "PLL input (range)\t2 .. 24\n"..
               "PLL output(range)\t2 .. 32\n"..
               "SYSCLK_FREQ (max)\t0 .. 32"
    end
    if c_is_provided("HW_MCU_STM32L4") then
        return "System clock target frequency with SYSCLK_PLL selected.\n"..
               "With SYSCLK_FREQ undefined, setup will try to reach the\n"..
               "\thighest possible value.\n\n"..
               "If in doubt, leave undefined!\n\n"..
               "With SYSCLK_FREQ given, User must set usefull values for:\n"..
               " - PLLCLK_PREDIV\t: Division factor at Pll input.\n"..
               " - PLLCLK_MULT\t: Pll multiplier factor.\n"..
               " - PLLCLK_DIV\t\t: PLL VCO output division factor.\n\n"..
               "Value (MHz)\t\tRange1\tRange2\n"..
               "PLL input (range)\t4 .. 16\t4 .. 16\n"..
               "PLL VCO output(range)\t64 .. 344\t64 .. 128\n"..
               "PLLR output(range)\t8 .. 80\t8 .. 26\n"..
               "SYSCLK_FREQ\t\t0 .. 80\t0 .. 26"
    end
    if c_is_provided("HW_MCU_STM32F0") then
        return "System clock target frequency with SYSCLK_PLL selected.\n\n"..
               "With SYSCLK_FREQ undefined, setup will try to reach the\n"..
               "highest possible value.\n\n"..
               "If in doubt, leave undefined!\n\n"..
               "With SYSCLK_FREQ given, User must set usefull values for:\n"..
               " - PLLCLK_PREDIV\t: HSE Division factor at Pll input.\n"..
               " - PLLCLK_MULT\t: Pll multiplier factor.\n\n"..
               "Value (MHz)\t\tRange1\n"..
               "SYSCLK_FREQ (max)\t16 .. 48"
    end
    if c_is_provided("HW_RCC_STM32F1_CL") then
        return "System clock target frequency with SYSCLK_PLL selected.\n\n"..
               "With SYSCLK_FREQ undefined, setup will try to reach the\n"..
               "highest possible value.\n\n"..
               "If in doubt, leave undefined!\n\n"..
               "With SYSCLK_FREQ given, User must set usefull values for:\n"..
               " - PLLCLK_PREDIV\t: HSE Division factor at Pll input (1 or 2).\n"..
               " - PLLCLK_MULT\t: Pll multiplier factor.\n"..
               "And with PLL2 as PLL1 input selected:\n"..
               " - PLL2CLK_PREDIV\t: HSE Division factor at Pll2 input.\n"..
               " - PLL2CLK_MULT\t: Pll2 multiplier factor.\n\n"..
               "Value (MHz)\t\tRange1\n"..
               "PLL2 input (range)\t3 .. 5\n"..
               "PLL2 output (range)\t40 .. 74\n"..
               "PLL input (range)\t3 .. 12\n"..
               "SYSCLK_FREQ (max)\t18 .. 72"
    end
    if c_is_provided("HW_MCU_STM32F100") then
        return "System clock target frequency with SYSCLK_PLL selected.\n\n"..
               "With SYSCLK_FREQ undefined, setup will try to reach the\n"..
               "highest possible value.\n\n"..
               "If in doubt, leave undefined!\n\n"..
               "With SYSCLK_FREQ given, User must set usefull values for:\n"..
               " - PLLCLK_PREDIV\t: HSE Division factor at Pll input (1 or 2).\n"..
               " - PLLCLK_MULT\t: Pll multiplier factor.\n\n"..
               "Value (MHz)\t\tRange1\n"..
               "PLL input (range)\t1 .. 24\n"..
               "SYSCLK_FREQ (max)\t16 .. 24"
    end
    if c_is_provided("HW_MCU_STM32F101") then
        return "System clock target frequency with SYSCLK_PLL selected.\n\n"..
               "With SYSCLK_FREQ undefined, setup will try to reach the\n"..
               "highest possible value.\n\n"..
               "If in doubt, leave undefined!\n\n"..
               "With SYSCLK_FREQ given, User must set usefull values for:\n"..
               " - PLLCLK_PREDIV\t: HSE Division factor at Pll input (1 or 2).\n"..
               " - PLLCLK_MULT\t: Pll multiplier factor.\n\n"..
               "Value (MHz)\t\tRange1\n"..
               "PLL input (range)\t1 .. 25\n"..
               "SYSCLK_FREQ (max)\t16 .. 36"
    end
    if c_is_provided("HW_MCU_STM32F102") then
        return "System clock target frequency with SYSCLK_PLL selected.\n\n"..
               "With SYSCLK_FREQ undefined, setup will try to reach the\n"..
               "highest possible value.\n\n"..
               "If in doubt, leave undefined!\n\n"..
               "With SYSCLK_FREQ given, User must set usefull values for:\n"..
               " - PLLCLK_PREDIV\t: HSE Division factor at Pll input (1 or 2).\n"..
               " - PLLCLK_MULT\t: Pll multiplier factor.\n\n"..
               "Value (MHz)\t\tRange1\n"..
               "SYSCLK_FREQ (max)\t16 .. 48\n"..
               "PLL input (range)\t1 .. 25"
    end
    if c_is_provided("HW_MCU_STM32F103") then
        return "System clock target frequency with SYSCLK_PLL selected.\n\n"..
               "With SYSCLK_FREQ undefined, setup will try to reach the\n"..
               "highest possible value.\n\n"..
               "If in doubt, leave undefined!\n\n"..
               "With SYSCLK_FREQ given, User must set usefull values for:\n"..
               " - PLLCLK_PREDIV\t: HSE Division factor at Pll input (1 or 2).\n"..
               " - PLLCLK_MULT\t: Pll multiplier factor.\n\n"..
               "Value (MHz)\t\tRange1\n"..
               "PLL input (range)\t1 .. 25\n"..
               "SYSCLK_FREQ (max)\t16 .. 72"
    end
    if c_is_provided("HW_MCU_STM32F2") then
        return "System clock target frequency with SYSCLK_PLL selected.\n\n"..
               "With SYSCLK_FREQ undefined, setup will try to reach the\n"..
               "highest possible value.\n\n"..
               "If in doubt, leave undefined!\n\n"..
               "With SYSCLK_FREQ given, User must set usefull values for:\n"..
               " - PLLCLK_PREDIV\t: Division factor at Pll input.\n"..
               " - PLLCLK_MULT\t: Pll multiplier factor.\n\n"..
               " - PLLCLK_DIV\t: Pll VCO output division factor.\n\n"..
               "Value (MHz)\t\tRange1\n"..
               "PLL input (range)\t0.95 .. 2.10\n"..
               "PLL VCO output(range)\t192 .. 432\n"..
               "SYSCLK_FREQ (max)\t24 .. 120"
    end
    if c_is_provided("HW_MCU_STM32F3") then
        return "System clock target frequency with SYSCLK_PLL selected.\n\n"..
               "With SYSCLK_FREQ undefined, setup will try to reach the\n"..
               "highest possible value.\n\n"..
               "If in doubt, leave undefined!\n\n"..
               "With SYSCLK_FREQ given, User must set usefull values for:\n"..
               " - PLLCLK_PREDIV\t: HSE Division factor at Pll input.\n"..
               " - PLLCLK_MULT\t: Pll multiplier factor.\n\n"..
               "Value (MHz)\t\tRange1\n"..
               "PLL input (range)\t1 .. 24\n"..
               "SYSCLK_FREQ (max)\t16 .. 72"
    end
    if c_is_provided("HW_MCU_STM32F401") then
        return "System clock target frequency with SYSCLK_PLL selected.\n"..
               "With SYSCLK_FREQ undefined, setup will try to reach the\n"..
               "\thighest possible value.\n\n"..
               "If in doubt, leave undefined!\n\n"..
               "With SYSCLK_FREQ given, User must set usefull values for:\n"..
               " - PLLCLK_PREDIV\t: Division factor at Pll input.\n"..
               " - PLLCLK_MULT\t: Pll multiplier factor.\n"..
               " - PLLCLK_DIV\t\t: PLL VCO output division factor.\n\n"..
               "PLL input (MHz)\t0.95 .. 2.10\n"..
               "PLL VCO output (MHz)\t192 .. 432\n"..
               "PLL output (MHz)\t24 .. 84\n\n"..
               "Voltage scaling\t2\t3\n"..
               "SYSCLK_FREQ (MHz)\t0 .. 84\t0 .. 60"
    end
    if c_is_provided("HW_MCU_STM32F411") then
        return "System clock target frequency with SYSCLK_PLL selected.\n"..
               "With SYSCLK_FREQ undefined, setup will try to reach the\n"..
               "\thighest possible value.\n\n"..
               "If in doubt, leave undefined!\n\n"..
               "With SYSCLK_FREQ given, User must set usefull values for:\n"..
               " - PLLCLK_PREDIV\t: Division factor at Pll input.\n"..
               " - PLLCLK_MULT\t: Pll multiplier factor.\n"..
               " - PLLCLK_DIV\t\t: PLL VCO output division factor.\n\n"..
               "PLL input (MHz)\t0.95 .. 2.10\n"..
               "PLL VCO output (MHz)\t100 .. 432\n"..
               "PLL output (MHz)\t24 .. 100\n\n"..
               "Voltage scaling\t1\t2\t3\n"..
               "SYSCLK_FREQ (MHz)\t0 .. 100\t0 .. 84\t0 .. 64"
    end
    if c_is_provided("HW_MCU_STM32F40") then
        return "System clock target frequency with SYSCLK_PLL selected.\n"..
               "With SYSCLK_FREQ undefined, setup will try to reach the\n"..
               "\thighest possible value.\n\n"..
               "If in doubt, leave undefined!\n\n"..
               "With SYSCLK_FREQ given, User must set usefull values for:\n"..
               " - PLLCLK_PREDIV\t: Division factor at Pll input.\n"..
               " - PLLCLK_MULT\t: Pll multiplier factor.\n"..
               " - PLLCLK_DIV\t\t: PLL VCO output division factor.\n\n"..
               "PLL input (MHz)\t0.95 .. 2.10\n"..
               "PLL VCO output (MHz)\t192 .. 432\n"..
               "PLL output (MHz)\t24 .. 168\n\n"..
               "Voltage scaling\t1\t0\n"..
               "SYSCLK_FREQ (MHz)\t0 .. 168\t0 .. 144"
    end
    if c_is_provided("HW_MCU_STM32F42") then
        return "System clock target frequency with SYSCLK_PLL selected.\n"..
               "With SYSCLK_FREQ undefined, setup will try to reach the\n"..
               "\thighest possible value.\n\n"..
               "If in doubt, leave undefined!\n\n"..
               "With SYSCLK_FREQ given, User must set usefull values for:\n"..
               " - PLLCLK_PREDIV\t: Division factor at Pll input.\n"..
               " - PLLCLK_MULT\t: Pll multiplier factor.\n"..
               " - PLLCLK_DIV\t\t: PLL VCO output division factor.\n\n"..
               "PLL input (MHz)\t0.95 .. 2.10\n"..
               "PLL VCO output (MHz)\t192 .. 432\n"..
               "PLL output (MHz)\t24 .. 180\n\n"..
               "Voltage scaling/Overdrive\t1, on\t1, off\t2, on\t2, off\t3\n"..
               "SYSCLK_FREQ (MHz)\t0 .. 180\t0 .. 168\t0 .. 168\t0 .. 144\t0 .. 120"
    end
    if c_is_provided("HW_MCU_STM32F446") then
        return "System clock target frequency with SYSCLK_PLL selected.\n"..
               "With SYSCLK_FREQ undefined, setup will try to reach the\n"..
               "\thighest possible value.\n\n"..
               "If in doubt, leave undefined!\n\n"..
               "With SYSCLK_FREQ given, User must set usefull values for:\n"..
               " - PLLCLK_PREDIV\t: Division factor at Pll input.\n"..
               " - PLLCLK_MULT\t: Pll multiplier factor.\n"..
               " - PLLCLK_DIV\t\t: PLL VCO output division factor.\n\n"..
               " - PLLCLK_DIV_R\t\t: PLL VCO output R division factor.\n\n"..
               "PLL input (MHz)\t0.95 .. 2.10\n"..
               "PLL VCO output (MHz)\t100 .. 432\n"..
               "PLL output (MHz)\t24 .. 180\n\n"..
               "Voltage scaling/Overdrive\t1, on\t1, off\t2, on\t2, off\t3\n"..
               "SYSCLK_FREQ (MHz)\t0 .. 180\t0 .. 168\t0 .. 168\t0 .. 144\t0 .. 120"
    end
    if c_is_provided("HW_MCU_STM32F7") then
        return "System clock target frequency with SYSCLK_PLL selected.\n"..
               "With SYSCLK_FREQ undefined, setup will try to reach the\n"..
               "\thighest possible value.\n\n"..
               "If in doubt, leave undefined!\n\n"..
               "With SYSCLK_FREQ given, User must set usefull values for:\n"..
               " - PLLCLK_PREDIV\t: Division factor at Pll input.\n"..
               " - PLLCLK_MULT\t: Pll multiplier factor.\n"..
               " - PLLCLK_DIV\t\t: PLL VCO output division factor.\n\n"..
               "PLL input (MHz)\t0.95 .. 2.10\n"..
               "PLL VCO output (MHz)\t100 .. 432\n"..
               "PLL output (MHz)\t24 .. 180\n\n"..
               "Voltage scaling/Overdrive\t1, on\t1, off\t2, on\t2, off\t3\n"..
               "SYSCLK_FREQ (MHz)\t0 .. 216\t0 .. 180\t0 .. 180\t0 .. 168\t0 .. 144"
    end
    return "Unhandled"
end

--
-- Retrieve SYSCLK available on the device.
--
function GetSysClkSrc()
    if c_is_provided("HW_RCC_STM32L") then
        return {"SYSCLK_MSI", "SYSCLK_HSI", "SYSCLK_HSE", "SYSCLK_PLL"}
    end
    if c_is_provided("HW_MCU_STM32F446") then
        return {"SYSCLK_HSI", "SYSCLK_HSE", "SYSCLK_PLL", "SYSCLK_PLLR"}
    end
    return {"SYSCLK_HSI", "SYSCLK_HSE", "SYSCLK_PLL"}
end

--
-- Retrieve SYSCLK description
--
function GetSysClockSourceDesc()
    if c_is_provided("HW_HSI8_STM32F") then
-- F0/F1/F3
       return "Select where SYSCLK should get its clock from.\n\n"..
              "SYSCLK_HSI is internal 8 MHz oscillator.\n"..
              "SYSCLK_HSE is external oscillator or crystal input.\n"..
              "SYSCLK_PLL is internal PLL output.\n"..
              "\tSelect PLL source with PLLCLK_SOURCE.\n\n"..
              "Default is SYSCLK_PLL."
    end
    if c_is_provided("HW_MCU_STM32F446") then
-- F44x: Choice from two outputs of the PLL
       return "Select where SYSCLK should get its clock from.\n\n"..
              "SYSCLK_HSI is internal 16 MHz oscillator.\n"..
              "SYSCLK_HSE is external oscillator or crystal input.\n"..
              "SYSCLK_PLL is internal PLL output.\n"..
              "SYSCLK_PLLR is internal PLL R output.\n"..
              "\tSelect PLL source with PLLCLK_SOURCE.\n"..
              "\tPLLR is also used for I2S, SA1 and SPDIF.\n"..
              "\tPLL is dedicated to core.\n\n"..
              "Default is SYSCLK_PLL."
    end
    if c_is_provided("HW_HSI16_STM32F") then
-- F2/F4/F7
       return "Select where SYSCLK should get its clock from.\n\n"..
              "SYSCLK_HSI is internal 16 MHz oscillator.\n"..
              "SYSCLK_HSE is external oscillator or crystal input.\n"..
              "SYSCLK_PLL is internal PLL output.\n"..
              "\tSelect PLL source with PLLCLK_SOURCE.\n\n"..
              "Default is SYSCLK_PLL."
    end
    if c_is_provided("HW_MCU_STM32L0") then
       return "Select where SYSCLK should get its clock from.\n\n"..
              "SYSCLK_HSI is internal 16 MHz oscillator, "..
                          "eventually divided by 4.\n"..
              "\tSelect with HSICLK_DIV4.\n"..
              "SYSCLK_HSE is external oscillator or crystal input.\n"..
              "SYSCLK_PLL is internal PLL output.\n"..
              "\tSelect PLL source with PLLCLK_SOURCE.\n"..
              "PLLCLK_MSI is internal multi-speed oscillator.\n"..
              "\tSelect MSI Frequency with MSI_RANGE.\n\n"..
              "Default is SYSCLK_PLL."
    end
    if c_is_provided("HW_MCU_STM32L1") then
       return "Select where SYSCLK should get its clock from.\n\n"..
              "SYSCLK_HSI is internal 16 MHz oscillator.\n"..
              "SYSCLK_HSE is external oscillator or crystal input.\n"..
              "SYSCLK_PLL is internal PLL output.\n"..
              "\tSelect PLL source with PLLCLK_SOURCE.\n"..
              "PLLCLK_MSI is internal multi-speed oscillator.\n"..
              "\tSelect MSI Frequency with MSI_RANGE.\n\n"..
              "Default is SYSCLK_PLL."
    end
    if c_is_provided("HW_MCU_STM32L4") then
       return "Select where SYSCLK should get its clock from.\n\n"..
              "SYSCLK_HSI is internal 16 MHz oscillator.\n"..
              "SYSCLK_HSE is external oscillator or crystal input.\n"..
              "SYSCLK_PLL is internal PLL output.\n"..
              "\tSelect PLL source with PLLCLK_SOURCE.\n"..
              "PLLCLK_MSI is internal multi-speed oscillator.\n"..
              "\tSelect MSI Frequency with MSI_RANGE.\n\n"..
              "Default is SYSCLK_PLL."
    end
    return "Unhandled case"
end

--
-- HSE Value Description
--
-- With value given, SYSCLK_SOURCE will be SYSCLK_PLL with PLLCLK_HSE
--
function GetHseValueDesc()
    if c_is_provided("HW_MCU_STM32L4") then
        return "Value of the external crystal or clock input in Hertz.\n"..
               "Allowed values:\n"..
               "External input at OSCIN (Range 1)\t: 0 .. 46 Mhz\n"..
               "External input at OSCIN (Range 2)\t: 0 .. 26 Mhz\n"..
               "External resonator\t\t: 4 .. 48 MHz\n"..
               "Typical Values is 8 MHz.\n\n"..
               "Default is undefined. This will turn off HSE.\n"
    end
    if c_is_provided("HW_RCC_STM32L") then
        return "Value of the external crystal or clock input in Hertz.\n"..
               "Allowed values:\n"..
               "External input at OSCIN\t: 1 .. 32 Mhz\n"..
               "With CSS off and PLL off\t: 0 .. 32 MHz\n"..
               "External resonator\t: 4 .. 25 MHz\n"..
               "Typical Values is 8 MHz.\n\n"..
               "Default is undefined. This will turn off HSE.\n"
    end
    if c_is_provided("HW_MCU_STM32F0") then
        return "Value of the external crystal or clock input in Hertz.\n"..
               "Allowed values:\n"..
               "External input at OSCIN\t: 1 .. 32 Mhz\n"..
               "External resonator\t: 4 .. 32 MHz\n"..
               "Typical Values is 8 MHz.\n\n"..
               "Default is undefined. This will turn off HSE.\n"
    end
    if c_is_provided("HW_MCU_STM32F100") then
        return "Value of the external crystal or clock input in Hertz.\n"..
               "Allowed values:\n"..
               "External input at OSCIN\t: 1 .. 24 Mhz\n"..
               "External resonator\t: 4 .. 24 MHz\n"..
               "Typical Values is 8 MHz.\n\n"..
               "Default is undefined. This will turn off HSE.\n"
    end
    if c_is_provided("HW_MCU_STM32F101") then
        return "Value of the external crystal or clock input in Hertz.\n"..
               "Allowed values:\n"..
               "External input at OSCIN\t: 1 .. 25 Mhz\n"..
               "External resonator\t: 4 .. 16 MHz\n"..
               "Typical Values is 8 MHz.\n\n"..
               "Default is undefined. This will turn off HSE.\n"
    end
    if c_is_provided("HW_MCU_STM32F102") then
        return "Value of the external crystal or clock input in Hertz.\n"..
               "Allowed values:\n"..
               "External input at OSCIN\t: 1 .. 25 Mhz\n"..
               "External resonator\t: 4 .. 16 MHz\n"..
               "Typical Values is 8 MHz.\n\n"..
               "Default is undefined. This will turn off HSE.\n"
    end
    if c_is_provided("HW_MCU_STM32F1_CL") then
        return "Value of the external crystal or clock input in Hertz.\n"..
               "Allowed values:\n"..
               "External input at OSCIN\t: 1 .. 50 Mhz\n"..
               "External resonator\t: 3 .. 25 MHz\n"..
               "Typical Values is 8 MHz.\n\n"..
               "Builtin USB bootloader starts much faster with 25 MHz.\n"..
               "Default is undefined. This will turn off HSE.\n"
    end
    if c_is_provided("HW_MCU_STM32F2") then
        return "Value of the external crystal or clock input in Hertz.\n"..
               "Allowed values:\n"..
               "External input at OSCIN\t: 1 .. 26 Mhz\n"..
               "External resonator\t: 4 .. 26 MHz\n"..
               "Typical Values is 8 MHz.\n\n"..
               "Default is undefined. This will turn off HSE.\n"
    end
    if c_is_provided("HW_MCU_STM32F3") then
        return "Value of the external crystal or clock input in Hertz.\n"..
               "Allowed values:\n"..
               "External input at OSCIN\t: 1 .. 32 Mhz\n"..
               "External resonator\t: 4 .. 32 MHz\n"..
               "Typical Values is 8 MHz.\n\n"..
               "Default is undefined. This will turn off HSE.\n"
    end
    if c_is_provided("HW_MCU_STM32F4") then
        return "Value of the external crystal or clock input in Hertz.\n"..
               "Allowed values:\n"..
               "External input at OSCIN\t: 1 .. 50 Mhz\n"..
               "External resonator\t: 4 .. 26 MHz\n"..
               "Typical Values is 8 MHz.\n\n"..
               "Default is undefined. This will turn off HSE.\n"
    end
    if c_is_provided("HW_MCU_STM32F7") then
        return "Value of the external crystal or clock input in Hertz.\n"..
               "Allowed values:\n"..
               "External input at OSCIN\t: 1 .. 50 Mhz\n"..
               "External resonator\t: 4 .. 26 MHz\n"..
               "Typical Values is 8 MHz.\n\n"..
               "Default is undefined. This will turn off HSE.\n"
    end
    return Unhandled
end

--
-- Retrieve PLL Input CLK description for the device.
--
function GetPllClockSourceDesc()
    if c_is_provided("HW_MCU_STM32L0") then
        return "Select source for PLL input.\n\n"..
            "PLLCLK_HSE is external resonator or input. "..
                        "See HSE_ related values.\n"..
            "PLLCLK_HSI is internal 16 MHz oscillator, "..
                        "eventually divided by 4.\n"..
            "Input clock must be in the range 2 .. 24 MHz.\n"..
            "PLL setup is done automatic for some default value.\n"..
            "Use SYSCLK_FREQ to disable automatic setup only when needed!\n\n"..
            "Default with HSE_VALUE provided is PLLCLK_HSE, PLLCLK_HSI else."
    end
    if c_is_provided("HW_MCU_STM32L1") then
        return "Select source for PLL input.\n\n"..
            "PLLCLK_HSE is external resonator or input. "..
                        "See HSE_ related values.\n"..
            "PLLCLK_HSI is internal 16 MHz oscillator.\n"..
            "Input clock must be in range 2 .. 24 MHz.\n"..
            "PLL setup is done automatic for some default value.\n"..
            "Use SYSCLK_FREQ to disable automatic setup only when needed!\n\n"..
            "Default with HSE_VALUE provided is PLLCLK_HSE, PLLCLK_HSI else."
    end
    if c_is_provided("HW_MCU_STM32L4") then
        return "Select source for PLL input.\n\n"..
            "PLLCLK_NONE is PLL off.\n"..
            "PLLCLK_HSE is external resonator or input. "..
                        "See HSE_ related values.\n"..
            "PLLCLK_HSI is internal 16 MHz oscillator.\n"..
            "PLLCLK_MSI is internal multispeed oscillator. "..
                        "See MSI_ related values.\n"..
            "Pll input clock range is 4 .. 16 MHz.\n"..
            "Clock division by 1 .. 8 is available.\n"..
            "PLL setup is done automatic for some default value.\n"..
            "Use SYSCLK_FREQ to disable automatic setup only when needed!\n\n"..
            "Default with HSE_VALUE provided is PLLCLK_HSE, with\n"..
            "LSE_VALUE provided PLLCLK_MSI, PLLCLK_HSI else."
   end
-- F03/F05/F100/F37
    if c_is_provided("HW_HSI8_DIV2_STM32") then
        return "Select source for PLL input.\n\n"..
            "PLLCLK_HSE is external resonator or input. "..
                        "See HSE_ related values.\n"..
            "PLLCLK_HSI is internal 8 MHz oscillator divided by 2.\n"..
            "Input clock must be in range 1 .. 24 MHz.\n"..
            "HSE Clock division by 1 .. 16 is available.\n"..
            "PLL setup is done automatic for some default value.\n"..
            "Use SYSCLK_FREQ to disable automatic setup only when needed!\n\n"..
            "Default with HSE_VALUE provided is PLLCLK_HSE, PLLCLK_HSI else."
    end
-- F04/F07/F09/F3_HD
    if c_is_provided("HW_HSI8_STM32") then
        return "Select source for PLL input.\n\n"..
            "PLLCLK_HSE is external resonator or input. "..
                        "See HSE_ related values.\n"..
            "PLLCLK_HSI is internal 8 MHz oscillator.\n"..
            "Input clock must be in range 1 .. 24 MHz.\n"..
            "Clock division by 1 .. 16 is available.\n"..
            "PLL setup is done automatic for some default value.\n"..
            "Use SYSCLK_FREQ to disable automatic setup only when needed!\n\n"..
            "Default with HSE_VALUE provided is PLLCLK_HSE, PLLCLK_HSI else."
    end
-- F105/F107
    if c_is_provided("HW_MCU_STM32F1_CL") then
        return "Select source for PLL input.\n\n"..
            "PLLCLK_HSE is external resonator or input. "..
                        "See HSE_ related values.\n"..
            "PLLCLK_HSI is internal 8 MHz oscillator divided by 2.\n"..
            "Input clock must be in range 1 .. 12 MHz.\n"..
            "HSE Clock division by 1 .. 16 is available.\n"..
            "HSE may also be provided by PLL2.\n"..
            "PLL setup is done automatic for some default value.\n"..
            "Use SYSCLK_FREQ to disable automatic setup only when needed!\n\n"..
            "Default with HSE_VALUE provided is PLLCLK_HSE, PLLCLK_HSI else."
    end
    if c_is_provided("HW_MCU_STM32F1") then
        return "Select source for PLL input.\n\n"..
            "PLLCLK_HSE is external resonator or input. "..
                        "See HSE_ related values.\n"..
            "PLLCLK_HSI is internal 8 MHz oscillator divided by 2.\n"..
            "Input clock must be in range 1 .. 25 MHz.\n"..
            "HSE Clock division by 2 is available.\n"..
            "PLL setup is done automatic for some default value.\n"..
            "Use SYSCLK_FREQ to disable automatic setup only when needed!\n\n"..
            "Default with HSE_VALUE provided is PLLCLK_HSE, PLLCLK_HSI else."
    end
    if c_is_provided("HW_MCU_STM32F3") then
        return "Select source for PLL input.\n\n"..
            "PLLCLK_HSE is external resonator or input. "..
                        "See HSE_ related values.\n"..
            "PLLCLK_HSI is internal 8 MHz oscillator divided by 2.\n"..
            "Input clock must be in range 1 .. 24 MHz.\n"..
            "HSE Clock division by 2 .. 16 is available.\n"..
            "PLL setup is done automatic for some default value.\n"..
            "Use SYSCLK_FREQ to disable automatic setup only when needed!\n\n"..
            "Default with HSE_VALUE provided is PLLCLK_HSE, PLLCLK_HSI else."
    end
-- F2/F4/F7
    return "Select source for PLL input.\n\n"..
        "PLLCLK_HSE is external resonator or input. "..
                    "See HSE_ related values.\n"..
        "PLLCLK_HSI is internal 16 MHz oscillator.\n"..
        "Input clock must be in range 0.95 .. 2.1 MHz.\n"..
        "HSE Clock division by 2 .. 63 is available.\n"..
        "PLL setup is done automatic for some default value.\n"..
        "Use SYSCLK_FREQ to disable automatic setup only when needed!\n\n"..
        "Default is PLLCLK_HSI."
end

--
-- Retrieve PLL Input CLK available on the device.
--
function GetPllClkSrc()
    if c_is_provided("HW_MCU_STM32L4") then
        return {"PLLCLK_NONE", "PLLCLK_HSI", "PLLCLK_HSE"}
    end
    return {"PLLCLK_HSI", "PLLCLK_HSE"}
end

--
-- Retrieve PLL Input CLK default.
--
function GetPllClkSrcDefault()
    if c_is_provided("HW_MCU_STM32L4") then
        if c_is_provided("LSE_VALUE") then
            return "PLLCLK_MSI"
        end
    end
    if c_is_provided("HSE_VALUE") then
        return "PLLCLK_HSE"
    end
    return "PLLCLK_HSI"
end

function GetPllMult()
    if c_is_provided("HW_MCU_STM32F1_CL") then
       return { "4", "5", "6", "7", "8", "9", "6.5"}
    end
    return { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16"}
end

function GetPllPrediv()
    if c_is_provided("HW_MCU_STM32F1") then
       if c_is_provided("HW_MCU_STM32F1_CL") then
          return { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16"}
       end
       if c_is_provided("HW_MCU_STM32F100") then
          return { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16"}
       end
       return { "1", "2"}
    end
    return { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16"}
end

function GetLseDriveLevel()
    if c_is_provided("HW_MCU_STM32F411") then
        return { "0" , "1"}
    end
    if c_is_provided("HW_MCU_STM32F446") then
        return { "0" , "1"}
    end
    if c_is_provided("HW_MCU_STM32L1") then
        return "0"
    end
    if c_is_provided("HW_MCU_STM32F1") then
        return "0"
    end
    if c_is_provided("HW_MCU_STM32F2") then
        return "0"
    end
    if c_is_provided("HW_MCU_STM32F4") then
        return "0"
    end
    return { "0", "1", "2", "3"}
end

function GetRtcClkSrcDefault()
    if c_is_provided("LSE_VALUE") then
       return "RTCCLK_LSE"
    end
    if c_is_provided("HSE_VALUE") then
       return "RTCCLK_HSE"
    end
    return "RTCCLK_LSI"
end

nutarch_cm3_stm32_pll =
{
        name = "nutarch_cm3_stm32_rcc_common",
        brief = "STM32 Clock and system settings.",
        description = "STM32 Clock and system settings.",
        options =
        {
            {
                macro = "SYSCLK_SOURCE",
                brief = "System clock source",
                description =function() return GetSysClockSourceDesc() end,
                type = "enumerated",
                choices = function() return GetSysClkSrc() end,
                default = "SYSCLK_PLL",
                file = "include/cfg/clock.h"
            },
            {
               macro = "PLLCLK_SOURCE",
                brief = "PLL Clock Source",
                description = function() return GetPllClockSourceDesc() end,
                type = "enumerated",
                choices = function() return GetPllClkSrc() end,
                default = function() return GetPllClkSrcDefault() end,
                file = "include/cfg/clock.h"
            },
            {
                macro = "SYSCLK_FREQ",
                brief = "CM3 System Clock",
                description = function() return GetSysClockFrequencyDesc() end,
                provides = {"SYSCLK_FREQ"},
                file = "include/cfg/clock.h"
            },
            {
                macro = "HSE_VALUE",
                brief = "External Oszillator Frequency",
                description = function() return GetHseValueDesc() end,
                type = "integer",
                provides = {"HSE_VALUE"},
                file = "include/cfg/clock.h"
            },
            {
                macro = "HSE_BYPASS",
                brief = "HSE from external source",
                description = "Use the clock signal applied to OSC_IN.",
                type = "enumerated",
                choices = {"DISABLE", "ENABLE"},
                default = "DISABLE",
                file = "include/cfg/clock.h",
            },
            {
                macro = "LSE_DRIVE_LEVEL",
                brief = "LSE osc power",
                description = "Power level of LSE oscillator\n"..
                              "0 = Low drive\n"..
                              "1 = Medium high drive\n"..
                              "2 = Medium low drive\n"..
                              "3 = High drive.",
                type = "enumerated",
                choices = function() return GetLseDriveLevel() end,
                default = "0",
                file = "include/cfg/clock.h"
            },
            {
                macro = "LSE_BYPASS",
                brief = "LSE from external source",
                description = "Use clock signal applied to OSC32_IN.",
                type = "enumerated",
                choices = {"DISABLE", "ENABLE"},
                default = "DISABLE",
                file = "include/cfg/clock.h"
            },
            {
                macro = "LSE_VALUE",
                brief = "Frequency of LSE Clock ",
                description = "Frequency of LSE quarz/external LSE input.\n"..
                              "Standard is 32768 Hz.\n"..
                              "Default is undefined.",
                type = "integer",
                provides = {"LSE_VALUE"},
                file = "include/cfg/clock.h"
            },
            {
                macro = "LSI_ON",
                brief = "Turn LSI on",
                description = "Turn LSI on(1) or off(0).\n"..
                              "Default is on.",
                type = "enumerated",
                choices = {"DISABLE", "ENABLE"},
                default = "DISABLE",
                file = "include/cfg/clock.h"
            },
            {
                macro = "RTCCLK_SOURCE",
                brief = "RTC(/LCD) clock source",
                description = "Clock used for RTC and LCD.",
                type = "enumerated",
                choices = {"RTCCLK_LSE", "RTCCLK_HSE", "RTCCLK_LSI"},
                default = function() return GetRtcClkSrcDefault() end,
                file = "include/cfg/clock.h"
            },
            {
                macro = "AHB_DIV",
                brief = "AHB clock divisor.",
                description = "Divisor between SYSCLK and HCLK.\n"..
                              "Allowed values are 1, 2, 4, 8, 16, 32, "..
                              "64,  128, 256 and 512."..
                              "Default is 1.",
                type = "enumerated",
                choices =  {"1", "2", "4", "8", "16", "32", "64",
                            "128", "256", "512"},
                default = "1",
                file = "include/cfg/clock.h"
            },
            {
                macro = "APB1_DIV",
                brief = "APB1 clock divisor.",
                description = "Divisor between HCLK and PCLK1 "..
                              "for low speed APB bus.\n"..
                              "Allowed values are AUTO, 1, 2, 4, 8, and 16.\n"..
                              "Auto tries to reach highest allowed "..
                              "frequency.\nDefault is AUTO",
                type = "enumerated",
                choices =  {"AUTO", "1", "2", "4", "8", "16"},
                default = "AUTO",
                file = "include/cfg/clock.h"
            },
            {
                macro = "APB2_DIV",
                brief = "APB2 clock divisor.",
                description = "Divisor between HCLK and PCLK1 "..
                              "for high speed APB bus.\n"..
                              "Allowed values are AUTO, 1, 2, 4, 8, and 16.\n"..
                              "Auto tries to reach highest allowed "..
                              "frequency.\nDefault is AUTO",
                type = "enumerated",
                choices =  {"AUTO", "1", "2", "4", "8", "16"},
                default = "AUTO",
                file = "include/cfg/clock.h"
            },
                   {
                        macro = "PLLCLK_MULT",
                        brief = "PLL Clock Multiplier",
                        type = "enumerated",
                        choices = function() return GetPllMult() end,
                        requires = {"SYSCLK_FREQ"},
                        file = "include/cfg/clock.h"
                  },
                  {
                        macro = "PLLCLK_PREDIV",
                        brief = "PLL Clock Divider",
                        description = "In many cases HSE_VALUE and SYSCLK_FREQ is enough for the code\n"..
                                      "to calculate PLLCLK_MULT and PLLCLK_DIV. If code is unable,\n"..
                                      "enter non-zero integer PLL Clock Divider value here\n",
                        requires = {"SYSCLK_FREQ"},
                        type = "enumerated",
                        choices =  function() return GetPllPrediv() end,
                        file = "include/cfg/clock.h"
                  },
--STM32F1_CL specific values
                  {
                        macro = "PLL2CLK_PREDIV",
                        brief = "PLL2 Clock Predivider",
                        description = "Divider between HSE and PLL2 input.\n",
                        type = "enumerated",
                        choices = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16"};
                        requires = {"HW_MCU_STM32F1_CL", "SYSCLK_FREQ"},
                        file = "include/cfg/clock.h"
                  },
                  {
                        macro = "PLL2CLK_MULT",
                        brief = "PLL2 Clock Multiplier",
                        description = "Multiplier for PLL2 clock.\n",
                        requires = {"HW_MCU_STM32F1_CL", "SYSCLK_FREQ"},
                        type = "enumerated",
                        choices = { "8", "9", "10", "11", "12", "13", "14", "16", "20"},
                        file = "include/cfg/clock.h"
                  },
    },
    {
        name = "nutarch_cm3_stm32l4_rccl",
        brief = "STM32L4 PLL Setup",
        requires = {"HW_MCU_STM32L4"},
        options =
        {
            {
                macro = "MSI_RANGE",
                brief = "MSI value",
                description = "Select MSI frequency.\n"..
                              "Possible values are 100/200/400/800 kHz "..
                              "and 1/2/4/8/16/24/32/48 MHz.\n"..
                              "Default value after reset is 4 MHz.\n",
                              "In Range 2, maximum allowed frequency is 24 MHz.\n"..
                              "Use MSI_OFF to disable MSI.",
                type = "enumerated",
                choices = {"MSI_OFF", "MSI_100k", "MSI_200k", "MSI_400k", "MSI_800k", "MSI_1M", "MSI_2M", "MSI_4M", "MSI_8M", "MSI_16M", "MSI_24M", "MSI_32M", "MSI_48M"},
                default = "MSI_4M",
                file = "include/cfg/clock.h"
            },
            {
                macro = "STM32_VRANGE",
                brief = "Voltage Scaling",
                description = "Use 2 for extra low power. Restrictions apply.\n"..
                              "E.g. max SYSCLK is  26 MHz in range 2.",
                type = "enumerated",
                choices = {"1", "2"},
                default = "1",
                file = "include/cfg/clock.h"
            },
            {
                macro = "PLLM",
                brief = "PLL input clock divisor",
                description = "PLL input clock M divisor.\n"..
                              "Must be 1..8. Input Clock must be 4..16 MHz.\n"..
                              "Only required if SYSCLK_FREQ is given.",
                requires = {"SYSCLK_FREQ"},
                type = "enumerated",
                choices =  {"1", "2", "3", "4", "5", "6", "7", "8"},
                file = "include/cfg/clock.h"
            },
            {
                macro = "PLLN",
                brief = "PLL clock multiplicator.",
                description = "PLL input clock Multiplicator N.\n"..
                              "Must be 8 .. 86.\n"..
                              "Resulting VCO output must be 64 .. 344(Range 1)/128 (Range2) Mhz.\n"..
                              "Only required if SYSCLK_FREQ is given.",
                requires = {"SYSCLK_FREQ"},
                type = "long",
                file = "include/cfg/clock.h"
            },
            {
                macro = "PLLR",
                brief = "PLL VCO Clock divisor.",
                description = "PLL input clock N Multiplicator.\n"..
                              "Must be 8 .. 86.\n"..
                              "Resulting PLL cloxk must be <= 80 (Range 1)/ 26 (Range2) Mhz.\n"..
                              "Only required if SYSCLK_FREQ is given.",
                requires = {"SYSCLK_FREQ"},
                type = "enumerated",
                choices =  {"2", "4", "6", "8"},
                file = "include/cfg/clock.h"
            },
            {
                macro = "AHB_PRE",
                brief = "AHB clock divisor.",
                description = "Divisor between SYSCLK and HCLK.\n"..
                              "Allowed values are 1, 2, 4, 8, 16, 32, 64. 128, 256 and 512.",
                type = "enumerated",
                choices =  {"1", "2", "4", "8", "16", "32", "64", "128", "256", "512"},
                default = "1",
                file = "include/cfg/clock.h"
            },
            {
                macro = "APB1_PRE",
                brief = "APB1 clock divisor.",
                description = "Divisor between HCLK and PCLK1.\n"..
                              "Allowed values are 1, 2, 4, 8, and 16.",
                type = "enumerated",
                choices =  {"1", "2", "4", "8", "16"},
                default = "1",
                file = "include/cfg/clock.h"
            },
            {
                macro = "APB2_PRE",
                brief = "APB2 clock divisor.",
                description = "Divisor between HCLK and PCLK2.\n"..
                              "Allowed values are 1, 2, 4, 8, and 16.",
                type = "enumerated",
                choices =  {"1", "2", "4", "8", "16"},
                default = "1",
                file = "include/cfg/clock.h"
            },
        },
    }
}
