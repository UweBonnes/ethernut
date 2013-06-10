#ifndef DEV_CONSOLE
#define DEV_CONSOLE devUsartStm32_6
#endif

#ifndef DEV_CONSOLE_NAME
#define DEV_CONSOLE_NAME devUsartStm32_6.dev_name
#endif

/* Ethernet interface */

#include <dev/stm32_emac.h>
#ifndef DEV_ETHER_NAME
#define DEV_ETHER_NAME  "eth0"
#endif

#include <arch/cm3/board/f4_discovery.h>
