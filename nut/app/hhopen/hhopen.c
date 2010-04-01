/*
 * Copyright (C) 2009 by Rittal GmbH & Co. KG,
 * Ulrich Prinz <prinz.u@rittal.de>
 *
 * All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * For additional information see http://www.ethernut.de/
 */

/*!
 * $Id$
 */

/*!
 * \example hhopen/hhopen.c
 *
 * Simple RS232-RS232 tranceiver. Used for the first test of the
 * Hand-Held-Open, an open source, open hardware OBD2 diagnosis device
 *
 */

#include <dev/board.h>

#include <sys/heap.h>
#include <sys/thread.h>
#include <sys/timer.h>
#include <sys/version.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>

#define HHOPENVERSION "1.0.0"

#define BUFFERSIZE  128

#define DEV_DXM 			DEV_UART0
#define DEV_DXM_NAME 		DEV_UART0_NAME
#define DEV_DXM_SPEED_LOW	9600
#define DEV_DXM_SPEED		250000

#define DEV_USB 			DEV_UART1
#define DEV_USB_NAME 		DEV_UART1_NAME
#define DEV_USB_SPEED 		125000

typedef struct {
	FILE * devusb;
	FILE * devdxm;
	char connected;
} CHANNEL_t;

const char str_start[] PROGMEM = "starting %S";
const char str_fail[] PROGMEM = "FAIL\r\n";
const char str_ok[]   PROGMEM = "OK\r\n";

/*
 * Transfer data from input stream to output stream.
 */
void StreamCopy(FILE * ostream, FILE * istream, char *cop)
{
    int cnt;
    char *buff;

    buff = malloc(BUFFERSIZE);
	while (*cop) {
        if ((cnt = fread(buff, 1, BUFFERSIZE, istream)) <= 0)
            break;
        if (*cop && (cnt = fwrite(buff, 1, cnt, ostream)) <= 0)
            break;
        if (*cop && fflush(ostream))
            break;
    }
    *cop = 0;
    free(buff);
}

/*
 * From DXM to USB.
 */
THREAD(dxm2usb, arg)
{
	CHANNEL_t *cdp = arg;
//	int got;
//  char *buff;

//  buff = malloc(BUFFERSIZE);

	for(;;)
	{
		if( cdp->connected) {
			NutThreadSetPriority(64);
			/*
			*buff = fgetc( cdp->devdxm);
			fputc( *buff, cdp->devusb);
		    */
			/*
			 * copy data from DXM to USB
		    got = _read(_fileno(cdp->devdxm), buff, BUFFERSIZE);
		    _write(_fileno(cdp->devusb), buff, got);
			 */
	        StreamCopy(cdp->devusb, cdp->devdxm, &cdp->connected);
	        NutThreadSetPriority(128);
		}
        NutThreadYield();
	}
}

/*
 * From USB to DXM
 */
THREAD(usb2dxm, arg)
{
	CHANNEL_t *cdp = arg;
//	int got;
//  char *buff;

//    buff = malloc(BUFFERSIZE);

	for(;;)
	{
		if( cdp->connected) {
			NutThreadSetPriority(64);
			/*
			*buff = fgetc( cdp->devusb);
			fputc( *buff, cdp->devusb);

			fputc( *buff, cdp->devdxm);
			*/
		    //got = _read(_fileno(cdp->devusb), buff, BUFFERSIZE);
			/* local echo
			 * disabled as DXM has echo itself
			_write(_fileno(cdp->devusb), buff, got);
			*/
		    // _write(_fileno(cdp->devdxm), buff, got);

	        StreamCopy(cdp->devdxm, cdp->devusb, &cdp->connected);
	        NutThreadSetPriority(128);
		}
        NutThreadYield();
	}
}


/*
 * ALIFE thread just toggling PORTF7
 *
 */
THREAD(ALife, arg)
{
	NutThreadSetPriority(64);
	for(;;) {
		PORTF ^= (1<<PF7);
		NutSleep( 500);
	}
}

/*
 * DXM control functions
 *
 */
#define HHBACKL_PORT	PORTB
#define HHBACKL_DDR		DDRB		/* Port init done in nutinit.c */
#define HHBACKL_RED		(1<<PB6)
#define HHBACKL_GREEN	(1<<PB5)
#define HHBACKL_BLUE	(1<<PB7)

#define HHDXM_PORT		PORTE
#define HHDXM_DDR		DDRE
#define HHDXM_RESET		(1<<PE7)
#define hhdxm_reset_h()	HHDXM_PORT |= HHDXM_RESET
#define hhdxm_reset_l()	HHDXM_PORT &= ~HHDXM_RESET
#define HHDXM_CTS		(1<<PE3)
#define HHDXM_RTS		(1<<PE2)
#define hhdxm_cts_ena()	HHDXM_PORT &= ~HHDXM_CTS
#define hhdxm_cts_dis()	HHDXM_PORT |= HHDXM_CTS
#define HHDXM_BOOT0		(1<<PE6)
#define hhdxm_boot0_h()	HHDXM_PORT |= HHDXM_BOOT0
#define hhdxm_boot0_l()	HHDXM_PORT &= ~HHDXM_BOOT0

void DXM_Reset( void)
{
	hhdxm_reset_l();	/* Set DXM to RESET */
	hhdxm_boot0_l();	/* Prevent from starting bootloader */
	NutSleep( 200);		/* Sleep for 100ms) */
	hhdxm_reset_h();	/* Release RESET from DXM */
}

void DXM_Init( void)
{
	HHDXM_DDR |= HHDXM_CTS|HHDXM_RESET|HHDXM_BOOT0;	/* Set DXM control line as output */
	HHDXM_DDR &= ~HHDXM_RTS;						/* Set DXM status lines as input */
	hhdxm_boot0_h();
	hhdxm_reset_l();
}

const char dxmstr_atz[]  PROGMEM = "ATZ\r\n";
const char dxmstr_dxm1[] PROGMEM = "DIAMEX";
const char dxmstr_atsb[] PROGMEM = "ATSB6\r\n";

int DMX_Reconfigure( FILE * dev, FILE * debug)
{
	char *buff;
	char trigger, l;
	u_long baud = 250000UL;

    buff = malloc(BUFFERSIZE);

	/* set baudrate 250kbaud */
	fputs_P(dxmstr_atsb, dev);
	fflush( dev);
	while( fgetc(dev) != '6');

	while( fgetc( dev) != '>');
	//fputs_P(PSTR("."), debug);

	/* restart with new baudrate */
	fputs_P(dxmstr_atz, dev);
	fflush( dev);
	_ioctl( _fileno(dev), UART_SETSPEED, &baud);

	/* wait for DIAMEX string */
	trigger = 1;
	do {
		fgets( buff, BUFFERSIZE, dev);
		fprintf_P( debug, PSTR("%s\r\n"), buff);
		l = strlen(buff);
		if( strncmp_P( buff, dxmstr_dxm1, l)==0) trigger = 0;
	} while( trigger);
	fputs(PSTR("250000\r\n"), debug);

	free(buff);
	return 0;
}

/*
 * LCD backlight control functions
 *
 */
void Backlight_Init( void)
{
	/* Set Backlight to yellow */
	HHBACKL_PORT |= HHBACKL_BLUE|HHBACKL_GREEN;
}

void Backlight_Color( int r, int g, int b)
{
}

/*
 * Main application routine.
 *
 * Nut/OS automatically calls this entry after initialization.
 */
int main(void)
{
	CHANNEL_t cd;
    u_long baud = DEV_DXM_SPEED;
	u_long bufsz = BUFFERSIZE;
	char * buff = NULL;
	int state;
#ifdef DEBUG_ISM
	int os = 0xff;
#endif

	/* Init backlight of LCD */
	Backlight_Init();
	/* Initalize DXM hardware interconnection */
	DXM_Init();

    /*
     * Register our devices.
     */
    NutRegisterDevice(&DEV_USB, 0, 0);	/* USART for communication to USB chip */
    NutRegisterDevice(&DEV_DXM, 0, 0);	/* USART fpr communication to DXM module */

    /*
     * Setup the uart1 device for USB0 access.
     */
	cd.devusb = fopen(DEV_USB_NAME, "r+b");
    baud = DEV_USB_SPEED;
    _ioctl(_fileno(cd.devusb), UART_SETSPEED, &baud);
	_ioctl(_fileno(cd.devusb), UART_SETRXBUFSIZ, &bufsz);
	_ioctl(_fileno(cd.devusb), UART_SETTXBUFSIZ, &bufsz);

	fprintf_P(cd.devusb, PSTR("\r\nNutO/S version %s"), NutVersionString());
	fputs_P( PSTR("\r\nHHopen DXM USB Bridge version "HHOPENVERSION"\r\n"), cd.devusb);

    /*
     * Setup the uart0 device for DXM access.
     */
    baud = DEV_DXM_SPEED_LOW;
    cd.devdxm = fopen(DEV_DXM_NAME, "r+b");
    _ioctl(_fileno(cd.devdxm), UART_SETSPEED, &baud);
	_ioctl(_fileno(cd.devdxm), UART_SETRXBUFSIZ, &bufsz);
	_ioctl(_fileno(cd.devdxm), UART_SETTXBUFSIZ, &bufsz);

	/*
     * Start simple life-check thread toggling port F7.
     */
    NutThreadCreate("alife", ALife, NULL, 64);

	/*
	 * Release DXM from RESET
	 */
	DXM_Reset();
	hhdxm_cts_ena();

	/*
	 * Talk to DXM and reconfigure it to 250000baud
	 */
//	DMX_Reconfigure( cd.devdxm, cd.devusb);

    /*
     * Start a RS232 receiver thread.
     */
    fputs_P( PSTR("start dxm2usb "), cd.devusb);
    if( NutThreadCreate("dxm2usb", dxm2usb, &cd, 128) == NULL)
		fputs_P( str_fail, cd.devusb);
	else
		fputs_P( str_ok, cd.devusb);

    /*
     * Start a RS232 sender thread.
     */
    fputs_P( PSTR("start dxm2usb "), cd.devusb);
	if( NutThreadCreate("usb2dxm", usb2dxm, &cd, 128) == NULL)
		fputs_P( str_fail, cd.devusb);
	else
		fputs_P( str_ok, cd.devusb);

    /*
     * Now loop endless for connections.
     */
    cd.connected = 0;
	state = 0;
    for (;;) {
		if( cd.connected == 0) {

#ifdef DEBUG_ISM
			if( os != state) {
				fprintf_P( cd.devusb, PSTR("ISM %d\r\n"), state);
				os = state;
			}
#endif
			switch( state)
			{
				case 0:
					buff = malloc(BUFFERSIZE);
					state = 1;
					break;
				case 1:
					fgets( buff, BUFFERSIZE, cd.devdxm);
					fputs( buff, cd.devusb);
					if( strlen(buff)==0)
						break;

					if( strncmp_P( buff, dxmstr_dxm1, strlen_P(dxmstr_dxm1))==0)
						state = 2;
					break;
				case 2:
					*buff = fgetc( cd.devdxm);
					fputc( *buff, cd.devusb);
					if( *buff == '>')
						state = 3;
					break;
				case 3:
					fputs_P( dxmstr_atsb, cd.devdxm);
					fflush(cd.devdxm);
					state = 4;
					break;
				case 4:
					*buff = fgetc( cd.devdxm);
					fputc( *buff, cd.devusb);
					if( *buff == '>')
						state = 5;
					break;
				case 5:
					fputs_P( dxmstr_atz, cd.devdxm);
					fflush(cd.devdxm);
					baud = DEV_DXM_SPEED;
					_ioctl( _fileno(cd.devdxm), UART_SETSPEED, &baud);
					state = 6;
					break;
				case 6:
					fgets( buff, BUFFERSIZE, cd.devdxm);
					fputs( buff, cd.devusb);
					if( strlen(buff)==0)
						break;

					if( strncmp_P( buff, dxmstr_dxm1, strlen_P(dxmstr_dxm1))==0)
						state = 7;
					break;
				case 7:
					*buff = fgetc( cd.devdxm);
					fputc( *buff, cd.devusb);
					if( *buff == '>') {
						cd.connected = 1;
						free(buff);
					}
					break;
				default:
					fputs_P(PSTR("ERROR ISM\r\n"), cd.devusb);
					break;
			}
			NutSleep( 1);
		}
		else {
			NutSleep(1000);
		}
    }
    return 0;
}
