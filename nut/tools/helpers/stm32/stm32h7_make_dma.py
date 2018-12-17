#!/usr/bin/env python3
import xml.etree.ElementTree as ET
import argparse
import time
import os
import sys

class DmaRequest(object):
    def __init__(self, request, value):
        self.request = request
        self.value = value

if __name__ == "__main__":
    print("stm32_make_dmah7\n")
    print("Generate a STM32H7 (B)DMA device Header for Nut/OS.")
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
    requests = []
    for mcu in mcunames:
        if mcu.startswith("DMA-STM32H7") and ".xml" in mcu:
            try:
                path = "%s/%s" % (args.dirpath, mcu)
                tree = ET.parse(path)
                root = tree.getroot()
            except:
                print("Unknown mcu file %s" % path)
                exit(-1)
            value = 0
            for refparameter in root.findall(
                    "{http://mcd.rou.st.com/modules.php?name=mcu}RefParameter") :
                if refparameter.get('Name') == "Request" :
                    for request in refparameter.findall(
                            "{http://mcd.rou.st.com/modules.php?name=mcu}PossibleValue") :
                        requests.append(DmaRequest(request.get('Value'), value))
                        value = value + 1
        if mcu.startswith("BDMA-STM32H7") and ".xml" in mcu:
            try:
                path = "%s/%s" % (args.dirpath, mcu)
                tree = ET.parse(path)
                root = tree.getroot()
            except:
                print("Unknown mcu file %s" % path)
                exit(-1)
            value = 0
            for refparameter in root.findall(
                    "{http://mcd.rou.st.com/modules.php?name=mcu}RefParameter") :
                if refparameter.get('Name') == "Request" :
                    for request in refparameter.findall(
                            "{http://mcd.rou.st.com/modules.php?name=mcu}PossibleValue") :
                        name = request.get('Value')
                        if not ("MEM2MEM" in name or "GENERATOR" in name or
                                "ADC3" in name):
                            name = name.replace("BDMA", "DMA")
                        requests.append(DmaRequest(name, value))
                        value = value + 1
    nutpath = "%s/../../../../" % os.path.dirname(os.path.realpath(sys.argv[0]))
    nutpath = os.path.realpath(nutpath)
    generated_h_path = "%s/nut/include/arch/cm3/stm/generated" % nutpath
    dmaname = "%s/stm32h753_dma.h" % generated_h_path
    try:
        dmafile = open(dmaname, 'w')
    except:
        print("Failed to create dma file %s" % dmaname)
        exit -1
    dmafile.write("/* Do not edit! Generated automatically from %s/(B)DMA_STM32H7**/\n\n" %
                  (dirpath))
    dmafile.write("#if !defined(_STM32H753_DMA_H_)\n")
    dmafile.write("# define _STM32H753_DMA_H_\n\n")
    dmafile.write("#define STM32_DMA_VERSION 13\n")
    for request in requests:
        dmafile.write("#define %-25s %3d\n" % (request.request, request.value))
    dmafile.write("\n")
    for request in requests:
        index = ""
        if "BDMA" in request.request:
            index = "BDMA_ANY"
        else:
            index = "DMA_ANY"
        name = request.request.replace("_REQUEST", "")
        dmafile.write("#define %-22s {{%8s, %-25s},{}}\n" %
                      (name, index, request.request))
        if "USART" in name:
            uart = name.replace("USART", "UART")
            dmafile.write("#define %s %s\n" % (uart, name))
    dmafile.write("\n#endif\n")


