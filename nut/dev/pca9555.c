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
 * First checkin, new twi driver for pca9555 (currently SAM7X256 is tested
 * only)
 *
 */
#include <cfg/os.h>
#include <dev/twif.h>

#include <sys/event.h>
#include <dev/pca9555.h>

#ifndef I2C_SLA_RTC
#define I2C_SLA_IOEXP     0x23
#endif

static uint8_t portValue[2] = { 0x00, 0x1f };

/*****************************************************************/
int IOExpInit( void )
/*****************************************************************/
{
	uint8_t cmd[2];

    cmd[0] = 0x0f;  /* port 0 input   */
	cmd[1] = 0x00;  /* port 1 output  */
	if( TwMasterRegWrite( I2C_SLA_IOEXP, 0x06, 1, cmd, 2, 50 ) == -1 )
	{
		// printf( "Init Chip not responding\n");
		return -1;
	}

	cmd[0] = 0xff;  /* 4 polarity reg 0 */
	if( TwMasterRegWrite( I2C_SLA_IOEXP, 0x04, 1, cmd, 1, 50 ) == -1 )
	{
		// printf( "Init Chip not responding\n");
		return -1;
	}
	cmd[0] = 0xf0;  /* 4 polarity reg 1 */
	if( TwMasterRegWrite( I2C_SLA_IOEXP, 0x02, 1, cmd, 1, 50 ) == -1 )
	{
		return -1;
	}

	return 0;
}

/*****************************************************************/
int IOExpRawWrite ( uint8_t port, uint8_t value )
/*****************************************************************/
{
	if ( port > 1 )	return -1;

	portValue[port] = value;

	if( TwMasterRegWrite( I2C_SLA_IOEXP, 0x02 + port, 1, &portValue[port], 1, 50 ) == -1 )
		return -1;
	return 0;
}

/*****************************************************************/
int IOExpRawRead ( uint8_t port, uint8_t *value )
/*****************************************************************/
{
	if( port > 1 ) return -1;

	if( TwMasterRegRead( I2C_SLA_IOEXP, port, 1, value, 1, 50) == -1)
		return -1;
	return 0;
}


/*****************************************************************/
int IOExpGetBit ( uint8_t port, uint8_t bit, uint8_t *value )
/*****************************************************************/
{
	uint8_t val;

	if( port > 1 ) return -1;

	if( TwMasterRegRead( I2C_SLA_IOEXP, 0, 1, &val, 1, 50))
		return -1;
	else
		*value = (( val & ( 1 << bit )) == 0 ) ? 0 : 1;

	return 0;
}

/*****************************************************************/
int IOExpSetBit ( uint8_t port, uint8_t bit )
/*****************************************************************/
{
	if( port > 1 ) return -1;

	portValue[port] |= ( 1 << bit );

	if( TwMasterRegWrite( I2C_SLA_IOEXP, 0x02+port, 1, &portValue[port], 1, 50 ) == -1 )
		return -1;
	return 0;
}

/*****************************************************************/
int IOExpClrBit ( uint8_t port, uint8_t bit )
/*****************************************************************/
{
	if( port > 1 ) return 0;

	portValue[port] &= ~( 1 << bit );

	if( TwMasterRegWrite( I2C_SLA_IOEXP, 0x02+port, 1, &portValue[port], 1, 50 ) == -1 )
		return -1;
	return 0;
}

