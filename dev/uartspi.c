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
 * Revision 1.2  2004/03/16 16:48:27  haraldkipp
 * Added Jan Dubiec's H8/300 port.
 *
 * Revision 1.1.1.1  2003/05/09 14:40:56  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.17  2003/02/04 17:50:55  harald
 * Version 3 released
 *
 * Revision 1.16  2002/11/02 15:15:13  harald
 * Library dependencies removed
 *
 * Revision 1.15  2002/09/15 16:44:02  harald
 * *** empty log message ***
 *
 * Revision 1.14  2002/08/11 12:25:49  harald
 * ICC mods
 *
 * Revision 1.13  2002/08/08 16:18:04  harald
 * Removed digital I/O control
 *
 * Revision 1.12  2002/07/03 16:45:38  harald
 * Using GCC 3.2
 *
 * Revision 1.11  2002/06/26 17:29:08  harald
 * First pre-release with 2.4 stack
 *
 */

#include <string.h>

#ifdef __GNUC__
#include <avr/parity.h>
#endif

#include <sys/atom.h>
#include <sys/event.h>
#include <sys/timer.h>
#include <sys/heap.h>
#include <sys/ifstream.h>

#include <dev/uart.h>
#include <dev/irqreg.h>
#include <dev/uartspi.h>

/*!
 * \addtogroup xgUartSpi
 */
/*@{*/

static HANDLE dio_change;
static u_long di_status;
static u_long do_set;           /* Set by application. */
static u_long do_status;        /* Status sent to I/O. */


/*!
 * \brief Basic SPI UART device communication.
 *
 * Exchange information with a SPI UART. This routine is 
 * called on SPI device interrupts and by some driver 
 * routines.
 *
 * SPI devices raise the interrupt line after reset
 * and if they received any serial data.
 *
 * Driver routines call this one in order to send
 * configuration data or serial output data to
 * the SPI device.
 *
 * \param arg Required by the interrupt handler but not used.
 *
 */
static void SpiDevRequest(void *arg)
{
    u_char vec;
    u_char msk;
    u_char tav = 0;
    u_char rav = 0;
    register u_char xav = 0;
    register u_char ch;
    u_char use7bit;
    u_char parity;
    u_char i;
    register u_char rx_idx = 0;
    u_char wr_idx = 0;
    register u_char tx_idx = 0;
    u_char rd_idx = 0;
    u_short to;
    u_char idx;
    u_char rx_post = 0;
    u_char tx_post = 0;
    IFSTREAM *ifs = 0;
    UARTSDCB *dcb = 0;
    u_char ict;                 /* Imagecraft dummy */

    /*
     * Find the device with the highest priority 
     * we want to talk to. Result is a bit vector
     * with the corresponding bit of the device 
     * cleared.
     */
    vec = 0xFF;
    for (idx = 7; idx < 8; idx--) {
        if ((ifs = devUarts[idx].dev_icb) != 0) {
            dcb = devUarts[idx].dev_dcb;

            /*
             * Check if device needs config data.
             */
            if (dcb->dcb_configured == 0) {
                vec = 0xFE << idx;
                break;
            }

            /*
             * Check if there's any data to transmit.
             */
            if (ifs->if_tx_idx != ifs->if_wr_idx) {
                vec = 0xFE << idx;
                break;
            }
        }
    }

    /*
     * Changing SPI role is perfomed in three steps:
     * 1. Wait until the previous SPI slave returned
     *    to the master mode.
     * 2. Raise the attention line.
     * 3. Wait until all devices are in slave mode.
     */
    for (to = 32000; to; to--) {
        if (bit_is_clear(PINB, 0))
            break;
    }
    if (to == 0) {
        sbi(PORTB, 6);
        return;
    }
    cbi(PORTB, 6);
    for (to = 32000; to; to--) {
        if (bit_is_set(PINB, 0))
            break;
    }
    if (to == 0) {
        sbi(PORTB, 6);
        return;
    }

    /*
     * Switch to master mode and exchange the 
     * bit vector.
     */
    sbi(SPCR, MSTR);
    outp(vec, SPDR);
    for (to = 32000; to; to--) {
        if (bit_is_set(SPSR, SPIF))
            break;
    }
    if (to == 0) {
        sbi(PORTB, 6);
        return;
    }
    vec &= inp(SPDR);

    /*
     * Determine the highest priority device, 
     * which will now talk to us.
     */
    if (vec != 0xFF) {
        msk = 0x80;
        for (idx = 7; idx < 8; idx--, msk >>= 1)
            if ((vec & msk) == 0)
                break;

        tav = 0;
        rav = 64;
        if ((ifs = devUarts[idx].dev_icb) != 0) {
            dcb = devUarts[idx].dev_dcb;
            if (dcb->dcb_configured == 1) {
                wr_idx = ifs->if_wr_idx;
                tx_idx = ifs->if_tx_idx;
                if (wr_idx != tx_idx)
                    tx_post = 1;

                rd_idx = ifs->if_rd_idx;
                rx_idx = ifs->if_rx_idx;
                if (rd_idx == rx_idx)
                    rx_post = 1;

                for (i = tx_idx; i != wr_idx; i++)
                    if (++tav >= 32)
                        break;

                i = rx_idx;
                if (i != rd_idx) {
                    for (rav = 0; ++i != rd_idx;) {
                        if (++rav >= 64)
                            break;
                    }
                }
            } else
                dcb->dcb_configured = 0;
        }
    }

    /*
     * Set the ATN line to inform all devices
     * that we are going to become the SPI slave.
     */
    sbi(PORTB, 6);

    /*
     * Wait until all devices switched us into
     * SPI slave mode.
     */
    for (to = 32000; to; to--) {
        if (bit_is_set(SPSR, SPIF))
            break;
    }
    if (to == 0) {
        cbi(SPCR, MSTR);
        return;
    }
    ict = inp(SPDR);

    /*
     * If all bits of the bit vector are set,
     * we don't want to talk to any serial device
     * and no serial device wants to talk to us.
     * So we are going to talk to the Diginut device.
     */
    if (vec == 0xFF) {
        /*
         * Send output status and receive 
         * input status low byte.
         */
        outp(do_set & 0xFF, SPDR);
        for (to = 4000; to; to--) {
            if (bit_is_set(SPSR, SPIF))
                break;
        }
        if (to == 0) {
            return;
        }
        di_status = inp(SPDR);

        /*
         * Receive input status high byte.
         */
        outp(0xFF, SPDR);
        for (to = 32000; to; to--) {
            if (bit_is_set(SPSR, SPIF))
                break;
        }
        if (to == 0) {
            return;
        }
        di_status |= ((u_long) inp(SPDR)) << 8;

        do_status = do_set;
        /*
         * FIXME: May be called with interrupts enabled??
         */
        NutEventPostAsync(&dio_change);
        return;
    }

    /*
     * Send configuration data.
     */
    if (ifs && dcb->dcb_configured == 0) {
        /*
         * Device recognizes a configuration telegram
         * by setting bit 7 of the first byte.
         */
        outp(0x80, SPDR);
        for (to = 32000; to; to--) {
            if (bit_is_set(SPSR, SPIF))
                break;
        }
        if (to == 0) {
            dcb->dcb_configured = 255;  /* Device not available */
            return;
        }
        ict = inp(SPDR);

        /*
         * First byte is the version number.
         */
        outp(0xFF, SPDR);
        loop_until_bit_is_set(SPSR, SPIF);
        ict = inp(SPDR);

        /*
         * Second byte is the baudrate factor.
         */
        outp(dcb->dcb_baudSelect, SPDR);
        loop_until_bit_is_set(SPSR, SPIF);
        ict = inp(SPDR);

        /*
         * Third byte contains the mode flags.
         */
        outp(dcb->dcb_mode, SPDR);
        loop_until_bit_is_set(SPSR, SPIF);
        ict = inp(SPDR);

        dcb->dcb_configured = 1;
        return;
    }

    /*
     * Exchange number of bytes to transmit
     */
    outp(tav, SPDR);
    for (to = 1000; to; to--) {
        if (bit_is_set(SPSR, SPIF))
            break;
    }
    if (to == 0) {
        return;
    }
    if ((i = inp(SPDR)) < tav)
        tav = i;

    if (ifs) {
        if (i == 32 && tav == 0) {
            ifs->if_tx_act = 0;
        } else {
            ifs->if_tx_act = 1;
        }
    }

    /*
     * Exchange number of bytes to receive
     */
    outp(rav, SPDR);
    for (to = 32000; to; to--) {
        if (bit_is_set(SPSR, SPIF))
            break;
    }
    if (to == 0) {
        return;
    }
    if ((i = inp(SPDR)) < rav)
        rav = i;

    xav = (rav >= tav) ? rav : tav;

    if (xav) {
        if (ifs) {
            /*! \brief Mode.
             *  xxxx xxx0  8 bit
             *  xxxx xxx1  7 bit
             *  xxxx x0xx  No parity bit
             *  xxxx x10x  Even parity
             *  xxxx x11x  Odd parity
             */
            if (dcb->dcb_mode & 0x01) {
                use7bit = 1;
                if (dcb->dcb_mode & 0x04) {
                    if (dcb->dcb_mode & 0x02)
                        parity = 1;
                    else
                        parity = 2;
                } else
                    parity = 0;
            } else {
                use7bit = 0;
                parity = 0;
            }
            while (xav--) {
                ch = ifs->if_tx_buf[tx_idx];
                if (tav) {
                    if (use7bit)
                        ch &= 0x7F;
                    if (parity == 1 && parity_even_bit(ch) == 0)
                        ch |= 0x80;
                    else if (parity == 2 && parity_even_bit(ch))
                        ch |= 0x80;
                    tav--;
                    tx_idx++;
                }
                outp(ch, SPDR);
                for (to = 32000; to; to--) {
                    if (bit_is_set(SPSR, SPIF))
                        break;
                }
                if (to == 0) {
                    return;
                }
                ch = inp(SPDR);
                if (rav) {
                    if (use7bit)
                        ch &= 0x7F;
                    ifs->if_rx_buf[rx_idx] = ch;
                    rav--;
                    rx_idx++;
                }
            }
            ifs->if_tx_idx = tx_idx;
            ifs->if_rx_idx = rx_idx;
            if (tx_post && tx_idx == wr_idx)
                NutEventPostAsync(&dcb->dcb_tx_rdy);
            if (rx_post && rx_idx != rd_idx)
                NutEventPostAsync(&dcb->dcb_rx_rdy);
        } else {
            while (xav--) {
                outp(0xff, SPDR);
                for (to = 32000; to; to--) {
                    if (bit_is_set(SPSR, SPIF))
                        break;
                }
                if (to == 0) {
                    return;
                }
                ict = inp(SPDR);
            }
        }
    }
}

/*!
 * \brief Wait for input.
 *
 * This function checks the input buffer for any data. If
 * the buffer is empty, the calling \ref xrThread "thread" 
 * will be blocked until at least one new character is 
 * received or a timeout occurs.
 *
 * \param dev Indicates the UART device.
 *
 * \return 0 on success, -1 on timeout.
 */
int UartSpiInput(NUTDEVICE * dev)
{
    int rc = 0;
    IFSTREAM *ifs = dev->dev_icb;
    UARTSDCB *dcb;

    NutEnterCritical();
    if (ifs->if_rd_idx == ifs->if_rx_idx) {
        dcb = dev->dev_dcb;
        do {
            rc = NutEventWait(&dcb->dcb_rx_rdy, dcb->dcb_rtimeout);
        } while (rc == 0 && ifs->if_rd_idx == ifs->if_rx_idx);
    }
    NutExitCritical();

    return rc;
}

/*!
 * \brief Initiate output.
 *
 * This function checks the output buffer for any data. If
 * the buffer contains at least one character, the transmitter
 * is started, if not already running. The function returns
 * immediately, without waiting for the character being
 * completely transmitted. Any remaining characters in the
 * output buffer are transmitted in the background.
 *
 * \param dev Indicates the UART device.
 *
 * \return 0 on success, -1 otherwise.
 */
int UartSpiOutput(NUTDEVICE * dev)
{
    IFSTREAM *ifs = dev->dev_icb;
    u_char retry = 0;

    NutEnterCritical();
    if (ifs->if_tx_idx != ifs->if_wr_idx) {
        while (ifs->if_tx_act == 0 && retry < 128) {
            SpiDevRequest(0);
            retry++;
        }
    }
    NutExitCritical();

    if (retry >= 128)
        return -1;
    return 0;
}

/*!
 * \brief Wait for output buffer empty.
 *
 * If the output buffer contains any data, the calling
 * thread is suspended until all data has been transmitted.
 *
 * \param dev Indicates the UART device.
 *
 * \return 0 on success, -1 otherwise.
 */
int UartSpiFlush(NUTDEVICE * dev)
{
    IFSTREAM *ifs = dev->dev_icb;
    UARTSDCB *dcb = dev->dev_dcb;

    /*
     * Start any pending output.
     */
    if (UartSpiOutput(dev))
        return -1;

    /*
     * Wait until output buffer empty.
     */
    NutEnterCritical();
    if (ifs->if_tx_idx != ifs->if_wr_idx)
        NutEventWait(&dcb->dcb_tx_rdy, dcb->dcb_wtimeout);
    NutExitCritical();

    return 0;
}

/*! 
 * \brief Set baudrate.
 *
 */
static int UartSpiSetSpeed(UARTSDCB * dcb, u_long baud)
{
    u_char retry = 0;

    dcb->dcb_baudSelect = dcb->dcb_crystal / (baud * 16L) - 1;

    NutEnterCritical();
    dcb->dcb_configured = 0;
    while (dcb->dcb_configured == 0 && retry < 128) {
        SpiDevRequest(0);
        retry++;
    }
    NutExitCritical();

    if (retry >= 128)
        return -1;
    return 0;
}

static int UartSpiGetSpeed(UARTSDCB * dcb, u_long * baud)
{
    *baud = dcb->dcb_crystal / (16L * ((u_long) dcb->dcb_baudSelect + 1L));
    return 0;
}

/*!
 * \brief Perform SPI UART control functions.
 *
 * \param dev  Identifies the device that receives the device-control
 *             function. This pointer must have been retrieved previously
 *             by using the NutDeviceOpen() function.
 * \param req  Requested control function. May be set to one of the
 *             following constants:
 *             - UART_SETSPEED, if conf points to an u_long value containing the baudrate.
 *             - UART_GETSPEED, if conf points to an u_long value receiving the current baudrate.
 *             - UART_SETDATABITS
 *             - UART_GETDATABITS
 *             - UART_SETPARITY
 *             - UART_GETPARITY
 *             - UART_SETSTOPBITS
 *             - UART_GETSTOPBITS
 *             - UART_SETSTATUS
 *             - UART_GETSTATUS
 *
 * \param conf Points to a buffer that contains any data required for
 *             the given control function or receives data from that
 *             function.
 * \return 0 on success, -1 otherwise.
 *
 */
int UartSpiIOCtl(NUTDEVICE * dev, int req, void *conf)
{
    int rc = -1;
    UARTSDCB *dcb;

    if (dev == 0)
        return rc;

    dcb = dev->dev_dcb;
    switch (req) {
    case UART_SETSPEED:
        rc = UartSpiSetSpeed(dcb, *((u_long *) conf));
        break;
    case UART_GETSPEED:
        rc = UartSpiGetSpeed(dcb, (u_long *) conf);
        break;
    case UART_SETDATABITS:
        if (*((u_long *) conf) == 8) {
            dcb->dcb_mode &= 0xF8;
            rc = 0;
        } else if (*((u_long *) conf) == 7) {
            dcb->dcb_mode |= 0x01;
            rc = 0;
        }
        break;
    case UART_GETDATABITS:
        if (dcb->dcb_mode & 1)
            *((u_long *) conf) = 7;
        else
            *((u_long *) conf) = 8;
        rc = 0;
        break;
    case UART_SETPARITY:
        if (*((u_long *) conf) == 0) {
            dcb->dcb_mode &= 0xF9;
            rc = 0;
        } else if (*((u_long *) conf) == 1) {
            dcb->dcb_mode &= 0xF8;
            dcb->dcb_mode |= 0x07;
            rc = 0;
        } else if (*((u_long *) conf) == 2) {
            dcb->dcb_mode &= 0xF8;
            dcb->dcb_mode |= 0x05;
            rc = 0;
        }
        break;
    case UART_GETPARITY:
        if ((dcb->dcb_mode & 0x04) == 0)
            *((u_long *) conf) = 0;
        else if (dcb->dcb_mode & 0x02)
            *((u_long *) conf) = 1;
        else
            *((u_long *) conf) = 2;
        rc = 0;
        break;
    case UART_SETSTOPBITS:
        if (*((u_long *) conf) == 1)
            rc = 0;
        break;
    case UART_GETSTOPBITS:
        *((u_long *) conf) = 1;
        rc = 0;
        break;
    case UART_GETSTATUS:
        //UartSpiGetStatus(dev, (u_long *)conf);
        break;
    case UART_SETSTATUS:
        break;
    case UART_SETREADTIMEOUT:
        dcb->dcb_rtimeout = *((u_long *) conf);
        rc = 0;
        break;
    case UART_GETREADTIMEOUT:
        *((u_long *) conf) = dcb->dcb_rtimeout;
        rc = 0;
        break;
    case UART_SETWRITETIMEOUT:
        dcb->dcb_wtimeout = *((u_long *) conf);
        rc = 0;
        break;
    case UART_GETWRITETIMEOUT:
        *((u_long *) conf) = dcb->dcb_wtimeout;
        rc = 0;
        break;
    }
    return rc;
}

/*!
 * \brief Initialize SPI UART device.
 *
 * Prepares the device for subsequent reading or writing.
 *
 * Applications should not use this function, but call
 * NutDeviceOpen().
 *
 * \param dev  Identifies the device to initialize.
 *
 * \return 0 on success, -1 otherwise.
 */
int UartSpiInit(NUTDEVICE * dev)
{
    static u_char initial = 1;
    IFSTREAM *ifs;
    UARTSDCB *dcb;

    /*
     * Check for valid device.
     */
    if (dev->dev_type != IFTYP_STREAM || dev->dev_base != 0
        || dev->dev_irq != 0)
        return -1;

    /*
     * Initialize interface control block.
     */
    if (dev->dev_icb == 0) {
        dev->dev_icb = NutHeapAllocClear(sizeof(IFSTREAM));
    }
    ifs = dev->dev_icb;
    ifs->if_input = UartSpiInput;
    ifs->if_output = UartSpiOutput;
    ifs->if_flush = UartSpiFlush;

    /*
     * Initialize driver control block.
     */
    dcb = dev->dev_dcb;
    memset(dcb, 0, sizeof(UARTSDCB));
    dcb->dcb_crystal = 3686400;
    dcb->dcb_baudSelect = 23;
    dcb->dcb_configured = 1;

    if (initial) {
        initial = 0;

        /*
         * PB0(I): SS from IO-ATN
         * PB1(-): SCK to IO-SCK
         * PB2(-): MOSI to IO-MOSI
         * PB3(-): MISO to IO-MISO
         * PB4(O): SIORESET
         * PB5(N): Not used
         * PB6(O): ATN to IO-SS
         * PB7(N): Not used
         */
        sbi(PORTB, 0);
        cbi(DDRB, 0);

        sbi(PORTB, 1);
        sbi(DDRB, 1);

        sbi(PORTB, 2);
        sbi(DDRB, 2);

        sbi(PORTB, 3);
        sbi(DDRB, 3);

        sbi(PORTB, 4);
        sbi(DDRB, 4);

        sbi(PORTB, 6);
        sbi(DDRB, 6);

        /*
         * PD0(I): IOIRQ
         */
        sbi(PORTD, 0);
        cbi(DDRD, 0);

        /*
         * Register interrupt handler.
         */
        if (NutRegisterIrqHandler(&sig_INTERRUPT0, SpiDevRequest, 0))
            return -1;

        /*
         * Configure SPI hardware.
         */
        outp(BV(SPE) | BV(CPOL) | BV(SPR1), SPCR);

        /*
         * Reset devices.
         */
        cbi(PORTB, 4);
#ifdef __IMAGECRAFT__
        asm("nop\nnop\nnop\nnop\n");
#else
        asm volatile ("nop\n\tnop\n\tnop\n\tnop\n\t");
#endif
        sbi(PORTB, 4);
    }
    return 0;
}


/*@}*/
