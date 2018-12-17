#!/usr/bin/env python3
import xml.etree.ElementTree as ET
import argparse
import time
import os
import sys

class Stm32pin(object):
    def __init__(self, gpio, pin_nr, func, af_nr):
        self.gpio = gpio
        self.pin = pin_nr
        self.func = func
        self.af = af_nr

class Stm32pinlist(object):
    def __init__(self, chip, func, liste):
        self.chip = chip
        self.func = func
        self.liste = liste

funcpinalt = []

def generate(gpiopath, gpiofile, headerpath):
    devicename = gpiofile.partition('-')[2].partition('_')[0]
    try:
        path = "%s/%s" %(gpiopath, gpiofile)
        print(path)
        tree = ET.parse(path)
        root = tree.getroot()
    except:
        print("Unknown file")
        return
    muxname = "%s/%s_pinmux.h" % (headerpath, devicename.lower())
    try:
        muxfile = open(muxname, 'w')
    except:
        print("Failed to create pinmux file %s" % muxname)
        return
    port = ""
    muxfile.write("/* Do not edit!\n")
    muxfile.write(" * Generated from %s%s*/\n\n" % (gpiopath, gpiofile))
    muxfile.write("#if !defined(_%s_PINMUX_H_)\n" %
                  (gpiofile.partition('-')[2].partition('_')[0]))
    muxfile.write("# define _%s_PINMUX_H_\n\n" %
                  (gpiofile.partition('-')[2].partition('_')[0]))
    pinlist = []
    functionlist = []
    for gpios in root.findall('{http://mcd.rou.st.com/modules.php?name=mcu}GPIO_Pin'):
        port = gpios.get('PortName')
        try:
            pin_nr = int((gpios.get('Name').partition('-')[0].partition(' ')[0].partition('/')[0])[2:])
            for functions in gpios.findall('{http://mcd.rou.st.com/modules.php?name=mcu}PinSignal'):
                for afs in functions.findall('{http://mcd.rou.st.com/modules.php?name=mcu}SpecificParameter'):
                    val = afs.find('{http://mcd.rou.st.com/modules.php?name=mcu}PossibleValue').text.split('_')[1]
                    af_nr = int(val[2:])
                    # Replace  hyphen with underscore
                    function_name = functions.get('Name').replace("-", "_")
                    if "Stingray"      in function_name :
                        continue
                    functionlist.append(function_name)
                    pinlist.append(Stm32pin(port, pin_nr, function_name, af_nr))
        except:
            continue
    i = 0
    functionlist = sorted(set(functionlist))
    for funcname in functionlist:
        function = funcname + "_FUNC"
        muxfile.write("#define %-25s%d\n" % (function, i))
        if "USART" in function :
            # Provide UART with USART given.
            muxfile.write("#define %-25s%d\n" % (function.replace("USART", "UART"), i))
        elif "CAN_" in function :
            # Provide CAN1_ with CAN_ given
            muxfile.write("#define %-25s%d\n" % (function.replace("CAN_", "CAN1_"), i))
        i = i + 1
    muxfile.write("\n")
    sorted_pinlist = sorted(pinlist, key=lambda x: (x.gpio, x.pin, x.af))
    muxfile.write("# define PINMUX(gpio, func) ((gpio == PIN_NONE) ? AF_PIN_NONE : \t\\\n")
    for y in sorted_pinlist :
        function = y.func + "_FUNC"
        muxfile.write("\t((gpio == %s%02d) && (func == %-25s)) ? %2d : \\\n" %
                      (y.gpio, y.pin, function, y.af))
    muxfile.write("\tAF_NO_SUCH_PINFUNC)\n")
    muxfile.write("\n# define TIMERMUX(gpio, timer_nr, channel_nr) ((gpio == PIN_NONE) ? AF_PIN_NONE : \t\\\n")
    for y in sorted_pinlist :
        if y.func.find("TIM") == 0 and y.func.partition('_')[2].find("CH") == 0 :
            timer_nr = int((y.func.partition('_')[0])[3:])
            channel = y.func.partition('_')[2]
            channel_nr = int(channel[2:3])
            if channel.find("N") == 3:
                channel_nr = -channel_nr
            muxfile.write("\t((gpio == %s%02d) && (timer_nr == %2d) && (channel_nr == %2d)) ? %2d : \\\n" %
                          (y.gpio, y.pin, timer_nr, channel_nr, y.af))
    muxfile.write("\tAF_NO_SUCH_PINFUNC)\n#endif\n")
    muxfile.close()

    for funcname in functionlist:
        if "USART" in funcname :
            # Provide UART with USART given.
            function = funcname.replace("USART", "UART").partition('_')[0]
        elif "CAN_" in funcname :
            # Provide CAN1_ with CAN_ given
            function = funcname.replace("CAN_", "CAN1_").partition('_')[0]
        else :
            function = funcname.partition('_')[0]
        if "USART" in function :
            # Provide UART with USART given.
            function = function.replace("USART", "UART"),
        function = function[0] + function[1:].lower()
        subfs = funcname.partition('_')[2].split('_')
        for subf in subfs :
            if len(subf) > 0 :
                subf = subf[0] + subf[1:].lower()
                function += subf
        luastring = "function Get%s()\n  return " % (function)
        pinstring = "{"
        for y in sorted_pinlist :
            if y.func == funcname :
                pinstring += ("\"%s%02d\", " % (y.gpio, y.pin))
        pinstring += "\"PIN_NONE\"}"
        funcpinalt.append(Stm32pinlist(devicename, function, pinstring))

class Stm32Die(object):
    def __init__(self, die, gpio, dma, mcu):
        self.die = die
        self.gpio = gpio
        self.dma = dma
        self.mcu = mcu

dielist = []

if __name__ == "__main__":
    print("stm32_make_pinmux\n")
    print("Generate a PINMUX Header for Nut/OS.")
    print("Give the full path to the .../STM32CubeMX/db/mcu directory as argument.")
    print("Run in .../<nutos>/nut/include/arch/cm3/stm/generated>.")
    parser = argparse.ArgumentParser()
    parser.add_argument("dirpath", nargs='?', help="Directory to parse", default="/devel/cubemx/db/mcu")
    args = parser.parse_args()
    print("Looking in %s" % args.dirpath)
    mcunames = []
    for (dirpath, dnames, filenames) in os.walk(args.dirpath):
        mcunames.extend(filenames)
        break
    mcunames = sorted(mcunames)
    for mcu in mcunames:
        if "STM32" in mcu and ".xml" in mcu:
            try:
                path = "%s/%s" % (args.dirpath, mcu)
                tree = ET.parse(path)
                root = tree.getroot()
            except:
                print("Unknown mcu file %s" % path)
                exit(-1)
            xml_die  = root.find("{http://mcd.rou.st.com/modules.php?name=mcu}Die")
            die = xml_die.text.replace("DIE", "")
            gpio = ""
            dma = ""
            for config in root.findall('{http://mcd.rou.st.com/modules.php?name=mcu}IP'):
                if "GPIO" in config.get('InstanceName'):
                    gpio = config.get('Version').partition('_')[0]
                if "DMA" in config.get('InstanceName'):
                    dma = config.get('Version').partition('_')[0]
            mcu_name = mcu.split(".xml")[0]
            mcu_name = mcu_name.replace("x", "")
            if mcu_name[-1] == 'A':
                mcu_name = "%s%s" % ( mcu_name[0:-2], "A")
            else :
                mcu_name = mcu_name[0:-1]
            dielist.append(Stm32Die(die,gpio, dma, mcu_name))

    f = []
    ip_path = "%s/IP" % args.dirpath
    for (ip_path, dnames, filenames) in os.walk(ip_path):
        f.extend(filenames)
        break
    nutpath = "%s/../../../../" % os.path.dirname(os.path.realpath(sys.argv[0]))
    nutpath = os.path.realpath(nutpath)
    generated_h_path = "%s/nut/include/arch/cm3/stm/generated" % nutpath
    if not os.path.isdir(generated_h_path) :
        try:
            os.mkdir(generated_h_path)
        except:
            print("Failed to create header directory %s" % generated_h_path)
    for f in filenames :
        # Skip F1 file
        # Skip IP/GPIO-STM32H743_gpio_v1_0_Modes.xml  from cubemx 5.0.0
        if "GPIO-STM32" in f and "GPIO-STM32F1" not in f and "GPIO-STM32H743" not in f:
            filename = "%s/%s" % (ip_path, f)
            print("Warping %s" % (filename))
            generate(ip_path, f,generated_h_path)
    generated_conf_path = "%s/nut/conf/arch/cm3/generated" % nutpath
    if not os.path.isdir(generated_conf_path) :
        try:
            os.mkdir(generated_conf_path)
        except:
            print("Failed to create configuration directory%s" % generated_conf_path)
    try:
        luaname = "%s/stm32pinlist.nut" % generated_conf_path
        luafile = open(luaname, 'w')
    except:
        print("Failed to create lua file %s" % luaname)
        exit (-1)
    luafile.write("-- Do not edit! Generated automatically from data at %s*/\n\n" %
                  (dirpath))
    sorted_funcpinalt = sorted(funcpinalt, key=lambda x: (x.func, x.chip))
    lastfunc = ""
    for y in sorted_funcpinalt:
        function = y.func
        if   "Eventout" in function :
            continue
        else:
            dienumber = ""
            for diename in dielist:
                if diename.gpio == y.chip:
                    dienumber = diename.die
                    break
            if dienumber == "":
                print("Failed to match die name to number for %s" % diename.gpio)
            if (function != lastfunc) :
                if (lastfunc != "") :
                    luafile.write("  end\n  return {\"PIN_NONE\"}\nend\n\n")
                luafile.write("function Get%s()\n" % function)
                luafile.write("  if     ")
                lastfunc = function
            else :
                luafile.write("  elseif ")
            luafile.write("c_is_provided(\"HW_MCU_STM32_DIE_%s\")\tthen return %s\n" %
                              (dienumber, y.liste))
    luafile.write("  end\nend\n")
    luafile.close()
    try:
        mcuname = "%s/stm32_mcus.h" % generated_h_path
        mcufile = open(mcuname, 'w')
    except:
        print("Failed to create mcu file %s" % mciname)
        exit (-1)
    mcu_list = []
    for die in dielist:
        mcu = die.mcu[0:9]
        sizes = die.mcu[10]
        if '(' in die.mcu:
            list_string =  die.mcu.split('(')[1]
            list_string =  list_string.split(')')[0]
            sizes   =  list_string.split('-')
        suffix = ''
        if die.mcu[-1] == "A":
            suffix = "A"
        for size in sizes:
            mcu_name = "%sx%s%s" %  (mcu.split('x')[0], size, suffix)
            mcu_list.append(Stm32Die(die.die, die.gpio, die.dma, mcu_name))
    sorted_mcu = sorted(mcu_list, key=lambda x: (x.mcu))
    last_mcu = ""
    mcufile.write("/* Do not edit! Generated automatically from %s.*/\n\n" %
                  dirpath)
    mcufile.write("#if !defined(_STM32_MCUS_H_)\n# define _STM32_MCUS_H_\n\n")
    elsestring = "if"
    for mcu in sorted_mcu:
        if last_mcu == mcu.mcu:
            continue
        mcufile.write("#%s defined(MCU_%s)\n" % (elsestring, mcu.mcu))
        mcufile.write("# define DIE 0x%s\n" % mcu.die)
        if "STM32F1"  in mcu.mcu:
            mcufile.write("# include <arch/cm3/stm/stm32f1_pinremap.h>\n")
        else:
            mcufile.write("# include <arch/cm3/stm/generated/%s_pinmux.h>\n" %
                          mcu.gpio.lower())
        mcufile.write("# include <arch/cm3/stm/generated/%s_dma.h>\n" %
                      mcu.dma.lower())
        last_mcu = mcu.mcu
        elsestring = "elif"
    mcufile.write("#endif\n\n#endif\n")
    mcufile.close()
