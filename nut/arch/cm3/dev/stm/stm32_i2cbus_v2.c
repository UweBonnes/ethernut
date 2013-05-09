/*
 * Copyright (C) 2013 Uwe Bonnes(bon@elektron.ikp.physik.tu-darmstadt.de
 *
 * All rights reserved.
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

/*!
 * \file arch/cm3/dev/stm/stm32_i2cbus_v2.c
 * \brief I2C bus driver for I2C hardware in STM32F0 and STM32F3.
 *
 * This driver is in an early stage and has been tested on STM32F3 only.
 * It doesn't consider Slave operation yet
 *
 * It is intended that this driver replaces the current STM TWI driver,
 * which doesn't allow to have different types of busses in a single
 * application, for example TWI hardware and bit banging interfaces.
 * This new I2C driver layout allows to attach any I2C slave driver to
 * any I2C bus driver by calling NutRegisterI2cSlave().
 *
 * \verbatim
 * $Id$
 * \endverbatim
 */

/*!
 * \brief I2C bus driver for STM32F0/3 hardware.
 *
 * This is an interrupt driver, which supports master mode only.
 * No support for FM+
 * No error handling
 */

#include <dev/irqreg.h>
#include <sys/nutdebug.h>
#include <sys/timer.h>
#include <sys/event.h>

#include <stdlib.h>

#include <cfg/arch.h>
#include <arch/cm3.h>
#include <dev/gpio.h>
#include <arch/cm3/stm/stm32xxxx_gpio.h>
#include <arch/cm3/stm/stm32_irqreg.h>
#include <dev/i2cbus.h>
#include <cfg/twi.h>
/*!
 * \addtogroup xgI2cBusSTM32
 */
/*@{*/

/*!
 * \brief Local data of the STM32 I2C bus driver.
 */
typedef struct _STM32_I2CCB {
    /*! \brief Register base. */
    uptr_t icb_base;
    /*! \brief SDA_PIN. */
    uint32_t sda_pin;
    /*! \brief SCL_PIN. */
    uint32_t scl_pin;
    /*! \brief SMBA_PIN. */
    uint32_t smba_pin;
    /*! \brief System event handler. */
    IRQ_HANDLER *icb_sig_ev;
    /*! \brief System error handler. */
    IRQ_HANDLER *icb_sig_er;
    /*! \brief I2C message. */
    NUTI2C_MSG *icb_msg;
    /*! \brief Thread waiting for completion. */
    HANDLE icb_queue;
} STM32_I2CCB;

/*
 * STM32V2 I2C Event interrupt function.
 */
static void I2cEventBusIrqHandler(void *arg)
{
    STM32_I2CCB *icb = (STM32_I2CCB *) arg;
    NUTI2C_MSG *msg = icb->icb_msg;
    I2C_TypeDef *i2c;
    uint32_t cr2;

    i2c = (I2C_TypeDef *) icb->icb_base;
    cr2 = i2c->CR2;

    /* TX parts*/
    if ((i2c->ISR & I2C_ISR_TCR) && !(cr2 & I2C_CR2_RD_WRN))
    {
        uint32_t txbytes_left = msg->msg_wlen - msg->msg_widx;
        if (txbytes_left <= 0xff)
        {
            cr2 &= ~(I2C_CR2_NBYTES | I2C_CR2_RELOAD);
            cr2 |= txbytes_left << 16;
        }
        /* else I2C_CR2_NBYTES and I2C_CR2_RELOAD already set*/
        i2c->CR2 = cr2; /* Write to NBYTES clears TCR*/
    }
    if ((i2c->ISR & I2C_ISR_TC) && !(cr2 & I2C_CR2_RD_WRN))
    {
        i2c->CR1 &= ~I2C_CR1_TXIE;
        cr2 &= ~I2C_CR2_NBYTES;
        if (msg->msg_rsiz == 0) /* No read transaction, stop after write*/
        {
            i2c->CR2 = cr2 | I2C_CR2_STOP  ;
        }
        else
        {
            i2c->CR1 |= I2C_CR1_RXIE;
            cr2 |= I2C_CR2_RD_WRN | msg->msg_rsiz << 16;
            i2c->CR2 = cr2 | I2C_CR2_START  ;
        }
    }
    if ((i2c->ISR & I2C_ISR_TXIS) && !(cr2 & I2C_CR2_RD_WRN))
    {
        i2c->TXDR = msg->msg_wdat[msg->msg_widx];
        msg->msg_widx++;
    }
    /* RX parts*/
    if ((i2c->ISR & I2C_ISR_TCR) && (cr2 & I2C_CR2_RD_WRN))
    {
        uint32_t rxbytes_left = msg->msg_rsiz - msg->msg_ridx;
        if (rxbytes_left <= 0xff)
        {
            cr2 &= ~(I2C_CR2_NBYTES | I2C_CR2_RELOAD);
            cr2 |= rxbytes_left << 16;
        }
        /* else I2C_CR2_NBYTES and I2C_CR2_RELOAD already set*/
        i2c->CR2 = cr2; /* Write to NBYTES clears TCR*/
    }
    if ((i2c->ISR & I2C_ISR_TC) && (cr2 & I2C_CR2_RD_WRN))
    {
        i2c->CR1 &= ~(I2C_CR1_TCIE | I2C_CR1_RXIE);
        i2c->CR2 = cr2 | I2C_CR2_STOP  ;
    }
    if ((i2c->ISR & I2C_ISR_RXNE) && (cr2 & I2C_CR2_RD_WRN))
    {
        msg->msg_rdat[msg->msg_ridx] = i2c->RXDR;
        msg->msg_ridx++;
        if (msg->msg_ridx == msg->msg_rsiz)
            /* No more RX Interrupts*/
            i2c->CR1 &= ~I2C_CR1_RXIE;
    }
    if (i2c->ISR & I2C_ISR_STOPF)
    {
        i2c->CR1 &= ~I2C_CR1_STOPIE;
        i2c->ICR |=  I2C_ICR_STOPCF;
        NutEventPostFromIrq(&icb->icb_queue);
    }
}

/*
 * STM32V2 I2C Error interrupt function.
 */
static void I2cErrorBusIrqHandler(void *arg)
{
    STM32_I2CCB *icb = (STM32_I2CCB *) arg;
    I2C_TypeDef *i2c;
    uint32_t isr;

    i2c = (I2C_TypeDef *) icb->icb_base;
    isr = i2c->ISR;
    if (isr & I2C_ISR_ALERT)
        i2c->ICR |= I2C_ICR_ALERTCF;
    if (isr & I2C_ISR_PECERR)
        i2c->ICR |= I2C_ICR_PECCF;
    if (isr & I2C_ISR_OVR)
        i2c->ICR |= I2C_ICR_OVRCF;
    if (isr & I2C_ISR_ARLO)
        i2c->ICR |= I2C_ICR_ARLOCF;
    if (isr & I2C_ISR_BERR)
        i2c->ICR |= I2C_ICR_BERRCF;
    if (isr & I2C_ISR_TIMEOUT)
        i2c->ICR |= I2C_ICR_TIMOUTCF;
}

/*!
 * \brief I2C bus transfer (STM I2C implementation).
 *
 * This function is called by the platform independent code via the
 * NUTI2C_BUS::bus_tran function pointer.
 */
static int I2cBusTran(NUTI2C_SLAVE *slave, NUTI2C_MSG *msg)
{
    NUTI2C_BUS *bus;
    STM32_I2CCB *icb;
    I2C_TypeDef *i2c;
    uint32_t cr2;
    int rc = 0;

    bus = slave->slave_bus;
    NUTASSERT(bus != NULL);
    NUTASSERT(bus->bus_icb != NULL);
    icb = (STM32_I2CCB *) bus->bus_icb;
    icb->icb_msg = msg;
    i2c = (I2C_TypeDef *) icb->icb_base;
    cr2 = i2c->CR2;
    cr2 &= 0xf80000; /* Clean out */
    cr2 |= slave->slave_address << 1;
    msg->msg_widx = 0;
    msg->msg_ridx = 0;
    /* are there bytes to write? */
    if (msg->msg_wlen)
    {
        i2c->CR1 |= I2C_CR1_TXIE | I2C_CR1_TCIE | I2C_CR1_STOPIE ;
        if (msg->msg_wlen > 0xff)
            cr2 |= I2C_CR2_NBYTES | I2C_CR2_RELOAD;
        else
            cr2 |= msg->msg_wlen << 16;
    }
    else if (msg->msg_rsiz)
    {
        i2c->CR1 |= I2C_CR1_RXIE|I2C_CR1_STOPIE ;
        if (msg->msg_rsiz > 0xff)
            cr2 |= I2C_CR2_RD_WRN | I2C_CR2_NBYTES | I2C_CR2_RELOAD;
        else
            cr2 |= I2C_CR2_RD_WRN | msg->msg_rsiz << 16;
    }
    i2c->CR2 = cr2 | I2C_CR2_START;
    rc = NutEventWait(&icb->icb_queue, slave->slave_timeout);
    if(rc)
        return -1;
    return msg->msg_ridx;
}

static int checkpin_and_config(STM32_I2CCB *icb)
{
    uint32_t sda_port, scl_port;
    if (icb->icb_base == I2C1_BASE)
    {
        if ((icb->sda_pin != 7) && (icb->sda_pin != 9) && (icb->sda_pin != 14))
            return -1;
        if ((icb->scl_pin != 6) && (icb->scl_pin != 8) && (icb->sda_pin != 15))
            return -1;
        if ((icb->smba_pin != 5) && (icb->smba_pin != -1))
            return -1;
        if (icb->sda_pin == 15)
            sda_port= NUTGPIO_PORTA;
        else
            sda_port= NUTGPIO_PORTB;
        if (icb->scl_pin == 15)
            scl_port= NUTGPIO_PORTA;
        else
            scl_port= NUTGPIO_PORTB;

        GpioPinConfigSet(
            sda_port, icb->sda_pin, GPIO_CFG_OUTPUT| GPIO_CFG_PERIPHAL|
            GPIO_CFG_MULTIDRIVE| GPIO_CFG_PULLUP | GPIO_CFG_SPEED_FAST);

        GpioPinConfigSet(
            scl_port ,icb->scl_pin, GPIO_CFG_OUTPUT| GPIO_CFG_PERIPHAL|
            GPIO_CFG_MULTIDRIVE| GPIO_CFG_PULLUP | GPIO_CFG_SPEED_FAST);
        GPIO_PinAFConfig((GPIO_TypeDef*) sda_port, icb->sda_pin, GPIO_AF_4);
        GPIO_PinAFConfig((GPIO_TypeDef*) scl_port, icb->scl_pin, GPIO_AF_4);
        if (icb->smba_pin == 5)
        {
            /* TODO: How should SMBA pin be set?*/
            GpioPinConfigSet(
                NUTGPIO_PORTA, icb->scl_pin, GPIO_CFG_OUTPUT|
                GPIO_CFG_PERIPHAL| GPIO_CFG_MULTIDRIVE| GPIO_CFG_PULLUP |
                GPIO_CFG_SPEED_FAST);
            GPIO_PinAFConfig(GPIOA, icb->smba_pin, GPIO_AF_4);
        }
        RCC->APB1ENR  &= ~RCC_APB1ENR_I2C1EN;
        /* Use HSI clock*/
        RCC->CFGR3    &= ~RCC_CFGR3_I2C1SW;
        RCC->APB1ENR  |=  RCC_APB1ENR_I2C1EN;
        RCC->APB1RSTR |=  RCC_APB1RSTR_I2C1RST;
        RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C1RST;
    }
    else if (icb->icb_base == I2C2_BASE)
    {
        if ((icb->sda_pin != 0) && (icb->sda_pin != 10)&& (icb->sda_pin != 0))
            return -1;
        if ((icb->scl_pin != 1) && (icb->scl_pin != 9) && (icb->sda_pin != 6)
            && (icb->sda_pin != 1))
            return -1;
        if ((icb->smba_pin != 12) && (icb->smba_pin != 8)
            && (icb->smba_pin != -1))
            return -1;
        if (icb->sda_pin == 10)
            sda_port= NUTGPIO_PORTA;
        else
            sda_port= NUTGPIO_PORTF;
        if (icb->scl_pin == 9)
            scl_port= NUTGPIO_PORTA;
        else /* Both other alternate pins on PORTF*/
            scl_port= NUTGPIO_PORTF;
        /* Fixme: Handle SMNa Pin*/
        GpioPinConfigSet(
            sda_port, icb->sda_pin, GPIO_CFG_OUTPUT| GPIO_CFG_PERIPHAL|
            GPIO_CFG_MULTIDRIVE| GPIO_CFG_PULLUP | GPIO_CFG_SPEED_FAST);

        GpioPinConfigSet(
            scl_port ,icb->scl_pin, GPIO_CFG_OUTPUT| GPIO_CFG_PERIPHAL|
            GPIO_CFG_MULTIDRIVE| GPIO_CFG_PULLUP | GPIO_CFG_SPEED_FAST);
        GPIO_PinAFConfig((GPIO_TypeDef*) sda_port, icb->sda_pin, GPIO_AF_4);
        GPIO_PinAFConfig((GPIO_TypeDef*) scl_port, icb->scl_pin, GPIO_AF_4);
        RCC->APB1ENR  &= ~RCC_APB1ENR_I2C2EN;
        /* Use HSI clock*/
        RCC->CFGR3    &= ~RCC_CFGR3_I2C2SW;
        RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
        RCC->APB1RSTR |=  RCC_APB1RSTR_I2C2RST;
        RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C2RST;
    }
    else
        return -1;
    return 0;
}

/*!
 * \brief Configure the I2C bus controller (STM32V2 I2C implementation).
 *
 * This function is called by the platform independent code via the
 * NUTI2C_BUS::bus_conf function pointer. Most implementations will
 * also call this function during initialization to set the
 * default configuration.
 *
 * Right now only the bus clock rate is configurable,
 * and only at 10/100/400 kHz
 */
static int I2cBusConf(NUTI2C_BUS *bus)
{
    STM32_I2CCB *icb;
    long rate;
    uint32_t timing;
    I2C_TypeDef *i2c;

    /* Check parameters. */
    NUTASSERT(bus != NULL);
    NUTASSERT(bus->bus_icb != NULL);
    icb = (STM32_I2CCB *) bus->bus_icb;
    i2c = (I2C_TypeDef*) icb->icb_base;

    /* Get requested rate or use the default. */
    rate = bus->bus_rate;
    if (rate == 0) {
        rate = 100000L;
    }
    if (rate > 400000) {
        /* Speed out of range */
        return -1;
    }
    if(rate == 400000)
        timing = 0x0 <<28 |0x3 << 20 | 0x1 << 16| 0x03 << 8 | 0x09 << 0;
    else if (rate >= 100000)
        timing = 0x1 <<28 |0x4 << 20 | 0x2 << 16| 0x0f << 8 | 0x13 << 0;
    else if (rate >= 50000)
        timing = 0x1 <<28 |0x4 << 20 | 0x2 << 16| 0x23 << 8 | 0x27 << 0;
    else
        timing = 0x1 <<28 |0x4 << 20 | 0x2 << 16| 0xc3 << 8 | 0xc7 << 0;
    if ((i2c->TIMINGR & 0xf0ffffff) != timing)
    {
        /* Disable I2C and set new timing */
        while(i2c->CR1 & I2C_CR1_PE)
            i2c->CR1 &= ~I2C_CR1_PE;
        i2c->TIMINGR = timing;
    }

    return 0;
}

/*!
 * \brief Initialize the I2C bus controller (STM32 implementation).
 *
 * This function is called by the platform independent code via the
 * NUTI2C_BUS::bus_init function pointer when the first slave is
 * attached to this bus. Note, that NUTI2C_BUS::bus_rate must be zero
 * initially. Otherwise no call to this function will take place.
 *
 * This function must do all required initializations so that the
 * driver will be ready to process transfers via NUTI2C_BUS::bus_tran.
 *
 * This function must return 0 on success or -1 otherwise.
 */
static int I2cBusInit(NUTI2C_BUS *bus)
{
    I2C_TypeDef *i2c;
    STM32_I2CCB *icb;

    icb = (STM32_I2CCB *) bus->bus_icb;

    if (checkpin_and_config(icb))
        return -1;
    /* Try to configure the bus*/
    if (I2cBusConf(bus)) {
        return -1;
    }
    i2c = (I2C_TypeDef*) icb->icb_base;
    i2c->CR1 |= I2C_CR1_PE;
    if (NutRegisterIrqHandler(icb->icb_sig_ev, I2cEventBusIrqHandler, icb))
        return -1;
    if (NutRegisterIrqHandler(icb->icb_sig_er, I2cErrorBusIrqHandler, icb))
        return -1;
    NutIrqEnable(icb->icb_sig_ev);
    NutIrqDisable(icb->icb_sig_er);

    return 0;
}

/*!
 * \brief Probe the I2C bus for a specified slave address
 *        (STM32V2 implementation).
 *
 * This function is called by the platform independent code via the
 * NUTI2C_BUS::bus_probe function pointer. This may happen even if no
 * slave device had been attached to the bus and thus without any
 * previous call to NUTI2C_BUS::bus_init. However, in that case
 * NUTI2C_BUS::bus_configure will have been called.
 *
 *
 */
static int I2cBusProbe(NUTI2C_BUS *bus, int sla)
{
    STM32_I2CCB *icb;
    I2C_TypeDef *i2c;
    uint32_t isr;
    int i;
    int limit;

    if ((bus->bus_flags & I2C_BF_INITIALIZED) == 0) {
        int res;
        res = I2cBusInit(bus);
        if (res)
            return res;
    }
    icb = (STM32_I2CCB *) bus->bus_icb;
    i2c = (I2C_TypeDef*) icb->icb_base;
    i2c->ISR = 0;
    i2c->CR2 = I2C_CR2_AUTOEND | 0*I2C_CR2_NBYTES| I2C_CR2_STOP |
        I2C_CR2_START| 0*I2C_CR2_ADD10 | 0* I2C_CR2_RD_WRN |sla<<1;
    limit = (20000/(bus->bus_rate)) + 1;
    for (i = 0; !(i2c->ISR & I2C_ISR_STOPF) && (i< limit); i++)
        NutSleep(1);
    isr = i2c->ISR;
    i2c->ICR |= (I2C_ICR_ARLOCF | I2C_ICR_STOPCF | I2C_ICR_NACKCF );
    if ((isr & I2C_ICR_NACKCF ) || i > limit)
    {
        return -1;
    }
    return 0;
}

/*
F30: AF4
I2C1 SDA  PA14 PB7  PB9
I2C1_SCL  PA15 PB6  PB8
I2C1_SMBA PB5

I2C2_SDA  PA10 PF0
I2C2_SCL  PA9  PF1  PF6
I2C2_SMBA PA8  PB12

F0: AF1
I2C1 SDA  PB7  PB9
I2C1_SCL  PB6  PB8
I2C1_SMBA PB5 (AF3)


I2C2_SDA  PB11
I2C2_SCL  PB10
I2C2_SMBA NA
*/

static STM32_I2CCB i2c1cb = {
    I2C1_BASE,             /* Register Base   */
#if defined (I2C1_SDA_PIN)
    I2C1_SDA_PIN,          /* SDA Pin number  */
#else
    -1,                    /* SDA Pin number  */
#endif
#if defined (I2C1_SCL_PIN)
    I2C1_SCL_PIN,          /* SDA Pin number  */
#else
    -1,                    /* SDA Pin number  */
#endif
#if defined (I2C1_SMBA_PIN)
    I2C1_SMBA_PIN,          /* SDA Pin number */
#else
    -1,                     /* SDA Pin number */
#endif
    &sig_TWI1_EV,           /* Event signal   */
    &sig_TWI1_ER,           /* Error signal   */
    NULL,                   /* icb_msg        */
    NULL                    /* icb_queue      */
};

static STM32_I2CCB i2c2cb = {
    I2C2_BASE,             /* Register Base   */
#if defined (I2C2_SDA_PIN)
    I2C2_SDA_PIN,          /* SDA Pin number  */
#else
    -1,                    /* SDA Pin number  */
#endif
#if defined (I2C2_SCL_PIN)
    I2C2_SCL_PIN,          /* SDA Pin number  */
#else
    -1,                    /* SDA Pin number  */
#endif
#if defined (I2C2_SMBA_PIN)
    I2C2_SMBA_PIN,          /* SDA Pin number */
#else
    -1,                     /* SDA Pin number */
#endif
    &sig_TWI2_EV,           /* Event signal   */
    &sig_TWI2_ER,           /* Error signal   */
    NULL,                   /* icb_msg        */
    NULL                    /* icb_queue      */
};

NUTI2C_BUS i2cBus1Stm32V2 = {
    &i2c1cb,    /* bus_icb */
    I2cBusInit, /* bus_init */
    I2cBusConf, /* bus_configure */
    I2cBusProbe,/* bus_probe */
    I2cBusTran, /* bus_transceive */
    100,        /* bus_timeout */
    0,          /* bus_rate */
    0,          /* bus_flags */
    NULL        /* bus_mutex */
};

NUTI2C_BUS i2cBus2Stm32V2 = {
    &i2c2cb,    /* bus_icb */
    I2cBusInit, /* bus_init */
    I2cBusConf, /* bus_configure */
    I2cBusProbe,/* bus_probe */
    I2cBusTran, /* bus_transceive */
    100,        /* bus_timeout */
    0,          /* bus_rate */
    0,          /* bus_flags */
    NULL        /* bus_mutex */
};
