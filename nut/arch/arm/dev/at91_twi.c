/*
 * Copyright (C) 2001-2005 by EmbeddedIT,
 * Ole Reinhardt <ole.reinhardt@embedded-it.de> All rights reserved.
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
 * Revision 1.8  2009/01/17 11:26:37  haraldkipp
 * Getting rid of two remaining BSD types in favor of stdint.
 * Replaced 'u_int' by 'unsinged int' and 'uptr_t' by 'uintptr_t'.
 *
 * Revision 1.7  2008/10/03 11:31:27  haraldkipp
 * Added TWI support for the AT91SAM9260.
 *
 * Revision 1.6  2008/08/11 06:59:04  haraldkipp
 * BSD types replaced by stdint types (feature request #1282721).
 *
 * Revision 1.5  2008/02/15 16:58:41  haraldkipp
 * Spport for AT91SAM7SE512 added.
 *
 * Revision 1.4  2007/12/09 22:17:23  olereinhardt
 * fixed typo
 *
 * Revision 1.3  2007/12/09 21:52:27  olereinhardt
 * Added doxygen tags
 *
 * Revision 1.2  2007/10/04 19:51:56  olereinhardt
 * Support for sam7s added
 *
 * Revision 1.1  2007/09/06 19:36:00  olereinhardt
 * First checkin, new twi driver for at91 (currently SAM7X256 is supported
 * only)
 *
 */

#include <arch/arm.h>
#include <dev/irqreg.h>

#include <sys/event.h>
#include <sys/atom.h>
#include <sys/timer.h>
#include <sys/thread.h>
#include <sys/heap.h>

#include <dev/twif.h>

/*!
 * \addtogroup xgAt91Twi
 */
/*@{*/

HANDLE tw_mm_mutex;                 /* Exclusive master access. */
HANDLE tw_mm_que;                   /* Threads waiting for master transfer done. */

static uint8_t tw_mm_sla;            /* Destination slave address. */
static volatile uint8_t tw_mm_err;   /* Current master mode error. */
static uint8_t tw_mm_error;          /* Last master mode error. */

static CONST uint8_t *tw_mt_buf;     /* Pointer to the master transmit buffer. */
static volatile uint16_t tw_mt_len;  /* Number of bytes to transmit in master mode. */
static volatile uint16_t tw_mt_idx;  /* Current master transmit buffer index. */

static uint8_t *tw_mr_buf;           /* Pointer to the master receive buffer. */
static volatile uint16_t tw_mr_siz;  /* Size of the master receive buffer. */
static volatile uint16_t tw_mr_idx;  /* Current master receive buffer index. */

#if defined (MCU_AT91SAM7X256) || defined (MCU_AT91SAM7S256) || defined (MCU_AT91SAM7SE512)

#define TWI_PIO_ASR PIOA_ASR
#define TWI_PIO_PDR PIOA_PDR
#define TWI_PIO_MDER PIOA_MDER

   #if defined (MCU_AT91SAM7X256)
      #define TWI_TWD  PA10_TWD_A
      #define TWI_TWCK PA11_TWCK_A
   #elif defined (MCU_AT91SAM7S256) || defined (MCU_AT91SAM7SE512)
      #define TWI_TWD  PA3_TWD_A
      #define TWI_TWCK PA4_TWCK_A
   #endif
#endif

#if defined (MCU_AT91SAM9260)
#define TWI_PIO_ASR     PIOA_ASR
#define TWI_PIO_PDR     PIOA_PDR
#define TWI_PIO_MDER    PIOA_MDER
#define TWI_TWD         PA23_TWD_A
#define TWI_TWCK        PA24_TWCK_A
#endif

/*!
 * \brief Hardware near debugging of TWI system
 */
#define US_DBG_PINA 19  // Scope D6: INT
#define US_DBG_PINB 20  // Scope D6: INT

#define US_DBG_PIN0	19	// Scope D5: NACK
#define US_DBG_PIN1 20	// Scope D2: TXCOMP
#define US_DBG_PIN2 21	// Scope D3: RXRDY
#define US_DBG_PIN3 22	// Scope D4: TXRDY

#if defined(US_DBG_PIN0) || defined(US_DBG_PIN1) || defined(US_DBG_PIN2) || defined(US_DBG_PIN3)
#include <dev/gpio.h>
#endif

#if defined(US_DBG_PINA)
#define USDBG_PINA_INIT() GpioPinConfigSet(NUTGPIO_PORTA, US_DBG_PINA, GPIO_CFG_OUTPUT)
#define USDBG_PINA_HIGH() GpioPinSetHigh(NUTGPIO_PORTA, US_DBG_PINA)
#define USDBG_PINA_LOW()  GpioPinSetLow(NUTGPIO_PORTA, US_DBG_PINA)
#else
#define USDBG_PINA_INIT()
#define USDBG_PINA_HIGH()
#define USDBG_PINA_LOW()
#endif

#if defined(US_DBG_PINB)
#define USDBG_PINB_INIT() GpioPinConfigSet(NUTGPIO_PORTA, US_DBG_PINB, GPIO_CFG_OUTPUT)
#define USDBG_PINB_HIGH() GpioPinSetHigh(NUTGPIO_PORTA, US_DBG_PINB)
#define USDBG_PINB_LOW()  GpioPinSetLow(NUTGPIO_PORTA, US_DBG_PINB)
#else
#define USDBG_PINB_INIT()
#define USDBG_PINB_HIGH()
#define USDBG_PINB_LOW()
#endif

#if defined(US_DBG_PIN0)
#define USDBG_PIN0_INIT() GpioPinConfigSet(NUTGPIO_PORTB, US_DBG_PIN0, GPIO_CFG_OUTPUT)
#define USDBG_PIN0_HIGH() GpioPinSetHigh(NUTGPIO_PORTB, US_DBG_PIN0)
#define USDBG_PIN0_LOW()  GpioPinSetLow(NUTGPIO_PORTB, US_DBG_PIN0)
#else
#define USDBG_PIN0_INIT()
#define USDBG_PIN0_HIGH()
#define USDBG_PIN0_LOW()
#endif

#if defined(US_DBG_PIN1)
#define USDBG_PIN1_INIT() GpioPinConfigSet(NUTGPIO_PORTB, US_DBG_PIN1, GPIO_CFG_OUTPUT)
#define USDBG_PIN1_HIGH() GpioPinSetHigh(NUTGPIO_PORTB, US_DBG_PIN1)
#define USDBG_PIN1_LOW()  GpioPinSetLow(NUTGPIO_PORTB, US_DBG_PIN1)
#else
#define USDBG_PIN1_INIT()
#define USDBG_PIN1_HIGH()
#define USDBG_PIN1_LOW()
#endif

#if defined(US_DBG_PIN2)
#define USDBG_PIN2_INIT() GpioPinConfigSet(NUTGPIO_PORTB, US_DBG_PIN2, GPIO_CFG_OUTPUT)
#define USDBG_PIN2_HIGH() GpioPinSetHigh(NUTGPIO_PORTB, US_DBG_PIN2)
#define USDBG_PIN2_LOW()  GpioPinSetLow(NUTGPIO_PORTB, US_DBG_PIN2)
#else
#define USDBG_PIN2_INIT()
#define USDBG_PIN2_HIGH()
#define USDBG_PIN2_LOW()
#endif

#if defined(US_DBG_PIN3)
#define USDBG_PIN3_INIT() GpioPinConfigSet(NUTGPIO_PORTB, US_DBG_PIN3, GPIO_CFG_OUTPUT)
#define USDBG_PIN3_HIGH() GpioPinSetHigh(NUTGPIO_PORTB, US_DBG_PIN3)
#define USDBG_PIN3_LOW()  GpioPinSetLow(NUTGPIO_PORTB, US_DBG_PIN3)
#else
#define USDBG_PIN3_INIT()
#define USDBG_PIN3_HIGH()
#define USDBG_PIN3_LOW()
#endif

/*
 * TWI interrupt handler.
 */
static void TwInterrupt(void *arg)
{
    register unsigned int twsr = inr(TWI_SR) & (TWI_NACK | TWI_RXRDY | TWI_TXRDY | TWI_TXCOMP);;

    /* We got a nack, stop transmission and wait for TWI_TXCOMP */
    if (twsr & TWI_NACK) {;
		USDBG_PIN0_HIGH();
		/* send stop condition and wake up threads */
		/* NACK will automatically set a STOP condition */
        outr(TWI_CR, TWI_STOP);

		if( (tw_mt_len && (tw_mt_idx>1)) || (tw_mr_siz && tw_mr_idx)) {
			/* slave gave NACK while data transmission*/
	        tw_mm_err = TWERR_DATA_NACK;
		}
		else {								
			/* slave gave NACK on addressing */
			tw_mm_err = TWERR_SLA_NACK;
		}
		tw_mt_idx = 0;
		tw_mt_len = 0;
		tw_mr_siz = 0;
		
		/* Datashet says: TWI_NACK is set together with TWI_TXCOMP,
		 * but measurements show, that adding another byte into THT will
		 * start transmission again. So we have to disable and wakeup 
		 * threads directly here and quit early. */
        outr(TWI_IDR, 0xFFFFFFFF);
		//outr( TWI_CR, TWI_MSDIS);
        NutEventPostFromIrq(&tw_mm_que);
		return;
    }
	else {
		USDBG_PIN0_LOW();
	}

    if (twsr & TWI_RXRDY) {
		USDBG_PIN2_HIGH();
        if (tw_mr_idx < tw_mr_siz) {
            tw_mr_buf[tw_mr_idx++] = inb(TWI_RHR);

            if (tw_mr_idx == (tw_mr_siz - 1)) {
	            /* The last byte will follow, just set the STOP condition */
                outr(TWI_CR, TWI_STOP);
            }
			else {
	            if (tw_mr_idx == tw_mr_siz) {
	                /* Last byte received. Just wait till STOP is reached */
	                outr(TWI_IDR, TWI_RXRDY);
	                outr(TWI_IER, TWI_TXCOMP);
	            }
			}
        }
    }
	else {
		USDBG_PIN2_LOW();
		if (twsr & TWI_TXRDY) {
			USDBG_PIN3_HIGH();
	        if (tw_mt_idx < tw_mt_len) {
	            outb(TWI_THR, tw_mt_buf[tw_mt_idx++]);
	        }
	        /* Last byte? Add STOP condition */
	        if (tw_mt_idx == tw_mt_len) {
	//                if (tw_mr_siz == 0) {
	                outr(TWI_CR, TWI_STOP);
	                outr(TWI_IDR, TWI_TXRDY);
	                outr(TWI_IER, TWI_TXCOMP);
#if 0
	            } else {
	                /* Ok, now switch to read mode and send second start condition */
	                outr(TWI_MMR, inb(TWI_MMR) | TWI_MREAD);
	                outr(TWI_CR,  TWI_START | (tw_mr_siz == 1) ? TWI_STOP : 0);
	                outr(TWI_IDR, TWI_TXRDY);
	                outr(TWI_IER, TWI_RXRDY);
	            }
#endif
	        }
		}
		else {
			USDBG_PIN3_LOW();
		}
	}

    /* Transmission is complete, signal waiting threads */
    if (twsr & TWI_TXCOMP) {
		USDBG_PIN1_HIGH();
        outr(TWI_IDR, 0xFFFFFFFF);
		//outr( TWI_CR, TWI_MSDIS);
        NutEventPostFromIrq(&tw_mm_que);
    }
	else
		USDBG_PIN1_LOW();


}

/*!
 * \brief Transmit and/or receive data as a master.
 *
 * The two-wire serial interface must have been initialized by calling
 * TwInit() before this function can be used.
 *
 * \param sla    Slave address of the destination. This slave address
 *               must be specified as a 7-bit address. For example, the
 *               PCF8574A may be configured to slave addresses from 0x38
 *               to 0x3F.
 * \param txdata Points to the data to transmit. Ignored, if the number
 *               of data bytes to transmit is zero.
 * \param txlen  Number of data bytes to transmit. If zero, then the
 *               interface will not send any data to the slave device
 *               and will directly enter the master receive mode.
 * \param rxdata Points to a buffer, where the received data will be
 *               stored. Ignored, if the maximum number of bytes to
 *               receive is zero.
 * \param rxsiz  Maximum number of bytes to receive. Set to zero, if
 *               no bytes are expected from the slave device.
 * \param tmo    Timeout in milliseconds. To disable timeout, set this
 *               parameter to NUT_WAIT_INFINITE.
 *
 * \return The number of bytes received, -1 in case of an error or timeout.
 */

int TwMasterTransact(uint8_t sla, CONST void *txdata, uint16_t txlen, void *rxdata, uint16_t rxsiz, uint32_t tmo)
{
    int rc = -1;

    /* This routine is marked reentrant, so lock the interface. */
    if(NutEventWait(&tw_mm_mutex, 500)) {
        tw_mm_err = TWERR_IF_LOCKED;
        NutEventPost(&tw_mm_mutex);
        return -1;
    }

    if ((txlen == 0) && (rxsiz == 0)) {
        return -1;
    }

    NutEnterCritical();
    /* Set all parameters for master mode. */
    tw_mm_sla = sla;
    tw_mm_err = 0;
    tw_mt_len = txlen;
    tw_mt_idx = 0;
    tw_mt_buf = txdata;
    tw_mr_siz = rxsiz;
    tw_mr_buf = rxdata;
    tw_mr_idx = 0;


    /*
     * Prepare Transmission:
     * Enable interrupts for write direction and data size
     * and start transmission
     */

    if( tw_mt_len) {
	    /* Enable Master, set slave address enable interrupts and start transmission */
		outr(TWI_IDR, 0xFFFFFFFF);
		outr(TWI_CR, TWI_MSEN);
	    outr(TWI_MMR, (tw_mm_sla << 16) );
		/* we first send data before we receive */
	    outb(TWI_THR, tw_mt_buf[tw_mt_idx++]);		/* put first byte into THR */
        outr(TWI_IER, TWI_NACK);
        if( tw_mt_len == 1) {						/* only 1 byte to send? */
            outr(TWI_IER, TWI_TXCOMP);
			outr(TWI_CR, TWI_START | TWI_STOP);		/* START TX of 1 byte */
        } else {
            outr(TWI_IER, TWI_TXRDY);				/* else wait for RXRDY interrupt for next byte */
			outr(TWI_CR, TWI_START);				/* START TX of multiple bytes */
        }
    }
    NutIrqEnable(&sig_TWI);
    NutExitCritical();

    /* Wait for master transmission to be done. */
    if (NutEventWait(&tw_mm_que, tmo)) {
        tw_mm_error = TWERR_TIMEOUT;
    } else {
        NutEnterCritical();
        if (tw_mm_err) {
            tw_mm_error = tw_mm_err;
        } else {
            rc = 0;
        }
        NutExitCritical();
    }

    /*
     * Prepare Receiving:
     * Enable interrupts depending on read direction and data size
     * and start receiver
     */
	if( (tw_mm_error==0) && tw_mr_siz) {
		outr(TWI_IDR, 0xFFFFFFFF);
		outr(TWI_CR, TWI_MSEN);
	    outr(TWI_MMR, (tw_mm_sla << 16) | TWI_MREAD);
		/* we only receive data */
		if( tw_mr_siz == 1) {
            outr(TWI_IER, TWI_TXCOMP);				/* wait for transaction is complete */
			outr(TWI_CR, TWI_START | TWI_STOP);		/* START RX of 1 byte */
		} else {
	        outr(TWI_IER, TWI_RXRDY);				/* wait on next byte to receive */
			outr(TWI_CR, TWI_START);				/* START TX of multiple bytes */
		}

	    /* Wait for master transmission to be done. */
	    if (NutEventWait(&tw_mm_que, tmo)) {
	        tw_mm_error = TWERR_TIMEOUT;
	    } else {
	        NutEnterCritical();
	        if (tw_mm_err) {
	            tw_mm_error = tw_mm_err;
	        } else {
	            rc = tw_mr_idx;
	        }
	        NutExitCritical();
	    }

    }

    NutIrqDisable(&sig_TWI);

    /* Release the interface. */
    NutEventPost(&tw_mm_mutex);

    return rc;
}

/*!
 * \brief Receive data as a master from a device having internal addressable registers
 *
 * The two-wire serial interface must have been initialized by calling
 * TwInit() before this function can be used.
 *
 * \param sla     Slave address of the destination. This slave address
 *                must be specified as a 7-bit address. For example, the
 *                PCF8574A may be configured to slave addresses from 0x38
 *                to 0x3F.
 * \param iadr    Address send to the device to access certain registers
 *                or memory addresses of it.
 * \param iadrlen Number of bytes to send as address, maximum 3 bytes are
 *                supported from AT91SAM7
 * \param rxdata  Points to a buffer, where the received data will be
 *                stored.
 * \param rxsiz   Maximum number of bytes to receive.
 * \param tmo     Timeout in milliseconds. To disable timeout, set this
 *                parameter to NUT_WAIT_INFINITE.
 *
 * \return The number of bytes received, -1 in case of an error or timeout.
 */

int TwMasterRegRead(uint8_t sla, uint32_t iadr, uint8_t iadrlen, void *rxdata, uint8_t rxsiz, uint32_t tmo)
{
    int rc = -1;

    /* This routine is marked reentrant, so lock the interface. */
    if(NutEventWait(&tw_mm_mutex, 500)) {
        tw_mm_err = TWERR_IF_LOCKED;
        NutEventPost(&tw_mm_mutex);
        return -1;
    }

    if(rxsiz == 0) {
        return -1;
    }

    NutEnterCritical();

    /* Set all parameters for master mode. */
    tw_mm_sla = sla;
    tw_mm_err = 0;
    tw_mt_len = 0;
    tw_mt_idx = 0;
    tw_mt_buf = NULL;
    tw_mr_siz = rxsiz;
    tw_mr_buf = rxdata;
    tw_mr_idx = 0;

    /* Set TWI Master Mode */
	//outr( TWI_CR, TWI_MSEN);

    /* Set the TWI Master Mode Register */
	outr( TWI_MMR, (tw_mm_sla << 16) | ((iadrlen&3)<<8) | TWI_MREAD );

    /* Set TWI Internal Address Register if needed */
	outr( TWI_IADRR, iadr );

    /* Enable interrupts for read direction */
	outr(TWI_IDR, 0xFFFFFFFF);
    NutIrqEnable(&sig_TWI);
    outr(TWI_IER, TWI_RXRDY | TWI_NACK);
	USDBG_PINB_HIGH();

    /* Send start condition. If read only one byte send stop as well */
	if (rxsiz == 1) outr(TWI_CR, TWI_START | TWI_STOP);
	else outr(TWI_CR, TWI_START);

    NutExitCritical();

    /* Wait for master transmission to be done. */
    if (NutEventWait(&tw_mm_que, tmo))
	{
        tw_mm_error = TWERR_TIMEOUT;
    } else {
        NutEnterCritical();
        if (tw_mm_err) {
            tw_mm_error = tw_mm_err;	// set twi error
        } else {
            rc = tw_mr_idx;				// return number of successfully received bytes
        }
        NutExitCritical();
    }

	USDBG_PINB_LOW();
    NutIrqDisable(&sig_TWI);

    /* Release the interface. */
    NutEventPost(&tw_mm_mutex);

    return rc;
}

/*!
 * \brief Transmit data as a master to a device having internal addressable registers
 *
 * The two-wire serial interface must have been initialized by calling
 * TwInit() before this function can be used.
 *
 * \param sla     Slave address of the destination. This slave address
 *                must be specified as a 7-bit address. For example, the
 *                PCF8574A may be configured to slave addresses from 0x38
 *                to 0x3F.
 * \param iadr    Address send to the device to access certain registers
 *                or memory addresses of it.
 * \param iadrlen Number of bytes to send as address, maximum 3 bytes are
 *                supported from AT91SAM7
 * \param txdata  Points to a buffer, where the data to transmit will be
 *                stored.
 * \param txsiz   Maximum number of bytes to transmit.
 * \param tmo     Timeout in milliseconds. To disable timeout, set this
 *                parameter to NUT_WAIT_INFINITE.
 *
 * \return        The number of bytes transmitted, -1 in case of an error
 *                or timeout. Number could be less if slave end transmission
 *                with NAK.
 */

int TwMasterRegWrite(uint8_t sla, uint32_t iadr, uint8_t iadrlen, void *txdata, uint8_t txsiz, uint32_t tmo)
{
    int rc = -1;

    /* This routine is marked reentrant, so lock the interface. */
    if(NutEventWait(&tw_mm_mutex, 500)) {
        tw_mm_err = TWERR_IF_LOCKED;
        NutEventPost(&tw_mm_mutex);
        return -1;
    }

    if(txsiz == 0) {
        return -1;
    }

    NutEnterCritical();

    /* Set all parameters for master mode. */
    tw_mm_sla = sla;
    tw_mm_err = 0;
    tw_mt_len = txsiz;
    tw_mt_idx = 0;
    tw_mt_buf = txdata;
    tw_mr_siz = 0;
    tw_mr_buf = NULL;
    tw_mr_idx = 0;

    /* Set TWI Master Mode */
//	outr( TWI_CR, TWI_MSEN);

    /* Set TWI Internal Address Register */
	outr( TWI_IADRR, iadr );

    /* Set the TWI Master Mode Register */
	outr( TWI_MMR, (tw_mm_sla << 16) | ((iadrlen&3)<<8) );
	
    /* Enable interrupts for write direction and data size */
	outr(TWI_IDR, 0xFFFFFFFF);
//	USDBG_PINB_HIGH();
    NutIrqEnable(&sig_TWI);

//    if( tw_mt_len == 1) {						/* only 1 byte to send? */
//		outr(TWI_CR, TWI_START | TWI_STOP);		/* START TX of 1 byte */
//        outr(TWI_IER, TWI_TXCOMP | TWI_NACK);	/* Don't forget to allow the slave to NACK */
//    } else {
    	/* else wait for TXRDY interrupt for next byte */
	    outr(TWI_IER, TWI_TXRDY | TWI_NACK);
//    }

	/* put first byte to the bus */
    outb(TWI_THR, tw_mt_buf[tw_mt_idx++]);		/* put first byte into THR */

	outr(TWI_CR, TWI_START);					/* START TX of multiple bytes */
//	USDBG_PINB_LOW();
    NutExitCritical();

    /* Send start condition. If read / write only one byte send stop as well */
	if (tw_mt_len == 1) outr(TWI_CR, TWI_START | TWI_STOP);
	else outr(TWI_CR, TWI_START);


    /* Wait for master transmission to be done. */
    if (NutEventWait(&tw_mm_que, tmo))
	{
        tw_mm_error = TWERR_TIMEOUT;
    } else {
        NutEnterCritical();
        if (tw_mm_err) {
            tw_mm_error = tw_mm_err;
        } else {
            rc = tw_mr_idx;
        }
        NutExitCritical();
    }

    NutIrqDisable(&sig_TWI);

    /* Release the interface. */
    NutEventPost(&tw_mm_mutex);

    return rc;
}

/*!
 * \brief Get last master mode error.
 *
 * You may call this function to determine the specific cause
 * of an error after twi transaction failed.
 *
 */
int TwMasterError(void)
{
    int rc = (int) tw_mm_error;
    tw_mm_error = 0;
    return rc;
}

/*!
 * \brief Get last transfer results.
 *
 * You may call this function to determine how many bytes where
 * transferred before the twi transaction failed.
 *
 */
uint16_t TwMasterIndexes( uint8_t idx)
{
	switch (idx)
	{
		case 0: return tw_mt_idx;
		default: return tw_mr_idx;
	}
}

/*!
 * \brief Perform TWI control functions.
 *
 * \param req  Requested control function. May be set to one of the
 *	       following constants:
 *	       - TWI_SETSPEED, if conf points to an uint32_t value containing the bitrate.
 *	       - TWI_GETSPEED, if conf points to an uint32_t value receiving the current bitrate.
 * \param conf Points to a buffer that contains any data required for
 *	       the given control function or receives data from that
 *	       function.
 * \return 0 on success, -1 otherwise.
 *
 * \note Timeout is limited to the granularity of the system timer.
 *
 */
int TwIOCtl(int req, void *conf)
{
    int rc = 0;
    unsigned int cldiv, ckdiv;
    unsigned int twi_clk;
    switch (req) {

    case TWI_SETSPEED:
        ckdiv=1 ;
        twi_clk = *((uint32_t *) conf);

        if (twi_clk > 400000) return -1;

        /*
         * CLDIV = ((Tlow x 2^CKDIV) -3) x Tmck
         * CHDIV = ((THigh x 2^CKDIV) -3) x Tmck
         * Only CLDIV is computed since CLDIV = CHDIV (50% duty cycle)
         */

        while ((cldiv = ((NutClockGet(NUT_HWCLK_PERIPHERAL) / (2*twi_clk))-3 ) / (1 << ckdiv)) > 255) {
            ckdiv++;
        }

        /* BUG 41.2.7.1, datasheet SAM7X256  p. 626 */
        if (cldiv * (1 << ckdiv) > 8191) return -1;

        outr(TWI_CWGR, (ckdiv << 16) | ((unsigned int) cldiv << 8) | (unsigned int) cldiv);
        break;

    case TWI_GETSPEED:
        ckdiv=1 ;
        twi_clk = *((uint32_t *) conf);

        cldiv = inr(TWI_CWGR) & 0x000000FF;
        ckdiv = (inr(TWI_CWGR) >> 16) & 0x00000007;

        *((uint32_t *) conf) = NutGetCpuClock() * ((cldiv * 2 << ckdiv) - 3);
        break;

    case TWI_GETSTATUS:
        break;

    case TWI_SETSTATUS:
        break;

    default:
        rc = -1;
        break;
    }
    return rc;
}

/*!
 * \brief Initialize TWI interface.
 *
 * The specified slave address is not used here as we don't support twi-slave
 * on AT91SAM7X
 *
 * \note This function is only available on AT91SAM7xxxx systems.
 *
 * \param sla Slave address, must be specified as a 7-bit address,
 *            always lower than 128.
 */

int TwInit(uint8_t sla)
{
    uint32_t speed = 4800;

    if (NutRegisterIrqHandler(&sig_TWI, TwInterrupt, 0)) {
        return -1;
    }

    outr(TWI_PIO_ASR, _BV(TWI_TWD) | _BV(TWI_TWCK));  // Set TWD and TWCK as peripheral line
    outr(TWI_PIO_PDR, _BV(TWI_TWD) | _BV(TWI_TWCK));  // Let periperal control the PIO lines

    outr(TWI_PIO_MDER, _BV(TWI_TWD) | _BV(TWI_TWCK)); // Enabled OpenDrain output on both lines

    outr(PMC_PCER, _BV(TWI_ID));              // Enable TWI clock in PMC

    outr(TWI_IDR, 0xFFFFFFFF);                // Disable all interrupts
    outr(TWI_CR, TWI_SWRST);                  // Reset bus
    outr(TWI_CR, TWI_MSEN | TWI_SVDIS);       // Enable master mode

    if( TwIOCtl(TWI_SETSPEED, &speed)) {
		return -1;
    }

	USDBG_PINA_INIT();
	USDBG_PINB_INIT();
	USDBG_PIN0_INIT();
	USDBG_PIN1_INIT();
	USDBG_PIN2_INIT();
	USDBG_PIN3_INIT();

	USDBG_PINA_LOW();
	USDBG_PINB_LOW();
	USDBG_PIN0_LOW();
	USDBG_PIN1_LOW();
	USDBG_PIN2_LOW();
	USDBG_PIN3_LOW();

    /* Initialize mutex semaphores. */
    NutEventPost(&tw_mm_mutex);

    return 0;
}
/*@}*/
