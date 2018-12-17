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
-- Retrieve U(S)ARTs available on the device.
--
function GetStmUarts()
    local a = {}
    if c_is_provided("HW_UART1_STM32") then
        table.insert (a, "UART1")
    end
    if c_is_provided("HW_UART2_STM32") then
        table.insert (a, "UART2")
    end
    if c_is_provided("HW_UART3_STM32") then
        table.insert (a, "UART3")
    end
    if c_is_provided("HW_UART4_STM32") then
        table.insert (a, "UART4")
    end
    if c_is_provided("HW_UART5_STM32") then
        table.insert (a, "UART5")
    end
    if c_is_provided("HW_UART6_STM32") then
        table.insert (a, "UART6")
    end
    if c_is_provided("HW_UART7_STM32") then
        table.insert (a, "UART7")
    end
    if c_is_provided("HW_UART8_STM32") then
        table.insert (a, "UART8")
    end
    if c_is_provided("HW_UART9_STM32") then
        table.insert (a, "UART9")
    end
    if c_is_provided("HW_UART10_STM32") then
        table.insert (a, "UART10")
    end
    if c_is_provided("HW_LPUART1_STM32") then
        table.insert (a, "LPUART1")
    end
    return a
end

-- UART1 TX DMA
function GetTxDmaUart1()
    if c_is_provided("HW_DMA_STM32F2") then
        return {"DMA_NONE",
                "(DMA_CONTROL2 | DMA_STREAM7 | DMA_CHANNEL4)"}
    elseif c_is_provided("HW_MCU_STM32L4") then
        return {"DMA_NONE", "DMA1_CH4", "DMA2_CH6"}
    elseif c_is_provided("HW_DMA1_STM32F0") then
        return {"DMA_NONE", "DMA1_CH2", "DMA2_CH4"}
    elseif c_is_provided("HW_DMA1_STM32F07") then
        return {"DMA_NONE", "DMA1_CH2", "DMA2_CH4"}
    elseif c_is_provided("HW_DMA1_2_STM32F09") then
        return {"DMA_NONE",
                "DMA1_CH2", "DMA1_CH4", "DMA1_CH7",
                "DMA2_CH1", "DMA2_CH4", "DMA2_CH5"}
    elseif c_is_provided("HW_STM32L0") then
        return {"DMA_NONE", "DMA1_CH2", "DMA1_CH4"}
    else
        return {"DMA_NONE", "DMA1_CH4"}
    end
end

-- UART1 DMA RX
function GetRxDmaUart1()
    if c_is_provided("HW_DMA_STM32F2") then
        return {"DMA_NONE",
                "(DMA_CONTROL2 | DMA_STREAM2 | DMA_CHANNEL4)",
                "(DMA_CONTROL2 | DMA_STREAM5 | DMA_CHANNEL4)"}
    elseif c_is_provided("HW_MCU_STM32L4") then
        return {"DMA_NONE", "DMA1_CH5", "DMA2_CH5"}
    elseif c_is_provided("HW_DMA1_STM32F0") then
        return {"DMA_NONE", "DMA1_CH3", "DMA2_CH5"}
    elseif c_is_provided("HW_DMA1_STM32F07") then
        return {"DMA_NONE", "DMA1_CH3", "DMA2_CH5"}
    elseif c_is_provided("HW_DMA1_2_STM32F09") then
        return {"DMA_NONE",
                "DMA1_CH1", "DMA1_CH3", "DMA1_CH5", "DMA1_CH6",
                "DMA2_CH2", "DMA2_CH3"}
    elseif c_is_provided("HW_STM32L0") then
        return {"DMA_NONE", "DMA1_CH3", "DMA1_CH5"}
    else
        return {"DMA_NONE", "DMA1_CH2"}
    end
end

-- UART2 TX DMA
function GetTxDmaUart2()
    if c_is_provided("HW_DMA_STM32F2") then
        return {"DMA_NONE",
                "(DMA_CONTROL1 | DMA_STREAM6 | DMA_CHANNEL4)"}
    elseif c_is_provided("HW_DMA1_STM32F0") then
        return {"DMA_NONE", "DMA1_CH4"}
    elseif c_is_provided("HW_DMA1_STM32F0") then
        return {"DMA_NONE", "DMA1_CH4", "DMA2_CH2"}
    elseif c_is_provided("HW_DMA1_STM32F07") then
        return {"DMA_NONE", "DMA1_CH4", "DMA2_CH7"}
    elseif c_is_provided("HW_DMA1_2_STM32F09") then
        return {"DMA_NONE",
                "DMA1_CH2", "DMA1_CH4", "DMA1_CH7",
                "DMA2_CH1", "DMA2_CH4", "DMA2_CH5"}
    elseif c_is_provided("HW_STM32L0") then
        return {"DMA_NONE", "DMA1_CH4", "DMA1_CH6", "DMA1_CH2"}
    else
        return {"DMA_NONE", "DMA1_CH7"}
    end
end
-- UART2 DMA RX
function GetRxDmaUart2s()
    if c_is_provided("HW_DMA_STM32F2") then
        return {"DMA_NONE",
                "(DMA_CONTROL1 | DMA_STREAM5 | DMA_CHANNEL4)",
                "(DMA_CONTROL1 | DMA_STREAM7 | DMA_CHANNEL6)"}
    elseif c_is_provided("HW_DMA1_STM32F0") then
        return {"DMA_NONE", "DMA1_CH5"}
    elseif c_is_provided("HW_DMA1_STM32F0") then
        return {"DMA_NONE", "DMA1_CH5", "DMA2_CH3"}
    elseif c_is_provided("HW_DMA1_STM32F07") then
        return {"DMA_NONE", "DMA1_CH5", "DMA2_CH6"}
    elseif c_is_provided("HW_DMA1_2_STM32F09") then
        return {"DMA_NONE",
                "DMA1_CH1", "DMA1_CH3", "DMA1_CH5", "DMA1_CH6",
                "DMA2_CH2", "DMA2_CH3"}
    elseif c_is_provided("HW_STM32L0") then
        return {"DMA_NONE", "DMA1_CH3", "DMA1_CH7", "DMA1_CH1"}
    else
        return {"DMA_NONE", "DMA1_CH6"}
    end
end

-- UART3 TX DMA
function GetTxDmaUart3()
    if c_is_provided("HW_DMA_STM32F2") then
        return {"DMA_NONE",
                "(DMA_CONTROL1 | DMA_STREAM3 | DMA_CHANNEL4)",
                "(DMA_CONTROL1 | DMA_STREAM4 | DMA_CHANNEL7)"}
    elseif c_is_provided("HW_DMA1_STM32F07") then
        return {"DMA_NONE", "DMA1_CH7", "DMA2_CH2"}
    elseif c_is_provided("HW_DMA1_2_STM32F09") then
        return {"DMA_NONE",
                "DMA1_CH2", "DMA1_CH4", "DMA1_CH7",
                "DMA2_CH1", "DMA2_CH4", "DMA2_CH5"}
    elseif c_is_provided("HW_MCU_STM32F0") then
        return {"DMA_NONE", "DMA1_CH4"}
    else
        return {"DMA_NONE", "DMA_CH2"}
    end
end
-- UART3 DMA RX
function GetRxDmaUart3()
    if c_is_provided("HW_DMA_STM32F2") then
        return {"DMA_NONE",
                "(DMA_CONTROL1 | DMA_STREAM1 | DMA_CHANNEL4)"}
    elseif c_is_provided("HW_DMA1_STM32F07") then
        return {"DMA_NONE", "DMA1_CH6", "DMA2_CH3"}
    elseif c_is_provided("HW_DMA1_2_STM32F09") then
        return {"DMA_NONE",
                "DMA1_CH1", "DMA1_CH3", "DMA1_CH5", "DMA1_CH6",
                "DMA2_CH2", "DMA2_CH3"}
    else
        return {"DMA_NONE", "DMA1_CH3"}
    end
end

-- UART4 TX DMA
function GetTxDmaUart4()
    if c_is_provided("HW_DMA_STM32F2") then
        return {"DMA_NONE",
                "(DMA_CONTROL1 | DMA_STREAM4 | DMA_CHANNEL4)"}
    elseif c_is_provided("HW_DMA1_STM32F07") then
        return {"DMA_NONE", "DMA1_CH7"}
    elseif c_is_provided("HW_DMA1_2_STM32F09") then
        return {"DMA_NONE",
                "DMA1_CH2", "DMA1_CH4", "DMA1_CH7",
                "DMA2_CH1", "DMA2_CH4", "DMA2_CH5"}
    elseif c_is_provided("HW_STM32L0") then
        return {"DMA_NONE", "DMA1_CH3", "DMA1_CH7"}
    else
        return {"DMA_NONE", "DMA2_CH3"}
    end
end
-- UART4 DMA RX
function GetRxDmaUart4()
    if c_is_provided("HW_DMA_STM32F2") then
        return {"DMA_NONE",
                "(DMA_CONTROL1 | DMA_STREAM2 | DMA_CHANNEL4)"}
    elseif c_is_provided("HW_DMA1_STM32F07") then
        return {"DMA_NONE", "DMA1_CH6"}
    elseif c_is_provided("HW_DMA1_2_STM32F09") then
        return {"DMA_NONE",
                "DMA1_CH1", "DMA1_CH3", "DMA1_CH5", "DMA1_CH6",
                "DMA2_CH2", "DMA2_CH3"}
    elseif c_is_provided("HW_STM32L0") then
        return {"DMA_NONE", "DMA1_CH2", "DMA1_CH6"}
    else
        return {"DMA_NONE", "DMA2_CH5"}
    end
end

-- UART5 TX DMA
function GetTxDmaUart5s()
    if c_is_provided("HW_DMA_STM32F2") then
        return {"DMA_NONE",
                "(DMA_CONTROL1 | DMA_STREAM7 | DMA_CHANNEL4)"}
    elseif c_is_provided("HW_DMA1_2_STM32F09") then
        return {"DMA_NONE",
                "DMA1_CH2", "DMA1_CH4", "DMA1_CH7",
                "DMA2_CH1", "DMA2_CH4", "DMA2_CH5"}
    elseif c_is_provided("HW_STM32L0") then
        return {"DMA_NONE", "DMA1_CH3", "DMA1_CH7"}
    else
        return {"DMA_NONE", "DMA2_CH1"}
    end
end
-- UART5 DMA RX
function GetRxDmaUart5()
    if c_is_provided("HW_DMA_STM32F2") then
        return {"DMA_NONE",
                "(DMA_CONTROL1 | DMA_STREAM0 | DMA_CHANNEL4)"}
    elseif c_is_provided("HW_DMA1_2_STM32F09") then
        return {"DMA_NONE",
                "DMA1_CH1", "DMA1_CH3", "DMA1_CH5", "DMA1_CH6",
                "DMA2_CH2", "DMA2_CH3"}
    elseif c_is_provided("HW_STM32L0") then
        return {"DMA_NONE", "DMA1_CH2", "DMA2_CH6"}
    else
        return {"DMA_NONE", "DMA2_CH2"}
    end
end

-- UART6 TX DMA
function GetTxDmaUart6()
    if c_is_provided("HW_DMA_STM32F2") then
        return {"DMA_NONE",
                "(DMA_CONTROL2 | DMA_STREAM6 | DMA_CHANNEL5)",
                "(DMA_CONTROL2 | DMA_STREAM7 | DMA_CHANNEL5)"}
    elseif c_is_provided("HW_DMA1_2_STM32F09") then
        return {"DMA_NONE",
                "DMA1_CH2", "DMA1_CH4", "DMA1_CH7",
                "DMA2_CH1", "DMA2_CH4", "DMA2_CH5"}
    else
        return {"DMA_NONE", "DMA2_CH1"}
    end
end
-- UART6 DMA RX
function GetRxDmaUart6()
    if c_is_provided("HW_DMA_STM32F2") then
        return {"DMA_NONE",
                "(DMA_CONTROL2 | DMA_STREAM1 | DMA_CHANNEL5)",
                "(DMA_CONTROL2 | DMA_STREAM2 | DMA_CHANNEL5)"}
    elseif c_is_provided("HW_DMA1_2_STM32F09") then
        return {"DMA_NONE",
                "DMA1_CH1", "DMA1_CH3", "DMA1_CH5", "DMA1_CH6",
                "DMA2_CH2", "DMA2_CH3"}
    else
        return {"DMA_NONE", "DMA2_CH2"}
    end
end

-- UART7 TX DMA
function GetTxDmaUart7()
    if c_is_provided("HW_DMA_STM32F2") then
        return {"DMA_NONE",
                "(DMA_CONTROL1 | DMA_STREAM1 | DMA_CHANNEL5)"}
    elseif c_is_provided("HW_DMA1_2_STM32F09") then
        return {"DMA_NONE",
                "DMA1_CH2", "DMA1_CH4", "DMA1_CH7",
                "DMA2_CH1", "DMA2_CH4", "DMA2_CH5"}
    else
        return {"DMA_NONE", "DMA2_CH1"}
    end
end
-- UART7 DMA RX
function GetRxDmaUart7()
    if c_is_provided("HW_DMA_STM32F2") then
        return {"DMA_NONE",
                "(DMA_CONTROL1 | DMA_STREAM3 | DMA_CHANNEL5)"}
    elseif c_is_provided("HW_DMA1_2_STM32F09") then
        return {"DMA_NONE",
                "DMA1_CH1", "DMA1_CH3", "DMA1_CH5", "DMA1_CH6",
                "DMA2_CH2", "DMA2_CH3"}
    else
        return {"DMA_NONE", "DMA2_CH2"}
    end
end

-- UART8 TX DMA
function GetUart8TxDmaChoices()
    if c_is_provided("HW_DMA_STM32F2") then
        return {"DMA_NONE",
                "(DMA_CONTROL1 | DMA_STREAM0 | DMA_CHANNEL5)"}
    elseif c_is_provided("HW_DMA1_2_STM32F09") then
        return {"DMA_NONE",
                "DMA1_CH2", "DMA1_CH4", "DMA1_CH7",
                "DMA2_CH1", "DMA2_CH4", "DMA2_CH5"}
    else
        return {"DMA_NONE", "DMA2_CH1"}
    end
end
-- UART8 DMA RX
function GetRxDmaUart8()
    if c_is_provided("HW_DMA_STM32F2") then
        return {"DMA_NONE",
                "(DMA_CONTROL1 | DMA_STREAM6 | DMA_CHANNEL5)"}
    elseif c_is_provided("HW_DMA1_2_STM32F09") then
        return {"DMA_NONE",
                "DMA1_CH1", "DMA1_CH3", "DMA1_CH5", "DMA1_CH6",
                "DMA2_CH2", "DMA2_CH3"}
    else
        return {"DMA_NONE", "DMA2_CH2"}
    end
end

-- UART9 TX DMA
function GetTxDmaUart9()
    if c_is_provided("HW_DMA_STM32F2") then
        return {"DMA_NONE",
                "(DMA_CONTROL2 | DMA_STREAM0 | DMA_CHANNEL1)"}
    end
end

-- UART9 RX DMA
function GetTxDmaUart9()
    if c_is_provided("HW_DMA_STM32F2") then
        return {"DMA_NONE",
                "(DMA_CONTROL2 | DMA_STREAM7 | DMA_CHANNEL0)"}
    end
end

-- UART10 TX DMA
function GetTxDmaUart10()
    if c_is_provided("HW_DMA_STM32F2") then
        return {"DMA_NONE",
                "(DMA_CONTROL2 | DMA_STREAM7 | DMA_CHANNEL6)"}
    end
end

-- UART10 RX DMA
function GetTxDmaUart10()
    if c_is_provided("HW_DMA_STM32F2") then
        return {"DMA_NONE",
                "(DMA_CONTROL2 | DMA_STREAM0 | DMA_CHANNEL5)"}
    end
end

function UartDevice(
         USARTX,
         USARTX_GPIOV1,
         USARTX_GPIOV2,
         DEVICE_FLAG,
         SRCX,
         REMAP_DESC,
         REMAP_CHOICE,
         TX_CHOICE,
         RX_CHOICE,
         RTS_CHOICE,
         CTS_CHOICE,
         CK_CHOICE,
         TX_DMA_CHOICE,
         RX_DMA_CHOICE
         )
return
    {
        name = function() return 'nutarch_cm3_stm32_' .. USARTX ;end,
        brief = function() return 'STM32 ' .. USARTX .. ' Controller' ;end,
        description = function() return USARTX .. ' Controller.' ;end,
        requires = function() return DEVICE_FLAG ;end,
        sources =  function() return SRCX ;end,
        options =
        {
            {
                macro = function() return USARTX .. '_TX'; end,
                brief = function() return USARTX .. ' TX Pin selection'; end,
                description = function() return 'Choose ' ..USARTX.. ' TX Pin. Use PIN_NONE if no transmit is needed.'; end,
                requires = {"HW_GPIO_STM32V2"},
                type = "enumerated",
                choices = function() return TX_CHOICE(); end,
                default = function() return TX_CHOICE()[1]; end,
                file = "include/cfg/uart.h"
            },
            {
                macro = function() return USARTX .. '_RX'; end,
                brief = function() return USARTX .. ' RX Pin selection'; end,
                description = function() return 'Choose ' ..USARTX.. ' RX Pin. Use PIN_NONE if no receive is needed.'; end,
                requires = {"HW_GPIO_STM32V2"},
                type = "enumerated",
                choices = function() return RX_CHOICE(); end,
                default = function() return RX_CHOICE()[1]; end,
                file = "include/cfg/uart.h",
            },
            {
                macro = function() return USARTX .. '_REMAP'; end,
                brief = "Use Alternate Pins",
                description = function() return REMAP_DESC; end,
                requires = {"HW_GPIO_STM32V1"},
                type = "enumerated",
                choices = function() return REMAP_CHOICE; end,
                default = "0",
                file = "include/cfg/uart.h"
            },
            {
                macro = function() return USARTX .. '_USE_RTS'; end,
                brief = function() return 'Use ' ..USARTX.. ' RTS'; end,
                description = function() return 'Use RTS Flow control on ' ..USARTX; end,
                requires = { "HW_GPIO_STM32V1" },
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = function() return USARTX .. '_USE_CTS'; end,
                brief = function() return 'Use ' ..USARTX.. ' CTS'; end,
                description = function() return 'Use CTS Flow control on ' ..USARTX; end,
                requires = { "HW_GPIO_STM32V1" },
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = function() return USARTX .. '_USE_CK'; end,
                brief = function() return 'Use ' ..USARTX.. ' CK'; end,
                description = function() return 'Use CK pin for synchronous TXRX on ' ..USARTX; end,
                requires = function() return USARTX_GPIOV1; end,
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = function() return USARTX .. '_RTS'; end,
                brief = function() return USARTX .. ' RTS Pin selection'; end,
                description = function() return 'Choose ' ..USARTX.. ' RTS Pin. Use PIN_NONE if no RTS Flow is needed.'; end,
                requires = {"HW_GPIO_STM32V2"},
                type = "enumerated",
                choices = function() return RTS_CHOICE(); end,
                default = "PIN_NONE",
                file = "include/cfg/uart.h"
            },
            {
                macro = function() return USARTX .. '_CTS'; end,
                brief = function() return USARTX .. ' CTS Pin selection'; end,
                description = function() return 'Choose ' ..USARTX.. ' CTS Pin. Use PIN_NONE if no CTS Flow is needed.'; end,
                requires = {"HW_GPIO_STM32V2"},
                type = "enumerated",
                choices = function() return CTS_CHOICE(); end,
                default = "PIN_NONE",
                file = "include/cfg/uart.h"
            },
            {
                macro = function() return USARTX .. '_DE'; end,
                brief = function() return USARTX .. ' Driver enable function on RTS'; end,
                description = "Enable Driver enable output ob RTS pin. If enabled, needs RTS pin selected.",
                requires = {"HW_GPIO_STM32V2"},
                type = "enumerated",
                choices = {"DISABLE", "ENABLE"},
                default = "DISABLE",
                file = "include/cfg/uart.h"
            },
            {
                macro = function() return USARTX .. '_CK'; end,
                brief = function() return USARTX .. ' CK Pin selection'; end,
                description = function() return 'Choose ' ..USARTX.. ' CK Pin. Use PIN_NONE if no synchrounis TXRX is needed.'; end,
                requires = function() return USARTX_GPIOV2; end,
                type = "enumerated",
                choices = function() return CK_CHOICE(); end,
                file = "include/cfg/uart.h"
            },
            {
                macro = function() return USARTX .. '_DMA_TX'; end,
                brief = function() return USARTX .. ' TX DMA selection'; end,
                description = function() return 'Select ' ..USARTX.. ' DMA TX. Use DMA_NONE if no TX DMA is needed.'; end,
                type = "enumerated",
                choices = function() return TX_DMA_CHOICE(); end,
                default = "DMA_NONE",
               file = "include/cfg/uart.h"
            },
            {
                macro = function() return USARTX .. '_DMA_RX'; end,
                brief = function() return USARTX .. ' RX DMA selection'; end,
                description = function() return 'Select ' ..USARTX.. ' DMA RX. Use DMA_NONE if no RX DMA is needed.'; end,
                type = "enumerated",
                choices = function() return RX_DMA_CHOICE(); end,
                default = "DMA_NONE",
                file = "include/cfg/uart.h"
            },
            {
                macro = function() return USARTX .. '_TX_INV'; end,
                brief = function() return USARTX .. ' invert TX level.'; end,
                description = function() return USARTX .. ' invert TX level.\n\n'..
                              'Default is DISABLE for normal level'; end,
                requires = {"HW_USART_STM32V2"},
                type = "enumerated",
                choices = {"DISABLE", "ENABLE"},
                default = "DISABLE",
                file = "include/cfg/uart.h"
            },
            {
                macro = function() return USARTX .. '_RX_INV'; end,
                brief = function() return USARTX .. ' invert RX level.'; end,
                description = function() return USARTX .. ' invert RX level.\n\n'..
                              'Default is DISABLE for normal level'; end,
                requires = {"HW_USART_STM32V2"},
                type = "enumerated",
                choices = {"DISABLE", "ENABLE"},
                default = "DISABLE",
                file = "include/cfg/uart.h"
            },
            {
                macro = function() return USARTX .. '_DE_INV'; end,
                brief = function() return USARTX .. ' invert driver enable level.'; end,
                description = function() return USARTX .. ' invert driver enable level.\n\n'..
                              'Default is DISABLE for normal level'; end,
                requires = {"HW_USART_STM32V2"},
                type = "enumerated",
                choices = {"DISABLE", "ENABLE"},
                default = "DISABLE",
                file = "include/cfg/uart.h"
            },
            {
                macro = function() return USARTX .. '_TX_MULTIDRIVE'; end,
                brief = function() return USARTX .. ' TX Pin as Open-Drain.'; end,
                description = function() return 'Drive ' .. USARTX .. ' TX as Open-Drain\n\n'..
                              'Default is DISABLE for Push/Pull'; end,
                type = "enumerated",
                choices = {"DISABLE", "ENABLE"},
                default = "DISABLE",
                file = "include/cfg/uart.h"
            },
            {
                macro = function() return USARTX .. '_TX_RX_SWAP'; end,
                brief = function() return USARTX .. ' TX and RX pin swap.'; end,
                description = function() return USARTX .. ' TX and RX pin swap.\n\n'..
                              'Default is DISABLE for no swap'; end,
                requires = {"HW_USART_STM32V2"},
                type = "enumerated",
                choices = {"DISABLE", "ENABLE"},
                default = "DISABLE",
                file = "include/cfg/uart.h"
            },
            {
                macro = function() return USARTX .. '_TX_RX_SWAP'; end,
                brief = function() return USARTX .. ' TX and RX pin swap.'; end,
                description = function() return USARTX .. ' TX and RX pin swap.\n\n'..
                              'Default is DISABLE for no swap'; end,
                requires = {"HW_USART_STM32V2"},
                type = "enumerated",
                choices = {"DISABLE", "ENABLE"},
                default = "DISABLE",
                file = "include/cfg/uart.h"
            },
-- half duplex handled as diver io control
            {
                macro = function() return USARTX .. '_DATA_INV'; end,
                brief = function() return USARTX .. ' Binary data inversion.'; end,
                description = function() return USARTX .. ' Binary data inversion.\n\n'..
                              'Default is DISABLE for no data inversion'; end,
                requires = {"HW_USART_STM32V2"},
                type = "enumerated",
                choices = {"DISABLE", "ENABLE"},
                default = "DISABLE",
                file = "include/cfg/uart.h"
            },
            {
                macro = function() return USARTX .. '_MSBFIRST'; end,
                brief = function() return USARTX .. ' send MSB first.'; end,
                description = function() return USARTX .. ' send MSB first.\n\n'..
                              'Default is DISABLE for to send LSB first'; end,
                type = "enumerated",
                choices = {"DISABLE", "ENABLE"},
                default = "DISABLE",
                file = "include/cfg/uart.h"
            },
            {
                macro =  function() return USARTX .. '_XONXOFF_CONTROL'; end,
                brief = "XON/XOFF Protocol",
                description = "When selected, the driver will use software XON/XOFF protocol.",
                flavor = "booldata",
                file = "include/cfg/uart.h"
            },
            {
                macro = function() return USARTX .. '_INIT_BAUDRATE'; end,
                brief = "Initial Baudrate",
                description = "Initial baudrate the Port is set to.",
                type = "integer",
                default = 115200,
                file = "include/cfg/uart.h"
            },
            {
                macro = function() return USARTX .. '_INIT_TX_BUFSIZE'; end,
                brief = "Initial size of TX buffer",
                description = "Bytes used for TX buffer.\n"..
                    "Only size - 1 bytes can be written.\n"..
                    "Guard byte is used for Full/Empty recognition",
                type = "integer",
                default = 64,
                file = "include/cfg/uart.h"
            },
             {
                macro = function() return USARTX .. '_INIT_RX_BUFSIZE'; end,
                brief = "Initial size of RX buffer",
                description = "Bytes used for rX buffer.\n"..
                    "Only size - 1 bytes can be written.\n"..
                    "Guard byte is used for Full/Empty recognition",
                type = "integer",
                default = 64,
                file = "include/cfg/uart.h"
            },
            {
                macro = function() return USARTX .. '_PIN_SPEED'; end,
                brief = "IO Pin speed setting UART device",
                description = function() return PinSpeedDesc() end,
                choices = function() return GetPinSpeedChoices() end,
                type = "enumerated",
                default = "GPIO_MED",
                file = "include/cfg/uart.h"
            },
-- Handle RTS/CTS/Driver enabel on normal pins
         }
    }
end

nutarch_cm3_stm32_usart_devices =
{
   -- ******************************************
    --
    -- STM32 UART Device Configuration and drivers
    --
    -- ******************************************
    --
    --
    -- STM32 DEBUG Macro (Low-Level Debug UART definition)
    --
    {
        name = "nutarch_cm3_stm32_debugmacro",
        brief = "Low-Level Debug UART macros.",
        description = "Polling UART function (macro) to use in exception handlers.",
        provides = { "DEBUG_MACRO"},
		sources = { "cm3/cmsis/cortex_debug.c" },
        options =
        {
            {
                macro = "DEBUG_MACRO",
                brief = "Enabled",
                description = "Check to enable debug output of exception handlers",
                flavor = "boolean",
                file = "include/cfg/cortex_debug.h"
            },
            {
                macro = "DEBUG_BACKTRACE",
                brief = "Enable call backtrace (experimental)",
                description = "Check to enable call stack back trace in case of an exception.\n\n"..
                              "In case of an exception the last 128 words on the stack will\n"..
                              "be printed out and a call trace will be generated and printed out\n"..
                              "(Address of the function entry and offset in the function).\n"..
                              "Together with the .map file and / or the assembler listing\n"..
                              "the call trace allows you to trace back your code flow in\n"..
                              "which led to the exception. The assembler listing can be\n"..
                              "created by 'arm-none-eabi-objdump -d myfile.elf'\n\n"..
                              "This function is highly experimental and relies on a very\n"..
                              "defined function entry code and existance of a stack\n"..
                              "frame pointer\n"..
                              "To ensure the correct function layout this option disables\n"..
                              "compiler optimisation and enables a stack frame pointer\n"..
                              "generation. The following compiler options will be set:\n\n"..
                              "-O0 -mtpcs-frame -mtpcs-leaf-frame -fno-omit-frame-pointer\n\n"..
                              "This will enlarge the resulkting binary size significantly\n"..
                              "any may influence execution performance.\n\n"..
                              "The backtrace will only work for functions compiled with\n"..
                              "above compiler parameters. E.g. it will very likely not work"..
                              "for any newlib functions.\n",
                flavor = "boolean",
                makedefs = { "MDEBUG+=-O0 -mtpcs-frame -mtpcs-leaf-frame -fno-omit-frame-pointer" },
                file = "include/cfg/cortex_debug.h"
            },
            {
                macro = "DEBUG_UART_NR",
                brief = "Debug UART",
                description = "Select the UART to use for low level debugging",
                type = "enumerated",
                choices = function() return GetStmUarts() end,
                file = "include/cfg/cortex_debug.h"
            }
        }
    },

    --
    -- STM32 DEBUG UART configuration
    --
    {
        name = "nutarch_cm3_stm32_devices_debug",
        brief = "UART Debug Output (STM32)",
        requires = { "DEV_UART_STM32" },
        provides = { "DEV_UART", "DEV_FILE", "DEV_WRITE" },
        options =
        {
            {
                macro = "DEBUG_USE_UART",
                brief = "Select debug UART channel",
                description = "Select the UART to use as debug output\n"..
                              "This is only to adapt the application examples and to tell them\n"..
                              "which UART to use as stdout.",
                type = "enumerated",
                default = "DEV_UART1",
                choices = function() return GetStmUarts() end,
                file = "include/cfg/uart.h",
            }
        }
    },
    --
    -- STM32 UART1 Interface
    --
    UartDevice(
        "USART1",
        {"HW_GPIO_STM32V1", "HW_USART1_STM32"},
        {"HW_GPIO_STM32V2", "HW_USART1_STM32"},
        {"HW_USART1_STM32"},
        {"cm3/dev/stm/stm32_usart1.c"},
        "Choose Pin remap. Default = 0 means no remap.\n\n"..
        "Default 0 Pin Mapping is:\n TXD PA09\n RXD PA10\n CTS PB13\n RTS PB14\n"..
        "Remap   1 Pin Mapping is:\n TXD PB06\n RXD PB07\n CTS PB13\n RTS PB14.",
        {"0", "1"},
        GetUart1Tx,
        GetUart1Rx,
        GetUart1Rts,
        GetUart1Cts,
        GetUart1Ck,
        GetTxDmaUart1,
        GetRxDmaUart1
    ),
    UartDevice(
        "USART2",
        {"HW_GPIO_STM32V1", "HW_USART2_STM32"},
        {"HW_GPIO_STM32V2", "HW_USART2_STM32"},
        {"HW_USART2_STM32"},
        {"cm3/dev/stm/stm32_usart2.c"},
        "Choose Pin remap. Default = 0 means no remap.\n\n"..
        "Default 0 Pin Mapping is:\n TXD PA02\n RXD PA03\n CTS PA00\n RTS PA01\n CK PA04"..
        "Remap   1 Pin Mapping is:\n TXD PD05\n RXD PD06\n CTS PD03\n RTS PD04\n CK PD07.",
        {"0", "1"},
        GetUart2Tx,
        GetUart2Rx,
        GetUart2Rts,
        GetUart2Cts,
        GetUart2Ck,
        GetTxDmaUart2,
        GetRxDmaUart2
    ),
    UartDevice(
        "USART3",
        {"HW_GPIO_STM32V1", "HW_USART3_STM32"},
        {"HW_GPIO_STM32V2", "HW_USART3_STM32"},
        {"HW_USART3_STM32"},
        {"cm3/dev/stm/stm32_usart3.c"},
        "Choose Pin remap. Default = 0 means no remap.\n\n"..
        "Default 0 Pin Mapping is:\n TXD PB10\n RXD PB11\n CTS PB13\n RTS PB14\n CK PC12."..
        "Remap1  1 Pin Mapping is:\n TXD PC10\n RXD PC11\n CTS PB13\n RTS PB14\n CK PC12."..
        "Remap3  3 Pin Mapping is:\n TXD PD08\n RXD PD09\n CTS PD11\n RTS PD12\n CK PD11.",
        {"0", "1", "3"},
        GetUart3Tx,
        GetUart3Rx,
        GetUart3Rts,
        GetUart3Cts,
        GeUart3tCk,
        GetTxDmaUart3,
        GetRxDmaUart3
    ),
    UartDevice(
        "USART4",
        {"HW_GPIO_STM32V1", "HW_USART4_STM32"},
        {"HW_GPIO_STM32V2", "HW_USART4_STM32"},
        {"HW_UART4_STM32"},
        {"cm3/dev/stm/stm32_uart4.c"},
        "No remap.",
        "0",
        GetUart4Tx,
        GetUart4Rx,
        GetUart4Rts,
        GetUart4Cts,
        GetUart4Ck,
        GetTxDmaUart4,
        GetRxDmaUart4
    ),
    UartDevice(
        "USART5",
        {"HW_GPIO_STM32V1", "HW_USART5_STM32"},
        {"HW_GPIO_STM32V2", "HW_USART5_STM32"},
        {"HW_UART5_STM32"},
        {"cm3/dev/stm/stm32_uart5.c"},
        "No remap.",
        "0",
        GetUart5Tx,
        GetUart5Rx,
        GetUart5Rts,
        GetUart5Cts,
        GetUart5Ck,
        GetTxDmaUart5,
        GetRxDmaUart5
    ),
    UartDevice(
        "USART6",
        {"HW_GPIO_STM32V1", "HW_USART6_STM32"},
        {"HW_GPIO_STM32V2", "HW_USART6_STM32"},
        {"HW_USART6_STM32"},
        {"cm3/dev/stm/stm32_usart6.c"},
        "No remap.",
        "0",
        GetUart6Tx,
        GetUart6Rx,
        GetUart6Rts,
        GetUart6Cts,
        GetUart6Ck,
        GetTxDmaUart6,
        GetRxDmaUart6
    ),
    UartDevice(
        "USART7",
        {"HW_GPIO_STM32V1", "HW_USART7_STM32"},
        {"HW_GPIO_STM32V2", "HW_USART7_STM32"},
        {"HW_UART7_STM32"},
        "",
        "No remap.",
        "0",
        GetUart7Tx,
        GetUart7Rx,
        GetUart7Rts,
        GetUart7Cts,
        GetUart7Ck,
        GetTxDmaUart7,
        GetRxDmaUart7
    ),
    UartDevice(
        "USART8",
        {"HW_GPIO_STM32V1", "HW_USART8_STM32"},
        {"HW_GPIO_STM32V2", "HW_USART8_STM32"},
        {"HW_UART8_STM32"},
        "",
        "No remap.",
        "0",
        GetUart8Tx,
        GetUart8Rx,
        GetUart8Rts,
        GetUart8Cts,
        GetUart8Ck,
        GetTxDmaUart8,
        GetRxDmaUart8
    ),
    UartDevice(
        "USART9",
        {"HW_GPIO_STM32V1", "HW_USART9_STM32"},
        {"HW_GPIO_STM32V2", "HW_USART9_STM32"},
        {"HW_UART9_STM32"},
        "",
        "No remap.",
        "0",
        GetUart9Tx,
        GetUart9Rx,
        GetUart9Rts,
        GetUart9Cts,
        GetUart9Ck,
        GetTxDmaUart9,
        GetRxDmaUart9
    ),
    UartDevice(
        "USART10",
        {"HW_GPIO_STM32V1", "HW_USART10_STM32"},
        {"HW_GPIO_STM32V2", "HW_USART10_STM32"},
        {"HW_UART10_STM32"},
        "",
        "No remap",
        "0",
        GetUart10Tx,
        GetUart10Rx,
        GetUart10Rts,
        GetUart10Cts,
        GetUart10Ck,
        GetTxDmaUart10,
        GetRxDmaUart10
    ),
    UartDevice(
        "LPUART1",
        {"HW_GPIO_STM32V1", "HW_LPUSART1_STM32"},
        {"HW_GPIO_STM32V2", "HW_LPUSART1_STM32"},
        {"HW_LPUART1_STM32"},
        {"cm3/dev/stm/stm32_lpuart1.c"},
        "No remap",
        "0",
        GetLpuart1Tx,
        GetLpuart1Rx,
        GetLpuart1Rts,
        GetLpuart1Cts,
        GetLpuart1Ck,
        GetTxDmaLpuart1,
        GetRxDmaLpuart1
    ),
}
