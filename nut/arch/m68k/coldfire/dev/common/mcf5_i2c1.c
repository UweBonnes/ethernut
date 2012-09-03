/*
 * Copyright 2012 by Embedded Technologies s.r.o
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

#include <arch/m68k.h>
#include <cfg/twi.h>
#include <dev/twif.h>

/*!
 * \brief Processor specific Hardware Initiliaization
 *
 */
int Mcf5I2cBus1Init(void)
{
    /* Enable the I2C signals */
#if I2CBUS1_PINSET_MCF5225X == PIN_SET4
    MCF_GPIO_PTHPAR |= MCF_GPIO_PTHPAR_MB_D4_SDA1 | MCF_GPIO_PTHPAR_MB_D5_SCL1;
#elif I2CBUS1_PINSET_MCF5225X == PIN_SET3
    MCF_GPIO_PUCPAR |= MCF_GPIO_PUCPAR_URTS2_SDA1 | MCF_GPIO_PUCPAR_UCTS2_SCL1;
#elif I2CBUS1_PINSET_MCF5225X == PIN_SET2
    MCF_GPIO_PUBPAR |= MCF_GPIO_PUBPAR_URXD1_SDA1 | MCF_GPIO_PUBPAR_UTXD1_SCL1;
#else // PIN_SET1
    MCF_GPIO_PQSPAR |= MCF_GPIO_PQSPAR_QSPI_DIN_SDA1 | MCF_GPIO_PQSPAR_QSPI_DOUT_SCL1;
#endif
    return 0;
}

/*!
 * \brief TWI/I2C bus structure.
 */
NUTTWIBUS Mcf5TwiBus1 = {
  /*.bus_base =    */  0,               /* Bus base address. */
  /*.bus_sig_ev =  */  &sig_I2C1,       /* Bus data and event interrupt handler. */
  /*.bus_sig_er =  */  NULL,            /* Bus error interrupt handler. */
  /*.bus_mutex =   */  NULL,            /* Bus lock queue. */
  /*.bus_icb   =   */  NULL,            /* Bus Runtime Data Pointer */
  /*.bus_dma_tx =  */  0,
  /*.bus_dma_rx =  */  0,
  /*.bus_initbus = */  Mcf5I2cBus1Init, /* Initialize bus controller. */
  /*.bus_recover = */  NULL,            /* Recover bus in case a slave hangs with SCL low */
};
