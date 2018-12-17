--
-- Copyright (C) 2015-2017 Uwe Bonnes bon@elektron.ikp.physik.tu-darmstadt.de.
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
-- Retrive device specfic timer devices and configure devices using timers
--
--
-- Retrieve 32 bit Timers.
--
function GetStmTimers32bit()
-- Devices with no 32-bit Timer
   local list = {}
    if c_is_provided("HW_TIM2_32BIT_STM32") then
        table.insert(list, "2")
    end
    if c_is_provided("HW_TIM5_32BIT_STM32") then
        table.insert(list, "5")
    end
    return list
end

--
-- Retrieve Timer with at least one output channel
--
function GetStmTimers1Ch()
   local list = {}
    if c_is_provided("HW_TIM1_STM32") then
       table.insert(list,  "1")
    end
    if c_is_provided("HW_TIM2_STM32") then
       table.insert(list,  "2")
    end
    if c_is_provided("HW_TIM3_STM32") then
       table.insert(list,  "3")
    end
    if c_is_provided("HW_TIM4_STM32") then
       table.insert(list,  "4")
    end
    if c_is_provided("HW_TIM5_STM32") then
       table.insert(list,  "5")
    end
    if c_is_provided("HW_TIM8_STM32") then
       table.insert(list,  "8")
    end
    if c_is_provided("HW_TIM9_STM32") then
       table.insert(list,  "9")
    end
    if c_is_provided("HW_TIM10_STM32") then
       table.insert(list,  "10")
    end
    if c_is_provided("HW_TIM11_STM32") then
       table.insert(list,  "11")
    end
    if c_is_provided("HW_TIM12_STM32") then
       table.insert(list,  "12")
    end
    if c_is_provided("HW_TIM13_STM32") then
       table.insert(list,  "13")
    end
    if c_is_provided("HW_TIM14_STM32") then
       table.insert(list,  "14")
    end
    if c_is_provided("HW_TIM15_STM32") then
       table.insert(list,  "15")
    end
    if c_is_provided("HW_TIM16_STM32") then
       table.insert(list,  "16")
    end
    if c_is_provided("HW_TIM17_STM32") then
       table.insert(list,  "17")
    end
    if c_is_provided("HW_TIM18_STM32") then
       table.insert(list,  "18")
    end
    if c_is_provided("HW_TIM19_STM32") then
       table.insert(list,  "19")
    end
    if c_is_provided("HW_TIM20_STM32") then
       table.insert(list,  "20")
    end
    if c_is_provided("HW_TIM21_STM32") then
       table.insert(list,  "21")
    end
    if c_is_provided("HW_TIM22_STM32") then
       table.insert(list,  "22")
    end
    return list
end

--
-- Retrieve Timer with at least two channels.
--
function GetStmTimers2Ch()
   local list = {}
    if c_is_provided("HW_TIM1_STM32") then
       table.insert(list,  "1")
    end
    if c_is_provided("HW_TIM2_STM32") then
       table.insert(list,  "2")
    end
    if c_is_provided("HW_TIM3_STM32") then
       table.insert(list,  "3")
    end
    if c_is_provided("HW_TIM4_STM32") then
       table.insert(list,  "4")
    end
    if c_is_provided("HW_TIM5_STM32") then
       table.insert(list,  "5")
    end
    if c_is_provided("HW_TIM8_STM32") then
       table.insert(list,  "8")
    end
    if c_is_provided("HW_TIM9_STM32") then
       table.insert(list,  "9")
    end
    if c_is_provided("HW_TIM12_STM32") then
       table.insert(list,  "12")
    end
    if c_is_provided("HW_TIM15_STM32") then
       table.insert(list,  "15")
    end
    if c_is_provided("HW_TIM18_STM32") then
       table.insert(list,  "18")
    end
    if c_is_provided("HW_TIM19_STM32") then
       table.insert(list,  "19")
    end
    if c_is_provided("HW_TIM20_STM32") then
       table.insert(list,  "20")
    end
    if c_is_provided("HW_TIM21_STM32") then
       table.insert(list,  "21")
    end
    if c_is_provided("HW_TIM22_STM32") then
       table.insert(list,  "22")
    end
    return list
end

nutarch_cm3_stm32_timer_devices =
{
   -- ******************************************
    --
    -- STM32 Timer Device Configuration and drivers
    --
    -- ******************************************
    --
    --
     --
    -- STM32 32 bit Timers
    --
    {
        name = "nutarch_cm3_stm32_timer2_32bit",
        brief = "STM32 Timer2 32Bit",
        description = "STM32 Timer infrastructure.",
        requires = { "HW_TIM2_32BIT_STM32" },
        provides = { "HW_TIM2_STM32" , "HW_STM32_TIMER_32BIT" },
    },
    {
        name = "nutarch_cm3_stm32_timer5_32bit",
        brief = "STM32 Timer5 32Bit",
        description = "STM32 Timer infrastructure.",
        requires = { "HW_TIM5_32BIT_STM32" },
        provides = { "HW_TIM5_STM32" , "HW_STM32_TIMER_32BIT" },
    },
    {
        name = "nutarch_cm3_stm32f1_timer_remap",
        brief = "STM32F1 Timer Pin remap",
        description = "STM32F1 can only remap functions global.",
        requires = { "HW_MCU_STM32F1" },
        options =
        {
           {
              macro = "STM32F1_TIM1_REMAP",
              brief = "STM32F1 Timer 1 Remap",
              description = "Select Timer 1 global remap\n"..
                            "FUNCTION\t0\t1\t3\n"..
                            "TIM1_ETR\tPA12\tPA12\tPE07\n"..
                            "TIM1_CH1\tPA08\tPA08\tPE09\n"..
                            "TIM1_CH2\tPA09\tPA09\tPE11\n"..
                            "TIM1_CH3\tPA10\tPA10\tPE13\n"..
                            "TIM1_CH4\tPA11\tPA11\tPE14\n"..
                            "TIM1_BKIN\tPB12\tPA06\tPE15\n"..
                            "TIM1_CH1N\tPB13\tPA07\tPE08\n"..
                            "TIM1_CH2N\tPB14\tPB00\tPE08\n"..
                            "TIM1_CH3N\tPB15\tPB01\tPE12",
              requires = { "HW_TIM1_STM32" },
              type = "enumerated",
              choices = { "0", "1", "3"},
              default = "0",
              file = "include/cfg/timer.h",
           },
           {
              macro = "STM32F1_TIM2_REMAP",
              brief = "STM32F1 Timer 2 Remap",
              description = "Select Timer 2 global remap\n\n"..
                            "Check PA15/PB03 against JTAG mapping!\n\n"..
                            "FUNCTION\t\t0\t1\t2\t3\n"..
                            "TIM2_CH1_ETR\tPA00\tPA15\tPA00\tPA15\n"..
                            "TIM2_CH2\t\tPA01\tPB03\tPA01\tPB03\n"..
                            "TIM2_CH3\t\tPA02\tPA02\tPB10\tPB10\n"..
                            "TIM2_CH4\t\tPA03\tPA03\tPB11\tPB12",
              requires = { "HW_TIM2_STM32" },
              type = "enumerated",
              choices = { "0", "1", "2", "3"},
              default = "0",
              file = "include/cfg/timer.h",
         },
         {
              macro = "STM32F1_TIM3_REMAP",
              brief = "STM32F1 Timer 3 Remap",
              description = "Select Timer 3 global remap\n\n"..
                            "Check PB04 against JTAG mapping.\n\n"..
                            "FUNCTION\t0\t2\t3\n"..
                            "TIM3_CH1\tPA06\tPB04\tPC06\n"..
                            "TIM3_CH2\tPA07\tPB05\tPC07\n"..
                            "TIM3_CH3\tPB00\tPB00\tPC08\n"..
                            "TIM3_CH4\tPB01\tPB01\tPC09",
              requires = { "HW_TIM3_STM32" },
              type = "enumerated",
              choices = { "0", "2", "3"},
              default = "0",
              file = "include/cfg/timer.h",
         },
         {
              macro = "STM32F1_TIM4_REMAP",
              brief = "STM32F1 Timer 4 Remap",
              description = "Select Timer 4 global remap\n"..
                            "FUNCTION\t0\t1\n"..
                            "TIM4_CH1\tPB06\tPD12\n"..
                            "TIM4_CH2\tPB07\tPD13\n"..
                            "TIM4_CH3\tPB08\tPD14\n"..
                            "TIM4_CH4\tPB09\tPD15",
              requires = { "HW_TIM4_STM32" },
              type = "enumerated",
              choices = { "0", "1"},
              default = "0",
              file = "include/cfg/timer.h",
         },
         {
              macro = "STM32F1_TIM5_REMAP",
              brief = "STM32F1 Timer 5 Remap",
              description = "Select Timer 4 global remap\n"..
                            "FUNCTION\t0\t1\n"..
                            "TIM5_CH4\tPA03\tInternal LSI clock",
              requires = { "HW_TIM5_STM32" },
              type = "enumerated",
              choices = { "0", "1"},
              default = "0",
              file = "include/cfg/timer.h",
         },
         {
              macro = "STM32F1_TIM9_REMAP",
              brief = "STM32F1 Timer 9 Remap",
              description = "Select Timer 9 global remap\n"..
                            "FUNCTION\t0\t1\n"..
                            "TIM9_CH1\tPA02\tPE05\n"..
                            "TIM9_CH2\tPA03\tPE06",
              requires = { "HW_TIM9_STM32" },
              type = "enumerated",
              choices = { "0", "1"},
              default = "0",
              file = "include/cfg/timer.h",
         },
         {
              macro = "STM32F1_TIM10_REMAP",
              brief = "STM32F1 Timer 10 Remap",
              description = "Select Timer 10 global remap\n"..
                            "FUNCTION\t0\t1\n"..
                            "TIM10_CH1\tPB08\tPF06",
              requires = { "HW_TIM10_STM32" },
              type = "enumerated",
              choices = { "0", "1"},
              default = "0",
              file = "include/cfg/timer.h",
         },
         {
              macro = "STM32F1_TIM11_REMAP",
              brief = "STM32F1 Timer 11 Remap",
              description = "Select Timer 11 global remap\n"..
                            "FUNCTION\t0\t1\n"..
                            "TIM11_CH1\tPB09\tPF07",
              requires = { "HW_TIM11_STM32" },
              type = "enumerated",
              choices = { "0", "1"},
              default = "0",
              file = "include/cfg/timer.h",
         },
         {
              macro = "STM32F1_TIM12_REMAP",
              brief = "STM32F1 Timer 12 Remap",
              description = "Select Timer 12 global remap\n"..
                            "FUNCTION\t0\t1\n"..
                            "TIM12_CH1\tPC04\tPB12\n"..
                            "TIM12_CH2\tPC05\tPB13",
              requires = { "HW_TIM12_STM32" },
              type = "enumerated",
              choices = { "0", "1"},
              default = "0",
              file = "include/cfg/timer.h",
         },
         {
              macro = "STM32F1_TIM13_REMAP",
              brief = "STM32F1 Timer 13 Remap",
              description = "Select Timer 13 global remap\n"..
                            "FUNCTION\t0\t1\n"..
                            "TIM13_CH1\tPC08\tPB00",
              requires = { "HW_TIM13_STM32" },
              type = "enumerated",
              choices = { "0", "1"},
              default = "0",
              file = "include/cfg/timer.h",
         },
         {
              macro = "STM32F1_TIM14_REMAP",
              brief = "STM32F1 Timer 14 Remap",
              description = "Select Timer 14 global remap\n"..
                            "FUNCTION\t0\t1\n"..
                            "TIM14_CH1\tPC09\tPB01",
              requires = { "HW_TIM14_STM32" },
              type = "enumerated",
              choices = { "0", "1"},
              default = "0",
              file = "include/cfg/timer.h",
         },
         {
              macro = "STM32F1_TIM15_REMAP",
              brief = "STM32F1 Timer 15 Remap",
              description = "Select Timer 15 global remap\n"..
                            "FUNCTION\t0\t1\n"..
                            "TIM15_CH1\tPA02\tPB14\n",
                            "TIM15_CH1\tPA03\tPB15",
              requires = { "HW_TIM15_STM32" },
              type = "enumerated",
              choices = { "0", "1"},
              default = "0",
              file = "include/cfg/timer.h",
         },
         {
              macro = "STM32F1_TIM16_REMAP",
              brief = "STM32F1 Timer 16 Remap",
              description = "Select Timer 16 global remap\n"..
                            "FUNCTION\t0\t1\n"..
                            "TIM16_CH1\tPB08\tPA06",
              requires = { "HW_TIM16_STM32" },
              type = "enumerated",
              choices = { "0", "1"},
              default = "0",
              file = "include/cfg/timer.h",
         },
          {
              macro = "STM32F1_TIM17_REMAP",
              brief = "STM32F1 Timer 17 Remap",
              description = "Select Timer 17 global remap\n"..
                            "FUNCTION\t0\t1\n"..
                            "TIM17_CH1\tPB09\tPA07",
              requires = { "HW_TIM17_STM32" },
              type = "enumerated",
              choices = { "0", "1"},
              default = "0",
              file = "include/cfg/timer.h",
         },
      }
    },
    {
        name = "nutarch_cm3_stm32_qenc0",
        brief = "STM32 Quadrature Encoder0",
        description = "STM32 Quadrature Encoder 0.\n\nAny dual channel "..
                      "STM32 timer can count quadrature encoded signals.\n"..
                      "Hardware used 32-bit feature, if available.\n"..
                      "For 16-bit timers, make sure readout happens before overflow!",
        sources = {"cm3/dev/stm/stm32_qenc.c"},
        options =
        {
            {
                macro = "STM32_QENC0_TIMER_ID",
                brief = "STM32 Quadrature Encoder Timer ID",
                description = "Select Timer for 32 bit Quadrature Encoder.\n\n"..
                              "Check for availability on selected device.",
                type = "enumerated",
                choices = function() return GetStmTimers2Ch() end,
                file = "include/cfg/qenc.h",
            },
            {
                macro = "STM32_QENC0_I_GPIO",
                brief = "STM32 Quadrature Encoder I input gpio",
                description = "STM32 Quadrature Encoder I input port. Must be on TI1.",
                file = "include/cfg/qenc.h",
            },
            {
                macro = "STM32_QENC0_Q_GPIO",
                brief = "STM32 Quadrature Encoder Q input gpio",
                description = "STM32 Quadrature Encoder I input pin. Must be on TI2",
                file = "include/cfg/qenc.h",
            },
            {
                macro = "STM32_QENC0_INVERT",
                brief = "STM32 Quadrature Encoder reverse count direction",
                description = "STM32 Quadrature Encoder reverse count "..
                              " direction.\n"..
                              "Effective exchanges I and Q.\n"..
                              "Default is DISABLE.",
                type = "enumerated",
                default = "DISABLE",
                choices = { "ENABLE", "DISABLE" },
                file = "include/cfg/qenc.h",
            },
            {
                macro = "STM32_QENC0_INDEX_GPIO",
                brief = "STM32 Quadrature Encoder Index input gpio",
                description = "STM32 Quadrature Encoder Index pulse gpio.\n"..
                            "If hardware has index pulse, the "..
                            "captured index pulses allow consistancy check\n"..
                            "Leave undefined if not used.",
                file = "include/cfg/qenc.h",
            },
            {
                macro = "STM32_QENC0_INDEX_TIMER_CHANNEL",
                brief = "STM32 Quadrature Encoder Index input timer channel.",
                description = "STM32 Quadrature Encoder Index pulse "..
                              "timer channel.\n"..
                            "Leave undefined if not used.",
                file = "include/cfg/qenc.h",
            },
        },
    },
    --
    -- STM32 PWMs using hardware timer
    --
    {
        name = "nutarch_cm3_stm32_pwm0",
        brief = "STM32 PWM Output 0",
        description = "STM32 PWM_Output 0.",
        sources = { "cm3/dev/stm/stm32_pwm.c" },
        options =
        {
            {
                macro = "STM32_PWM0_TIMER_ID",
                brief = "STM32 32Bit PWM0 Timer ID",
                description = "Select Timer for PWM0 output.\n\n"..
                              "Check for availability on selected device!",
                type = "enumerated",
                choices = function() return  GetStmTimers1Ch() end,
                file = "include/cfg/pwm.h",
            },
            {
                macro = "STM32_PWM0_TIMER_CHANNEL",
                brief = "STM32 32Bit PWM0 Timer Channel",
                description = "Select Timer Channel for PWM0 output.\n\n"..
                              "Negative numbers x indicate CHxN.\n"..
                              "Check for availability on selected device.",
                type = "enumerated",
                choices = { "1", "2", "3", "4", "-1", "-2", "-3" },
                file = "include/cfg/pwm.h",
            },
            {
                macro = "STM32_PWM0",
                brief = "Gpio for STM32 PWM0 output",
                description = "Gpio used for PWM0. Only some pins are valid!\n"..
                            "For STM32F1, check Timer global remap!",
                file = "include/cfg/pwm.h",
            },
        }
    },
    {
        name = "nutarch_cm3_stm32_pwm1",
        brief = "STM32 PWM Output 1",
        description = "STM32 PWM_Output 1.",
        sources = { "cm3/dev/stm/stm32_pwm.c" },
        options =
        {
            {
                macro = "STM32_PWM1_TIMER_ID",
                brief = "STM32 32Bit PWM1 Timer ID",
                description = "Select Timer for PWM1 output.\n\n"..
                              "Check for availability on selected device.",
                type = "enumerated",
                choices = function() return  GetStmTimers1Ch() end,
                file = "include/cfg/pwm.h",
            },
            {
                macro = "STM32_PWM1_TIMER_CHANNEL",
                brief = "STM32 32Bit PWM1 Timer Channel",
                description = "Select Timer Channel for PWM1 output.\n\n"..
                              "Negative numbers x indicate CHxN.\n"..
                              "Check for availability on selected device.",
                type = "enumerated",
                choices = { "1", "2", "3", "4", "-1", "-2", "-3" },
                file = "include/cfg/pwm.h",
            },
            {
                macro = "STM32_PWM1",
                brief = "Gpio for STM32 PWM1 output",
                description = "Gpio used for PWM1. Only some pins are valid!\n"..
                            "For STM32F1, check Timer global remap!",
                file = "include/cfg/pwm.h",
            },
        }
    },
    --
    -- STM32 OWIBUS0 using hardware timer
    --
    {
        name = "nutarch_cm3_stm32_owibus0tim",
        brief = "STM32 OWIBUS0 with hardware timer",
        description = "STM32 OWIBUS0 with hardware timer.",
        provides = { "OWIBUS" },
        sources = { "cm3/dev/stm/stm32_owitim.c" },
        options =
        {
            {
                macro = "STM32TIM_OWI0_GPIO",
                brief = "Gpio for STM32 hardware timer OWIBUS0",
                description = "Gpio used for OWIBUS0. "..
                            "Check datasheet for Port/Pin function!\n"..
                            "For STM32F1, check Timer global remap!",
                file = "include/cfg/owi.h",
            },
            {
                macro = "STM32TIM_OWI0_TIMER_ID",
                brief = "Timer for STM32 hardware timer OWIBUS0",
                description = "Timer used for OWIBUS0.\n\n"..
                               "Requires at least dual channel!\n"..
                              "Check for availability on selected device.",
                type = "enumerated",
                choices = function() return GetStmTimers2Ch() end,
                file = "include/cfg/owi.h",
            },
            {
                macro = "STM32TIM_OWI0_CHANNEL",
                brief = "Channel for STM32 hardware timer OWIBUS0",
                description = "Channel for STM32 hardware timer OWIBUS0.\n\n"..
                               "Check for Channel 3/4 availability!",
                type = "enumerated",
                choices = { "1", "2", "3", "4" },
                file = "include/cfg/owi.h",
            },
            {
                macro = "STM32TIM_OWI0_TX_GPIO",
                brief = "TX GPIO for STM32 hardware timer OWIBUS0",
                description = "If OWI Tx and Rx use different pins,"..
                            "enter the GPIO for the TX pin here!\n"..
                            "For STM32F1, check Timer global remap!\n"..
                            "Leave undefined when not used!",
                file = "include/cfg/owi.h",
                provides = {"STM32TIM_OWI0_TX"},
            },
            {
                macro = "STM32TIM_OWI0_TX_CHANNEL",
                brief = "TX Channel for STM32 hardware timer OWIBUS0",
                description = "TX Channel for separate TX/RX.\n\n"..
                              "Check for Channel 3/4 availability!",
                type = "enumerated",
                requires = {"STM32TIM_OWI0_TX"},
                choices = { "1", "2", "3", "4" },
                file = "include/cfg/owi.h",
            },
            {
                macro = "STM32TIM_OWI0_TX_TRUE",
                brief = "Invert TX Channel for OWIBUS0",
                description = "Separated TX/RX is normally used for "..
                            "slew-rate controlled TX.\n"..
                            "This normally results in logic inversion.\n"..
                            "Default is DISABLE for inverting buffer",
                type = "enumerated",
                requires = {"STM32TIM_OWI0_TX"},
                default = "DISABLE",
                choices = { "ENABLE", "DISABLE" },
                file = "include/cfg/owi.h",
            },
         }
     },

    --
    -- STM32 OWIBUS1 using hardware timer
    --
    {
        name = "nutarch_cm3_stm32_owibus1tim",
        brief = "STM32 OWIBUS1 with hardware timer",
        description = "STM32 OWIBUS1 with hardware timer, "..
                    "library compile time configured",
        provides = { "OWIBUS" },
        sources = { "cm3/dev/stm/stm32_owitim.c" },
        options =
        {
            {
                macro = "STM32TIM_OWI1_GPIO",
                brief = "GPIO for STM32 hardware timer OWIBUS1",
                description = "GPIO used for OWIBUS1. "..
                            "Check datasheet for Port/Pin function!\n"..
                            "For STM32F1, check Timer global remap!",
                file = "include/cfg/owi.h",
            },
            {
                macro = "STM32TIM_OWI1_TIMER_ID",
                brief = "Timer for STM32 hardware timer OWIBUS1",
                description = "Timer used for OWIBUS1.\n\n"..
                              "Requires at least dual channel!\n"..
                              "Check for availability on selected device.",
                type = "enumerated",
                choices = function() return GetStmTimers2Ch() end,
                file = "include/cfg/owi.h",
            },
            {
                macro = "STM32TIM_OWI1_CHANNEL",
                brief = "Channel for STM32 hardware timer OWIBUS1",
                description = "Channel for STM32 hardware timer OWIBUS1.\n\n"..
                              "Check for Channel 3/4 availability!",
                type = "enumerated",
                choices = { "1", "2", "3", "4" },
                file = "include/cfg/owi.h",
            },
            {
                macro = "STM32TIM_OWI1_TX_GPIO",
                brief = "TX GPIO for STM32 hardware timer OWIBUS0",
                description = "If OWI Tx and Rx use different pins,"..
                            "enter the GPIO for the TX pin here!\n"..
                            "For STM32F1, check Timer global remap!\n",
                file = "include/cfg/owi.h",
                provides = {"STM32TIM_OWI1_TX"},
            },
            {
                macro = "STM32TIM_OWI1_TX_CHANNEL",
                brief = "TX Channel for STM32 hardware timer OWIBUS0",
                description = "OWI1 TX Channel separate TX/RX.\n\n"..
                              "Negative values X (1|2|3) mean CHxN.\n"..
                              "Check for availability!",
                type = "enumerated",
                requires = {"STM32TIM_OWI1_TX"},
                choices = { "1", "2", "3", "4", "-1", "-2", "-3" },
                file = "include/cfg/owi.h",
            },
            {
                macro = "STM32TIM_OWI1_TX_INVERT",
                brief = "Invert TX Channel for OWIBUS0",
                description = "Separated TX/RX is normally used for "..
                            "slew-rate controlled TX.\n"..
                            "This normally results in logic inversion.\n"..
                            "Default is ENABLE to compensate inversion",
                type = "enumerated",
                requires = {"STM32TIM_OWI1_TX"},
                default = "ENABLE",
                choices = { "ENABLE", "DISABLE" },
                file = "include/cfg/owi.h",
            },
       }
     },

    --
    -- STM32 OWIBUS2 using hardware timer
    --
    {
        name = "nutarch_cm3_stm32_owibus2tim",
        brief = "STM32 OWIBUS2 with hardware timer",
        description = "STM32 OWIBUS2 with hardware timer, "..
                    "library compile time configured",
        provides = {"OWIBUS"},
        sources = { "cm3/dev/stm/stm32_owitim.c" },
        options =
        {
            {
                macro = "STM32TIM_OWI2_GPIO",
                brief = "Gpio for STM32 hardware timer OWIBUS2",
                description = "Gpio used for OWIBUS2. "..
                            "Check datasheet for Port/Pin function!\n"..
                            "For STM32F1, check Timer global remap!",
                file = "include/cfg/owi.h",
            },
            {
                macro = "STM32TIM_OWI2_TIMER_ID",
                brief = "Timer for STM32 hardware timer OWIBUS2",
                description = "Timer used for OWIBUS2.\n\n"..
                              "Requires at least dual channel!\n"..
                              "Check for availability on selected device.",
                type = "enumerated",
                choices = function() return GetStmTimers2Ch() end,
                file = "include/cfg/owi.h",
            },
            {
                macro = "STM32TIM_OWI2_CHANNEL",
                brief = "Channel for STM32 hardware timer OWIBUS2",
                description = "Channel for STM32 hardware timer OWIBUS2.\n\n"..
                              "Check for Channel 3/4 availability!",
                type = "enumerated",
                choices = { "1", "2", "3", "4" },
                file = "include/cfg/owi.h",
            },
            {
                macro = "STM32TIM_OWI2_TX_GPIO",
                brief = "TX GPIO for STM32 hardware timer OWIBUS2",
                description = "If OWI Tx and Rx use different pins,"..
                            "enter the GPIO for the TX pin here!\n"..
                            "For STM32F1, check Timer global remap!\n",
                file = "include/cfg/owi.h",
                provides = {"STM32TIM_OWI2_TX"},
            },
            {
                macro = "STM32TIM_OWI2_TX_CHANNEL",
                brief = "TX Channel for STM32 hardware timer OWIBUS2",
                description = "OWI2 TX Channel separate TX/RX.\n\n"..
                            "Negative values X (1|2|3) mean CHxN.\n"..
                            "Check for availability!",
                type = "enumerated",
                requires = {"STM32TIM_OWI2_TX"},
                choices = { "1", "2", "3", "4", "-1", "-2", "-3" },
                file = "include/cfg/owi.h",
            },
            {
                macro = "STM32TIM_OWI2_TX_INVERT",
                brief = "Invert TX Channel for OWIBUS2",
                description = "Separated TX/RX is normally used for "..
                            "slew-rate controlled TX.\n"..
                            "This normally results in logic inversion.\n"..
                            "Default is ENABLE to compensate inversion",
                type = "enumerated",
                requires = {"STM32TIM_OWI2_TX"},
                default = "ENABLE",
                choices = { "ENABLE", "DISABLE" },
                file = "include/cfg/owi.h",
            },
        }
     },
}
