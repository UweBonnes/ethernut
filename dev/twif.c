/*
 * Copyright (C) 2001-2003 by egnite Software GmbH. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY EGNITE SOFTWARE GMBH AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL EGNITE
 * SOFTWARE GMBH OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * For additional information see http://www.ethernut.de/
 *
 */

/*
 * $Log$
 * Revision 1.3  2003/07/20 18:28:10  haraldkipp
 * Explain how to disable timeout.
 *
 * Revision 1.2  2003/07/17 09:38:07  haraldkipp
 * Setting different speeds is now supported.
 *
 * Revision 1.1.1.1  2003/05/09 14:40:53  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.2  2003/03/31 14:53:08  harald
 * Prepare release 3.1
 *
 * Revision 1.1  2003/02/04 17:54:59  harald
 * *** empty log message ***
 *
 */

#include <string.h>

#include <dev/irqreg.h>

#include <sys/event.h>
#include <sys/atom.h>
#include <sys/timer.h>
#include <sys/thread.h>
#include <sys/heap.h>

#include <dev/twif.h>

//#include <sys/print.h>

#ifdef __AVR_ATmega128__

HANDLE tw_if_mutex;

volatile u_char tw_if_bsy;      /*!< Set while interface is busy. */
HANDLE tw_if_que;               /*!< Threads waiting for interface becoming idle. */

HANDLE tw_mm_que;               /*!< Threads waiting for master mode done. */
HANDLE tw_sm_que;               /*!< Threads waiting for slave mode done. */

volatile u_char tw_mm_sla;      /*!< Destination slave address. */
volatile u_char tw_mm_err;      /*!< Last master mode error. */
volatile u_short tw_mt_idx;
volatile u_short tw_mt_len;
volatile u_char *tw_mt_buf;

volatile u_short tw_mr_siz;
volatile u_short tw_mr_idx;
volatile u_char *tw_mr_buf;

volatile u_char tw_sm_sla;      /*!< Slave address received. */
volatile u_char tw_sm_err;      /*!< Last slave mode error. */
volatile u_short tw_sr_siz;
volatile u_short tw_sr_idx;
volatile u_char *tw_sr_buf;

volatile u_short tw_st_len;
volatile u_short tw_st_idx;     /*!< Slave transmit index. */
volatile u_char *tw_st_buf;

static void TwInterrupt(void *arg)
{
    u_char twsr;

    /*
     * Read the status.
     */
    twsr = TWSR & 0xF8;
    //NutPrintFormat(0, "[%02X]", twsr);

    switch (twsr) {

        /*
         * 0x10: Repeated start condition has been transmitted.
         */
    case TW_REP_START:
        /*
         * If outgoing data not available, transmit SLA+R. Logic will 
         * switch to master receiver mode.
         */
        if (tw_mt_len == 0) {
            TWDR = tw_mm_sla | 1;
            TWCR = (TWCR & 0xC5);
            break;
        }

        /*
         * If outgoing data is available, fall through.
         */

        /*
         * 0x08: Start condition has been transmitted.
         */
    case TW_START:
        /*
         * Transmit SLA+W.
         */
        TWDR = tw_mm_sla;
        TWCR = (TWCR & 0xC5);
        tw_mm_err = 0;
        break;

        /*
         * 0x20: SLA+W has been transmitted, but not acknowledged.
         */
    case TW_MT_SLA_NACK:
        /*
         * If all retries failed, transmit a stop condition, disable 
         * further device interrupts and inform the application.
         */
        tw_mm_err = TWERR_SLA_NACK;
        if (NutEventPostAsync(&tw_mm_que))
            TWCR = (TWCR & 0x74) | (1 << TWSTO);
        else {
            tw_if_bsy = 0;
            TWCR = (TWCR & 0xC5) | (1 << TWSTO);
        }
        break;

        /*
         * 0x18: SLA+W has been transmitted and ACK has been received.
         */
    case TW_MT_SLA_ACK:
        /*
         * If no transmit data is waiting, transmit repeated start 
         * condition.
         */
        if (tw_mt_len == 0) {
            TWCR = (TWCR & 0xC5) | (1 << TWSTA);
            break;
        }

        /*
         * Reset transmit index and fall through for outgoing data.
         */
        tw_mt_idx = 0;

        /*
         * 0x28: Data byte has been transmitted and ACK has been received.
         */
    case TW_MT_DATA_ACK:
        /*
         * If outgoing data left to send, put the next byte in
         * the data register.
         */
        if (tw_mt_idx < tw_mt_len) {
            TWDR = tw_mt_buf[tw_mt_idx++];
            TWCR = (TWCR & 0xC5);
            break;
        }

        /*
         * All outgoing data has been sent. If a response is expected, 
         * transmit a repeated start condition.
         */
        tw_mt_len = 0;
        if (tw_mr_siz)
            TWCR = (TWCR & 0xC5) | (1 << TWEA) | (1 << TWSTA);

        /*
         * No response expected. Transmit stop condition, disable device 
         * interrupts and inform application.
         */
        else if (NutEventPostAsync(&tw_mm_que))
            TWCR = (TWCR & 0x44) | (1 << TWSTO);
        else {
            tw_if_bsy = 0;
            TWCR = (TWCR & 0xC5) | (1 << TWSTO);
        }
        break;

        /*
         * 0x30: Data byte has been transmitted, but not acknowledged.
         */
    case TW_MT_DATA_NACK:
        /*
         * Transmit stop condition, disable device interrupts and 
         * inform the application.
         */
        tw_mm_err = TWERR_DATA_NACK;
        if (NutEventPostAsync(&tw_mm_que))
            TWCR = (TWCR & 0x74) | (1 << TWSTO);
        else {
            tw_if_bsy = 0;
            TWCR = (TWCR & 0xC5) | (1 << TWSTO);
        }
        break;

        /*
         * 0x38: Arbitration lost while in master mode.
         */
    case TW_MT_ARB_LOST:
        /*
         * The start condition will be automatically resend after 
         * the bus becomes available.
         */
        TWCR = (TWCR & 0xC5) | (1 << TWSTA);
        break;

        /*
         * 0x48: SLA+R has been transmitted, but not acknowledged.
         */
    case TW_MR_SLA_NACK:
        /*
         * Transmit stop condition, disable device interrupts and 
         * inform the application.
         */

        tw_mm_err = TWERR_SLA_NACK;
        if (NutEventPostAsync(&tw_mm_que))
            TWCR = (TWCR & 0x74) | (1 << TWSTO);
        else {
            tw_if_bsy = 0;
            TWCR = (TWCR & 0xC5) | (1 << TWSTO);
        }
        break;

        /*
         * 0x40: SLA+R has been transmitted and ACK has been received.
         */
    case TW_MR_SLA_ACK:
        /*
         * Reset index for incoming data.
         */
        tw_mr_idx = 0;
        TWCR = (TWCR & 0xC5);
        break;

        /*
         * 0x50: Data received, ACK returned.
         */
    case TW_MR_DATA_ACK:
        /*
         * Store data byte, if buffer space for incoming data is 
         * available and transmit an ACK.
         */
        if (tw_mr_idx < tw_mr_siz) {
            tw_mr_buf[tw_mr_idx++] = TWDR;
            TWCR = (TWCR & 0x85) | (1 << TWEA);
        }

        /*
         * If no more data expected, do not transmit ACKs. 
         */
        else
            TWCR &= 0x85;
        break;

        /*
         * 0x58: Data byte has been received, but not acknowledged.
         */
    case TW_MR_DATA_NACK:
        /*
         * Transmit stop condition, disable device interrupts and 
         * inform the application.
         */
        if (NutEventPostAsync(&tw_mm_que))
            TWCR = (TWCR & 0x74) | (1 << TWSTO);
        else {
            tw_if_bsy = 0;
            TWCR = (TWCR & 0xC5) | (1 << TWSTO);
        }
        break;

        /*
         * 0x68: Arbitration lost as master. Own SLA+W has been received 
         *       and acknowledged.
         */
    case TW_SR_ARB_LOST_SLA_ACK:
        /*
         * 0x78: Arbitration lost as master. General call address has been
         *       received and acknowledged.
         */
    case TW_SR_ARB_LOST_GCALL_ACK:
        /*
         * Send start as soon as bus becomes available again.
         */
        TWCR = (TWCR & 0x75) | (1 << TWSTA);

        /*
         * 0x60: Own SLA+W has been received and acknowledged. 
         */
    case TW_SR_SLA_ACK:
        /*
         * 0x70: General call address has been received and acknowledged.
         */
    case TW_SR_GCALL_ACK:
        /*
         * Do only acknowledge incoming data bytes, if we got receive 
         * buffer space.
         * Fetch the slave address from the data register and reset the
         * receive index and receive error status.
         */
        tw_if_bsy = 1;
        if (tw_sr_siz) {
            TWCR = (TWCR & 0xF5);
            tw_sm_sla = TWDR;
            tw_sr_idx = 0;
            tw_sm_err = 0;
        } else
            TWCR = (TWCR & 0xB5);
        break;

        /*
         * 0x80: Data byte for own SLA has been received and acknowledged.
         */
    case TW_SR_DATA_ACK:
        /*
         * 0x90: Data byte for general call address has been received and 
         *       acknowledged.
         */
    case TW_SR_GCALL_DATA_ACK:
        /*
         * If the receive buffer hasn't been filled, store data byte.
         */
        if (tw_sr_idx < tw_sr_siz)
            tw_sr_buf[tw_sr_idx++] = TWDR;

        /*
         * If more space is available for incoming data, then continue
         * receiving. Otherwise do not acknowledge new data bytes.
         */
        if (tw_sr_idx < tw_sr_siz)
            TWCR = (TWCR & 0xF5);
        else
            TWCR = (TWCR & 0xB5);
        break;

        /*
         * 0x88: Data byte received, but not acknowledged.
         */
    case TW_SR_DATA_NACK:
        /*
         * 0x98: Data byte for general call address received, but not 
         *       acknowledged.
         */
    case TW_SR_GCALL_DATA_NACK:
        /*
         * Continue not accepting more data.
         */
        TWCR = (TWCR & 0xB5);
        break;

        /*
         * 0xA0: Stop condition or repeated start condition received.
         */
    case TW_SR_STOP:
        /*
         * Disable device interrupts and inform the application. Note,
         * that SCL is kept low.
         */
        if (tw_sr_siz) {
            tw_sr_siz = 0;
            if (NutEventPostAsync(&tw_sm_que)) {
                TWCR &= 0x74;
            } else {
                tw_if_bsy = 0;
                TWCR &= 0xB5;
            }
        } else {
            TWCR &= 0xB5;
            tw_if_bsy = 0;
        }
        break;

        /*
         * 0xA8: Own SLA+R has been received and acknowledged.
         */
    case TW_ST_SLA_ACK:
        /*
         * Fall through to start slave transmit mode.
         */

        /*
         * 0xB0: Arbitration lost in master mode. Own SLA has been received
         *       and acknowledged.
         */
    case TW_ST_ARB_LOST_SLA_ACK:
        /*
         * Reset transmit index and fall through for outgoing data.
         */
        tw_st_idx = 0;
        tw_if_bsy = 1;

        /*
         * 0xB8: Data bytes has been transmitted and acknowledged.
         */
    case TW_ST_DATA_ACK:
        /*
         * If outgoing data left to send, put the next byte in the 
         * data register. Otherwise transmit a dummy byte.
         */
        if (tw_st_idx < tw_st_len)
            TWDR = tw_st_buf[tw_st_idx++];
        else
            TWDR = 0;
        TWCR &= 0xF5;
        break;


        /*
         * 0xC0: Data byte has been transmitted, but not acknowledged.
         */
    case TW_ST_DATA_NACK:
        if (NutEventPostAsync(&tw_sm_que))
            TWCR &= 0x74;
        else {
            tw_if_bsy = 0;
            TWCR &= 0xB5;
        }
        break;

        /*
         * 0xC8: Last data byte has been transmitted and acknowledged.
         */
    case TW_ST_LAST_DATA:
        TWCR = (TWCR & 0xF5);
        break;

        /*
         * 0xF8: No info.
         */
    case TW_NO_INFO:
        break;

        /*
         * 0x00: Bus error.
         */
    case TW_BUS_ERROR:
        TWCR = (TWCR & 0xF5) | (1 << TWSTO);
        break;
    }
}

/*
 * Make sure, that only one application thread is dealing with our
 * interface and disable interface interrupts.
 */
static int TwAccess(u_long tmo)
{
    int rc;
    u_long wt = tmo ? tmo : 250;
    u_char retry = 0;

    for (;;) {
        rc = NutEventWait(&tw_if_mutex, wt);

        /*
         * Disable TWI interrupts.
         */
        NutEnterCritical();
        TWCR = (TWCR & 0x74);
        NutExitCritical();

        /*
         * Check the busy flag, even if we failed locking the mutex.
         * This gives us an extra bonus to avoid deadlocks.
         */
        if (tw_if_bsy == 0) {
            tw_if_bsy = 1;
            rc = 0;
            break;
        }
        //NutPrintString(0, "[BUSY]");

        /*
         * Enable TWI interrupts.
         */
        NutEnterCritical();
        TWCR = (TWCR & 0x74) | (1 << TWIE);
        NutExitCritical();

        /*
         * Release the mutex if we locked it before. Note, that if we
         * hold the lock, we usually didn't wait for the lock. So
         * let's take a nap now.
         */
        if (rc == 0) {
            NutEventPost(&tw_if_mutex);
            NutSleep(wt);
        }

        /*
         * If timeout has been specified by the caller, then return
         * on mutex timeout.
         */
        else if (tmo)
            break;

        /*
         * Do not try more than 5 times.
         */
        if (retry++ > 5) {
            NutEnterCritical();
            TWCR = (TWCR & 0xF5) | (1 << TWSTO);
            TWDR = 0;
            tw_if_bsy = 0;
            NutExitCritical();
            rc = -1;
            break;
        }

    }
    return rc;
}

/*! 
 * \brief Transmit and/or receive data as a master.
 *
 * This function is only available on ATmega128 systems.
 *
 * \param sla    Slave address of the destination.
 * \param txdata Points to the data to transmit. Ignored, if the
 *		 number of bytes to transmit is zero.
 * \param txlen  Number of bytes to transmit.
 * \param rxdata Points to a buffer, where the received data will be
 *               stored. Ignored, if the maximum number of bytes to 
 *               receive is zero.
 * \param rxsiz  Maximum number of bytes to receive.
 * \param tmo    Timeout in milliseconds. To disable timeout,
 *               set this parameter to NUT_WAIT_INFINITE.
 *
 * \return The number of bytes received, -1 in case of an error or 
 *         timeout.
 */
int TwMasterTransact(u_char sla, void *txdata, u_short txlen, void *rxdata,
                     u_short rxsiz, u_long tmo)
{
    int rc;

    /*
     * Lock the interface.
     */
    if (TwAccess(tmo)) {
        tw_mm_err = TWERR_IF_LOCKED;
        return -1;
    }

    /*
     * Set all parameters for master mode.
     */
    tw_mt_len = txlen;
    tw_mm_sla = sla << 1;
    tw_mt_buf = txdata;
    tw_mr_siz = rxsiz;
    tw_mr_buf = rxdata;

    /*
     * Send a start condition and enable device interrupts.
     */
    NutEnterCritical();
    TWCR = (TWCR & 0x44) | (1 << TWINT) | (1 << TWSTA) | (1 << TWIE);
    NutEventBroadcast(&tw_mm_que);
    NutExitCritical();

    /*
     * Wait for master transmission done.
     */
    rc = -1;
    if (NutEventWait(&tw_mm_que, tmo)) {
        tw_mm_err = TWERR_TIMEOUT;
    } else {
        if (tw_mm_err == 0)
            rc = tw_mr_idx;
        TWCR = (TWCR & 0xF4) | (1 << TWIE);
    }

    /*
     * Release the interface.
     */
    tw_if_bsy = 0;
    NutEventPost(&tw_if_mutex);

    return rc;
}

/*!
 * \brief Get last slave mode error.
 */
int TwMasterError(void)
{
    return (int) tw_mm_err;
}

/*!
 * \brief Listen for incoming data from a master.
 *
 * If this function returns without error, the bus is blocked. The caller 
 * must immediately process the request and return a response by calling 
 * TwSlaveRespond().
 *
 * This function is only available on ATmega128 systems.
 *
 * \param sla    Points to a byte variable, which receives the slave 
 *               address sent by the master. This can be used by the 
 *               caller to determine whether the the interface has been 
 *               addressed by a general call or its individual address.
 * \param rxdata Points to a data buffer where the received data bytes
 *               are stored.
 * \param rxsize Specifies the maximum number of data bytes to receive.
 * \param tmo	 Timeout in milliseconds. To disable timeout,
 *               set this parameter to NUT_WAIT_INFINITE.
 *
 * \return The number of bytes received, -1 in case of an error or timeout.
 */
int TwSlaveListen(u_char * sla, void *rxdata, u_short rxsiz, u_long tmo)
{
    int rc;

    /*
     * Lock the interface.
     */
    if (TwAccess(tmo)) {
        tw_sm_err = TWERR_IF_LOCKED;
        return -1;
    }

    /*
     * Set all parameters for slave receive.
     */
    tw_sr_siz = rxsiz;
    tw_sr_buf = rxdata;

    /*
     * Enable the receiver, clear the listener queue and release the 
     * interface.
     */
    NutEnterCritical();
    TWCR = (TWCR & 0x74) | (1 << TWEA) | (1 << TWIE);
    NutEventBroadcastAsync(&tw_sm_que);
    tw_if_bsy = 0;
    NutExitCritical();
    NutEventPost(&tw_if_mutex);

    /*
     * Wait for slave event and return on timeout.
     */
    rc = -1;
    if (NutEventWait(&tw_sm_que, tmo)) {
        tw_sm_err = TWERR_TIMEOUT;
    }

    /*
     * Return the number of bytes received and the destination slave
     * address, if no slave error occured. In this case the bus is
     * blocked.
     */
    else if (tw_sm_err == 0) {
        rc = tw_sr_idx;
        *sla = tw_sm_sla;
    }
    return rc;
}

/*
 *
 */
int TwSlaveRespond(void *txdata, u_short txlen, u_long tmo)
{
    int rc = -1;

    tw_st_buf = txdata;
    tw_st_len = txlen;
    tw_st_idx = 0;

    /*
     * Enable interface interrupts.
     */
    if (txlen) {
        tw_sm_err = 0;
        NutEnterCritical();
        TWDR = tw_st_buf[tw_st_idx++];
        TWCR = (TWCR & 0x34) | (1 << TWINT) | (1 << TWEA) | (1 << TWIE);
        NutEventBroadcastAsync(&tw_sm_que);
        tw_if_bsy = 0;
        NutExitCritical();
        if (NutEventWait(&tw_sm_que, tmo)) {
            tw_sm_err = TWERR_TIMEOUT;
        } else {
            if (tw_sm_err == 0)
                rc = tw_st_idx;
            NutEnterCritical();
            TWCR = (TWCR & 0x24) | (1 << TWINT) | (1 << TWIE);
            tw_if_bsy = 0;
            NutExitCritical();
        }
    } else {
        rc = 0;
        NutEnterCritical();
        TWDR = 0;
        TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE);
        NutExitCritical();
    }
    return rc;
}

/*!
 * \brief Get last slave mode error.
 *
 * This function is only available on ATmega128 systems.
 *
 */
int TwSlaveError(void)
{
    return (int) tw_sm_err;
}

/*!
 * \brief Perform TWI control functions.
 *
 * This function is only available on ATmega128 systems.
 *
 * \param req  Requested control function. May be set to one of the
 *	       following constants:
 *	       - TWI_SETSPEED, if conf points to an u_long value containing the baudrate.
 *	       - TWI_GETSPEED, if conf points to an u_long value receiving the current baudrate.
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
    u_long i;

    switch (req) {
    case TWI_SETSLAVEADDRESS:
        TWAR = (*((u_char *) conf) << 1) | 1;
        break;
    case TWI_GETSLAVEADDRESS:
        *((u_char *) conf) = TWAR;
        break;

    case TWI_SETSPEED:
        i = (NutGetCpuClock() / (*((u_long *) conf))) - 16;
        if (i < 500) {
            TWBR = i / 2;
            cbi(TWSR, TWPS0);
            cbi(TWSR, TWPS1);
        } else if (i < 2000) {
            TWBR = i / 8;
            sbi(TWSR, TWPS0);
            cbi(TWSR, TWPS1);
        } else if (i < 8000) {
            TWBR = i / 32;
            cbi(TWSR, TWPS0);
            sbi(TWSR, TWPS1);
        } else if (i < 32000) {
            TWBR = i / 128;
            sbi(TWSR, TWPS0);
            sbi(TWSR, TWPS1);
        } else {
            TWBR = 250;
            sbi(TWSR, TWPS0);
            sbi(TWSR, TWPS1);
        }
        break;
    case TWI_GETSPEED:
        *((u_long *) conf) = NutGetCpuClock() / (16 + 2 * TWBR);
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
 * This function is only available on ATmega128 systems.
 *
 * \param sla Slave address, must be lower than 128.
 */
int TwInit(u_char sla)
{
    if (NutRegisterIrqHandler(&sig_2WIRE_SERIAL, TwInterrupt, 0))
        return -1;

    /*
     * Set address register, enable general call address, set transfer 
     * speed and enable interface.
     */
    TWAR = (sla << 1) | 1;
    /* TWBR = NutGetCpuClock() / (2 * 76800) - 8; */
    TWBR = 16;
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);

    /*
     * Initialize mutex semaphore.
     */
    NutEventPost(&tw_if_mutex);

    return 0;
}

#endif
