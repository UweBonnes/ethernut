#include <stdio.h>
#include <string.h>

#include <sys/timer.h>
#include <arpa/inet.h>

#include "utils.h"
#include "uart.h"

prog_char presskey_P[] = "Press any key to stop\n";

char inbuff[32];

/*
 * Wait for a character from the serial port.
 */
u_char GetChar(void)
{
    u_char ch;

    if ((inb(USR) & BV(RXC)) == 0)
        return 0;
    ch = inb(UDR);
    return ch;
}

/*
 * Get a line of input.
 */
int GetLine(u_char * line, int size)
{
    int cnt = 0;
    u_char ch;

    GetChar();
    for (;;) {
        if (cnt > size - 1)
            break;
        while ((ch = GetChar()) == 0);
        if (ch < 32)
            break;
        putchar(ch);
        *line++ = ch;
        cnt++;
    }
    *line = 0;
    putchar('\n');

    return cnt;
}

/*
 * Get a line of input.
 */
u_char *GetIP(u_char * prompt, u_char * value)
{
    static prog_char badip_P[] = "Bad IP address";

    for (;;) {
        printf("%s (%s): ", prompt, value);
        if (GetLine(inbuff, sizeof(inbuff)) == 0)
            break;
        if (inet_addr(inbuff) != (u_long) (-1L)) {
            strcpy(value, inbuff);
            break;
        }
        puts_P(badip_P);
    }
    return value;
}

/*
 * Automatic baudrate detection.
 *
 * Switch to different baud rates and wait 
 * for a space character.
 */
int DetectSpeed(void)
{
    u_char bstab[] = { 
        1,      /* 115200 @  3.6864 */
        7,      /* 115200 @ 14.7456 */
        23,     /*   9600 @  3.6864, 19200 @  7.3728, 38400 @ 14.7456 */
        11,     /*  19200 @  3.6864, 38400 @  7.3728, 76800 @ 14.7456 */
        12,     /*  19200 @  4.0000, 38400 @  8.0000, 76800 @ 16.0000 */
        25,     /*   9600 @  4.0000, 19200 @  8.0000, 38400 @ 16.0000 */
        5,      /*  38400 @  3.6864, 76800 @  7.3728 */
        47,     /*   9600 @  7.3728, 19200 @ 14.7456 */
        51,     /*   9600 @  8.0000, 19200 @ 16.0000 */
        3,      /* 115200 @  7.3728 */
        9,      /* 115200 @ 18.4320 */
        2,      /*  76800 @  3.6864 */
        14,     /*  76800 @ 18.4320 */
        29,     /*  38400 @ 18.4320 */
        64,     /*  19200 @ 20.0000 */
        59,     /*  19200 @ 18.4320 */
        95,     /*   9600 @ 14.7456 */
        103,    /*   9600 @ 16.0000 */
        119,    /*   9600 @ 18.4320 */
        129     /*   9600 @ 20.0000 */
    };
    u_char bsx;
    int bs;
    u_short i;
    u_char t;
    u_char rec;
    u_char ict;                 /* Imagecraft dummy */

    /*
     * Enable UART transmitter and receiver.
     */
    outb(UCR, BV(RXEN) | BV(TXEN));

    /*
     * Sixteen retries.
     */
    for (t = 1; t < 16; t++) {
        for (bsx = 0; bsx < sizeof(bstab); bsx++) {
            bs = bstab[bsx];

            /*
             * Set baudrate selector.
             */
            Delay(1000);
            if ((inb(USR) & BV(RXC)) != 0)
                ict = inb(UDR);
            outb(UBRR, (u_char) bs);
            Delay(1000);
            if ((inb(USR) & BV(RXC)) != 0)
                ict = inb(UDR);

            /*
             * Now wait for some time for a character received.
             */
            for (i = 0; i < 40; i++) {
                if ((inb(USR) & BV(RXC)) != 0)
                    break;
                Delay(1000);
            }
            rec = 0;
            for (i = 1; i; i++) {
                if ((inb(USR) & BV(RXC)) != 0) {
                    if (inb(UDR) == 32) {
                        if (rec++)
                            return bs;
                        i = 1;
                    } else {
                        rec = 0;
                        break;
                    }
                } else if (rec)
                    Delay(100);
            }
        }
    }
    outb(UBRR, 23);
    return -1;
}

