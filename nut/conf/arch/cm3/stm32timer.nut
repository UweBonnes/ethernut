--
-- Copyright (C) 2015 Uwe Bonnes bon@elektron.ikp.physik.tu-darmstadt.de.
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
    if c_is_provided("HW_MCU_STM32F030x6") then
        return { "" }
    end
    if c_is_provided("HW_MCU_STM32F030x8") then
        return { "" }
    end
    if c_is_provided("HW_MCU_STM32F030xC") then
        return { "" }
    end
    if c_is_provided("HW_MCU_STM32F070x6") then
        return { "" }
    end
    if c_is_provided("HW_MCU_STM32F070xB") then
        return { "" }
    end
-- Devices with 2 32-bit timer
    if c_is_provided("HW_MCU_STM32F2") then
        return { "2", "5"}
    end
    if c_is_provided("HW_MCU_STM32F4") then
        return { "2", "5"}
    end
    if c_is_provided("HW_MCU_STM32F7") then
        return { "2", "5"}
    end
-- Default is one 32-Bit timer
    return { "2"}
end

--
-- Retrieve Timer with at least one output channel
--
function GetStmTimers1Ch()
    if c_is_provided("HW_MCU_STM32F030x6") then
        return { "", "1" , "3", "14" , "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F030x8") then
        return { "", "1" , "3", "14" , "15", "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F030xC") then
        return { "", "1" , "3", "14" , "15", "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F031x6") then
        return { "", "1" , "2", "3", "14" , "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F038xx") then
        return { "", "1" , "2", "3", "14" , "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F042x6") then
        return { "", "1" , "2", "3", "14" , "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F048xx") then
        return { "", "1" , "2", "3", "14" , "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F051x8") then
        return { "", "1" , "2", "3", "14" , "15", "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F058xx") then
        return { "", "1" , "2", "3", "14" , "15", "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F070x6") then
        return { "", "1" , "3", "14" , "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F070xB") then
        return { "", "1" , "3", "14" , "15", "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F071xB") then
        return { "", "1" , "2", "3", "14" , "15", "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F072xB") then
        return { "", "1" , "2", "3", "14" , "15", "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F078xx") then
        return { "", "1" , "2", "3", "14" , "15", "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F091xC") then
        return { "", "1" , "2", "3", "14" , "15", "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F098xx") then
        return { "", "1" , "2", "3", "14" , "15", "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F2") then
        return { "", "1" , "2", "3", "4", "5", "8", "9", "10", "11", "12" , "13" , "14" }
    end
    if c_is_provided("HW_MCU_STM32F301x8") then
        return { "", "1", "2",                                       "15", "16", "17"                  }
    end
    if c_is_provided("HW_MCU_STM32F302_LD") then
        return { "", "1", "2",                                       "15", "16", "17"                  }
    end
    if c_is_provided("HW_MCU_STM32F302_MD") then
        return { "", "1", "2", "3", "4",                             "15", "16", "17"                  }
    end
    if c_is_provided("HW_MCU_STM32F302_HD") then
        return { "", "1", "2", "3", "4",                             "15", "16", "17"                  }
    end
    if c_is_provided("HW_MCU_STM32F303_LD") then
        return { "", "1", "2", "3",                                  "15", "16", "17"                  }
    end
    if c_is_provided("HW_MCU_STM32F303_MD") then
        return { "", "1", "2", "3", "4",      "8",                   "15", "16", "17"                  }
    end
    if c_is_provided("HW_MCU_STM32F303_HD") then
        return { "", "1", "2", "3", "4",      "8",                   "15", "16", "17",             "20"}
    end
    if c_is_provided("HW_MCU_STM32F318xx") then
        return { "", "1", "2",                                       "15", "16", "17"                  }
    end
    if c_is_provided("HW_MCU_STM32F328xx") then
        return { "", "1", "2", "3",                                  "15", "16", "17"                  }
    end
    if c_is_provided("HW_MCU_STM32F334x8") then
        return { "", "1", "2", "3",                                  "15", "16", "17"                  }
    end
    if c_is_provided("HW_MCU_STM32F358xx") then
        return { "", "1", "2", "3", "4",      "8",                   "15", "16", "17"                  }
    end
    if c_is_provided("HW_MCU_STM32F373_MD") then
        return { "",      "2", "3", "4", "5",      "12", "13", "14", "15", "16", "17", "18", "19",     }
    end
    if c_is_provided("HW_MCU_STM32F378xx") then
        return { "",      "2", "3", "4", "5",      "12", "13", "14", "15", "16", "17", "18", "19",     }
    end
    if c_is_provided("HW_MCU_STM32F398xx") then
        return { "", "1", "2", "3", "4",      "8",                   "15", "16", "17",             "20"}
    end
    if c_is_provided("HW_MCU_STM32F401") then
        return { "", "1", "2", "3", "4", "5", "9", "10", "11"}
    end
    if c_is_provided("HW_MCU_STM32F411") then
        return { "", "1", "2", "3", "4", "5", "9", "10", "11"}
    end
    if c_is_provided("HW_MCU_STM32F40") then
        return { "", "1", "2", "3", "4", "5", "8", "9", "10", "11", "12", "13", "14"}
    end
    if c_is_provided("HW_MCU_STM32F42") then
        return { "", "1", "2", "3", "4", "5", "8", "9", "10", "11", "12", "13", "14"}
    end
    if c_is_provided("HW_MCU_STM32F446") then
        return { "", "1", "2", "3", "4", "5", "8", "9", "10", "11", "12", "13", "14"}
    end
    if c_is_provided("HW_MCU_STM32F7") then
        return { "", "1", "2", "3", "4", "5", "8", "9", "10", "11", "12", "13", "14"}
    end
    if c_is_provided("HW_MCU_STM32L1_MD") then
        return { "", "2", "3", "4", "9", "10", "11"}
    end
    if c_is_provided("HW_MCU_STM32L1_MDP") then
        return { "", "2", "3", "4", "5", "9", "10", "11"}
    end
    if c_is_provided("HW_MCU_STM32L1_HD") then
        return { "", "2", "3", "4", "5", "9", "10", "11"}
    end
    if c_is_provided("HW_MCU_STM32L1_XL") then
        return { "", "2", "3", "4", "5", "9", "10", "11"}
    end
--  For all other devices return all known values for now
    return { "", "1", "2", "3", "4", "5", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22"}
end

--
-- Retrieve Timer with at least two channels.
--
function GetStmTimers2Ch()
-- List timers with at least two channels
-- List of all known timers with two channels min:
-- {"", "1", "2", "3", "4", "5", "8", "9", "12", "15", "16", "17", "19"}
    if c_is_provided("HW_MCU_STM32F030x6") then
        return { "", "1" , "3",  "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F030x8") then
        return { "", "1" , "3",   "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F030xC") then
        return { "", "1" , "3",   "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F031x6") then
        return { "", "1" , "2", "3",  "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F038xx") then
        return { "", "1" , "2", "3",  "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F042x6") then
        return { "", "1" , "2", "3",  "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F048xx") then
        return { "", "1" , "2", "3",  "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F051x8") then
        return { "", "1" , "2", "3",   "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F058xx") then
        return { "", "1" , "2", "3",   "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F070x6") then
        return { "", "1" , "3",  "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F070xB") then
        return { "", "1" , "3",   "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F071xB") then
        return { "", "1" , "2", "3",   "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F072xB") then
        return { "", "1" , "2", "3",   "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F078xx") then
        return { "", "1" , "2", "3",   "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F091xC") then
        return { "", "1" , "2", "3",   "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F098xx") then
        return { "", "1" , "2", "3",   "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F10X_LD") then
        return { "", "1", "2", "3"}
    end
    if c_is_provided("HW_MCU_STM32F10X_LD_VL") then
        return { "", "1", "2", "3"}
    end
    if c_is_provided("HW_MCU_STM32F10X_MD") then
        return { "", "1", "2", "3", "4"}
    end
    if c_is_provided("HW_MCU_STM32F10X_MD_VL") then
        return { "", "1", "2", "3", "4"}
    end
    if c_is_provided("HW_MCU_STM32F10X_HD") then
        return { "", "1", "2", "3", "4", "5"}
    end
    if c_is_provided("HW_MCU_STM32F10X_HD_VL") then
        return { "", "1", "2", "3", "4", "5", "12"}
    end
    if c_is_provided("HW_MCU_STM32F10X_XL") then
        return { "", "1", "2", "3", "4", "5"}
    end
    if c_is_provided("HW_MCU_STM32F10X_CL") then
        return { "", "1", "2", "3", "4", "5"}
    end
    if c_is_provided("HW_MCU_STM32F2") then
        return { "", "1", "2", "3", "4", "5", "8", "9", "12"}
    end
    if c_is_provided("HW_MCU_STM32F301x8") then
        return { "", "1", "2",                           "15", "16", "17"           }
    end
    if c_is_provided("HW_MCU_STM32F302_LD") then
        return { "", "1", "2",                           "15", "16", "17"           }
    end
    if c_is_provided("HW_MCU_STM32F302_MD") then
        return { "", "1", "2", "3", "4",                 "15", "16", "17"           }
    end
    if c_is_provided("HW_MCU_STM32F302_HD") then
        return { "", "1", "2", "3", "4",                 "15", "16", "17"           }
    end
    if c_is_provided("HW_MCU_STM32F303_LD") then
        return { "", "1", "2", "3",                      "15", "16", "17"           }
    end
    if c_is_provided("HW_MCU_STM32F303_MD") then
        return { "", "1", "2", "3", "4",      "8",       "15", "16", "17"           }
    end
    if c_is_provided("HW_MCU_STM32F303_HD") then
        return { "", "1", "2", "3", "4",      "8",       "15", "16", "17",      "20"}
    end
    if c_is_provided("HW_MCU_STM32F318xx") then
        return { "", "1", "2",                           "15", "16", "17"           }
    end
    if c_is_provided("HW_MCU_STM32F328xx") then
        return { "", "1", "2", "3",                      "15", "16", "17"           }
    end
    if c_is_provided("HW_MCU_STM32F334x8") then
        return { "", "1", "2", "3",                      "15", "16", "17"           }
    end
    if c_is_provided("HW_MCU_STM32F358xx") then
        return { "", "1", "2", "3", "4",      "8",       "15", "16", "17"           }
    end
    if c_is_provided("HW_MCU_STM32F373_MD") then
        return { "",      "2", "3", "4", "5",      "12", "15", "16", "17", "19"      }
    end
    if c_is_provided("HW_MCU_STM32F378xx") then
        return { "",      "2", "3", "4", "5",      "12", "15", "16", "17", "19"      }
    end
    if c_is_provided("HW_MCU_STM32F398xx") then
        return { "", "1", "2", "3", "4",      "8",       "15", "16", "17",      "20"}
    end
    if c_is_provided("HW_MCU_STM32F401") then
        return { "", "1", "2", "3", "4", "5", "9"}
    end
    if c_is_provided("HW_MCU_STM32F411") then
        return { "", "1", "2", "3", "4", "5", "9", "10", "11"}
    end
    if c_is_provided("HW_MCU_STM32F40") then
        return { "", "1", "2", "3", "4", "5", "8", "9", "12"}
    end
    if c_is_provided("HW_MCU_STM32F42") then
        return { "", "1", "2", "3", "4", "5", "8", "9", "12"}
    end
    if c_is_provided("HW_MCU_STM32F446") then
        return { "", "1", "2", "3", "4", "5", "8", "9", "12"}
    end
    if c_is_provided("HW_MCU_STM32F7") then
        return { "", "1", "2", "3", "4", "5", "8", "9", "12"}
    end
    if c_is_provided("HW_MCU_STM32L1_MD") then
        return { "", "2", "3", "4", "9"}
    end
    if c_is_provided("HW_MCU_STM32L1_MDP") then
        return { "", "2", "3", "4", "5", "9"}
    end
    if c_is_provided("HW_MCU_STM32L1_HD") then
        return { "", "2", "3", "4", "5", "9"}
    end
    if c_is_provided("HW_MCU_STM32L1_XL") then
        return { "", "2", "3", "4", "5", "9"}
    end
end

--
-- Retrieve Timer with at least two channels and available interrupt.
--
function GetStmTimers2ChIrq()
-- List timers with at least two channels and a seperated interrupt
-- List of timers with two channels min:
-- {"", "1", "2", "3", "4", "5", "8", "9", "12", "16", "17", "19"}
-- TIM1/TIM8/TIM9/TIM12/TIM16/TIM17 with seperated/coupled interrupts on most devices
-- and so not usable yet
    if c_is_provided("HW_MCU_STM32F030x6") then
        return { "",  "3",  "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F030x8") then
        return { "",  "3",   "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F030xC") then
        return { "",  "3",   "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F031x6") then
        return { "",  "2", "3",  "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F038xx") then
        return { "",  "2", "3",  "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F042x6") then
        return { "",  "2", "3",  "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F048xx") then
        return { "",  "2", "3",  "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F051x8") then
        return { "",  "2", "3",   "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F058xx") then
        return { "",  "2", "3",   "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F070x6") then
        return { "",  "3",  "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F070xB") then
        return { "",  "3",   "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F071xB") then
        return { "",  "2", "3",   "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F072xB") then
        return { "",  "2", "3",   "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F078xx") then
        return { "",  "2", "3",   "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F091xC") then
        return { "",  "2", "3",   "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F098xx") then
        return { "",  "2", "3",   "16", "17" }
    end
    if c_is_provided("HW_MCU_STM32F10X_LD") then
        return { "", "2", "3"}
    end
    if c_is_provided("HW_MCU_STM32F10X_LD_VL") then
        return { "", "2", "3"}
    end
    if c_is_provided("HW_MCU_STM32F10X_MD") then
        return { "", "2", "3", "4"}
    end
    if c_is_provided("HW_MCU_STM32F10X_MD_VL") then
        return { "", "2", "3", "4"}
    end
    if c_is_provided("HW_MCU_STM32F10X_HD") then
        return { "", "2", "3", "4", "5"}
    end
    if c_is_provided("HW_MCU_STM32F10X_HD_VL") then
        return { "", "2", "3", "4", "5", "12"}
    end
    if c_is_provided("HW_MCU_STM32F10X_XL") then
        return { "", "2", "3", "4", "5"}
    end
    if c_is_provided("HW_MCU_STM32F10X_CL") then
        return { "", "2", "3", "4", "5"}
    end
    if c_is_provided("HW_MCU_STM32F2") then
        return { "", "2", "3", "4", "5"}
    end
    if c_is_provided("HW_MCU_STM32F301x8") then
        return { "", "2",                               }
    end
    if c_is_provided("HW_MCU_STM32F302_LD") then
        return { "", "2",                               }
    end
    if c_is_provided("HW_MCU_STM32F302_MD") then
        return { "", "2", "3", "4",                     }
    end
    if c_is_provided("HW_MCU_STM32F302_HD") then
        return { "", "2", "3", "4",                     }
    end
    if c_is_provided("HW_MCU_STM32F303_LD") then
        return { "", "2", "3",                          }
    end
    if c_is_provided("HW_MCU_STM32F303_MD") then
        return { "", "2", "3", "4",                     }
    end
    if c_is_provided("HW_MCU_STM32F303_HD") then
        return { "", "2", "3", "4",                     }
    end
    if c_is_provided("HW_MCU_STM32F318xx") then
        return { "", "2",                               }
    end
    if c_is_provided("HW_MCU_STM32F328xx") then
        return { "", "2", "3",                          }
    end
    if c_is_provided("HW_MCU_STM32F334x8") then
        return { "", "2", "3",                          }
    end
    if c_is_provided("HW_MCU_STM32F358xx") then
        return { "", "2", "3", "4",                     }
    end
    if c_is_provided("HW_MCU_STM32F373_MD") then
        return { "",      "2", "3", "4", "5", "12", "19"}
    end
    if c_is_provided("HW_MCU_STM32F378xx") then
        return { "",      "2", "3", "4", "5", "12", "19"}
    end
    if c_is_provided("HW_MCU_STM32F398xx") then
        return { "", "2", "3", "4",                     }
    end
    if c_is_provided("HW_MCU_STM32F4") then
        return { "", "2", "3", "4", "5"}
    end
    if c_is_provided("HW_MCU_STM32F7") then
        return { "", "2", "3", "4", "5"}
    end
    if c_is_provided("HW_MCU_STM32L1_MD") then
        return { "", "2", "3", "4", "9"}
    end
    if c_is_provided("HW_MCU_STM32L1_MDP") then
        return { "", "2", "3", "4", "5", "9"}
    end
    if c_is_provided("HW_MCU_STM32L1_HD") then
        return { "", "2", "3", "4", "5", "9"}
    end
    if c_is_provided("HW_MCU_STM32L1_XL") then
        return { "", "2", "3", "4", "5", "9"}
    end
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
        name = "nutarch_cm3_stm32_qenc32_0",
        brief = "STM32 32Bit Quadrature Encoder0",
        description = "STM32 32Bit Quadrature Encoder 0.",
        requires = { "HW_STM32_TIMER_32BIT" },
        sources = { "cm3/dev/stm/stm32_qenc32_0.c" },
        options =
        {
            {
                macro = "STM32_QENC32_0_TIMER_ID",
                brief = "STM32 32Bit Quadrature Encoder Timer ID",
                description = "Select Timer for 32 bit Quadrature Enoder",
                type = "enumerated",
                choices = function() return GetStmTimers32bit() end,
                file = "include/cfg/qenc.h",
            },
            {
                macro = "STM32_QENC32_0_I_PORT",
                brief = "STM32 qenc32 I input port",
                description = "STM32 32Bit Quadrature Encoder I input port. Can by TI1 or TI2",
               requires = { "HW_STM32_TIMER_32BIT" },
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                file = "include/cfg/qenc.h",
            },
            {
                macro = "STM32_QENC32_0_I_PIN",
                brief = "STM32 qenc32 I input pin",
                description = "STM32 32Bit Quadrature Encoder I input pin. Can by TI1 or TI2",
                requires = { "HW_STM32_TIMER_32BIT" },
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                file = "include/cfg/qenc.h",
            },
            {
                macro = "STM32_QENC32_0_Q_PORT",
                brief = "STM32 qenc32 Q input port",
                description = "STM32 32Bit Quadrature Encoder I input port. Can by TI1 or TI2",
                requires = { "HW_STM32_TIMER_32BIT" },
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                file = "include/cfg/qenc.h",
            },
            {
                macro = "STM32_QENC32_0_Q_PIN",
                brief = "STM32 qenc32 Q input pin",
                description = "STM32 32Bit Quadrature Encoder I input pin. Can by TI1 or TI2",
                requires = { "HW_STM32_TIMER_32BIT" },
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                file = "include/cfg/qenc.h",
            },
            {
                macro = "STM32_QENC32_0_INVERT",
                brief = "STM32 qenc32 reverse count direction",
                description = "STM32 32Bit Quadrature Encoder reverse count direction. Effective exchanges I and Q.",
                flavor = "booldata",
                file = "include/cfg/qenc.h",
            },
        },
    },
     --
    -- STM32 Timers Helper Functions
    --
    {
        name = "nutarch_cm3_stm32_timer_helper",
        brief = "STM32 Timer Helper functions",
        description = "STM32 Timer helper functions.",
        provides = { "HW_TIM_STM32_HELPER" },
        sources = { "cm3/dev/stm/stm32_timer.c" },
    },
     --
    -- STM32 16bit Timers for Encode
    --
    {
        name = "nutarch_cm3_stm32_qenc16_0",
        brief = "STM32 16 Bit Quadrature Encoder0.",
        description = "STM32 16 Bit Quadrature Encoder 0.",
        sources = { "cm3/dev/stm/stm32_qenc16_0.c" },
        options =
        {
            {
                macro = "STM32_QENC16_0_TIMER_ID",
                brief = "STM32 16 Bit Quadrature Encoder Timer ID",
                description = "Select Timer for 16 bit Quadrature Enoder",
                type = "enumerated",
                choices = function() return GetStmTimers2Ch() end,
                file = "include/cfg/qenc.h",
            },
            {
                macro = "STM32_QENC16_0_I_PORT",
                brief = "STM32 qenc16 I input port",
                description = "STM32 16 Bit Quadrature Encoder I input port. Can by TI1 or TI2",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                file = "include/cfg/qenc.h",
            },
            {
                macro = "STM32_QENC16_0_I_PIN",
                brief = "STM32 qenc16 I input pin",
                description = "STM32 16 Bit Quadrature Encoder I input pin. Can by TI1 or TI2",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                file = "include/cfg/qenc.h",
            },
            {
                macro = "STM32_QENC16_0_Q_PORT",
                brief = "STM32 qenc16 Q input port",
                description = "STM32 16 Bit Quadrature Encoder I input port. Can by TI1 or TI2",
                type = "enumerated",
                choices = function() return GetGpioBanks() end,
                file = "include/cfg/qenc.h",
            },
            {
                macro = "STM32_QENC16_0_Q_PIN",
                brief = "STM32 qenc16 Q input pin",
                description = "STM32 16 Bit Quadrature Encoder I input pin. Can by TI1 or TI2",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                file = "include/cfg/qenc.h",
            },
            {
                macro = "STM32_QENC16_0_INVERT",
                brief = "STM32 qenc16 reverse count direction",
                description = "STM32 16 Bit Quadrature Encoder reverse count direction. Effective exchanges I and Q.",
                flavor = "booldata",
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
        sources = { "cm3/dev/stm/stm32_pwm_0.c" },
        options =
        {
            {
                macro = "STM32_PWM0_TIMER_ID",
                brief = "STM32 32Bit PWM0 Timer ID",
                description = "Select Timer for PWM0 output. Check for availability on selected device.",
                type = "enumerated",
                choices = function() return  GetStmTimers1Ch() end,
                file = "include/cfg/pwm.h",
            },
            {
                macro = "STM32_PWM0_TIMER_CHANNEL",
                brief = "STM32 32Bit PWM0 Timer Channel",
                description = "Select Timer Channel for PWM0 output. Check for availability on selected device.",
                type = "enumerated",
                choices = { "1", "2", "3", "4" },
                file = "include/cfg/pwm.h",
            },
            {
                macro = "STM32_PWM0_PIN",
                brief = "PIN for STM32 PWM0 output",
                description = "Port bit used for PWM0. Only some pins are valid!",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                file = "include/cfg/pwm.h",
            },
            {
                macro = "STM32_PWM0_PORT",
                brief = "PORT for STM32 PWM0 output",
                description = "Port used for PWM0. Only some ports are valid!",
                type = "enumerated",
                choices = function() return GetGpioPortIds() end,
                file = "include/cfg/pwm.h",
            },
        }
    },
    {
        name = "nutarch_cm3_stm32_pwm1",
        brief = "STM32 PWM Output 1",
        description = "STM32 PWM_Output 1.",
        sources = { "cm3/dev/stm/stm32_pwm_1.c" },
        options =
        {
            {
                macro = "STM32_PWM1_TIMER_ID",
                brief = "STM32 32Bit PWM1 Timer ID",
                description = "Select Timer for PWM1 output. Check for availability on selected device.",
                type = "enumerated",
                choices = function() return  GetStmTimers1Ch() end,
                file = "include/cfg/pwm.h",
            },
            {
                macro = "STM32_PWM1_TIMER_CHANNEL",
                brief = "STM32 32Bit PWM1 Timer Channel",
                description = "Select Timer Channel for PWM1 output. Check for availability on selected device.",
                type = "enumerated",
                choices = { "1", "2", "3", "4" },
                file = "include/cfg/pwm.h",
            },
            {
                macro = "STM32_PWM1_PIN",
                brief = "PIN for STM32 PWM1 output",
                description = "Port bit used for PWM1. Only some pins are valid!",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                file = "include/cfg/pwm.h",
            },
            {
                macro = "STM32_PWM1_PORT",
                brief = "PORT for STM32 PWM1 output",
                description = "Port used for PWM1. Only some ports are valid!",
                type = "enumerated",
                choices = function() return GetGpioPortIds() end,
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
        description = "STM32 OWIBUS0 with hardware timer, "..
                    "library compile time configured",
        provides = { "OWIBUS" },
        sources = { "cm3/dev/stm/owibus0stm32tim.c" },
        options =
        {
            {
                macro = "STM32TIM_OWI0_PIN",
                brief = "PIN for STM32 hardware timer OWIBUS0",
                description = "Port bit used for OWIBUS0. "..
                            "Check datasheet for Port/Pin function!",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                file = "include/cfg/owi.h",
            },
            {
                macro = "STM32TIM_OWI0_PORT",
                brief = "PORT for STM32 hardware timer OWIBUS0",
                description = "Port used for OWIBUS0. "..
                            "Check datasheet for Port/Pin function!",
                type = "enumerated",
                choices = function() return GetGpioPortIds() end,
                file = "include/cfg/owi.h",
            },
            {
                macro = "STM32TIM_OWI0_TIMER_ID",
                brief = "Timer for STM32 hardware timer OWIBUS0",
                description = "Timer used for OWIBUS0. "..
                            "Requires at least dual channel!",
                type = "enumerated",
                choices = function() return GetStmTimers2ChIrq() end,
                file = "include/cfg/owi.h",
            },
            {
                macro = "STM32TIM_OWI0_CHANNEL",
                brief = "Channel for STM32 hardware timer OWIBUS0",
                description = "Channel for STM32 hardware timer OWIBUS0. "..
                            "Check for Channel 3/4 availability!",
                type = "enumerated",
                choices = { "1", "2", "3", "4" },
                file = "include/cfg/owi.h",
            },
            {
                macro = "STM32TIM_OWI0_REMAP",
                brief = "REMAP for STM32 hardware timer OWIBUS0",
                requires = { "HW_MCU_STM32F10X" },
                description = "Give Remap used for STM32F1 hardware "..
                            "timer OWIBUS0 if needed!",
                type = "interger",
                default = "0",
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
        sources = { "cm3/dev/stm/owibus1stm32tim.c" },
        options =
        {
            {
                macro = "STM32TIM_OWI1_PIN",
                brief = "PIN for STM32 hardware timer OWIBUS1",
                description = "Port bit used for OWIBUS1. "..
                            "Check datasheet for Port/Pin function!",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                file = "include/cfg/owi.h",
            },
            {
                macro = "STM32TIM_OWI1_PORT",
                brief = "PORT for STM32 hardware timer OWIBUS1",
                description = "Port used for OWIBUS1. "..
                            "Check datasheet for Port/Pin function!",
                type = "enumerated",
                choices = function() return GetGpioPortIds() end,
                file = "include/cfg/owi.h",
            },
            {
                macro = "STM32TIM_OWI1_TIMER_ID",
                brief = "Timer for STM32 hardware timer OWIBUS1",
                description = "Timer used for OWIBUS1. "..
                            "Requires at least dual channel!",
                type = "enumerated",
                choices = function() return GetStmTimers2ChIrq() end,
                file = "include/cfg/owi.h",
            },
            {
                macro = "STM32TIM_OWI1_CHANNEL",
                brief = "Channel for STM32 hardware timer OWIBUS1",
                description = "Channel for STM32 hardware timer OWIBUS1. "..
                            "Check for Channel 3/4 availability!",
                type = "enumerated",
                choices = { "1", "2", "3", "4" },
                file = "include/cfg/owi.h",
            },
            {
                macro = "STM32TIM_OWI1_REMAP",
                brief = "REMAP for STM32 hardware timer OWIBUS1",
                requires = { "HW_MCU_STM32F10X" },
                description = "Give Remap used for STM32F1 hardware "..
                            "timer OWIBUS1 if needed!",
                type = "interger",
                default = "0",
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
        provides = { "OWIBUS" },
        sources = { "cm3/dev/stm/owibus2stm32tim.c" },
        options =
        {
            {
                macro = "STM32TIM_OWI2_PIN",
                brief = "PIN for STM32 hardware timer OWIBUS2",
                description = "Port bit used for OWIBUS2. "..
                            "Check datasheet for Port/Pin function!",
                type = "enumerated",
                choices = function() return GetGpioBits() end,
                file = "include/cfg/owi.h",
            },
            {
                macro = "STM32TIM_OWI2_PORT",
                brief = "PORT for STM32 hardware timer OWIBUS2",
                description = "Port used for OWIBUS2. "..
                            "Check datasheet for Port/Pin function!",
                type = "enumerated",
                choices = function() return GetGpioPortIds() end,
                file = "include/cfg/owi.h",
            },
            {
                macro = "STM32TIM_OWI2_TIMER_ID",
                brief = "Timer for STM32 hardware timer OWIBUS2",
                description = "Timer used for OWIBUS2. "..
                            "Requires at least dual channel!",
                type = "enumerated",
                choices = function() return GetStmTimers2ChIrq() end,
                file = "include/cfg/owi.h",
            },
            {
                macro = "STM32TIM_OWI2_CHANNEL",
                brief = "Channel for STM32 hardware timer OWIBUS2",
                description = "Channel for STM32 hardware timer OWIBUS2. "..
                            "Check for Channel 3/4 availability!",
                type = "enumerated",
                choices = { "1", "2", "3", "4" },
                file = "include/cfg/owi.h",
            },
            {
                macro = "STM32TIM_OWI2_REMAP",
                brief = "REMAP for STM32 hardware timer OWIBUS2",
                requires = { "HW_MCU_STM32F10X" },
                description = "Give Remap used for STM32F1 hardware"..
                            " timer OWIBUS2 if needed!",
                type = "interger",
                default = "0",
                file = "include/cfg/owi.h",
            },
        }
     },
}
