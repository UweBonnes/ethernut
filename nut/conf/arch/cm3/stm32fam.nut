--
-- Copyright (C) 2004-2007 by egnite Software GmbH. All rights reserved.
-- Copyright (C) 2011-2015 Uwe Bonnes (bon@elektron.ikp.physik.tu-darmstadt.de)
--
-- Redistribution and use in source and binary forms, with or without
-- modification, are permitted provided that the following conditions
-- are met:
--
-- 1. Redistributions of source code must retain the above copyright
--    notice, this list of conditions and the following disclaimer.
-- 2. Redistributions in binary form must reproduce the above copyright
--    notice, this list of conditions and the following disclaimer in the
--    documentation and/or other materials provided with the distribution.
-- 3. Neither the name of the copyright holders nor the names of
--    contributors may be used to endorse or promote products derived
--    from this software without specific prior written permission.
--
-- THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
-- ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
-- LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
-- FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
-- COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
-- INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
-- BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
-- OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
-- AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
-- OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
-- THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
-- SUCH DAMAGE.
--
-- For additional information see http://www.ethernut.de/
--

-- Remarks for the STM Family selection
--
-- Define the Full device with flash size designator but without package
-- designator in the .conf File e.g. as MCU_STM32F429xG
-- arch.nut deciphers this as
-- - architecture (CM3)
-- - vendor iImplementation (MCU_STM32)
-- - vendor family (MCU_STM32F4)
-- - vendor subfamily (MCU_STM32F42) if applicable
-- - and the device (MCU_STM32F429).
-- Stm32f4.nut adds the devices in all F4,
-- devices in common in F427/429/437/439 and finally the specific
-- devices in STM32F420.
-- All internal defines start in MCU_, only the final clause for the STM32F429
-- defines "STM32F429xx", as this is the same define as used by STM Cube
-- to include the vendor header.

nutarch_cm3_stm32_family =
{
    --
    -- STM32 Based Cpu Directory
    --
    {
        name = "nutarch_cm3_stm32f0",
        brief = "STM32F0",
        requires = { "HW_MCU_STM32", "HW_MCU_STM32F0" },
        description = "ST Microelectronics STM32 F0 Series",
        sources = { "cm3/dev/stm/stm32f30_clk.c"},
        makedefs = { "MCU=cortex-m0", "LDPATH=$(LDINCLUDE)" },
        script = "arch/cm3/stm32f0.nut"
    },
    {
        name = "nutarch_cm3_stm32f1",
        brief = "STM32F1",
        requires = { "HW_MCU_STM32", "HW_MCU_STM32F1" },
        description = "ST Microelectronics STM32 F1 Series",
        sources = { "cm3/dev/stm/stm32f1_clk.c"},
        makedefs = {
            "MCU=cortex-m3",
            "MFIX=-mfix-cortex-m3-ldrd",
            "LDPATH=$(LDINCLUDE)",
        },
        script = "arch/cm3/stm32f1.nut"
    },
    {
        name = "nutarch_cm3_stm32l0",
        brief = "STM32L0",
        requires = { "HW_MCU_STM32", "HW_MCU_STM32L0" },
        description = "ST Microelectronics STM32 L0 Series",
        sources = { "cm3/dev/stm/stm32l0_clk.c" },
        makedefs = {
                  "MCU=cortex-m0plus",
                  "LDPATH=$(LDINCLUDE)"
        },
        script = "arch/cm3/stm32l0.nut"
    },
    {
        name = "nutarch_cm3_stm32l4",
        brief = "STM32L4",
        requires = { "HW_MCU_STM32", "HW_MCU_STM32L4" },
        description = "ST Microelectronics STM32 L4 Series",
        sources = { "cm3/dev/stm/stm32l4_clk.c" },
        makedefs = {
                  "MCU=cortex-m4",
                  "LDPATH=$(LDINCLUDE)"
        },
        script = "arch/cm3/stm32l4.nut"
    },
    {
        name = "nutarch_cm3_stm32l1",
        brief = "STM32L1",
        requires = { "HW_MCU_STM32", "HW_MCU_STM32L1" },
        description = "ST Microelectronics STM32 F1 Series",
        sources = { "cm3/dev/stm/stm32l1_clk.c" },
        makedefs = {
                  "MCU=cortex-m3",
                  "MFIX=-mfix-cortex-m3-ldrd",
                  "LDPATH=$(LDINCLUDE)"
        },
        script = "arch/cm3/stm32l1.nut"
    },
    {
        name = "nutarch_cm3_stm32f2",
        brief = "STM32F2",
        requires = { "HW_MCU_STM32", "HW_MCU_STM32F2" },
        description = "ST Microelectronics STM32 F2 Series",
        sources = { "cm3/dev/stm/stm32f4_clk.c" },
        makedefs = { "MCU=cortex-m3", "LDPATH=$(LDINCLUDE)"},
        script = "arch/cm3/stm32f2.nut"
    },
    {
        name = "nutarch_cm3_stm32f3",
        brief = "STM32F3",
        requires = { "HW_MCU_STM32", "HW_MCU_STM32F3" },
        description = "ST Microelectronics STM32 F3 Series",
        sources = { "cm3/dev/stm/stm32f30_clk.c" },
        makedefs = { "MCU=cortex-m4", "LDPATH=$(LDINCLUDE)" },
        script = "arch/cm3/stm32f3.nut"
    },
    {
        name = "nutarch_cm3_stm32f4",
        brief = "STM32F4",
        requires = { "HW_MCU_STM32", "HW_MCU_STM32F4" },
        description = "ST Microelectronics STM32 F4 Series",
        sources = { "cm3/dev/stm/stm32f4_clk.c" },
        makedefs = { "MCU=cortex-m4", "LDPATH=$(LDINCLUDE)" },
        script = "arch/cm3/stm32f4.nut"
    },
    {
        name = "nutarch_cm3_stm32f7",
        brief = "STM32F7",
        requires = { "HW_MCU_STM32", "HW_MCU_STM32F7" },
        description = "ST Microelectronics STM32 F4 Series",
        sources = { "cm3/dev/stm/stm32f7_clk.c" },
        makedefs = { "MCU=cortex-m7", "LDPATH=$(LDINCLUDE)" },
        script = "arch/cm3/stm32f7.nut"
    },
    {
        name = "nutarch_cm3_stm32_pll",
        brief = "Clock settings",
        description = "Clock settings",
        requires = { "HW_MCU_STM32" },
        script = "arch/cm3/stm32pll.nut"
    },
    {
        name = "nutarch_cm3_stm32_usart_devices",
        brief = "U(S)ART devices",
        description = "STM32 U(S)ART devices.",
        requires = { "HW_MCU_STM32" },
        script = "arch/cm3/stm32usart.nut"
    },
    {
        name = "nutarch_cm3_stm32_i2c_devices",
        brief = "I2C devices",
        description = "STM32 I2C devices.",
        requires = { "HW_MCU_STM32" },
        script = "arch/cm3/stm32i2c.nut"
    },
    {
        name = "nutarch_cm3_stm32_spi_devices",
        brief = "SPI devices",
        description = "STM32 SPI devices.",
        requires = { "HW_MCU_STM32" },
        script = "arch/cm3/stm32spi.nut"
    },
    {
        name = "nutarch_cm3_stm32_timer_devices",
        brief = "Timer devices",
        description = "STM32 Timer availability and devices using timers",
        requires = { "HW_MCU_STM32" },
        script = "arch/cm3/stm32timer.nut"
    },
    {
        name = "nutarch_cm3_stm32_devices",
        brief = "Other devices",
        description = "Other devices of the STM32 families",
        requires = { "HW_MCU_STM32" },
        script = "arch/cm3/stm32dev.nut"
    },
}

