/*
 * Copyright (C) 2004 by Ole Reinhardt <ole.reinhardt@kernelconcepts.de>,
 *                       Kernelconcepts http://www.kernelconcepts.de
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

/*!
 * \file dev/sja1000.c
 * \brief Driver for SJA1000 CAN-Bus controller
 * 
 *
 * The SJA1000 controller is connected to the memory bus. It's base
 * address and interrupt is set by NutRegisterDevice.
 *
 * Have a look to our m-can board if you have questions.
 */


/*
 * $Log$
 * Revision 1.1  2004/06/07 15:11:49  olereinhardt
 * Initial checkin
 *
 */

/*!
 * \addtogroup xgCanSJA1000
 */
/*@{*/

/* Not ported. */
#ifdef __GNUC__

#include <string.h>

#include <sys/heap.h>
#include <sys/thread.h>
#include <sys/event.h>
#include <sys/atom.h>
#include <sys/timer.h>
#include <sys/semaphore.h>
#include <sys/nutconfig.h>

#include <dev/irqreg.h>
#include <dev/can_dev.h>
#include <dev/sja1000.h>


CANINFO dcb_sja1000;


volatile u_short sja_base = 0x0000;


struct _CANBuffer {
    CANFRAME *dataptr;          // the physical memory address where the buffer is stored
    u_short size;               // the allocated size of the buffer
    u_short datalength;         // the length of the data currently in the buffer
    u_short dataindex;          // the index into the buffer where the data starts
    SEM empty;
    SEM mutex;
    SEM full;
};

typedef struct _CANBuffer CANBuffer;

#ifndef CAN_BufSize
#define CAN_BufSize 64
#endif

CANFRAME *CAN_RX_BUF_Frame;
CANFRAME *CAN_TX_BUF_Frame;

CANBuffer CAN_RX_BUF;
CANBuffer CAN_TX_BUF;

void CANBufferInit(CANBuffer * buffer, CANFRAME * start, u_short size)
{
    start = NutHeapAlloc(size * sizeof(CANFRAME));

    NutSemInit(&buffer->mutex, 1);
    NutSemInit(&buffer->full, 0);
    NutSemInit(&buffer->empty, CAN_BufSize - 1);
    // set start pointer of the buffer
    buffer->dataptr = start;
    buffer->size = size;
    // initialize index and length
    buffer->dataindex = 0;
    buffer->datalength = 0;
}

// access routines

CANFRAME CANBufferGet(CANBuffer * buffer)
{
    CANFRAME data;

    NutSemWait(&buffer->full);
    NutSemWait(&buffer->mutex);
    // check to see if there's data in the buffer
    if (buffer->datalength) {
        // get the first frame from buffer
        data = buffer->dataptr[buffer->dataindex];
        // move index down and decrement length
        buffer->dataindex++;
        if (buffer->dataindex >= buffer->size) {
            buffer->dataindex %= buffer->size;
        }
        buffer->datalength--;
    }
    NutSemPost(&buffer->mutex);
    NutSemPost(&buffer->empty);
    // return
    return data;
}

void CANBufferPut(CANBuffer * buffer, CANFRAME * data)
{
    NutSemWait(&buffer->empty);
    NutSemWait(&buffer->mutex);

    // make sure the buffer has room
    if (buffer->datalength < buffer->size) {
        // save frame at end of buffer
        buffer->dataptr[(buffer->dataindex + buffer->datalength) % buffer->size] = *data;
        // increment the length
        buffer->datalength++;
        // return success
    }

    NutSemPost(&buffer->mutex);
    NutSemPost(&buffer->full);
}

u_short CANBufferFree(CANBuffer * buffer)
{
    // check to see if the buffer has room
    // return true if there is room
    return (buffer->size - buffer->datalength);
}

/*!
 * \fn    SJARxAvail(NUTDEVICE * dev)
 * \brief checks if data is available in input buffer
 *
 * \param dev Pointer to the device structure
 */

inline u_char SJARxAvail(NUTDEVICE * dev)
{
    return CAN_RX_BUF.datalength;
}

/*!
 * \fn    SJATxAvail(NUTDEVICE * dev)
 * \brief checks if there's still space in output buffer
 *
 * \param dev Pointer to the device structure
 */

inline u_char SJATxFree(NUTDEVICE * dev)
{
    return CANBufferFree(&CAN_TX_BUF);
}

/*!
 * \fn    SJAOutput(NUTDEVICE * dev, CANFRAME * frame)
 * \brief Write a frame from to output buffer
 *
 * This function writes a frame to the output buffer. If the output buffer
 * is full the function will block until frames are send.
 *
 * \param dev Pointer to the device structure
 * 
 * \param frame Pointer to the receive frame
 */

void SJAOutput(NUTDEVICE * dev, CANFRAME * frame)
{
    CANINFO *ci;

    ci = (CANINFO *) dev->dev_dcb;

    CANBufferPut(&CAN_TX_BUF, frame);
    NutEventPostAsync(&ci->can_tx_rdy);
}

/*!
 * \fn    SJAInput(NUTDEVICE * dev, CANFRAME * frame)
 * \brief Reads a frame from input buffer
 *
 * This function reads a frame from the input buffer. If the input buffer
 * is empty the function will block unitl new frames are received.
 *
 * \param dev Pointer to the device structure
 * 
 * \param frame Pointer to the receive frame
 */


void SJAInput(NUTDEVICE * dev, CANFRAME * frame)
{
    *frame = CANBufferGet(&CAN_RX_BUF);
}

/*!
 * \fn    SJASetAccCode(NUTDEVICE * dev, u_char * ac)
 * \brief Sets the acceptance code
 *
 *
 * \param dev Pointer to the device structure
 * 
 * \param ac 4 byte char array with the acceptance code
 */



void SJASetAccCode(NUTDEVICE * dev, u_char * ac)
{
    memcpy(((IFCAN *) (dev->dev_icb))->can_acc_code, ac, 4);

    while ((SJA1000_MODECTRL & RM_RR_Bit) == 0x00)      // enter reset state
        SJA1000_MODECTRL = (RM_RR_Bit | SJA1000_MODECTRL);

    SJA1000_AC0 = ac[0];
    SJA1000_AC1 = ac[1];
    SJA1000_AC2 = ac[2];
    SJA1000_AC3 = ac[3];
    SJA1000_MODECTRL = (AFM_Bit);
    //*** Note - if you change SJA1000_MODECTRL, change it in
    // functions CAN_Init and CAN_SetAccMask also.

    do {
        SJA1000_MODECTRL = 0x00;
    }
    while ((SJA1000_MODECTRL & RM_RR_Bit) != 0x00);     // leave reset state
}

/*!
 * \fn    SJASetAccMask(NUTDEVICE * dev, u_char * am)
 * \brief Sets the acceptance mask
 *
 *
 * \param dev Pointer to the device structure
 * 
 * \param am 4 byte char array with the acceptance mask
 */

void SJASetAccMask(NUTDEVICE * dev, u_char * am)
{
    memcpy(((IFCAN *) (dev->dev_icb))->can_acc_mask, am, 4);

    while ((SJA1000_MODECTRL & RM_RR_Bit) == 0x00)      // enter reset state
        SJA1000_MODECTRL = (RM_RR_Bit | SJA1000_MODECTRL);

    SJA1000_AM0 = am[0];
    SJA1000_AM1 = am[1];        // mask off lower nibble (SJA manual p44)
    SJA1000_AM2 = am[2];
    SJA1000_AM3 = am[3];
    SJA1000_MODECTRL = (AFM_Bit);
    //*** Note - if you change SJA1000_MODECTRL, change it in
    // functions CAN_Init and CAN_SetAccCode also.

    do {
        SJA1000_MODECTRL = 0x00;
    }
    while ((SJA1000_MODECTRL & RM_RR_Bit) != 0x00);     // leave reset state
}

/*!
 * \fn    SJASetBaudrate(NUTDEVICE * dev, u_long baudrate)
 * \brief Sets the baudrate 
 *
 *
 * \param dev Pointer to the device structure
 * 
 * \param baudrate Baudrate (One of the defined baudrates. See sja1000.h)
 */


u_char SJASetBaudrate(NUTDEVICE * dev, u_long baudrate)
{
    u_char result = 0;

    ((IFCAN *) (dev->dev_icb))->can_baudrate = baudrate;

    while ((SJA1000_MODECTRL & RM_RR_Bit) == 0x00)      // enter reset state
        SJA1000_MODECTRL = (RM_RR_Bit | SJA1000_MODECTRL);

    switch (baudrate)           // setting actual bustiming
    {                           // all @ 16 Mhz
    case CAN_SPEED_10K:
        SJA1000_BT0 = 113;
        SJA1000_BT1 = 28;
        break;
    case CAN_SPEED_20K:
        SJA1000_BT0 = 88;
        SJA1000_BT1 = 28;
        break;
    case CAN_SPEED_50K:
        SJA1000_BT0 = 73;
        SJA1000_BT1 = 28;
        break;
    case CAN_SPEED_100K:
        SJA1000_BT0 = 68;
        SJA1000_BT1 = 28;
        break;
    case CAN_SPEED_125K:
        SJA1000_BT0 = 67;
        SJA1000_BT1 = 28;
        break;
    case CAN_SPEED_250K:
        SJA1000_BT0 = 65;
        SJA1000_BT1 = 28;
        break;
    case CAN_SPEED_500K:
        SJA1000_BT0 = 64;
        SJA1000_BT1 = 28;
        break;
    case CAN_SPEED_800K:
        SJA1000_BT0 = 64;
        SJA1000_BT1 = 22;
        break;
    case CAN_SPEED_1M:
        SJA1000_BT0 = 64;
        SJA1000_BT1 = 20;
        break;
    default:
        result = 1;
    }

    do {
        SJA1000_MODECTRL = 0x00;
    }
    while ((SJA1000_MODECTRL & RM_RR_Bit) != 0x00);     // leave reset state

    return result;
}

/*!
 * \fn    SJATxFrame(CANFRAME * CAN_frame)
 * \brief Sends a CAN Frane
 *
 *
 * \param CAN_frame Pointer to the send frame
 */


void SJATxFrame(CANFRAME * CAN_frame)
{
    u_long temp_id;

    temp_id = CAN_frame->id << 3;

    if (CAN_frame->ext) {
        SJA1000_TxFrameInfo = CAN_frame->len | CAN_29 | (CAN_frame->rtr ? CAN_RTR : 0);

        SJA1000_Tx1 = (uint8_t) (temp_id >> 24);        // load High Byte
        SJA1000_Tx2 = (uint8_t) (temp_id >> 16);        // load High Byte
        SJA1000_Tx3 = (uint8_t) (temp_id >> 8); // load High Byte
        SJA1000_Tx4 = (uint8_t) (temp_id & 0x00F8);     // Low Byte and ignore bit 0-2

        SJA1000_Tx5 = CAN_frame->byte[0];
        SJA1000_Tx6 = CAN_frame->byte[1];
        SJA1000_Tx7 = CAN_frame->byte[2];
        SJA1000_Tx8 = CAN_frame->byte[3];
        SJA1000_Tx9 = CAN_frame->byte[4];
        SJA1000_Tx10 = CAN_frame->byte[5];
        SJA1000_Tx11 = CAN_frame->byte[6];
        SJA1000_Tx12 = CAN_frame->byte[7];

    } else {
        SJA1000_TxFrameInfo = CAN_frame->len | (CAN_frame->rtr ? CAN_RTR : 0);

        SJA1000_Tx1 = (uint8_t) ((CAN_frame->id) >> 24);        // load High Byte
        SJA1000_Tx2 = (uint8_t) ((CAN_frame->id) >> 16) & 0xE0; // Low Byte and ignore bit 0-4

        SJA1000_Tx3 = CAN_frame->byte[0];
        SJA1000_Tx4 = CAN_frame->byte[1];
        SJA1000_Tx5 = CAN_frame->byte[2];
        SJA1000_Tx6 = CAN_frame->byte[3];
        SJA1000_Tx7 = CAN_frame->byte[4];
        SJA1000_Tx8 = CAN_frame->byte[5];
        SJA1000_Tx9 = CAN_frame->byte[6];
        SJA1000_Tx10 = CAN_frame->byte[7];
    }
    SJA1000_CMD = (TR_Bit);     // Start Transmission
}

/*!
 * \fn    SJARxFrame(CANFRAME * CAN_frame)
 * \brief Receives a CAN Frane
 *
 *
 * \param CAN_frame Pointer to the receive frame
 */

void SJARxFrame(CANFRAME * CAN_frame)
{
    u_char FrameInfo = SJA1000_RxFrameInfo;
    CAN_frame->len = FrameInfo & 0x0F;  // frame info mask off higher 4 bits
    CAN_frame->ext = FrameInfo & CAN_29 ? 1 : 0;
    CAN_frame->rtr = FrameInfo & CAN_RTR ? 1 : 0;

    if (CAN_frame->ext) {
        CAN_frame->id = (((uint32_t) SJA1000_Rx1 << 24) |
                         ((uint32_t) SJA1000_Rx2 << 16) | 
                         ((uint32_t) SJA1000_Rx3 << 8)  | 
                         ((uint32_t) SJA1000_Rx4 & 0xF8)) >> 3;


        CAN_frame->byte[0] = SJA1000_Rx5;
        CAN_frame->byte[1] = SJA1000_Rx6;
        CAN_frame->byte[2] = SJA1000_Rx7;
        CAN_frame->byte[3] = SJA1000_Rx8;
        CAN_frame->byte[4] = SJA1000_Rx9;
        CAN_frame->byte[5] = SJA1000_Rx10;
        CAN_frame->byte[6] = SJA1000_Rx11;
        CAN_frame->byte[7] = SJA1000_Rx12;      // just fill the whole struct, less CPU cycles
    } else {
        CAN_frame->id = (((uint32_t) SJA1000_Rx1 << 24) | 
                          (uint32_t) SJA1000_Rx2 << 16) >> 3;   // id_h and id_l

        CAN_frame->byte[0] = SJA1000_Rx3;
        CAN_frame->byte[1] = SJA1000_Rx4;
        CAN_frame->byte[2] = SJA1000_Rx5;
        CAN_frame->byte[3] = SJA1000_Rx6;
        CAN_frame->byte[4] = SJA1000_Rx7;
        CAN_frame->byte[5] = SJA1000_Rx8;
        CAN_frame->byte[6] = SJA1000_Rx9;
        CAN_frame->byte[7] = SJA1000_Rx10;      // just fill the whole struct, less CPU cycles
    }
    SJA1000_CMD = RRB_Bit;      // release the receive buffer
}



/*! 
 * \fn CAN_Tx(void *arg)
 * \brief CAN transmitter thread.
 *
 * 
 * This thread transmits data if there's some in the output buffer.
 * It runs with high priority.
 */
THREAD(CAN_Tx, arg)
{
    NUTDEVICE *dev;
    CANINFO *ci;
    CANFRAME out_frame;

    dev = arg;
    ci = (CANINFO *) dev->dev_dcb;

    NutThreadSetPriority(9);

    while (1) {
        NutEventWait(&ci->can_tx_rdy, NUT_WAIT_INFINITE);
        while ((SJA1000_STATUS & TBS_Bit) == TBS_Bit)   // if transmit buffer released
        {
            out_frame = CANBufferGet(&CAN_TX_BUF);
            SJATxFrame(&out_frame);     // using SJA1000 TX buffer
            ci->can_tx_frames++;
        }
    }
}


/*! 
 * \fn CAN_Rx(void *arg)
 * \brief CAN receiver thread.
 *
 * This thread is woken up by the interrupt handler and reads data from
 * the SJA1000 input buffer. Then it wries this data into our own input buffer
 * and reenables interrupts. If input buffer is full, the write opration will 
 * block and interrupts will only be enabled on reading from this buffer again.
 * 
 * It runs with high priority.
 */
THREAD(CAN_Rx, arg)
{
    NUTDEVICE *dev;
    CANINFO *ci;
    CANFRAME in_frame;

    dev = arg;
    ci = (CANINFO *) dev->dev_dcb;

    NutThreadSetPriority(9);

    while (1) {
        NutEventWait(&ci->can_rx_rdy, NUT_WAIT_INFINITE);

        SJARxFrame(&in_frame);
        ci->can_rx_frames++;
        CANBufferPut(&CAN_RX_BUF, &in_frame);
        SJA1000_IEN |= (RIE_Bit);       // enables IRQ since buffer has space
    }
}

/*! 
 * \fn  SJAInterrupt (void * arg)
 * \brief SJA interrupt entry.
 * 
 * The interrupt handler posts events to the rx and tx thread wait queue.
 * receive interrupt will be disabled on reception and will be enabled by there
 * rx thread again. Otherwise interrupt would not stop (level triggered)
 */
static void SJAInterrupt(void *arg)
{
    CANINFO *ci;
    volatile u_char irq = SJA1000_INT;

    ci = (CANINFO *) (((NUTDEVICE *) arg)->dev_dcb);

    ci->can_interrupts++;

    if (((irq & TI_Bit) == TI_Bit))     // transmit IRQ fired
    {
        NutEventPostAsync(&ci->can_tx_rdy);
    }

    if ((irq & RI_Bit) == RI_Bit)       // Receive IRQ fired
    {
        SJA1000_IEN &= (~RIE_Bit);      // Disable RX IRQ until data has been poped from input buffer
        NutEventPostAsync(&ci->can_rx_rdy);
    }

    if ((irq & EI_Bit) == EI_Bit)       //Error IRQ fired
    {
        ci->can_errors++;
        // TODO: Handle error
    } else if ((irq & DOI_Bit) == DOI_Bit)      //Error IRQ fired
    {
        ci->can_overruns++;
        // TODO: Handle overrun
    }
}

/*!
 * \fn    SJAInit(NUTDEVICE * dev)
 * \brief Initialize SJA1000 Canbus interface.
 *
 *
 * Applications typically do not use this function, but
 * call NutRegisterDevice().
 *
 * \param dev Identifies the device to initialize. The
 *            structure must be properly set.
 */
int SJAInit(NUTDEVICE * dev)
{
    IFCAN *ifc;
    CANINFO *ci;
    volatile u_char temp;

    sja_base = dev->dev_base;

    ifc = dev->dev_icb;

    memset(dev->dev_dcb, 0, sizeof(CANINFO));
    ci = (CANINFO *) dev->dev_dcb;

    CANBufferInit(&CAN_RX_BUF, CAN_RX_BUF_Frame, CAN_BufSize);
    CANBufferInit(&CAN_TX_BUF, CAN_TX_BUF_Frame, CAN_BufSize);

    while ((SJA1000_MODECTRL & RM_RR_Bit) == 0x00)      // entering reset mode
        SJA1000_MODECTRL = (RM_RR_Bit | SJA1000_MODECTRL);

    SJA1000_CLK_DIV = (CANMode_Bit | CBP_Bit | DivBy2 | ClkOff_Bit);    // sets clock divide register

    SJA1000_IEN = (ClrIntEnSJA);        // Disables CAN IRQ

    SJA1000_AC0 = ifc->can_acc_code[0];
    SJA1000_AC1 = ifc->can_acc_code[1];
    SJA1000_AC2 = ifc->can_acc_code[2];
    SJA1000_AC3 = ifc->can_acc_code[3];

    SJA1000_AM0 = ifc->can_acc_mask[0];
    SJA1000_AM1 = ifc->can_acc_mask[1];
    SJA1000_AM2 = ifc->can_acc_mask[2];
    SJA1000_AM3 = ifc->can_acc_mask[3];

    switch (ifc->can_baudrate)  // setting actual bustiming
    {                           // all @ 16 Mhz
    case CAN_SPEED_10K:
        SJA1000_BT0 = 113;
        SJA1000_BT1 = 28;
        break;
    case CAN_SPEED_20K:
        SJA1000_BT0 = 88;
        SJA1000_BT1 = 28;
        break;
    case CAN_SPEED_50K:
        SJA1000_BT0 = 73;
        SJA1000_BT1 = 28;
        break;
    case CAN_SPEED_100K:
        SJA1000_BT0 = 68;
        SJA1000_BT1 = 28;
        break;
    case CAN_SPEED_125K:
        SJA1000_BT0 = 67;
        SJA1000_BT1 = 28;
        break;
    case CAN_SPEED_250K:
        SJA1000_BT0 = 65;
        SJA1000_BT1 = 28;
        break;
    case CAN_SPEED_500K:
        SJA1000_BT0 = 64;
        SJA1000_BT1 = 28;
        break;
    case CAN_SPEED_800K:
        SJA1000_BT0 = 64;
        SJA1000_BT1 = 22;
        break;
    case CAN_SPEED_1M:
        SJA1000_BT0 = 64;
        SJA1000_BT1 = 20;
        break;
    default:
        return errCAN_INVALID_BAUD;
    }

    SJA1000_OUTCTRL = (Tx1Float | Tx0PshPull | NormalMode);     // Set up Output Control Register

    SJA1000_IEN = (RIE_Bit | TIE_Bit /*| EIE_Bit | DOIE_Bit */ );       // Enables receive IRQ

    SJA1000_MODECTRL = (AFM_Bit);       // set single filter mode + sleep
    // *** Note - if you change SJA1000_MODECTRL, change it in
    // functions SJASetAccMask and SJASetAccCode also.

    do {
        SJA1000_MODECTRL = 0x00;
    }
    while ((SJA1000_MODECTRL & RM_RR_Bit) != 0x00);     // leaves reset mode

    NutEnterCritical();

    if (NutRegisterIrqHandler(&SJA_SIGNAL, SJAInterrupt, dev)) {
        NutExitCritical();
        return -1;
    }

    cbi(EIMSK, SJA_SIGNAL_BIT);
    if (SJA_SIGNAL_BIT < 4)     // Set corresponding interrupt to low
    {                           // level interrupt
        cbi(EICRA, (SJA_SIGNAL_BIT << 1));
        cbi(EICRA, (SJA_SIGNAL_BIT << 1) + 1);
    } else {
        cbi(EICRB, ((SJA_SIGNAL_BIT - 4) << 1));
        cbi(EICRB, ((SJA_SIGNAL_BIT - 4) << 1) + 1);
    }
    temp = SJA1000_INT;         // Read interrupt register to clear pendin bits    
    sbi(EIMSK, SJA_SIGNAL_BIT);
    sbi(PORTE, 7);
    NutThreadCreate("sjacanrx", CAN_Rx, dev, 256);
    NutThreadCreate("sjacantx", CAN_Tx, dev, 256);

    NutExitCritical();

    return 0;
}

#else
void keep_icc_happy(void)
{
}

#endif

/*!
 * \brief Interface information structure.
 *
 * This struct stores some interface parameters like bautdate and 
 * acceptance mask / code. Beside this Callback handlers are registered.
 */


IFCAN ifc_sja1000 = {
    CAN_IF_2B,                  /*!< \brief Interface type. */
    CAN_SPEED_500K,             /*!< \brief Baudrate of device. */
    {0xFF, 0xFF, 0xFF, 0xFF}
    ,                           /*!< \brief Acceptance mask */
    {0x00, 0x00, 0x00, 0x00}
    ,                           /*!< \brief Acceptance code */
    SJARxAvail,                 /*!< \brief Data in RxBuffer available? */
    SJATxFree,                  /*!< \brief TxBuffer free? */
    SJAInput,                   /*!< \brief CAN Input routine */
    SJAOutput,                  /*!< \brief CAN Output routine */
    SJASetAccCode,              /*!< \brief Set acceptance code */
    SJASetAccMask,              /*!< \brief Set acceptance mask */
    SJASetBaudrate              /*!< \brief Set baudrate */
};

/*!
 * \brief Device information structure.
 *
 * Applications must pass this structure to NutRegisterDevice() 
 * to bind this CAN device driver to the Nut/OS kernel.
 */

NUTDEVICE devSJA1000 = {
    0,                          /*!< Pointer to next device. */
    {'s', 'j', 'a', '1', '0', '0', '0', 0, 0}
    ,                           /*!< Unique device name. */
    IFTYP_CAN,                  /*!< Type of device. */
    0,                          /*!< Base address. */
    0,                          /*!< First interrupt number. */
    &ifc_sja1000,               /*!< Interface control block. */
    &dcb_sja1000,               /*!< Driver control block. */
    SJAInit,                    /*!< Driver initialization routine. */
    0,                          /*!< Driver specific control function. */
    0,                          /*!< Read from device. */
    0,                          /*!< Write to device. */
    0,                          /*!< Write from program space data to device. */
    0,                          /*!< Open a device or file. */
    0,                          /*!< Close a device or file. */
    0                           /*!< Request file size. */
};
/*@}*/
