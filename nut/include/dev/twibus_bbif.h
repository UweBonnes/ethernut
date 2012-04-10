#ifndef _DEV_TWIBUS_BBIF_H_
#define _DEV_TWIBUS_BBIF_H_

#include <sys/types.h>
#include <dev/twif.h>

extern NUTTWIBUS TwBbifBus;

#ifndef DEF_TWIBUS
#define DEF_TWIBUS TwBbifBus
#endif

#endif /* _DEV_TWIBUS_BBIF_H_ */
