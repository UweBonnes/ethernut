/*
 * Copyright (C) 2001-2004 by egnite Software GmbH. All rights reserved.
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
 * Revision 1.2  2005/04/05 17:49:05  haraldkipp
 * Make it work on Wolf, but breaks AT91EB40A.
 *
 * Revision 1.1  2004/09/08 10:53:13  haraldkipp
 * Our first device for the EB40A
 *
 */

#include <dev/debug.h>

/*!
 * \addtogroup xgDebugDev0
 */
/*@{*/

#include <sys/device.h>
#include <sys/file.h>

#define _BV(bit)    (1 << bit)

#define LED1            _BV(16)
#define LED2            _BV(17)
#define LED3            _BV(18)
#define LED4            _BV(19)
#define LED5            _BV(3)
#define LED6            _BV(4)
#define LED7            _BV(5)
#define LED8            _BV(6)

#define PIO_PER  ((volatile unsigned int *)0xFFFF0000)
#define PIO_PDR  ((volatile unsigned int *)0xFFFF0004)
#define PIO_OER  ((volatile unsigned int *)0xFFFF0010)
#define PIO_IFDR ((volatile unsigned int *)0xFFFF0024)
#define PIO_SODR ((volatile unsigned int *)0xFFFF0030)
#define PIO_CODR ((volatile unsigned int *)0xFFFF0034)
#define PIO_IDR  ((volatile unsigned int *)0xFFFF0044)

#define usart0_reg  ((volatile unsigned int *)0xFFFD0000)

#define usart1_reg  ((volatile unsigned int *)0xFFFCC000)


#define AT91_US_CR  0  // Control register
#define AT91_US_CR_RxRESET (1<<2)
#define AT91_US_CR_TxRESET (1<<3)
#define AT91_US_CR_RxENAB  (1<<4)
#define AT91_US_CR_RxDISAB (1<<5)
#define AT91_US_CR_TxENAB  (1<<6)
#define AT91_US_CR_TxDISAB (1<<7)
#define AT91_US_CR_RSTATUS (1<<8)
#define AT91_US_CR_STTTO   (1<<11)

#define AT91_US_MR  1  // Mode register
#define AT91_US_MR_CLOCK   4
#define AT91_US_MR_CLOCK_MCK  (0<<AT91_US_MR_CLOCK)
#define AT91_US_MR_CLOCK_MCK8 (1<<AT91_US_MR_CLOCK)
#define AT91_US_MR_CLOCK_SCK  (2<<AT91_US_MR_CLOCK)
#define AT91_US_MR_LENGTH  6
#define AT91_US_MR_LENGTH_5   (0<<AT91_US_MR_LENGTH)
#define AT91_US_MR_LENGTH_6   (1<<AT91_US_MR_LENGTH)
#define AT91_US_MR_LENGTH_7   (2<<AT91_US_MR_LENGTH)
#define AT91_US_MR_LENGTH_8   (3<<AT91_US_MR_LENGTH)
#define AT91_US_MR_SYNC    8
#define AT91_US_MR_SYNC_ASYNC (0<<AT91_US_MR_SYNC)
#define AT91_US_MR_SYNC_SYNC  (1<<AT91_US_MR_SYNC)
#define AT91_US_MR_PARITY  9
#define AT91_US_MR_PARITY_EVEN  (0<<AT91_US_MR_PARITY)
#define AT91_US_MR_PARITY_ODD   (1<<AT91_US_MR_PARITY)
#define AT91_US_MR_PARITY_SPACE (2<<AT91_US_MR_PARITY)
#define AT91_US_MR_PARITY_MARK  (3<<AT91_US_MR_PARITY)
#define AT91_US_MR_PARITY_NONE  (4<<AT91_US_MR_PARITY)
#define AT91_US_MR_PARITY_MULTI (6<<AT91_US_MR_PARITY)
#define AT91_US_MR_STOP   12
#define AT91_US_MR_STOP_1       (0<<AT91_US_MR_STOP)
#define AT91_US_MR_STOP_1_5     (1<<AT91_US_MR_STOP)
#define AT91_US_MR_STOP_2       (2<<AT91_US_MR_STOP)
#define AT91_US_MR_MODE   14
#define AT91_US_MR_MODE_NORMAL  (0<<AT91_US_MR_MODE)
#define AT91_US_MR_MODE_ECHO    (1<<AT91_US_MR_MODE)
#define AT91_US_MR_MODE_LOCAL   (2<<AT91_US_MR_MODE)
#define AT91_US_MR_MODE_REMOTE  (3<<AT91_US_MR_MODE)
#define AT91_US_MR_MODE9  17
#define AT91_US_MR_CLKO   18

#define AT91_US_IER 2  // Interrupt enable register
#define AT91_US_IER_RxRDY   (1<<0)  // Receive data ready
#define AT91_US_IER_TxRDY   (1<<1)  // Transmitter ready
#define AT91_US_IER_RxBRK   (1<<2)  // Break received
#define AT91_US_IER_ENDRX   (1<<3)  // Rx end
#define AT91_US_IER_ENDTX   (1<<4)  // Tx end
#define AT91_US_IER_OVRE    (1<<5)  // Rx overflow
#define AT91_US_IER_FRAME   (1<<6)  // Rx framing error
#define AT91_US_IER_PARITY  (1<<7)  // Rx parity
#define AT91_US_IER_TIMEOUT (1<<8)  // Rx timeout
#define AT91_US_IER_TxEMPTY (1<<9)  // Tx empty

#define AT91_US_IDR 3  // Interrupt disable register

#define AT91_US_IMR 4  // Interrupt mask register

#define AT91_US_CSR 5  // Channel status register
#define AT91_US_CSR_RxRDY 0x01 // Receive data ready
#define AT91_US_CSR_TxRDY 0x02 // Transmit ready
#define AT91_US_CSR_OVRE  0x20 // Overrun error
#define AT91_US_CSR_FRAME 0x40 // Framing error

#define AT91_US_RHR 6  // Receive holding register

#define AT91_US_THR 7  // Transmit holding register

#define AT91_US_BRG 8  // Baud rate generator

#define AT91_US_RTO 9  // Receive time out

#define AT91_US_TTG 10  // Transmit timer guard

#define AT91_US_RPR 12  // Receive pointer register
#define AT91_US_RCR 13  // Receive counter register
#define AT91_US_TPR 14  // Transmit pointer register
#define AT91_US_TCR 15  // Transmit counter register


#define US0_THR ((volatile unsigned int *)0xFFFD001C)


#define AT91_US_BAUD(baud) ((66000000/(8*(baud))+1)/2)

#define PIOTXD0         14         /* USART 0 transmit data signal */
#define PIORXD0         15         /* USART 0 receive data signal */

#define PIOTXD1         21         /* USART 1 transmit data signal */
#define PIORXD1         22         /* USART 1 receive data signal */

#define ps_reg  ((volatile unsigned int *)0xFFFF4000)

#define AT91_PS_CR      0
#define AT91_PS_PCER    1
#define AT91_PS_PDCR    2

#define US0_ID          2       /* USART Channel 0 interrupt */
#define US1_ID          3       /* USART Channel 1 interrupt */

static NUTFILE dbgfile;

/*!
 * \brief Handle I/O controls for debug device 0.
 *
 * The debug device doesn't support any.
 *
 * \return Always -1.
 */
int DebugIOCtl(NUTDEVICE * dev, int req, void *conf)
{
    return -1;
}

/*!
 * \brief Initialize debug device 0.
 *
 * Simply enable the device. Baudrate divisor set to 7 for
 * 115.2 kBaud at 14.7456 MHz.
 *
 * \return Always 0.
 */
int DebugInit(NUTDEVICE * dev)
{
#if 0
    *(ps_reg + AT91_PS_PCER) = (1 << US0_ID) | (1 << US1_ID);

    *PIO_PDR = (1 << PIOTXD1) | (1 << PIORXD1);

    // Reset device
    *(usart1_reg + AT91_US_CR) = AT91_US_CR_RxRESET | AT91_US_CR_TxRESET |
                                 AT91_US_CR_RxDISAB | AT91_US_CR_TxDISAB;



    *(usart1_reg + AT91_US_IDR) = 0xFFFFFFFF;

    *(usart1_reg + AT91_US_RCR) = 0;
    *(usart1_reg + AT91_US_TCR) = 0;

    *(usart1_reg + AT91_US_BRG) = AT91_US_BAUD(115200);

    // 8-1-no parity.
    *(usart1_reg + AT91_US_MR) = AT91_US_MR_MODE_NORMAL |
                                 AT91_US_MR_CLOCK_MCK | AT91_US_MR_LENGTH_8 |
                                 AT91_US_MR_PARITY_NONE | AT91_US_MR_STOP_1;

    // Enable RX and TX
    *(usart1_reg + AT91_US_CR) = AT91_US_CR_RxENAB | AT91_US_CR_TxENAB;
#endif
    return 0;
}

/*!
 * \brief Send a single character to debug device 0.
 *
 * A carriage return character will be automatically appended 
 * to any linefeed.
 */
void DebugPut(char ch)
{
    while((usart0_reg[AT91_US_CSR] & AT91_US_CSR_TxRDY) == 0);
    usart0_reg[AT91_US_THR] = ch;

    if(ch == '\n') 
        DebugPut('\r');
}

/*!
 * \brief Send characters to debug device 0.
 *
 * A carriage return character will be automatically appended 
 * to any linefeed.
 *
 * \return Number of characters sent.
 */
int DebugWrite(NUTFILE * fp, CONST void *buffer, int len)
{
    int c = len;
    CONST char *cp = buffer;

    while(c--)
        DebugPut(*cp++);
    return len;
}

/*!
 * \brief Open debug device 0.
 *
 * \return Pointer to a static NUTFILE structure.
 */
NUTFILE *DebugOpen(NUTDEVICE * dev, CONST char *name, int mode, int acc)
{
    dbgfile.nf_next = 0;
    dbgfile.nf_dev = dev;
    dbgfile.nf_fcb = 0;

    return &dbgfile;
}

/*! 
 * \brief Close debug device 0.
 *
 * \return Always 0.
 */
int DebugClose(NUTFILE * fp)
{
    return 0;
}

/*!
 * \brief Debug device 0 information structure.
 */
NUTDEVICE devDebug0 = {
    0,                          /*!< Pointer to next device, dev_next. */
    {'u', 'a', 'r', 't', '0', 0, 0, 0, 0},      /*!< Unique device name, dev_name. */
    0,                          /*!< Type of device, dev_type. */
    0xFFFD0000,                 /*!< Base address, dev_base. */
    0,                          /*!< First interrupt number, dev_irq. */
    0,                          /*!< Interface control block, dev_icb. */
    0,                          /*!< Driver control block, dev_dcb. */
    DebugInit,                  /*!< Driver initialization routine, dev_init. */
    DebugIOCtl,                 /*!< Driver specific control function, dev_ioctl. */
    0,                          /*!< dev_read. */
    DebugWrite,                 /*!< dev_write. */
    DebugOpen,                  /*!< dev_opem. */
    DebugClose,                 /*!< dev_close. */
    0                           /*!< dev_size. */
};

/*!
 * \brief Debug device 1 information structure.
 */
NUTDEVICE devDebug1 = {
    0,                          /*!< Pointer to next device, dev_next. */
    {'u', 'a', 'r', 't', '1', 0, 0, 0, 0},      /*!< Unique device name, dev_name. */
    0,                          /*!< Type of device, dev_type. */
    0xFFFCC000,                 /*!< Base address, dev_base. */
    0,                          /*!< First interrupt number, dev_irq. */
    0,                          /*!< Interface control block, dev_icb. */
    0,                          /*!< Driver control block, dev_dcb. */
    DebugInit,                  /*!< Driver initialization routine, dev_init. */
    DebugIOCtl,                 /*!< Driver specific control function, dev_ioctl. */
    0,                          /*!< dev_read. */
    DebugWrite,                 /*!< dev_write. */
    DebugOpen,                  /*!< dev_opem. */
    DebugClose,                 /*!< dev_close. */
    0                           /*!< dev_size. */
};

/*@}*/
