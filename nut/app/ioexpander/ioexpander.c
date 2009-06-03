/*
 * Copyright (C) 2009 by Rittal GmbH & Co. KG,
 * Ulrich Prinz <prinz.u@rittal.de> All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor the names of
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY EMBEDDED IT AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL EMBEDDED IT
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * For additional information see http://www.ethernut.de/
 *
 */

/*
 * $Log$
 *
 * Revision 1.0  2009/04/13 ulrichprinz
 * First checkin, driver for PCA9555 I2C I/O-Expander (currently SAM7X256 is 
 * tested only)
 *
 */

/*!
 * \example ioexpander/ioexpander.c
 */

#include <stdio.h>
#include <io.h>

#include <cfg/arch.h>
#include <dev/board.h>

#include <sys/thread.h>
#include <sys/timer.h>
#include <dev/pca9555.h>

#define	LED_OFF		0
#define	LED_ON		1

#define	LED(led,state)	if( state == LED_OFF )IOExpSetBit( 1, led );else IOExpClrBit( 1, led )
#define	LED1(state)	LED( 0, state )
#define	LED2(state)	LED( 1, state )
#define	LED3(state)	LED( 2, state )
#define	LED4(state)	LED( 3, state )
#define	LED5(state)	LED( 4, state )

#define	DLED(led,state)	if( state == LED_OFF )IOExpSetBit( 0, led + 4 );else IOExpClrBit( 0, led + 4 )
#define	DLED1(state)	DLED( 0, state )
#define	DLED2(state)	DLED( 1, state )
#define	DLED3(state)	DLED( 2, state )
#define	DLED4(state)	DLED( 3, state )

#define KEY1 (1<<0)
#define KEY2 (1<<1)
#define KEY3 (1<<2)
#define KEY4 (1<<3)

uint8_t ledid = 0;

/******************************************************************/
THREAD(Thread1, arg)
/******************************************************************/
{
    NutThreadSetPriority(128);
    for (;;) {
		LED( ledid, LED_OFF );
		if( ++ledid > 4 )
		{
			ledid = 0;
		}
		LED( ledid, LED_ON );

        NutSleep(250);
    }
}

/******************************************************************/
THREAD(Thread2, arg)
/******************************************************************/
{
	uint8_t key;
	uint8_t flag = 1;
	int rc;

	printf( "Key and LED test for PCA9555\n" );

    NutThreadSetPriority(128);
    for (;;)
	{
		key = 0;
		rc = IOExpRawRead( 0, &key);
		printf( "IOER rc=%d key=0x%02x\n", rc, key);
		if( rc >= 0)
		{
			if( flag == 0 )
			{
				flag = 1;

				if( key & KEY1)	printf( "Key 1 pressed\n" );
				if( key & KEY2)	printf( "Key 2 pressed\n" );
				if( key & KEY3)	printf( "Key 3 pressed\n" );
				if( key & KEY4)	printf( "Key 4 pressed\n" );
			}
		}
		else
		{
			flag = 0;
		}

        NutSleep(125);
    }
}

/******************************************************************/
int main(void)
/******************************************************************/
{
    u_long baud = 115200;
    /*
     * Register the UART device, open it, assign stdout to it and set
     * the baudrate.
     */
    NutRegisterDevice(&DEV_DEBUG, 0, 0);
    freopen(DEV_DEBUG_NAME, "w", stdout);
    _ioctl(_fileno(stdout), UART_SETSPEED, &baud);

	baud = 100000;
	TwInit( 0 ); /* par = slave address but we are master */
	TwIOCtl( TWI_SETSPEED, &baud);
	IOExpInit();

    /*
     * Start two additional threads. All threads are started with
     * priority 64.
     */
    NutThreadCreate("led", Thread1, 0, 512);
    NutThreadCreate("key", Thread2, 0, 512);

    /*
     * Endless loop in main thread.
     */
    for (;;)
	{
        NutSleep(5000);
    }
    return 0;
}
