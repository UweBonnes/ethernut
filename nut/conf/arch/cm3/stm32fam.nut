--
-- Copyright (C) 2004-2007 by egnite Software GmbH. All rights reserved.
-- Copyright (C) 2011-2018 Uwe Bonnes
--                         (bon@elektron.ikp.physik.tu-darmstadt.de)
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
-- - vendor family (e.g MCU_STM32F4, "serie" in stmcufinder notation)
-- - vendor subfamily (e.g.MCU_STM32F42, "line in stmcufinder notation)
-- -     if applicable
-- - and the device (MCU_STM32F429).
-- Stm32f4.nut adds the devices in all F4,
-- devices in common in F427/429/437/439 and finally the specific
-- devices in STM32F420.
-- All internal defines start in MCU_, only the final clause for the STM32F429
-- defines "STM32F429xx", as this is the same define as used by STM Cube
-- to include the vendor header.

dofile(c_repo_path().."/arch/cm3/generated/stm32pinlist.nut")

function GetF1Files()
   local a =
      {
         "cm3/dev/stm/stm32f1_rtc.c",
         "cm3/dev/stm/stm32f1_backup.c",
         "cm3/dev/stm/stm32f1_3_flash.c",
         "cm3/dev/stm/stm32_gpio_v1.c",
      }
   if c_is_provided("HW_MCU_STM32F100") then
      table.insert(a, "cm3/dev/stm/stm32f30_clk.c")
   else
      table.insert(a, "cm3/dev/stm/stm32f1_clk.c")
   end
   return a
end


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
        sources = {
           "cm3/dev/stm/stm32f30_clk.c",
           "cm3/dev/stm/stm32f1_3_flash.c",
        },
        makedefs = {"MCU=cortex-m0"},
        script = "arch/cm3/generated/stm32f0.nut",
        options =
        {
            {
                macro = "MCU_STM32F0",
                brief = "STM32F0",
                description = "Devices available in all STM32F0 devices",
                default = 1,
                provides = {
                    "HW_EEPROM_EMUL_STM32",
                    "HW_HSI8_STM32F",
                    "HW_RTC_STM32_V2",
                    "HW_GPIO_STM32V2",
                    "HW_I2C_STM32V2",
                    "HW_USART_STM32V2",
                },
                file = "include/cfg/arch.h"
            },
            {
                macro = "MCU_CM_NO_BITBAND",
                brief = "No Bitbanding",
                description = "Selected family has no bitband support",
                default = 1,
                file = "include/cfg/arch.h"
            },
            {
                macro = "MCU_STM32F09",
                brief = "STM32F09",
                description = "DMA on STM32F09 differs.",
                requires = {"HW_MCU_STM32F09"},
                type = "integer",
                default = 1,
                file = "include/cfg/arch.h"
            },
        }
    },
    {
        name = "nutarch_cm3_stm32f1",
        brief = "STM32F1",
        requires = { "HW_MCU_STM32", "HW_MCU_STM32F1" },
        description = "ST Microelectronics STM32 F1 Series",
        sources = function() return GetF1Files() end,
        makedefs = {
            "MCU=cortex-m3",
            "MFIX=-mfix-cortex-m3-ldrd",
        },
        script = "arch/cm3/generated/stm32f1.nut",
        options =
        {
            {
                macro = "MCU_STM32F1",
                brief = "STM32F1",
                description = "STM32F1 family device.",
                default = 1,
                provides = {
                   "HW_EEPROM_EMUL_STM32",
                   "HW_GPIO_STM32V1",
                   "HW_DMA1_STM32F1",
                   "HW_I2C_STM32V1",
                },
                file = "include/cfg/arch.h"
            },
            {
                macro = "MCU_STM32F101",
                brief = "STM32F101 Series",
                description = "STM32F101 variants, running at 24 MHz.",
                requires = {"HW_MCU_STM32F101"},
                default = 1,
                file = "include/cfg/arch.h"
            },
            {
                macro = "MCU_STM32F102",
                brief = "STM32F102 Series",
                description = "STM32F102 variants, running at 36 MHz..",
                requires = {"HW_MCU_STM32F102"},
                default = 1,
                file = "include/cfg/arch.h"
            },
        }
    },
    {
        name = "nutarch_cm3_stm32l0",
        brief = "STM32L0",
        requires = { "HW_MCU_STM32", "HW_MCU_STM32L0" },
        description = "ST Microelectronics STM32 L0 Series",
        sources = {
           "cm3/dev/stm/stm32l1_clk.c",
           "cm3/dev/stm/stm32l1_eeprom.c",
           "cm3/dev/stm/stm32l1_flash.c",
        },
        makedefs = {"MCU=cortex-m0plus"},
        script = "arch/cm3/generated/stm32l0.nut",
        options =
        {
            {
                macro = "MCU_CM_NO_BITBAND",
                brief = "No Bitbanding",
                description = "Selected family has no bitband support",
                default = 1,
                file = "include/cfg/arch.h"
            },
            {
                macro = "MCU_STM32L0",
                brief = "STM32L0 common devices",
                description = "Provides devices common in all STM32L0 devices.",
                default = 1,
                provides =
                {
                    "HW_DMA1_STM32F1",
                    "HW_DMA_COMBINED_IRQ_STM32",
                    "HW_DMA_CSELR_STM32",
                    "HW_EEPROM_STM32",
                    "HW_RCC_STM32L",
-- Todo: FLASH_WRPROT
                    "HW_VOS_STM32",
                    "HW_RTC_STM32_V2",
                    "HW_GPIO_STM32L_0_1",
                    "HW_GPIO_STM32V2",
                    "HW_USART_STM32V2",
                    "HW_DMA1_STM32L0",
                    "HW_I2C_STM32V2",
                },
                file = "include/cfg/arch.h",
            },
        }
    },
    {
        name = "nutarch_cm3_stm32l4",
        brief = "STM32L4",
        requires = { "HW_MCU_STM32", "HW_MCU_STM32L4" },
        description = "ST Microelectronics STM32 L4 Series",
        sources = {
            "cm3/dev/stm/stm32l4_clk.c",
            "cm3/dev/stm/stm32l4_flash.c",
        },
        makedefs = {"MCU=cortex-m4"},
        script = "arch/cm3/generated/stm32l4.nut",
        options =
        {
            {
                macro = "MCU_STM32L4",
                brief = "STM32L4 common devices",
                description = "Provides devices common in all STM32L4 devices.",
                default = 1,
                provides =
                {
                   "HW_DMA2_STM32F1",
                   "HW_DMA2_7CH_STM32",
                   "HW_RCC_STM32L",
                   "HW_DMA_CSELR_STM32",
                   "HW_EEPROM_EMUL_STM32",
-- Todo: FLASH_WRPROT
                    "HW_VOS_STM32",
                    "HW_ICACHE_STM32",
                    "HW_DCACHE_STM32",
--                   "HW_CAN1_STM32",  TODO: Can Timing
                    "HW_RTC_STM32_V2",
                    "HW_GPIO_STM32V2",
                    "HW_DMA1_STM32F1",
                    "HW_DMA2_STM32L4",
                    "HW_I2C_STM32V2",
                    "HW_USART_STM32V2",
                },
                file = "include/cfg/arch.h",
            },
            {
                macro = "MCU_STM32L43",
                brief = "STM32L43x/44x.",
                description = "Provides common devices in L43x/L44x",
                default = 1,
                requires = {"HW_MCU_STM32L43"},
                file = "include/cfg/arch.h",
            },
            {
                macro = "MCU_STM32L45",
                brief = "STM32L45x/46x with DFSDM.",
                description = "Provides common devices in L45x/L46x",
                default = 1,
                requires = {"HW_MCU_STM32L45"},
                file = "include/cfg/arch.h",
            },
            {
                macro = "MCU_STM32L47",
                brief = "STM32L47x/48x.",
                description = "Provides common devices in L47x/L48x",
                default = 1,
                requires = {"HW_MCU_STM32L47"},
                file = "include/cfg/arch.h",
            },
            {
                macro = "MCU_STM32L4R",
                brief = "STM32L4Rx/4Sx.",
                description = "Provides common features for L4Rx/L4Sx",
                default = 1,
                requires = {"HW_MCU_STM32L4R"},
                provides = {"HW_OVERDRIVE_STM32"},
                file = "include/cfg/arch.h",
            },
         }
    },
    {
        name = "nutarch_cm3_stm32g4",
        brief = "STM32G4",
        requires = { "HW_MCU_STM32G4" },
        description = "ST Microelectronics STM32 G4 Series",
        sources = {
            "cm3/dev/stm/stm32l4_clk.c",
            "cm3/dev/stm/stm32l4_flash.c",
        },
        makedefs = {"MCU=cortex-m4"},
        script = "arch/cm3/generated/stm32g4.nut",
        options =
        {
            {
                macro = "MCU_STM32G4",
                brief = "STM32G4 common devices",
                description = "Provides devices common in all STM32G4 devices.",
                default = 1,
                provides =
                {
                   "HW_DMA2_STM32F1",
                   "HW_DMA2_7CH_STM32",
                   "HW_RCC_STM32L",
                   "HW_EEPROM_EMUL_STM32",
-- Todo: FLASH_WRPROT
                    "HW_VOS_STM32",
                    "HW_ICACHE_STM32",
                    "HW_DCACHE_STM32",
--                   "HW_CAN1_STM32",  TODO: Can Timing
                    "HW_RTC_STM32_V2",
                    "HW_GPIO_STM32V2",
                    "HW_DMA1_STM32F1",
                    "HW_DMA2_STM32L4",
                    "HW_I2C_STM32V2",
                    "HW_USART_STM32V2",
                },
                file = "include/cfg/arch.h",
            },
            {
                macro = "MCU_STM32G43",
                brief = "STM32G43x/44x.",
                description = "Provides common devices in G43x/G44x",
                default = 1,
                requires = {"HW_MCU_STM32G43"},
                file = "include/cfg/arch.h",
                makedefs = {"RAM0_LENGTH=16k", "RAM1_LENGTH=6K", "RAM2_LENGHT = 10k"}
            },
            {
                macro = "MCU_STM32G47",
                brief = "STM32G47x/48x.",
                description = "Provides common devices in G47x/G48x",
                default = 1,
                requires = {"HW_MCU_STM32G47"},
                file = "include/cfg/arch.h",
                makedefs = {"RAM0_LENGTH=80k", "RAM1_LENGTH=16K", "RAM2_LENGHT = 32k"}
            },
         }
    },
    {
        name = "nutarch_cm3_stm32l1",
        brief = "STM32L1",
        requires = { "HW_MCU_STM32", "HW_MCU_STM32L1" },
        description = "ST Microelectronics STM32 L1 Series",
        sources = {
           "cm3/dev/stm/stm32l1_clk.c",
           "cm3/dev/stm/stm32l1_eeprom.c",
           "cm3/dev/stm/stm32l1_flash.c",
        },
        makedefs = {
                  "MCU=cortex-m3",
                  "MFIX=-mfix-cortex-m3-ldrd",
        },
        script = "arch/cm3/generated/stm32l1.nut",
        options =
        {
            {
                macro = "MCU_STM32L1",
                brief = "Provides devices available in all STM32L1 devices",
                default = 1,
                provides =
                {
                    "HW_EEPROM_STM32",
                    "HW_VOS_STM32",
                    "HW_RCC_STM32L",
                    "HW_GPIO_STM32L_0_1",
                    "HW_GPIO_STM32V2",
                    "HW_DMA1_STM32F1",
                    "HW_I2C_STM32V1",
                    "HW_RTC_STM32_V2",
                },
                file = "include/cfg/arch.h",
            },
            {
                macro = "MCU_STM32L1_CAT1",
                brief = "STM32L1 Cat1.",
                description = "Additional devices in STM32L1 Cat 1 Devices.",
                default = 1,
                requires = { "HW_MCU_STM32_DIE_416" },
                file = "include/cfg/arch.h",
            },
        }
    },
    {
        name = "nutarch_cm3_stm32f2",
        brief = "STM32F2",
        requires = { "HW_MCU_STM32", "HW_MCU_STM32F2" },
        description = "ST Microelectronics STM32 F2 Series",
        sources = { "cm3/dev/stm/stm32f7_clk.c" },
        makedefs = { "MCU=cortex-m3"},
        script = "arch/cm3/generated/stm32f2.nut",
        options =
        {
            {
                macro = "MCU_STM32F2",
                brief = "STM32F2",
                description = "Common devices in STM32F2 family.",
                default = 1,
                provides =
                {
                    "HW_EEPROM_EMUL_STM32",
                    "HW_ICACHE_STM32",
                    "HW_DCACHE_STM32",
                    "HW_FLASH_STM32F2_4",
                    "HW_GPIO_STM32V2",
                    "HW_DMA_STM32F2",
                    "HW_I2C_STM32V1",
                    "HW_RTC_STM32_V2",
                },
                file = "include/cfg/arch.h"
            },
        }
     },
     {
        name = "nutarch_cm3_stm32f3",
        brief = "STM32F3",
        requires = { "HW_MCU_STM32", "HW_MCU_STM32F3" },
        description = "ST Microelectronics STM32 F3 Series",
        sources = {
            "cm3/dev/stm/stm32f30_clk.c",
            "cm3/dev/stm/stm32f1_3_flash.c",
        },
        makedefs = { "MCU=cortex-m4", "FLASH_PAGE_SIZE=2048"},
        script = "arch/cm3/generated/stm32f3.nut",
        options =
        {
            {
               macro = "MCU_STM32F3",
               brief = "STM32F3 family",
               description = "Common devices in all STM32F3",
               default = 1,
               provides =
               {
                   "HW_EEPROM_EMUL_STM32",
                   "HW_GPIO_STM32V2",
                   "HW_DMA1_STM32F1",
                   "HW_I2C_STM32V2",
                   "HW_USART_STM32V2",
                   "HW_RTC_STM32_V2",
                   "HW_MCU_FPU",
               },
               file = "include/cfg/arch.h"
            },
            {
                macro = "MCU_STM32F30",
                brief = "STM32F30 family",
                requires = {"HW_MCU_STM32F30"},
                default = 1,
                file = "include/cfg/arch.h"
            },
            {
                macro = "MCU_STM32F37",
                brief = "STM32F37x family",
                description = "STM32F37x subfamily",
                requires = { "HW_MCU_STM32F37" },
                default = 1,
                provides = {"HW_HSI8_DIV2_STM32"},
            },
            {
                macro = "STM32F3_USB_REMAP",
                brief = "Separate CAN/USB IRQ",
                description = "Separate CAN/USB interrupts i startup",
                requires = {HW_USBWAKEUP_RMP_STM32},
                default = 1,
                provides = {"HW_CAN_STM32"},
                file = "include/cfg/arch.h"
            },
        }
    },
    {
        name = "nutarch_cm3_stm32f4",
        brief = "STM32F4",
        requires = { "HW_MCU_STM32", "HW_MCU_STM32F4" },
        description = "ST Microelectronics STM32 F4 Series",
        sources = { "cm3/dev/stm/stm32f7_clk.c" },
        makedefs = { "MCU=cortex-m4"},
        script = "arch/cm3/generated/stm32f4.nut",
        options =
        {
            {
                macro = "MCU_STM32F4",
                brief = "STM32F4",
                description = "Devices available in all STM32F4xx devices",
                default = 1,
                provides =
                {
                    "HW_EEPROM_EMUL_STM32",
                    "HW_VOS_STM32",
                    "HW_ICACHE_STM32",
                    "HW_DCACHE_STM32",
                    "HW_MCU_FPU",
                    "HW_FLASH_STM32F2_4",
                    "HW_GPIO_STM32V2",
                    "HW_DMA_STM32F2",
                    "HW_I2C_STM32V1",
                    "HW_RTC_STM32_V2",
                 },
                file = "include/cfg/arch.h"
             },
             {
                 macro = "MCU_STM32F401",
                 brief = "STM32F401",
                 description = "STM32F401.",
                 requires = {"HW_MCU_STM32F401" },
                 default = 1,
                 file = "include/cfg/arch.h"
             },
             {
                 macro = "MCU_STM32F40",
                 brief = "STM32F405/407/415/417",
                 description = "Subfamily for STM32F405/407/415/417.",
                 requires = { "HW_MCU_STM32F40" },
                 default = 1,
                 file = "include/cfg/arch.h"
             },
             {
                 macro = "MCU_STM32F41",
                 brief = "STM32F41[0-3] Access line 100 MHz",
                 description = "Subfamily for STM32F411/412/413",
                 requires = { "HW_MCU_STM32F41" },
                 default = 1,
                 file = "include/cfg/arch.h"
             },
             {
                 macro = "MCU_STM32F42",
                 brief = "STM32F427/29/37/39",
                 description = "Subfamily STM32F427/29/37/39.",
                 requires = { "HW_MCU_STM32F42" },
                 default = 1,
                 file = "include/cfg/arch.h"
             },
             {
                 macro = "MCU_STM32F46",
                 brief = "STM32F469/79",
                 description = "Subfamily STM32F469/79.",
                 requires = { "HW_MCU_STM32F46" },
                 default = 1,
                 file = "include/cfg/arch.h"
             },
        }
    },
    {
        name = "nutarch_cm3_stm32f7",
        brief = "STM32F7",
        requires = { "HW_MCU_STM32", "HW_MCU_STM32F7" },
        description = "ST Microelectronics STM32 F4 Series",
        sources = { "cm3/dev/stm/stm32f7_clk.c" },
        makedefs = { "MCU=cortex-m7"},
        script = "arch/cm3/generated/stm32f7.nut",
        options =
        {
            {
                macro = "MCU_CM_NO_BITBAND",
                brief = "No Bitbanding",
                description = "Selected family has no bitband support",
                default = 1,
                file = "include/cfg/arch.h"
            },
            {
                macro = "MCU_STM32F7",
                brief = "STM32F7 Devices",
                description = "Provide devices available in all STM32F7xx devices",
                default = 1,
                provides =
                {
                    "HW_EEPROM_EMUL_STM32",
                    "HW_MCU_CM7",
                    "HW_VOS_STM32",
                    "HW_OVERDRIVE_STM32",
                    "HW_FLASH_STM32F2_4",
                    "HW_GPIO_STM32V2",
                    "HW_DMA_STM32F2",
                    "HW_I2C_STM32V2",
                    "HW_USART_STM32V2",
                },
                file = "include/cfg/arch.h",
                makedefs = {"ITCM_LENGTH=16k", "RAM2_LENGTH=16K"}
            },
            {
                macro = "MCU_STM32F72",
                brief = "STM32F72 family, 256/512, 256 kB Ram",
                description = "STM32F72 family with single precision FPU.",
                requires = {"HW_MCU_STM32F72"},
                default = 1,
                file = "include/cfg/arch.h",
                makedefs = {"RAM0_LENGTH=64k", "RAM1_LENGTH=176K", "RAM2_LENGHT = 16k"}
            },
            {
                macro = "MCU_STM32F74",
                brief = "STM32F745 family, 512/1024 kB flash, 320 kB Ram",
                description = "STM32F74 family with single precision FPU.",
                requires = {"HW_MCU_STM32F74"},
                default = 1,
                file = "include/cfg/arch.h",
                makedefs = {"RAM0_LENGTH=64k", "RAM1_LENGTH=240K"}
             },
             {
                macro = "MCU_STM32F76",
                brief = "STM32F76 family 1024/2048 KB flash, 512 kB ram",
                description = "STM32F76 family with double precision FPU.",
                requires = { "HW_MCU_STM32F76" },
                default = 1,
                file = "include/cfg/arch.h",
                makedefs = {"RAM0_LENGTH=128k", "RAM1_LENGTH=368K"}
             },
         }
    },
    {
        name = "nutarch_cm3_stm32h7",
        brief = "STM32H7",
        requires = {"HW_MCU_STM32H7"},
        description = "ST Microelectronics STM32 H7 Series",
        sources = {
            "cm3/dev/stm/stm32h7_clk.c",
            "cm3/dev/stm/stm32h7_flash.c",
        },
        makedefs = {"MCU=cortex-m7"},
        script = "arch/cm3/generated/stm32h7.nut",
        options =
        {
            {
                macro = "MCU_CM_NO_BITBAND",
                brief = "No Bitbanding",
                description = "Selected family has no bitband support",
                default = 1,
                file = "include/cfg/arch.h"
            },
            {
                macro = "MCU_STM32H7",
                brief = "STM32H7 Devices",
                description = "Provide devices available in all STM32F7xx devices",
                default = 1,
                provides =
                {
                    "HW_EEPROM_EMUL_STM32",
                    "HW_MCU_CM7",
                    "HW_VOS_STM32",
                    "HW_OVERDRIVE_STM32",
                    "HW_GPIO_STM32V2",
                    "HW_I2C_STM32V2",
                    "HW_USART_STM32V2",
                },
                file = "include/cfg/arch.h",
                makedefs = {"ITCM_LENGTH=64k", "RAM0_LENGTH=128K"}
            },
        },
    },
}

