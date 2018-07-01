# Nutos GDB thread awareness

This is a python script designed to add NutOs thread awareness to gdb.
Heavily base on https://github.com/gsmcmullin/gdb_chibios and ideas from
https://github.com/glward/chibios_gdb/blob/master/chibios.py

Tested with Stm32F3 and L4.

# Requirements

This script requires a copy of arm-none-eabi-gdb which was built with
Python support. The GNU Arm Embedded Toolchain at
https://developer.arm.com/open-source/gnu-toolchain/gnu-rm
contains arm-none-eabi-gdb-py
The elf file for the debugger needs to contain debugging symbols.
At the moment (20180701), MCU_USE_CORTEX_FPU or stack layout will not fit

# Usage

Source the nutos.py script in gdb:

```
(gdb) source /path/to/nutos.py
```

You can then use the following commands:

* info threads

# Example
> arm-none-eabi-gdb-py *elf
Reading symbols from gpio.elf...done.
(gdb) source ~/devel/ethernut_sf/nut/tools/gdb/nutos.py
Interrupt handler!
[New thread 'main']
[New thread 'idle']
(gdb) inf thr
  Id   Target    Status     Frame
  1    main      Sleeping   0x800046b in NutThreadResume ()
* 2    idle      Running    0x800057c in NutTimerIntr ()
(gdb) thr 1
(gdb) bt
#0  NutThreadResume () at /home/bon/devel/ethernut_sf/nut/os/thread.c:232
#1  0x080007ba in NutSleep (ms=ms@entry=255) at /home/bon/devel/ethernut_sf/nut/os/timer.c:743
#2  0x080001e8 in main () at /home/bon/devel/ethernut_projects/gpio/nucleo_l476rg/../gpio.c:109
(gdb)
