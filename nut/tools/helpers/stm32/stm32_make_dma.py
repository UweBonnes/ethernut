#!/usr/bin/env python3
import xml.etree.ElementTree as ET
import argparse
import time
import os
import sys

class DmaRequest(object):
    def __init__(self, name, request):
        self.name = name
        self.request = request

class DmaChannel(object):
    def __init__(self, name, channel, mask, request):
        self.name = name
        self.channel = channel
        self.mask = mask
        self.request = request

if __name__ == "__main__":
    print("stm32_make_dma\n")
    print("Generate a STM32 DMA device Header for Nut/OS.")
    print("Give the full path to the .../STM32CubeMX/db/mcu/IP directory as argument.")
    print("Run in .../<nutos>/nut/include/arch/cm3/stm/generated>.")
    parser = argparse.ArgumentParser()
    parser.add_argument("dirpath", nargs='?', help="Directory to parse", default="/devel/cubemx/db/mcu/IP")
    args = parser.parse_args()
    print("Looking in %s" % args.dirpath)

    mcunames = []
    for (dirpath, dnames, filenames) in os.walk(args.dirpath):
        mcunames.extend(filenames)
        break
    mcunames = sorted(mcunames)
    dma_version = 0
    for mcu in mcunames:
        if mcu.startswith("DMA-STM32") and ".xml" in mcu:
            if "DMA-STM32H7" in mcu :
                continue
            try:
                path = "%s/%s" % (args.dirpath, mcu)
                tree = ET.parse(path)
                root = tree.getroot()
            except:
                print("Unknown mcu file %s" % path)
                exit(-1)
            nutpath = "%s/../../../../" % os.path.dirname(os.path.realpath(sys.argv[0]))
            nutpath = os.path.realpath(nutpath)
            generated_h_path = "%s/nut/include/arch/cm3/stm/generated" % nutpath
            devicename = mcu.partition('-')[2].partition('_')[0]
            muxname = "%s/%s_dma.h" % (generated_h_path, devicename.lower())
            print("Warping %s to %s_dma.h" % (mcu, devicename.lower()))
            try:
                muxfile = open(muxname, 'w')
            except:
                print("Failed to create pinmux file %s" % muxname)
                exit -1
            muxfile.write("/* Do not edit! Generated automatically from %s/%s.*/\n\n" %
                          (dirpath, mcu))
            muxfile.write("#if !defined(_%s_DMA_H_)\n" % devicename)
            muxfile.write("# define _%s_DMA_H_\n\n" % devicename)
            if "1_0_Modes.xml" in mcu:
                dma_version = 10
            elif "1_1_Modes.xml" in mcu:
                dma_version = 11
            elif "1_2_Modes.xml" in mcu:
                dma_version = 12
            elif "1_3_Modes.xml" in mcu:
                dma_version = 13
            elif "2_0_Modes.xml" in mcu:
                dma_version = 20
            requests = []
            dmas = []
            remaps = []
            muxfile.write("#define STM32_DMA_VERSION %d\n" % dma_version)
            if "STM32L4S" in mcu:
                for refparameter in root.findall(
                        "{http://mcd.rou.st.com/modules.php?name=mcu}RefParameter") :
                    if refparameter.get('Name') == "Request" :
                        value = 0
                        for request in refparameter.findall(
                                "{http://mcd.rou.st.com/modules.php?name=mcu}PossibleValue") :
                            muxfile.write("#define  %-26s %3d\n" % (request.get('Value'), value))
                            value = value + 1
            for refparameter in root.findall(
                    "{http://mcd.rou.st.com/modules.php?name=mcu}RefParameter") :
                if refparameter.get('Name') == "DMA_Remap" :
                    defaultval = refparameter.get('DefaultValue')
                    value = ""
                    if  "DMA_REMAP" in defaultval :
                        value = defaultval.replace("DMA_REMAP_", "")
                    elif  "HAL_REMAPDMA" in defaultval :
                        value = defaultval.replace("HAL_REMAPDMA_", "")
                    if value :
                        remaps.append(value)
            for refmode in root.findall(
                    "{http://mcd.rou.st.com/modules.php?name=mcu}RefMode") :
                if refmode.get('BaseMode') == "DMA_Request" :
                    channel = refmode.get('Name').split(':')[0];
                    for parameter in refmode.findall(
                            "{http://mcd.rou.st.com/modules.php?name=mcu}Parameter"):
                        pn = parameter.get('Name')
                        if "Request" in pn or "Channel" in pn :
                            val = parameter.find(
                                "{http://mcd.rou.st.com/modules.php?name=mcu}PossibleValue")
                            requests.append(DmaRequest(channel, val.text))
            list  = root.find(
                "{http://mcd.rou.st.com/modules.php?name=mcu}ModeLogicOperator")
            for dma in list.findall('{http://mcd.rou.st.com/modules.php?name=mcu}Mode'):
                logic = dma.find(
                    "{http://mcd.rou.st.com/modules.php?name=mcu}ModeLogicOperator")
                for channel in logic.findall(
                        "{http://mcd.rou.st.com/modules.php?name=mcu}Mode"):
                    item = channel.get("Name")
                    ml = channel.find(
                        "{http://mcd.rou.st.com/modules.php?name=mcu}ModeLogicOperator")
                    for request in ml.findall(
                            "{http://mcd.rou.st.com/modules.php?name=mcu}Mode"):
                        name = request.get("Name")
                        val = name.split(':')[0]
                        function = val.partition('_')[0]
                        sel = "0"
                        mask = "0"
                        for request in requests :
                            x = "%s:%s" % (request.name, request.request)
                            if "CHANNEL" in name and x == name :
                                sel = name.split(':')[-1]
                                break
                            elif "Channel" in name and name[-1] == x[-1]:
                                sel = "DMA_%s" % name.split('_')[-1]
                                break
                            elif name == request.name :
                                sel = request.request
                                break
                        if remaps:
                            for remap in remaps:
                                search_item = val
                                if "STM32F3" in mcu:
                                    if "ADC2" in val or "ADC4" in val:
                                        search_item = "ADC24"
                                #print("remap %s %s %s v %s" % (name, item, remap, search_item))
                                if ("TIM" in remap and remap.partition('_')[0] == function) or search_item in remap:
                                    #print("item %s %s %s v %s" % (name, item, remap, search_item))
                                    if "STM32F07" in mcu and "TIM16" in remap:
                                        mask = "SYSCFG_CFGR1_TIM16_DMA_RMP | SYSCFG_CFGR1_TIM16_DMA_RMP"
                                    elif "STM32F07" in mcu and "TIM17" in remap:
                                        mask = "SYSCFG_CFGR1_TIM17_DMA_RMP | SYSCFG_CFGR1_TIM17_DMA_RMP"
                                    elif "STM32F3" in mcu and "DAC1_CH1" in remap:
                                        mask = "SYSCFG_CFGR1_TIM6DAC1Ch1_DMA_RMP"
                                    elif "STM32F3" in mcu and "DAC1_CH2" in remap:
                                        mask =  "SYSCFG_CFGR1_TIM7DAC1Ch2_DMA_RMP"
                                    elif "USART1_TX" in remap:
                                        mask =  "SYSCFG_CFGR1_USART1TX_DMA_RMP"
                                    elif "USART1_RX" in remap:
                                        mask =  "SYSCFG_CFGR1_USART1RX_DMA_RMP"
                                    elif "ADC2" in remap or "ADC4" in remap:
                                        mask =  "SYSCFG_CFGR1_ADC24_DMA_RMP"
                                    else :
                                        mask =  "SYSCFG_CFGR1_%s_DMA_RMP" % function
                                    ch_nr = item[-1]
                                    if "STM32F07" in mcu and "TIM16" in remap:
                                        if ch_nr == "4":
                                            sel = "SYSCFG_CFGR1_TIM16_DMA_RMP"
                                        elif ch_nr == "6":
                                            sel = "SYSCFG_CFGR1_TIM16_DMA_RMP2"
                                    elif "STM32F07" in mcu and "TIM17" in remap:
                                        if ch_nr == "1":
                                            sel = "SYSCFG_CFGR1_TIM17_DMA_RMP"
                                        elif ch_nr == "7":
                                            sel = "SYSCFG_CFGR1_TIM17_DMA_RMP2"
                                    elif search_item == "ADC24":
                                        if val.partition(':')[0] == "ADC2"  and ch_nr == "3":
                                            sel = mask
                                        if val.partition(':')[0] == "ADC4"  and ch_nr == "4":
                                            sel = mask
                                    elif "STM32F3" in mcu and ("DAC1_CH1" in name or "TIM6"):
                                        if item.partition('_')[0] == "DMA1":
                                            sel = mask
                                    elif "STM32F3" in mcu and ("DAC1_CH2" in name or "TIM7"):
                                        if  item.partition('_')[0] == "DMA1":
                                            sel = mask
                                    elif remap[-1] == item[-1]:
                                         sel = mask
                        if mask != "0":
                            mask = "%s_Msk" % mask
                        dev =  val.partition("_")[0]
                        for function in val.partition("_")[2].split('/') :
                            if len(function):
                                ch_func = "%s_%s" % ( dev, function)
                            else :
                                ch_func = val
                            dmas.append(DmaChannel(ch_func, item, mask, sel))
            dmas = sorted(dmas, key=lambda x: (x.name))
            last_dma =""
            if "STM32L4R" in mcu:
                for request in requests:
                    muxfile.write("#define %s %d\n" % (request.name, request.value))
                continue
            if dma_version == 10 :
                if "STM32F33" in mcu:
                    muxfile.write("#define STM32_DMA_DUAL_SYSCFG_REMAP\n")
                if "STM32F0" in mcu or "STM32F3" in mcu:
                    muxfile.write("#define STM32_DMA_SYSCFG_REMAP\n")
            muxfile.write("\n")
            combined_irq = 0
            for dma in dmas :
                is_group = "DMA_IRQ_SINGLE"
                if "MEMTOMEM" in dma.name:
                    continue
                if dma.name != last_dma :
                    if last_dma != "" :
                        alias = ""
                        if  "USART" in last_dma:
                            alias ="#define DMA_%s    DMA_%s\n" % (
                                last_dma.replace("USART", "UART"), last_dma)
                        elif  "LPUART_" in last_dma:
                            alias ="#define DMA_%s    DMA_%s\n" % (
                                last_dma.replace("LPUART_", "LPUART1_"), last_dma)
                        muxfile.write(", {}}\n%s" % alias)
                    last_dma = dma.name
                    dma.name = dma.name.replace("SD/MMC","SDMMC1")
                    dma_define = "#define DMA_%s " % dma.name
                    muxfile.write("%-27s{" % dma_define)
                else :
                    muxfile.write(",\\\n%-28s" % "")
                if dma_version == 12 or dma_version == 13:
                    muxfile.write("{DMA_ANY, DMA_REQUEST_%s}" % dma.name)
                    continue
                sig = dma.channel
                dma_nr = int(sig.partition('_')[0][-1])
                ch_nr = int(sig.partition('_')[2][-1])
                if "STM32F1" in mcu:
                    if dma_nr == 2 and ch_nr > 3:
                        sig = "DMA_GROUP1"
                elif "STM32L0" in mcu or "STM32F0" in mcu:
                    if dma_nr > 1 :
                        if ch_nr > 2 :
                            sig = "DMA_GROUP2"
                        else :
                            sig = "DMA_GROUP1"
                    else :
                        if ch_nr > 3 :
                            sig = "DMA_GROUP2"
                        elif ch_nr > 1 :
                            sig = "DMA_GROUP1"
                sig = sig.replace("Channel", "CH")
                sig = sig.replace("Stream", "STREAM")
                if "GROUP" in sig:
                    is_group = "DMA_IRQ_GROUP"
                    combined_irq = combined_irq + 1;
                muxfile.write("{%s_BASE, &sig_%-10s, " % (dma.channel, sig))
                if dma_version >= 20 :
                    muxfile.write("DMA_IRQ_SINGLE" )
                else :
                    muxfile.write("%-14s" % is_group)
                if "STM32F33" in mcu:
                    request = "0"
                    mask = dma.mask
                    request = dma.request
                    mapping ="0"
                    mapping_mask = ""
                    if "SPI1_RX" in dma.name:
                        mapping_mask = "SYSCFG_CFGR3_SPI1_RX_DMA_RMP_Msk"
                        mask = "0"
                        if dma.channel[-1] == "2":
                            mapping = "0"
                        if dma.channel[-1] == "4":
                            mapping = "1"
                        if dma.channel[-1] == "6":
                            mapping = "2"
                    if "SPI1_TX" in dma.name:
                        mapping_mask = "SYSCFG_CFGR3_SPI1_TX_DMA_RMP_Msk"
                        mask = "0"
                        if dma.channel[-1] == "3":
                            mapping = "0"
                        if dma.channel[-1] == "5":
                            mapping = "1"
                        if dma.channel[-1] == "7":
                            mapping = "2"
                    if "I2C1_RX" in dma.name:
                        mapping_mask = "SYSCFG_CFGR3_I2C1_RX_DMA_RMP_Msk"
                        mask = "0"
                        if dma.channel[-1] == "7":
                            mapping = "0"
                        if dma.channel[-1] == "3":
                            mapping = "1"
                        if dma.channel[-1] == "5":
                            mapping = "2"
                    if "I2C1_TX" in dma.name:
                        mapping_mask = "SYSCFG_CFGR3_I2C1_TX_DMA_RMP_Msk"
                        mask = "0"
                        if dma.channel[-1] == "6":
                            mapping = "0"
                        if dma.channel[-1] == "6":
                            mapping = "1"
                        if dma.channel[-1] == "4":
                            mapping = "2"
                    if mapping_mask:
                        mask = "0"
                        request = "0"
                    else:
                        mapping_mask = "0"
                    muxfile.write(", %s, %s, %s, %s}" % (
                        mask, request, mapping_mask, mapping))
                elif dma_version == 11 or dma_version == 20 :
                    muxfile.write(", %-14s}" % (dma.request))
                elif "STM32F0" in mcu or "STM32F3" in mcu :
                    muxfile.write(", %s, %s}" % (dma.mask, dma.request ))
                else:
                    muxfile.write("}")
            if "USART" in last_dma:
                muxfile.write("}\n#define DMA_%s    DMA_%s\n" % (last_dma.replace("USART", "UART"), last_dma))
            if combined_irq > 0:
                muxfile.write("#define STM32_DMA_COMBINED_IRQ\n");
            muxfile.write("\n#endif\n")
            muxfile.close()
