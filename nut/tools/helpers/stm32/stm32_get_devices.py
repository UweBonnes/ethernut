#!/usr/bin/env python3
import xml.etree.ElementTree as ET
import argparse
import time
import os
import sys
import glob

def finddie(devicename, dbpath):
    mcu_files = "%s/mcu/%s*" % (dbpath, devicename[0:9])
    files = glob.glob(mcu_files)
    matched_file = ""
    last_file = ""
    for myfile in files:
        filename = myfile.split('/')[-1]
        if "xx" in devicename :
            matched_file = myfile
            break
        elif  "STM32F41" in devicename and devicename[9] in "crtvz":
            # Exception for F41[0|1|2|3] devices
            matched_file = myfile
            break
        elif len(devicename) == len("stm32l162xca"):
            # exception for STM32L1???a.h headers
            if devicename[10] in filename[:-6] and devicename[11].upper() == filename[-5] :
                matched_file = myfile
                break
        elif devicename[10] == 'x':
            # exception for STM32L1??xdh.h headers
            if devicename[9] in filename[0:-6]:
                matched_file = myfile
                break
        else:
            if devicename[10] in filename[0:-6]:
                matched_file = myfile
                break
    try:
        xml = open(matched_file, 'r', encoding='utf-8', errors='ignore')
    except:
        print("Failed to open mcufile %s, search %s" % (matched_file, mcu_files))
        return
    iterator = iter(xml)
    die = ""
    gpio_device = ""
    dma_device = ""
    while True:
        try:
            line = next(iterator)
        except:
            break;
        if "<Die>" in line:
            die = line.partition("<Die>")[2].partition("</Die>")[0]
            die = die.partition("DIE")[2]
        if "<IP ConfigFile=\"GPIO-" in line:
            gpio_device = line.partition("Version=\"")[2].partition("_gpio")[0]
        if "<IP ConfigFile=\"DMA-" in line:
            dma_device = line.partition("Version=\"")[2].partition("_dma")[0]
    return dma_device, gpio_device, die

def generate(name, headerfile, luafile, dbpath):
    devicestring = name.partition('.')[0]
    devicename = "%s%s%s" % (devicestring[0:9].upper(), devicestring[9],
                             devicestring[10:].upper())
    devicename = devicename.replace('X','x')
    print(devicename)
    try:
        headerdata = open(headerfile, 'r', encoding='utf-8', errors='ignore')
    except:
        print("Failed to open headerfile %s" % headerfile)
    timerdevice = [0] * 30
    luafile.write("         {\n")
    dma_device, gpio_device, die = finddie(devicename, dbpath)
    luafile.write("            macro = \"%s\",\n" % devicename)
    luafile.write("            requires = {\"HW_MCU_%s\"},\n" % devicename)
    luafile.write("            default = 1,\n")
    luafile.write("            provides =\n")
    luafile.write("            {\n")
    luafile.write("               \"HW_MCU_STM32_DIE_%s\",\n" % die)
    iterator = iter(headerdata)
    num_irq = 0;
    while True:
        try:
            line = next(iterator)
        except:
            break
        if "_IRQn" in line and "= " in line and "interrupt" in line.lower():
            num_irq = int(line.partition('= ')[2].partition(',')[0].partition(' ')[0])
            if not line or line[0] == '\n':
                continue
            device = line.partition('_IRQn')[0].strip().upper()
            if "TIM" in device:
                nr_underscores = device.count('_')
                if nr_underscores == 0:
                    nr_timer = int(device[-1])
                    timerdevice[nr_timer] = 1
                else:
                    timer = device.partition('_')[0]
                    nr_timer = int(timer[-1])
                    if timerdevice[nr_timer]  == 0:
                        luafile.write("               \"HW_%s_STM32\",\n" % timer)
                        timerdevice[nr_timer] = 1
                    if device.count("TIM") == 2:
                        timer = device.split('_')[-1]
                        try :
                            nr_timer = int(timer[-2:])
                        except :
                            try:
                                nr_timer = int(timer[-1:])
                            except:
                                nr_timer = 0
                        if nr_timer > 0 and timerdevice[nr_timer]  == 0:
                            luafile.write("               \"HW_%s_STM32\",\n" % timer)
                            timerdevice[nr_timer] = 1
            luafile.write("               \"HW_%s_STM32\",\n" % device)
            if "USART" in device:
                device = device.replace("USART", "UART")
                luafile.write("               \"HW_%s_STM32\",\n" % device)
            elif "I2C" in device and "_EV" in device:
                device = device.replace("_EV", "")
                luafile.write("               \"HW_%s_STM32\",\n" % device)
        elif "#define BKPSRAM_BASE" in line:
            luafile.write("               \"HW_BKPSRAM_STM32\",\n")
        elif "#define PWR_CR2_IOSV " in line:
            luafile.write("               \"HW_VDDIO2_ISOLATION_STM32\",\n")
        elif "define __DCACHE_PRESENT" in line and "0" not in line:
            luafile.write("               \"HW_DCACHE_CM7\",\n")
        elif "define __ICACHE_PRESENT" in line and "0" not in line:
            luafile.write("               \"HW_ICACHE_CM7\",\n")
        elif "#define IS_TIM_32B_COUNTER_INSTANCE" in line:
            while True:
                if "0U" in line :
                    break
                if "TIM" in line and "== " in line:
                    timer = line.split("== ")[1]
                    timer = timer.split(")")[0]
                    luafile.write("               \"HW_%s_32BIT_STM32\",\n" % timer)
                if "\\" in line:
                    try:
                        line = next(iterator)
                    except:
                        break
                else:
                    break

    luafile.write("            },\n")
    luafile.write("            file = \"include/cfg/arch.h\",\n")
    luafile.write("            makedefs =\n")
    luafile.write("            {\n")
    luafile.write("               \"UCPFLAGS+=-DIRQn_MAX=%d\",\n" % (num_irq + 1))
    luafile.write("               \"HWDEF += -DSTM32DIE=%s\",\n" % die)
    luafile.write("               \"HWDEF += -DVENDOR=\\\"<arch/cm3/stm/vendor/%s.h>\\\"\",\n" % devicename.lower())
    if "STM32F1" in devicename:
        luafile.write("               \"HWDEF += -DPINMUXHEADER=\\\"<arch/cm3/stm/stm32f1_pinremap.h>\\\"\",\n")
    else:
        luafile.write("               \"HWDEF += -DPINMUXHEADER=\\\"<arch/cm3/stm/generated/%s_pinmux.h>\\\"\",\n" % gpio_device.lower())
    luafile.write("               \"HWDEF += -DDMACHANNELHEADER=\\\"<arch/cm3/stm/generated/%s_dma.h>\\\"\",\n" % dma_device.lower())
    luafile.write("            },\n")
    luafile.write("         },\n")
    headerdata.close()

if __name__ == "__main__":
    print("stm32_get_devices: Generate Lua function for devices invokation in Nut/OS configuration.")
    f = []
    nutpath = "%s/../../../../" % os.path.dirname(os.path.realpath(sys.argv[0]))
    nutpath = os.path.realpath(nutpath)
    dirpath = "%s/nut/include/arch/cm3/stm/vendor/" % nutpath
    parser = argparse.ArgumentParser()
    parser.add_argument("dbpath", nargs='?', help="STM32CubeMX databank path", default="/devel/STM32CubeMX/db")
    args = parser.parse_args()
    print("Headerpath %s, Databank Path %s" % (dirpath, args.dbpath))
    familylist = {"l0", "l1", "l4", "f0", "f1", "f2", "f3", "f4", "f7", "h7"}
    for (dirpath, dnames, filenames) in os.walk(dirpath):
        f.extend(filenames)
        break
    filenames = sorted(filenames)
    for family in familylist:
        devicelist = "%s/nut/conf/arch/cm3/generated/stm32%s.nut" % ( nutpath, family)
        try:
            luafile = open(devicelist, 'w')
        except:
            print("Failed to create file %s" % devicelist)
            exit (-1)
        luafile.write("-- Do not edit!\n")
        luafile.write("-- Generated from nut/include/arch/cm3/stm/vendor/stm32*.h\n\n")
        luafile.write("nutarch_cm3_stm32%s =\n" % family)
        luafile.write("{\n")
        luafile.write("   {\n")
        luafile.write("   name = \"nutarch_cm3_stm32%s_family\",\n" % family)
        luafile.write("   options =\n")
        luafile.write("      {\n")
        for f in filenames :
            devicename = "stm32%s" % family
            if devicename in f and ".h" in f:
                filename = "%s%s" % (dirpath, f)
                generate(f, filename, luafile, args.dbpath)
        luafile.write("      }\n")
        luafile.write("   }\n")
        luafile.write("}\n")
        luafile.close()
