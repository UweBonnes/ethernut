/*
 * Copyright (C) 2001-2006 by egnite Software GmbH. All rights reserved.
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
 * The 9-bit communication had been contributed by Brett Abbott,
 * Digital Telemetry Limited.
 *
 * Dave Smart contributed the synchronous mode support.
 */

/*
 * $Log$
 * Revision 1.12  2008/10/23 08:50:43  haraldkipp
 * Prepared AT91 UART hardware handshake.
 *
 * Revision 1.11  2008/08/11 06:59:13  haraldkipp
 * BSD types replaced by stdint types (feature request #1282721).
 *
 * Revision 1.10  2008/07/31 09:43:25  haraldkipp
 * Initializing peripheral control registers in a more general way.
 * Fixes bug #2032960.
 *
 * Revision 1.9  2008/04/18 13:24:56  haraldkipp
 * Added Szemzo Andras' RS485 patch.
 *
 * Revision 1.8  2008/02/15 16:59:27  haraldkipp
 * Spport for AT91SAM7SE512 added.
 *
 * Revision 1.7  2007/10/04 20:05:51  olereinhardt
 * Support for SAM7S256 added
 *
 * Revision 1.6  2006/08/31 19:01:44  haraldkipp
 * Using the processor clock for baud rate calculations failed
 * on the SAM9, if the master clock is further divided. This
 * had been fixed.
 *
 * Revision 1.5  2006/08/05 11:54:06  haraldkipp
 * Special register functions should not be based on MCU definitions but on
 * register definitions.
 *
 * Revision 1.4  2006/07/05 07:55:23  haraldkipp
 * Daidai's support for AT91SAM7X added.
 *
 * Revision 1.3  2006/06/28 17:18:40  haraldkipp
 * Temporarly exclude AT91R40008 specific register settings from building
 * for AT91SAM7X.
 *
 * Revision 1.2  2006/01/05 16:47:32  haraldkipp
 * Baudrate calculation is now based on NutGetCpuClock().
 *
 * Revision 1.1  2005/11/20 14:40:28  haraldkipp
 * Added interrupt driven UART driver for AT91.
 *
 */

#include <cfg/clock.h>

#include <sys/atom.h>
#include <sys/event.h>
#include <sys/timer.h>

#include <dev/irqreg.h>

#include <dev/usartat91.h>

/*!
 * \addtogroup xgNutArchArmAt91Us
 */
/*@{*/

/* \brief ASCII code for software flow control, starts transmitter. */
#define ASCII_XON   0x11
/* \brief ASCII code for software flow control, stops transmitter. */
#define ASCII_XOFF  0x13

/* \brief XON transmit pending flag. */
#define XON_PENDING     0x10
/* \brief XOFF transmit pending flag. */
#define XOFF_PENDING    0x20
/* \brief XOFF sent flag. */
#define XOFF_SENT       0x40
/* \brief XOFF received flag. */
#define XOFF_RCVD       0x80


/*!
 * \brief Receiver error flags.
 */
static ureg_t rx_errors;

/*!
 * \brief Enables software flow control if not equal zero.
 */
static uint_fast8_t flow_control;

/*!
 * \brief Transmit address frame, if not zero.
 */
static uint_fast8_t tx_aframe;

#ifdef UART_HDX_BIT
	/* define in cfg/modem.h */
	#ifdef UART_HDX_FLIP_BIT	/* same as RTS toggle by Windows NT driver */
		#define UART_HDX_TX		cbi
		#define UART_HDX_RX		sbi
	#else						/* previous usage by Ethernut */
		#define UART_HDX_TX		sbi
		#define UART_HDX_RX		cbi
	#endif
#endif


#ifdef UART_HDX_BIT
/*!
 * \brief Enables half duplex control if not equal zero.
 *
 * This variable exists only if the hardware configuration defines a
 * port bit to switch between receive and transmit mode.
 */
static uint_fast8_t hdx_control;
#endif

//#if defined(UART_RTS_BIT) || defined(US_MODE_HWHANDSHAKE)
/*!
 * \brief Enables RTS control if not equal zero.
 *
 * This variable exists only if the hardware configuration defines a
 * port bit to control the RTS signal.
 */
static uint_fast8_t rts_control;
//#endif

//#if defined(UART_CTS_BIT) || defined(US_MODE_HWHANDSHAKE)
/*!
 * \brief Enables CTS sense if not equal zero.
 *
 * This variable exists only if the hardware configuration defines a
 * port bit to sense the CTS signal.
 */
static uint_fast8_t cts_sense;
//#endif

#ifdef UART_CTS_BIT
/*!
 * \brief USARTn CTS sense interrupt handler.
 *
 * This interrupt routine is called when the CTS line level is low.
 * Typical line drivers negate the signal, thus driving our port
 * low when CTS is active.
 *
 * This routine exists only if the hardware configuration defines a
 * port bit to sense the CTS signal.
 */
static void At91UsartCts(void *arg)
{
    /* Enable transmit interrupt. */
    //sbi(UCSRnB, UDRIE);
    /* Disable CTS sense interrupt. */
    //cbi(EIMSK, UART_CTS_BIT);
}
#endif

#ifdef UART_HDX_BIT
/*
 * \brief USARTn transmitter empty interrupt handler.
 *
 * Used with half duplex communication to switch from tranmit to receive
 * mode after the last character has been transmitted.
 *
 * This routine exists only if the hardware configuration defines a
 * port bit to switch between receive and transmit mode.
 *
 * \param arg Pointer to the transmitter ring buffer.
 */
static void At91UsartTxEmpty(RINGBUF *rbf)
{
    /*
     * Check if half duplex mode has been enabled and if all characters
     * had been sent out.
     */
    if (hdx_control && rbf->rbf_cnt == 0) {
        /* Switch to receiver mode. */
        UART_HDX_RX(UART_HDX_PORT, UART_HDX_BIT);
    }
}
#endif

/*
 * \brief USARTn transmitter ready interrupt handler.
 *
 * \param rbf Pointer to the transmitter ring buffer.
 */
static void At91UsartTxReady(RINGBUF *rbf) 
{
    register uint8_t *cp = rbf->rbf_tail;

    /*
     * Process pending software flow controls first.
     */
    if (flow_control & (XON_PENDING | XOFF_PENDING)) {
        if (flow_control & XON_PENDING) {
            outr(USARTn_BASE + US_THR_OFF, ASCII_XOFF);
            flow_control |= XOFF_SENT;
        } else {
            outr(USARTn_BASE + US_THR_OFF, ASCII_XON);
            flow_control &= ~XOFF_SENT;
        }
        flow_control &= ~(XON_PENDING | XOFF_PENDING);
        return;
    }

    if (flow_control & XOFF_RCVD) {
        /*
         * If XOFF has been received, we disable the transmit interrupts
         * and return without sending anything.
         */
        outr(USARTn_BASE + US_IDR_OFF, US_TXRDY);
        return;
	}

    if (rbf->rbf_cnt) {

#ifdef UART_CTS_BIT
        /*
         * If CTS has been disabled, we disable the transmit interrupts
         * and return without sending anything.
         */
        if (cts_sense && bit_is_set(UART_CTS_PIN, UART_CTS_BIT)) {
            outr(USARTn_BASE + US_IDR_OFF, US_TXRDY);
            sbi(EIMSK, UART_CTS_BIT);
            return;
        }
#endif
        rbf->rbf_cnt--;

        /*
         * Send address in multidrop mode.
         */
        if (tx_aframe) {
            outr(USARTn_BASE + US_CR_OFF, US_SENDA);
        }

        /*
         * Start transmission of the next character.
         */
        outr(USARTn_BASE + US_THR_OFF, *cp);

        /*
         * Wrap around the buffer pointer if we reached its end.
         */
        if (++cp == rbf->rbf_last) {
            cp = rbf->rbf_start;
        }
        rbf->rbf_tail = cp;
        if (rbf->rbf_cnt == rbf->rbf_lwm) {
            NutEventPostFromIrq(&rbf->rbf_que);
        }
    }

    /*
     * Nothing left to transmit, disable interrupt.
     */
    else {
        outr(USARTn_BASE + US_IDR_OFF, US_TXRDY);
        rbf->rbf_cnt = 0;
        NutEventPostFromIrq(&rbf->rbf_que);
    }
}


/*
 * \brief USARTn receiver ready interrupt handler.
 *
 *
 * \param rbf Pointer to the receiver ring buffer.
 */

static void At91UsartRxReady(RINGBUF *rbf) 
{
    register size_t cnt;
    register uint8_t ch;

    /*
     * We read the received character as early as possible to avoid overflows
     * caused by interrupt latency.
     */
    ch = inb(USARTn_BASE + US_RHR_OFF);

    /* Collect receiver errors. */
    rx_errors |= inr(USARTn_BASE + US_CSR_OFF) & (US_OVRE | US_FRAME | US_PARE);

    /*
     * Handle software handshake. We have to do this before checking the
     * buffer, because flow control must work in write-only mode, where
     * there is no receive buffer.
     */
    if (flow_control) {
        /* XOFF character disables transmit interrupts. */
        if (ch == ASCII_XOFF) {
            outr(USARTn_BASE + US_IDR_OFF, US_TXRDY);
            flow_control |= XOFF_RCVD;
            return;
        }
        /* XON enables transmit interrupts. */
        else if (ch == ASCII_XON) {
            outr(USARTn_BASE + US_IER_OFF, US_TXRDY);
            flow_control &= ~XOFF_RCVD;
            return;
        }
    }

    /*
     * Check buffer overflow.
     */
    cnt = rbf->rbf_cnt;
    if (cnt >= rbf->rbf_siz) {
        rx_errors |= US_OVRE;
        return;
    }

    /* Wake up waiting threads if this is the first byte in the buffer. */
    if (cnt++ == 0){
        NutEventPostFromIrq(&rbf->rbf_que);
    }

    /*
     * Check the high watermark for software handshake. If the number of
     * buffered bytes is above this mark, then send XOFF.
     */
    else if (flow_control) {
        if(cnt >= rbf->rbf_hwm) {
            if((flow_control & XOFF_SENT) == 0) {
                if (inr(USARTn_BASE + US_CSR_OFF) & US_TXRDY) {
                    outb(USARTn_BASE + US_THR_OFF, ASCII_XOFF);
                    flow_control |= XOFF_SENT;
                    flow_control &= ~XOFF_PENDING;
                } else {
                    flow_control |= XOFF_PENDING;
                }
            }
        }
    }

#ifdef UART_RTS_BIT
    /*
     * Check the high watermark for hardware handshake. If the number of
     * buffered bytes is above this mark, then disable RTS.
     */
    else if (rts_control && cnt >= rbf->rbf_hwm) {
        sbi(UART_RTS_PORT, UART_RTS_BIT);
    }
#endif

    /*
     * Store the character and increment and the ring buffer pointer.
     */
    *rbf->rbf_head++ = ch;
    if (rbf->rbf_head == rbf->rbf_last) {
        rbf->rbf_head = rbf->rbf_start;
    }

    /* Update the ring buffer counter. */
    rbf->rbf_cnt = cnt;
}

/*!
 * \brief USART interrupt handler.
 *
 * \param arg Pointer to the device specific control block.
 */
static void At91UsartInterrupt(void *arg)
{
    USARTDCB *dcb = (USARTDCB *)arg;
    ureg_t csr = inr(USARTn_BASE + US_CSR_OFF);

    if (csr & US_RXRDY) {
        At91UsartRxReady(&dcb->dcb_rx_rbf);
    }
    if (csr & US_TXRDY) {
        At91UsartTxReady(&dcb->dcb_tx_rbf);
    }

#ifdef UART_HDX_BIT
    if (csr & US_TXEMPTY) {
        At91UsartTxEmpty(&dcb->dcb_tx_rbf);
    }
#endif /*  UART_HDX_BIT */
}

/*!
 * \brief Carefully enable USART hardware functions.
 *
 * Always enabale transmitter and receiver, even on read-only or
 * write-only mode. So we can support software flow control.
 */
static void At91UsartEnable(void)
{
    NutEnterCritical();

    /* Enable UART receiver and transmitter. */
    outr(USARTn_BASE + US_CR_OFF, US_RXEN | US_TXEN);

    /* Enable UART receiver and transmitter interrupts. */
    outr(USARTn_BASE + US_IER_OFF, US_RXRDY | US_TXRDY);
    NutIrqEnable(&SIG_UART);

#ifdef UART_HDX_BIT
    if (hdx_control) {
        /* Enable transmit complete interrupt. */
        sbi(UCSRnB, TXCIE);
    }
#endif

    NutExitCritical();
}

/*!
 * \brief Carefully disable USART hardware functions.
 */
static void At91UsartDisable(void)
{
    /*
     * Disable USART interrupts.
     */
    NutEnterCritical();
    outr(USARTn_BASE + US_IDR_OFF, 0xFFFFFFFF);
    NutExitCritical();

    /*
     * Allow incoming or outgoing character to finish.
     */
    NutDelay(10);

    /*
     * Disable USART transmit and receive.
     */
    outr(USARTn_BASE + US_CR_OFF, US_RXDIS | US_TXDIS);
}

/*!
 * \brief Query the USART hardware for the selected speed.
 *
 * This function is called by ioctl function of the upper level USART
 * driver through the USARTDCB jump table.
 *
 * \return The currently selected baudrate.
 */
static uint32_t At91UsartGetSpeed(void)
{
    ureg_t cs = inr(USARTn_BASE + US_MR_OFF);
    uint32_t clk;

    clk = NutClockGet(NUT_HWCLK_PERIPHERAL);
    if ((cs & US_CLKS) == US_CLKS_MCK8) {
        clk /= 8;
    }
    else if ((cs & US_CLKS) != US_CLKS_MCK) {
        clk = 0;
    }
    return clk / (16UL * (inr(USARTn_BASE + US_BRGR_OFF) & 0xFFFF));
}

/*!
 * \brief Set the USART hardware bit rate.
 *
 * This function is called by ioctl function of the upper level USART
 * driver through the USARTDCB jump table.
 *
 * \param rate Number of bits per second.
 *
 * \return 0 on success, -1 otherwise.
 */
static int At91UsartSetSpeed(uint32_t rate)
{
    At91UsartDisable();
    outr(USARTn_BASE + US_BRGR_OFF, (NutClockGet(NUT_HWCLK_PERIPHERAL) / (8 * (rate)) + 1) / 2);
    At91UsartEnable();

    return 0;
}

/*!
 * \brief Query the USART hardware for the number of data bits.
 *
 * This function is called by ioctl function of the upper level USART
 * driver through the USARTDCB jump table.
 *
 * \return The number of data bits set.
 */
static uint8_t At91UsartGetDataBits(void)
{
    ureg_t val = inr(USARTn_BASE + US_MR_OFF);

    if ((val & US_PAR) == US_PAR_MULTIDROP) {
        val = 9;
    }
    else {
        val &= US_CHRL;
        if (val == US_CHRL_5) {
            val = 5;
        }
        else if (val == US_CHRL_6) {
            val = 6;
        }
        else if (val == US_CHRL_7) {
            val = 7;
        }
        else {
            val = 8;
        }
    }
    return (uint8_t)val;
}

/*!
 * \brief Set the USART hardware to the number of data bits.
 *
 * This function is called by ioctl function of the upper level USART
 * driver through the USARTDCB jump table.
 *
 * \return 0 on success, -1 otherwise.
 */
static int At91UsartSetDataBits(uint8_t bits)
{
    ureg_t val = inr(USARTn_BASE + US_MR_OFF);

    if (bits == 9) {
        val &= ~US_PAR;
        val |= US_PAR_MULTIDROP;
    }
    else {
        val &= ~US_CHRL;
        if (bits == 5) {
            val |= US_CHRL_5;
        }
        else if (bits == 6) {
            val |= US_CHRL_6;
        }
        else if (bits == 7) {
            val |= US_CHRL_7;
        }
        else if (bits == 8) {
            val |= US_CHRL_8;
        }
    }

    At91UsartDisable();
    outr(USARTn_BASE + US_MR_OFF, val);
    At91UsartEnable();

    /*
     * Verify the result.
     */
    if (At91UsartGetDataBits() != bits) {
        return -1;
    }
    return 0;
}

/*!
 * \brief Query the USART hardware for the parity mode.
 *
 * This routine is called by ioctl function of the upper level USART
 * driver through the USARTDCB jump table.
 *
 * \return Parity mode, either 0 (disabled), 1 (odd), 2 (even) or 9 (multidrop).
 */
static uint8_t At91UsartGetParity(void)
{
    ureg_t val = inr(USARTn_BASE + US_MR_OFF) & US_PAR;

    if ((val & US_PAR) == US_PAR_MULTIDROP) {
        val = 9;
    }
    else {
        if (val == US_PAR_ODD) {
            val = 1;
        }
        else if (val == US_PAR_EVEN) {
            val = 2;
        }
        else {
            val = 0;
        }
    }
    return (uint8_t)val;
}

/*!
 * \brief Set the USART hardware to the specified parity mode.
 *
 * This routine is called by ioctl function of the upper level USART
 * driver through the USARTDCB jump table.
 *
 * \param mode 0 (disabled), 1 (odd) or 2 (even)
 *
 * \return 0 on success, -1 otherwise.
 */
static int At91UsartSetParity(uint8_t mode)
{
    ureg_t val = inr(USARTn_BASE + US_MR_OFF) & ~US_PAR;

    switch (mode) {
    case 0:
        val |= US_PAR_NO;
        break;
    case 1:
        val |= US_PAR_ODD;
        break;
    case 2:
        val |= US_PAR_EVEN;
        break;
    }
    At91UsartDisable();
    outr(USARTn_BASE + US_MR_OFF, val);
    At91UsartEnable();

    /*
     * Verify the result.
     */
    if (At91UsartGetParity() != mode) {
        return -1;
    }
    return 0;
}

/*!
 * \brief Query the USART hardware for the number of stop bits.
 *
 * This routine is called by ioctl function of the upper level USART
 * driver through the USARTDCB jump table.
 *
 * \return The number of stop bits set, either 1, 2 or 3 (1.5 bits).
 */
static uint8_t At91UsartGetStopBits(void)
{
    ureg_t val = inr(USARTn_BASE + US_MR_OFF) & US_NBSTOP;
    if (val == US_NBSTOP_1) {
        val = 1;
    }
    else if (val == US_NBSTOP_2) {
        val = 2;
    }
    else {
        val = 3;
    }
    return (uint8_t)val;
}

/*!
 * \brief Set the USART hardware to the number of stop bits.
 *
 * This routine is called by ioctl function of the upper level USART
 * driver through the USARTDCB jump table.
 *
 * \return 0 on success, -1 otherwise.
 */
static int At91UsartSetStopBits(uint8_t bits)
{
    ureg_t val = inr(USARTn_BASE + US_MR_OFF) & ~US_NBSTOP;

    switch(bits) {
    case 1:
        val |= US_NBSTOP_1;
        break;
    case 2:
        val |= US_NBSTOP_2;
        break;
    case 3:
        val |= US_NBSTOP_1_5;
        break;
    }
    At91UsartDisable();
    outr(USARTn_BASE + US_MR_OFF, val);
    At91UsartEnable();

    /*
     * Verify the result.
     */
    if (At91UsartGetStopBits() != bits) {
        return -1;
    }
    return 0;
}

/*!
 * \brief Query the USART hardware status.
 *
 * \return Status flags.
 */
static uint32_t At91UsartGetStatus(void)
{
    uint32_t rc = 0;
#if defined(US_MODE_HWHANDSHAKE)
    uint32_t csr = inr(USARTn_BASE + US_CSR_OFF);
#endif

    /*
     * Set receiver error flags.
     */
    if ((rx_errors & US_FRAME) != 0) {
        rc |= UART_FRAMINGERROR;
    }
    if ((rx_errors & US_OVRE) != 0) {
        rc |= UART_OVERRUNERROR;
    }
    if ((rx_errors & US_PARE) != 0) {
        rc |= UART_PARITYERROR;
    }

    /*
     * Determine software handshake status. The flow control status may
     * change during interrupt, but this doesn't really hurt us.
     */
    if (flow_control) {
        if (flow_control & XOFF_SENT) {
            rc |= UART_RXDISABLED;
        }
        if (flow_control & XOFF_RCVD) {
            rc |= UART_TXDISABLED;
        }
    }

    /*
     * Determine hardware handshake control status.
     */
#if defined(UART_RTS_BIT)
    if (bit_is_set(UART_RTS_PORT, UART_RTS_BIT)) {
        rc |= UART_RTSDISABLED;
        if (rts_control) {
            rc |= UART_RXDISABLED;
        }
    } else {
        rc |= UART_RTSENABLED;
    }
#elif defined(US_MODE_HWHANDSHAKE)
    /* How to find out? */
#endif

    /*
     * Determine hardware handshake sense status.
     */
#ifdef UART_CTS_BIT
    if (bit_is_set(UART_CTS_PIN, UART_CTS_BIT)) {
        rc |= UART_CTSDISABLED;
        if (cts_sense) {
            rc |= UART_RXDISABLED;
        }
    } else {
        rc |= UART_CTSENABLED;
    }
#elif defined(US_MODE_HWHANDSHAKE)
    if (csr & US_CTS) {
        rc |= UART_CTSDISABLED;
        if (cts_sense) {
            rc |= UART_RXDISABLED;
        }
    } else {
        rc |= UART_CTSENABLED;
    }
#endif

    /*
     * Determine hardware modem sense status.
     */
#if defined(US_MODE_HWHANDSHAKE) && 0
    /* I'm confused. Awful flag mismatch? Why do we have uart.h and usart.h? */
    if (csr & US_RI) {
        rc |= UART_RIDISABLED;
    } else {
        rc |= UART_RIENABLED;
    }
    if (csr & US_DSR) {
        rc |= UART_DSRDISABLED;
    } else {
        rc |= UART_DSRENABLED;
    }
    if (csr & US_DCD) {
        rc |= UART_DCDDISABLED;
    } else {
        rc |= UART_DCDENABLED;
    }
#endif

    /*
     * If transmitter and receiver haven't been detected disabled by any
     * of the checks above, then they are probably enabled.
     */
    if ((rc & UART_RXDISABLED) == 0) {
        rc |= UART_RXENABLED;
    }
    if ((rc & UART_TXDISABLED) == 0) {
        rc |= UART_TXENABLED;
    }

    /*
     * Process multidrop setting.
     */
    if (tx_aframe) {
        rc |= UART_TXADDRFRAME;
    } else {
        rc |= UART_TXNORMFRAME;
    }
    return rc;
}

/*!
 * \brief Set the USART hardware status.
 *
 * \param flags Status flags.
 *
 * \return 0 on success, -1 otherwise.
 */
static int At91UsartSetStatus(uint32_t flags)
{
    /*
     * Process software handshake control.
     */
    if (flow_control) {

        /* Access to the flow control status must be atomic. */
        NutEnterCritical();

        /*
         * Enabling or disabling the receiver means to behave like
         * having sent a XON or XOFF character resp.
         */
        if (flags & UART_RXENABLED) {
            flow_control &= ~XOFF_SENT;
        } else if (flags & UART_RXDISABLED) {
            flow_control |= XOFF_SENT;
        }

        /*
         * Enabling or disabling the transmitter means to behave like
         * having received a XON or XOFF character resp.
         */
        if (flags & UART_TXENABLED) {
            flow_control &= ~XOFF_RCVD;
        } else if (flags & UART_TXDISABLED) {
            flow_control |= XOFF_RCVD;
        }
        NutExitCritical();
    }

    /*
     * Process hardware handshake control.
     */
#if defined(UART_RTS_BIT)
    /* Manually controlled via GPIO. */
    if (rts_control) {
        if (flags & UART_RXDISABLED) {
            sbi(UART_RTS_PORT, UART_RTS_BIT);
        }
        if (flags & UART_RXENABLED) {
            cbi(UART_RTS_PORT, UART_RTS_BIT);
        }
    }
    if (flags & UART_RTSDISABLED) {
        sbi(UART_RTS_PORT, UART_RTS_BIT);
    }
    if (flags & UART_RTSENABLED) {
        cbi(UART_RTS_PORT, UART_RTS_BIT);
    }
#elif defined(US_MODE_HWHANDSHAKE)
    /* Build in hardware. */
    if (rts_control) {
        if (flags & UART_RXDISABLED) {
            outr(USARTn_BASE + US_CR_OFF, US_RTSDIS);
        }
        if (flags & UART_RXENABLED) {
            outr(USARTn_BASE + US_CR_OFF, US_RTSEN);
        }
    }
    if (flags & UART_RTSDISABLED) {
        outr(USARTn_BASE + US_CR_OFF, US_RTSDIS);
    }
    if (flags & UART_RTSENABLED) {
        outr(USARTn_BASE + US_CR_OFF, US_RTSEN);
    }
#endif

    /*
     * Process hardware modem control.
     */
#if defined(UART_DTR_BIT)
    /* Manually controlled via GPIO. */
    if (flags & UART_DTRDISABLED) {
        sbi(UART_DTR_PORT, UART_DTR_BIT);
    }
    if (flags & UART_DTRENABLED) {
        cbi(UART_DTR_PORT, UART_DTR_BIT);
    }
#elif defined(US_MODE_HWHANDSHAKE)
    /* Build in hardware. */
    if (flags & UART_DTRDISABLED) {
        outr(USARTn_BASE + US_CR_OFF, US_DTRDIS);
    }
    if (flags & UART_DTRENABLED) {
        outr(USARTn_BASE + US_CR_OFF, US_DTREN);
    }
#endif

    /*
     * Process multidrop setting.
     */
    if (flags & UART_TXADDRFRAME) {
        tx_aframe = 1;
    }
    if (flags & UART_TXNORMFRAME) {
        tx_aframe = 0;
    }

    /*
     * Clear UART receive errors.
     */
    if (flags & UART_ERRORS) {
        outr(USARTn_BASE + US_CR_OFF, US_RSTSTA);
    }

    /*
     * Verify the result.
     */
    if ((At91UsartGetStatus() & ~UART_ERRORS) != flags) {
        return -1;
    }
    return 0;
}

/*!
 * \brief Query the USART hardware for synchronous mode.
 *
 * This function is called by ioctl function of the upper level USART
 * driver through the USARTDCB jump table.
 *
 * \return Or-ed combination of \ref UART_SYNC, \ref UART_MASTER,
 *         \ref UART_NCLOCK and \ref UART_HIGHSPEED.
 */
static uint8_t At91UsartGetClockMode(void)
{
    uint8_t rc = 0;

    return rc;
}

/*!
 * \brief Set asynchronous or synchronous mode.
 *
 * This function is called by ioctl function of the upper level USART
 * driver through the USARTDCB jump table.
 *
 * \param mode Must be an or-ed combination of USART_SYNC, USART_MASTER,
 *             USART_NCLOCK and USART_HIGHSPEED.
 *
 * \return 0 on success, -1 otherwise.
 */
static int At91UsartSetClockMode(uint8_t mode)
{
    /*
     * Verify the result.
     */
    if (At91UsartGetClockMode() != mode) {
        return -1;
    }
    return 0;
}

/*!
 * \brief Query flow control mode.
 *
 * This routine is called by ioctl function of the upper level USART
 * driver through the USARTDCB jump table.
 *
 * \return See UsartIOCtl().
 */
static uint32_t At91UsartGetFlowControl(void)
{
    uint32_t rc = 0;

    if (flow_control) {
        rc |= USART_MF_XONXOFF;
    } else {
        rc &= ~USART_MF_XONXOFF;
    }

#ifdef UART_RTS_BIT
    if (rts_control) {
        rc |= USART_MF_RTSCONTROL;
    } else {
        rc &= ~USART_MF_RTSCONTROL;
    }
#endif

#ifdef UART_CTS_BIT
    if (cts_sense) {
        rc |= USART_MF_CTSSENSE;
    } else {
        rc &= ~USART_MF_CTSSENSE;
    }
#endif

#ifdef UART_HDX_BIT
    if (hdx_control) {
        rc |= USART_MF_HALFDUPLEX;
    } else {
        rc &= ~USART_MF_HALFDUPLEX;
    }
#endif

    return rc;
}

/*!
 * \brief Set flow control mode.
 *
 * This function is called by ioctl function of the upper level USART
 * driver through the USARTDCB jump table.
 *
 * \param flags See UsartIOCtl().
 *
 * \return 0 on success, -1 otherwise.
 */
static int At91UsartSetFlowControl(uint32_t flags)
{
    /*
     * Set software handshake mode.
     */
    if (flags & USART_MF_XONXOFF) {
        if(flow_control == 0) {
            NutEnterCritical();
            flow_control = 1 | XOFF_SENT;  /* force XON to be sent on next read */
            NutExitCritical();
        }
    } else {
        NutEnterCritical();
        flow_control = 0;
        NutExitCritical();
    }

    /*
     * Set RTS control mode.
     */
    if (flags & USART_MF_RTSCONTROL) {
#if defined(UART_RTS_BIT)
        sbi(UART_RTS_PORT, UART_RTS_BIT);
        sbi(UART_RTS_DDR, UART_RTS_BIT);
        rts_control = 1;
#endif
    } else if (rts_control) {
        rts_control = 0;
#if defined(UART_RTS_BIT)
        cbi(UART_RTS_DDR, UART_RTS_BIT);
#endif
    }

/*
 * Set CTS sense mode.
 */
    if (flags & USART_MF_CTSSENSE) {
#if defined(UART_CTS_BIT)
        /* Register CTS sense interrupt. */
        if (NutRegisterIrqHandler(&UART_CTS_SIGNAL, At91UsartCts, 0)) {
            return -1;
        }
        sbi(UART_CTS_PORT, UART_CTS_BIT);
        cbi(UART_CTS_DDR, UART_CTS_BIT);
        cts_sense = 1;
#elif defined(US_MODE_HWHANDSHAKE)
        uint32_t val = inr(USARTn_BASE + US_MR_OFF) & ~US_MODE;

        outr(USARTn_BASE + US_MR_OFF, val | US_MODE_HWHANDSHAKE);
        cts_sense = 1;
        rts_control = 1;
#endif
    } else if (cts_sense) {
#if defined(UART_CTS_BIT)
        /* Deregister CTS sense interrupt. */
        NutRegisterIrqHandler(&UART_CTS_SIGNAL, 0, 0);
        cbi(UART_CTS_DDR, UART_CTS_BIT);
#elif defined(US_MODE_HWHANDSHAKE)
        uint32_t val = inr(USARTn_BASE + US_MR_OFF) & ~US_MODE;

        outr(USARTn_BASE + US_MR_OFF, val);
        rts_control = 0;
#endif
        cts_sense = 0;
    }

#ifdef UART_HDX_BIT
    /*
     * Set half duplex mode.
     */
    if (flags & USART_MF_HALFDUPLEX) {
        /* Register transmit complete interrupt. */
        if (NutRegisterIrqHandler(&sig_UART_TRANS, At91UsartTxComplete, &dcb_usart.dcb_rx_rbf)) {
            return -1;
        }
        /* Initially enable the receiver. */
        UART_HDX_RX(UART_HDX_PORT, UART_HDX_BIT);
        sbi(UART_HDX_DDR, UART_HDX_BIT);
        hdx_control = 1;
        /* Enable transmit complete interrupt. */
        sbi(UCSRnB, TXCIE);
    } else if (hdx_control) {
        hdx_control = 0;
        /* disable transmit complete interrupt */
        cbi(UCSRnB, TXCIE);
        /* Deregister transmit complete interrupt. */
        NutRegisterIrqHandler(&sig_UART_TRANS, 0, 0);
        cbi(UART_HDX_DDR, UART_HDX_BIT);
    }
#endif

    /*
     * Verify the result.
     */
    if (At91UsartGetFlowControl() != flags) {
        return -1;
    }
    return 0;
}

/*!
 * \brief Start the USART transmitter hardware.
 *
 * The upper level USART driver will call this function through the
 * USARTDCB jump table each time it added one or more bytes to the
 * transmit buffer.
 */
static void At91UsartTxStart(void)
{
#ifdef UART_HDX_BIT
    if (hdx_control) {
        /* Enable half duplex transmitter. */
        UART_HDX_TX(UART_HDX_PORT, UART_HDX_BIT);
    }
#endif
    /* Enable transmit interrupts. */
    outr(USARTn_BASE + US_IER_OFF, US_TXRDY);
}

/*!
 * \brief Start the USART receiver hardware.
 *
 * The upper level USART driver will call this function through the
 * USARTDCB jump table each time it removed enough bytes from the
 * receive buffer. Enough means, that the number of bytes left in
 * the buffer is below the low watermark.
 */
static void At91UsartRxStart(void)
{
    /*
     * Do any required software flow control.
     */
    if (flow_control && (flow_control & XOFF_SENT) != 0) {
        NutEnterCritical();
        if ((inr(USARTn_BASE + US_CSR_OFF) & US_TXRDY)) {
            outr(USARTn_BASE + US_THR_OFF, ASCII_XON);
            flow_control &= ~XON_PENDING;
        } else {
            flow_control |= XON_PENDING;
        }
        flow_control &= ~(XOFF_SENT | XOFF_PENDING);
        NutExitCritical();
    }
#ifdef UART_RTS_BIT
    if (rts_control) {
        /* Enable RTS. */
        cbi(UART_RTS_PORT, UART_RTS_BIT);
    }
#endif
}

/*
 * \brief Initialize the USART hardware driver.
 *
 * This function is called during device registration by the upper level
 * USART driver through the USARTDCB jump table.
 *
 * \return 0 on success, -1 otherwise.
 */
static int At91UsartInit(void)
{
    /*
     * Register receive and transmit interrupts.
     */
    if (NutRegisterIrqHandler(&SIG_UART, At91UsartInterrupt, &dcb_usart)) {
        return -1;
    }

    /* Enable UART clock. */
#if defined(US_ID)
#if defined(PS_PCER)
    outr(PS_PCER, _BV(US_ID));
#endif
#if defined(PMC_PCER)
    outr(PMC_PCER, _BV(US_ID));
#endif
#endif

    /* Disable GPIO on UART tx/rx pins. */
#if defined(PIO_PDR) && defined(US_PIO_PINS)
    outr(PIO_PDR, US_PIO_PINS);
#endif

#if defined(PIOA_PDR) && defined(US_PIOA_PINS)
    outr(PIOA_PDR, US_PIOA_PINS);
#endif
#if defined(PIOA_ASR) && defined(US_PIOA_PINS_A)
    outr(PIOA_ASR, US_PIOA_PINS_A);
#endif
#if defined(PIOA_BSR) && defined(US_PIOA_PINS_B)
    outr(PIOA_BSR, US_PIOA_PINS_B);
#endif

#if defined(PIOB_PDR) && defined(US_PIOB_PINS)
    outr(PIOB_PDR, US_PIOB_PINS);
#endif
#if defined(PIOB_ASR) && defined(US_PIOB_PINS_A)
    outr(PIOB_ASR, US_PIOB_PINS_A);
#endif
#if defined(PIOB_BSR) && defined(US_PIOB_PINS_B)
    outr(PIOB_BSR, US_PIOB_PINS_B);
#endif

#if defined(PIOC_PDR) && defined(US_PIOC_PINS)
    outr(PIOC_PDR, US_PIOC_PINS);
#endif
#if defined(PIOC_ASR) && defined(US_PIOC_PINS_A)
    outr(PIOC_ASR, US_PIOC_PINS_A);
#endif
#if defined(PIOC_BSR) && defined(US_PIOC_PINS_B)
    outr(PIOC_BSR, US_PIOC_PINS_B);
#endif

    /* Reset UART. */
    outr(USARTn_BASE + US_CR_OFF, US_RSTRX | US_RSTTX | US_RXDIS | US_TXDIS);
    /* Disable all UART interrupts. */
    outr(USARTn_BASE + US_IDR_OFF, 0xFFFFFFFF);
    /* Clear UART counter registers. */
#if defined (US_RCR_OFF)
    outr(USARTn_BASE + US_RCR_OFF, 0);
#endif
#if defined (US_TCR_OFF)
    outr(USARTn_BASE + US_TCR_OFF, 0);
#endif
    /* Set UART baud rate generator register. */
#if defined(AT91_PLL_MAINCK)
    outr(USARTn_BASE + US_BRGR_OFF, (At91GetMasterClock() / (8 * (115200)) + 1) / 2);
#else
    outr(USARTn_BASE + US_BRGR_OFF, (NutGetCpuClock() / (8 * (115200)) + 1) / 2);
#endif
    /* Set UART mode to 8 data bits, no parity and 1 stop bit. */

#ifdef AT91_UART_RS485_MODE
    outr(USARTn_BASE + US_MR_OFF, US_CHMODE_NORMAL | US_CHRL_8 | US_PAR_NO | US_NBSTOP_1 | US_MODE_RS485);
#else
    outr(USARTn_BASE + US_MR_OFF, US_CHMODE_NORMAL | US_CHRL_8 | US_PAR_NO | US_NBSTOP_1);
#endif

    return 0;
}

/*
 * \brief Deinitialize the USART hardware driver.
 *
 * This function is called during device deregistration by the upper
 * level USART driver through the USARTDCB jump table.
 *
 * \return 0 on success, -1 otherwise.
 */
static int At91UsartDeinit(void)
{
    /* Deregister receive and transmit interrupts. */
    NutRegisterIrqHandler(&SIG_UART, 0, 0);

    /* Reset UART. */
    outr(USARTn_BASE + US_CR_OFF, US_RSTRX | US_RSTTX | US_RXDIS | US_TXDIS);
    /* Disable all UART interrupts. */
    outr(USARTn_BASE + US_IDR_OFF, 0xFFFFFFFF);

    /* Disable UART clock. */
#if defined (US_ID)
#if defined(PS_PCDR)
    outr(PS_PCDR, _BV(US_ID));
#endif
#if defined(PMC_PCDR)
    outr(PMC_PCDR, _BV(US_ID));
#endif
#endif

    /* Enable GPIO on UART tx/rx pins. */
#if defined(PIO_PER) && defined(US_PIO_PINS)
    outr(PIO_PER, US_PIO_PINS);
#endif
#if defined(PIOA_PER) && defined(US_PIOA_PINS)
    outr(PIOA_PER, US_PIOA_PINS);
#endif
#if defined(PIOB_PER) && defined(US_PIOB_PINS)
    outr(PIOB_PER, US_PIOB_PINS);
#endif
#if defined(PIOC_PER) && defined(US_PIOC_PINS)
    outr(PIOC_PER, US_PIOC_PINS);
#endif

    /*
     * Disabling flow control shouldn't be required here, because it's up
     * to the upper level to do this on the last close or during
     * deregistration.
     */
#ifdef UART_HDX_BIT
    /* Deregister transmit complete interrupt. */
    if (hdx_control) {
        hdx_control = 0;
        NutRegisterIrqHandler(&sig_UART_TRANS, 0, 0);
    }
#endif

#ifdef UART_CTS_BIT
    if (cts_sense) {
        cts_sense = 0;
        cbi(UART_CTS_DDR, UART_CTS_BIT);
        /* Deregister CTS sense interrupt. */
        NutRegisterIrqHandler(&UART_CTS_SIGNAL, 0, 0);
    }
#endif

#ifdef UART_RTS_BIT
    if (rts_control) {
        rts_control = 0;
        cbi(UART_RTS_DDR, UART_RTS_BIT);
    }
#endif

    return 0;
}

/*@}*/
