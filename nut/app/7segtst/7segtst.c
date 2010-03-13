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
 * First checkin, new twi driver for AS1108 3 Digit 7-Segment driver 
 * (currently SAM7X256 is tested only)
 *
 */

/*!
 * \example 7segtst/7segtst.c
 *
 */

#include <cfg/os.h>

#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>

#include <dev/board.h>
#include <dev/gpio.h>

#include <sys/heap.h>
#include <sys/thread.h>
#include <sys/timer.h>

#include <dev/spi_7seg.h>

#ifdef NUTDEBUG
#include <sys/osdebug.h>
#endif




/*
 * Main application routine.
 *
 * Nut/OS automatically calls this entry after initialization.
 */
/******************************************************************/
int main(void)
/******************************************************************/
{


	u_long baud = 115200;
	uint8_t rc;
	unsigned int count=0;
	/*
     * Register the UART device, open it, assign stdout to it and set 
     * the baudrate.
     */
    NutRegisterDevice(&DEV_DEBUG, 0, 0);
    freopen(DEV_DEBUG_NAME, "w", stdout);
    _ioctl(_fileno(stdout), UART_SETSPEED, &baud);
    /*
     * Initialize digital I/O.
     */
    printf("initSPI_disp7seg\n");
	
	//rc = NutRegisterSpiDevice(&devSpi7SEG,&nodeSpi7SEG.node_bus,nodeSpi7SEG.node_cs);
	rc = NutRegisterSpiDevice(&devSpi7SEG,&NUT_CONFIG_7SEG_SPIBUS,NUT_CONFIG_7SEG_CS);
	if (rc != 0){
		printf("spi init failed\n");
		while(1);
	}
	printf("display_7seg\n");

	 for (;;)
	{
		printf( devSpi7Seg, "%3d\n", count++);
		if(count >999)count=0;
		NutSleep(200);
	}
    return 0;
}
