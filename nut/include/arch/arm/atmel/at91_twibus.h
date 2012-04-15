#ifndef _DEV_TWIBUS_AT91_H_
#define _DEV_TWIBUS_AT91_H_

#include <sys/types.h>
#include <cfg/arch.h>

#include <dev/twif.h>

extern NUTTWIBUS At91TwiBus;

#ifndef DEF_TWIBUS
#define DEF_TWIBUS At91TwiBus
#endif

#endif /* _DEV_TWIBUS_AT91_H_ */
