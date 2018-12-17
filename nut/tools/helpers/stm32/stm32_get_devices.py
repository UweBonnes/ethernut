#!/usr/bin/env python3
import xml.etree.ElementTree as ET
import argparse
import time
import os
import sys

def generate(name, headerfile, luafile):
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
    luafile.write("            macro = \"%s\",\n" % devicename)
    luafile.write("            requires = {\"HW_MCU_%s\"},\n" % devicename)
    luafile.write("            default = 1,\n")
    luafile.write("            provides =\n")
    luafile.write("            {\n")
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
    luafile.write("            makedefs = {\"UCPFLAGS+=-DIRQn_MAX=%d\"},\n" % (num_irq + 1))
    luafile.write("         },\n")
    headerdata.close()

if __name__ == "__main__":
    print("stm32_get_devices: Generate Lua function for devices invokation in Nut/OS configuration.")
    f = []
    nutpath = "%s/../../../../" % os.path.dirname(os.path.realpath(sys.argv[0]))
    nutpath = os.path.realpath(nutpath)
    dirpath = "%s/nut/include/arch/cm3/stm/vendor/" % nutpath
    print(dirpath)
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
                generate(f, filename, luafile)
        luafile.write("      }\n")
        luafile.write("   }\n")
        luafile.write("}\n")
        luafile.close()
