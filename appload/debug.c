
#include <avr/io.h>

#include "debug.h"

#ifdef NUTDEBUG

static char buf[5];
static char hexdigit[] = "0123456789ABCDEF";

void Debug(char *cp)
{
    while (*cp) {
        while ((UCSR0A & _BV(UDRE)) == 0);
        UDR0 = *cp;
        if (*cp++ == '\n') {
            Debug("\r");
        }
    }
}

void DebugUShort(unsigned short val)
{
    register unsigned char i;

    for (i = 4; i-- > 0;) {
        buf[i] = hexdigit[val & 0x0F];
        val >>= 4;
    }
    buf[4] = 0;
    Debug(buf);
}

void DebugInit(void)
{
    UBRR0L = 7;
    UCSR0B = (1 << RXEN) | (1 << TXEN);
}

#endif
