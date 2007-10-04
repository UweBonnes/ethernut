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
 * Revision 1.2  2007/10/04 19:51:56  olereinhardt
 * Support for sam7s added
 *
 * Revision 1.1  2007/09/06 19:36:00  olereinhardt
 * First checkin, new twi driver for at91 (currently SAM7X256 is supported
 * only)
 *
 */

#include <stdio.h>
#include <arch/arm.h>
#include <dev/irqreg.h>

#include <sys/event.h>
#include <sys/atom.h>
#include <sys/timer.h>
#include <sys/thread.h>
#include <sys/heap.h>

#include <dev/twif.h>

HANDLE tw_mm_mutex;                 /* Exclusive master access. */
HANDLE tw_mm_que;                   /* Threads waiting for master transfer done. */

static u_char tw_mm_sla;            /* Destination slave address. */
static volatile u_char tw_mm_err;   /* Current master mode error. */
static u_char tw_mm_error;          /* Last master mode error. */

static CONST u_char *tw_mt_buf;     /* Pointer to the master transmit buffer. */
static volatile u_short tw_mt_len;  /* Number of bytes to transmit in master mode. */
static volatile u_short tw_mt_idx;  /* Current master transmit buffer index. */

static u_char *tw_mr_buf;           /* Pointer to the master receive buffer. */
static volatile u_short tw_mr_siz;  /* Size of the master receive buffer. */
static volatile u_short tw_mr_idx;  /* Current master receive buffer index. */

#if defined (MCU_AT91SAM7X256) || defined (MCU_AT91SAM7S256)

#define TWI_PIO_ASR PIOA_ASR
#define TWI_PIO_PDR PIOA_PDR
#define TWI_PIO_MDER PIOA_MDER

   #if defined (MCU_AT91SAM7X256)
      #define TWI_TWD  PA10_TWD_A
      #define TWI_TWCK PA11_TWCK_A
   #elif defined (MCU_AT91SAM7S256)
      #define TWI_TWD  PA3_TWD_A
      #define TWI_TWCK PA4_TWCK_A
   #endif
#endif

/*
 * TWI interrupt handler.
 */
static void TwInterrupt(void *arg)
{
    register u_int twsr = inr(TWI_SR) & (TWI_NACK | TWI_RXRDY | TWI_TXRDY | TWI_TXCOMP);;   

    /* Transmission is complete, signal waiting threads */
    if (twsr & TWI_TXCOMP) {
        outr(TWI_IDR, 0xFFFFFFFF);
        NutEventPostFromIrq(&tw_mm_que);
    }
    
    if (twsr & TWI_RXRDY) {
        if (tw_mr_idx < tw_mr_siz) {
            tw_mr_buf[tw_mr_idx++] = inb(TWI_RHR);
            /* The last byte will follow, just set the stop condition */
            if (tw_mr_idx == tw_mr_siz - 1) {
                outr(TWI_CR, TWI_STOP);
            }
                
            if (tw_mr_idx == tw_mr_siz) {
                /* Last byte received. Send stop condition and set IRQs */
                outr(TWI_IDR, TWI_RXRDY);
                outr(TWI_IER, TWI_TXCOMP);
            }
        } 
    }
    
    if (twsr & TWI_TXRDY) {
        if (tw_mt_idx < tw_mt_len) {
            outb(TWI_THR, tw_mt_buf[tw_mt_idx++]);
            /* Last byte? No bytes to read? So send stop condition else if bytes to read switch to read mode */
            if (tw_mt_idx == tw_mt_len) {
                if (tw_mr_siz == 0) {
                    outr(TWI_CR, TWI_STOP);
                    outr(TWI_IDR, TWI_TXRDY);
                    outr(TWI_IER, TWI_TXCOMP);
                } else {
                    /* Ok, now switch to read mode and send second start condition */
                    outr(TWI_MMR, inb(TWI_MMR) | TWI_MREAD);
                    outr(TWI_CR,  TWI_START | (tw_mr_siz == 1) ? TWI_STOP : 0);
                    outr(TWI_IDR, TWI_TXRDY);
                    outr(TWI_IER, TWI_RXRDY);
                }
            }
        } 
    }
    
    /* We got a nack, stop transmission and wait for TWI_TXCOMP */
    if (twsr & TWI_NACK) {;
        /* send stop condition and wake up threads */
        outr(TWI_CR, TWI_STOP);
        tw_mm_err = TWERR_DATA_NACK;
        tw_mt_idx = 0;
        tw_mt_len = 0;
        tw_mr_siz = 0;
        outr(TWI_IDR, 0xFFFFFFFF);
        /* Wake up the application. */
        NutEventPostFromIrq(&tw_mm_que);
    }
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

int TwMasterTransact(u_char sla, CONST void *txdata, u_short txlen, void *rxdata, u_short rxsiz, u_long tmo)
{
    int rc = -1;

    /* This routine is marked reentrant, so lock the interface. */
    if(NutEventWait(&tw_mm_mutex, 500)) {
        tw_mm_err = TWERR_IF_LOCKED;
        NutEventPost(&tw_mm_mutex);
        return -1;
    }
    NutIrqEnable(&sig_TWI);

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

    if ((tw_mt_len == 0) && (tw_mr_siz == 0)) return -1;

    /* Set slave address enable interrupts and start transmission */
    
    outr(TWI_MMR, (tw_mm_sla << 16) | (tw_mt_len == 0 ? TWI_MREAD : 0));
    
    /* Enable interrupts depending on read / write direction and data size */     
    if (tw_mt_len == 0) {  
        outr(TWI_IDR, TWI_TXRDY | TWI_TXCOMP);
        outr(TWI_IER, TWI_RXRDY | TWI_NACK);
    } else {
        outr(TWI_IDR, TWI_RXRDY);
        if ((tw_mt_len == 1) && (tw_mr_siz == 0)) {
            outr(TWI_IDR, TWI_TXRDY);
            outr(TWI_IER, TWI_TXCOMP);
        } else {            
            outr(TWI_IER, TWI_TXRDY);
            outr(TWI_IDR, TWI_TXCOMP);
        }
        outr(TWI_IER, TWI_NACK);        
    }

    /* Now start transmission if we have any data */
    if (tw_mt_len > 0) {
        outb(TWI_THR, tw_mt_buf[tw_mt_idx++]);
    }     
    
    /* Send start condition. If read / write only one byte send stop as well */
    outr(TWI_CR, TWI_START | (((tw_mt_len == 1) && (tw_mr_siz == 0)) || 
                              ((tw_mt_len == 0) && (tw_mr_siz == 1))) ? TWI_STOP : 0);
        
    NutExitCritical();
        
    /* Wait for master transmission to be done. */
    rc = -1;
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

    NutIrqDisable(&sig_TWI);
    
    /* Release the interface. */
    NutEventPost(&tw_mm_mutex);
    
    return rc;
}

/*!
 * \brief Get last master mode error.
 *
 * You may call this function to determine the specific cause
 * of an error after TwMasterTransact() failed.
 *
 * \note This function is only available on ATmega128 systems.
 *
 */
int TwMasterError(void)
{
    int rc = (int) tw_mm_error;
    tw_mm_error = 0;
    return rc;
}

/*!
 * \brief Perform TWI control functions.
 *
 * \param req  Requested control function. May be set to one of the
 *	       following constants:
 *	       - TWI_SETSPEED, if conf points to an u_long value containing the bitrate.
 *	       - TWI_GETSPEED, if conf points to an u_long value receiving the current bitrate.
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
        twi_clk = *((u_long *) conf);

        if (twi_clk > 400000) return -1;
        
        /*
         * CLDIV = ((Tlow x 2^CKDIV) -3) x Tmck
         * CHDIV = ((THigh x 2^CKDIV) -3) x Tmck
         * Only CLDIV is computed since CLDIV = CHDIV (50% duty cycle) 
         */

        while ((cldiv = ((NutGetCpuClock() / (2*twi_clk))-3 ) / (1 << ckdiv)) > 255) {
            ckdiv++;
        }

        /* BUG 41.2.7.1, datasheet SAM7X256  p. 626 */
        if (cldiv * (2 << ckdiv) > 8191) return -1; 
        
        outr(TWI_CWGR, (ckdiv << 16) | ((u_int) cldiv << 8) | (u_int) cldiv);
        break;

    case TWI_GETSPEED:
        ckdiv=1 ;
        twi_clk = *((u_long *) conf);
        
        cldiv = inr(TWI_CWGR) & 0x000000FF;
        ckdiv = (inr(TWI_CWGR) >> 16) & 0x00000007;
            
        *((u_long *) conf) = NutGetCpuClock() * ((cldiv * 2 << ckdiv) - 3);
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

int TwInit(u_char sla)
{
    u_long speed = 2400;

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
    
    TwIOCtl(TWI_SETSPEED, &speed);

    /* Initialize mutex semaphores. */
    NutEventPost(&tw_mm_mutex);

    return 0;
}
