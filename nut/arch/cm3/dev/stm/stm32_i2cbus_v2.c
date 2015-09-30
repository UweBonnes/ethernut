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
 * \brief I2C bus driver for STM32F0/3/7 hardware.
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
#include <arch/cm3/stm/stm32_gpio.h>
#include <arch/cm3/stm/stm32_irqreg.h>
#include <dev/i2cbus.h>
#include <cfg/twi.h>
#include <cfg/devices.h>
/*!
 * \addtogroup xgI2cBusSTM32
 */
/*@{*/

/*!
 * \brief Constant local data of the STM32 I2C hardware.
 */
typedef struct _STM32_I2C_HW{
    /*! \brief Register base. */
    const uptr_t icb_base;
    /*! \brief SDA_PIN. */
    const uint8_t sda_pin;
     /*! \brief SDA Pinmux. */
    const uint8_t sda_af;
    /*! \brief SDA_PORT. */
    const uint32_t sda_port;
    /*! \brief SCL_PIN. */
    const uint8_t scl_pin;
     /*! \brief SCL PinMux. */
    const uint8_t scl_af;
     /*! \brief SCL_PORT. */
    const uint32_t scl_port;
    /*! \brief SMBA_PIN. */
    const uint8_t smba_pin;
    /*! \brief SMBA PinMux. */
    const uint8_t smba_af;
    /*! \brief SMBA_PORT. */
    const uint32_t smba_port;
    /*! \brief System event handler. */
}STM32_I2C_HW;

/*!
 * \brief Local data of the STM32 I2C bus driver.
 */
typedef struct _STM32_I2CCB {
    const STM32_I2C_HW *hw;
    IRQ_HANDLER *icb_sig_ev;
    /*! \brief System error handler. */
    IRQ_HANDLER *icb_sig_er;
    /*! \brief I2C message. */
    NUTI2C_MSG *icb_msg;
    /*! \brief Thread waiting for completion. */
    HANDLE icb_queue;
    uint32_t errors;
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

    i2c = (I2C_TypeDef *) icb->hw->icb_base;
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
    if (i2c->ISR & I2C_ISR_NACKF)
    {
        /* Save error, but only exit through STOPF */
        icb->errors |= I2C_ISR_NACKF;
        i2c->CR1 &= ~I2C_CR1_NACKIE;
        i2c->ICR |=  I2C_ICR_NACKCF;
    }
#if defined(MCU_STM32F3)

}

/*
 * STM32V2 I2C Error interrupt function.
 */
static void I2cErrorBusIrqHandler(void *arg)
{
    STM32_I2CCB *icb = (STM32_I2CCB *) arg;
    I2C_TypeDef *i2c;

    /* Fixme: More error handling! */
    i2c = (I2C_TypeDef *) icb->hw->icb_base;
#endif
    if (i2c->ISR & I2C_ISR_ALERT)
        i2c->ICR |= I2C_ICR_ALERTCF;
    if (i2c->ISR & I2C_ISR_PECERR)
        i2c->ICR |= I2C_ICR_PECCF;
    if (i2c->ISR & I2C_ISR_OVR)
        i2c->ICR |= I2C_ICR_OVRCF;
    if (i2c->ISR & I2C_ISR_ARLO)
        i2c->ICR |= I2C_ICR_ARLOCF;
    if (i2c->ISR & I2C_ISR_BERR)
        i2c->ICR |= I2C_ICR_BERRCF;
    if (i2c->ISR & I2C_ISR_TIMEOUT)
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
    icb->errors = 0;
    i2c = (I2C_TypeDef *) icb->hw->icb_base;
    cr2 = i2c->CR2;
    cr2 &= 0xf8000000; /* Clean out */
    cr2 |= slave->slave_address << 1;
    msg->msg_widx = 0;
    msg->msg_ridx = 0;
    /* are there bytes to write? */
    if (msg->msg_wlen)
    {
        i2c->CR1 |= I2C_CR1_TXIE | I2C_CR1_TCIE | I2C_CR1_STOPIE | I2C_CR1_NACKIE;
        if (msg->msg_wlen > 0xff)
            cr2 |= I2C_CR2_NBYTES | I2C_CR2_RELOAD;
        else
            cr2 |= msg->msg_wlen << 16;
    }
    else if (msg->msg_rsiz)
    {
        i2c->CR1 |= I2C_CR1_RXIE | I2C_CR1_STOPIE  | I2C_CR1_NACKIE;
        if (msg->msg_rsiz > 0xff)
            cr2 |= I2C_CR2_RD_WRN | I2C_CR2_NBYTES | I2C_CR2_RELOAD;
        else
            cr2 |= I2C_CR2_RD_WRN | msg->msg_rsiz << 16;
    }
    i2c->CR2 = cr2 | I2C_CR2_START;
    rc = NutEventWait(&icb->icb_queue, slave->slave_timeout);
    if ((icb->errors) || (rc))
        msg->msg_ridx = -1;
    return msg->msg_ridx;
}

static int checkpin_and_config(STM32_I2CCB *icb)
{
    GpioPinConfigSet(
        icb->hw->sda_port, icb->hw->sda_pin, GPIO_CFG_OUTPUT| GPIO_CFG_PERIPHAL|
        GPIO_CFG_MULTIDRIVE| GPIO_CFG_PULLUP | GPIO_CFG_SPEED_FAST);
    GpioPinConfigSet(
        icb->hw->scl_port, icb->hw->scl_pin, GPIO_CFG_OUTPUT| GPIO_CFG_PERIPHAL|
        GPIO_CFG_MULTIDRIVE| GPIO_CFG_PULLUP | GPIO_CFG_SPEED_FAST);
    GPIO_PinAFConfig(icb->hw->sda_port, icb->hw->sda_pin, icb->hw->sda_af);
    GPIO_PinAFConfig(icb->hw->scl_port, icb->hw->scl_pin, icb->hw->scl_af);
    if (icb->hw->smba_pin != (uint8_t)-1)
    {
        /* TODO: How should SMBA pin be set?*/
        GpioPinConfigSet(
            icb->hw->smba_port, icb->hw->smba_pin, GPIO_CFG_OUTPUT|
            GPIO_CFG_PERIPHAL| GPIO_CFG_MULTIDRIVE| GPIO_CFG_PULLUP |
            GPIO_CFG_SPEED_FAST);
        GPIO_PinAFConfig(icb->hw->smba_port,
                         icb->hw->smba_pin, GPIO_AF_4);
    }
    if (icb->hw->icb_base == I2C1_BASE)
    {
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB1RSTR, _BI32(RCC_APB1RSTR_I2C1RST));
        CM3BBCLR(RCC_BASE, RCC_TypeDef, APB1ENR, _BI32(RCC_APB1ENR_I2C1EN));
        /* Use HSI clock*/
#if defined(RCC_CFGR3_I2C1SW)
        CM3BBCLR(RCC_BASE, RCC_TypeDef, CFGR3,    _BI32(RCC_CFGR3_I2C1SW));
#endif
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB1ENR_I2C1EN));
        CM3BBCLR(RCC_BASE, RCC_TypeDef, APB1RSTR, _BI32(RCC_APB1RSTR_I2C1RST));
    }
#if defined(I2C2_BASE)
    else if (icb->hw->icb_base == I2C2_BASE)
    {
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB1RSTR, _BI32(RCC_APB1RSTR_I2C2RST));
        CM3BBCLR(RCC_BASE, RCC_TypeDef, APB1ENR, _BI32(RCC_APB1ENR_I2C2EN));
        /* Use HSI clock*/
#if defined(RCC_CFGR3_I2C2SW)
        CM3BBCLR(RCC_BASE, RCC_TypeDef, CFGR3,    _BI32(RCC_CFGR3_I2C2SW));
#endif
        CM3BBSET(RCC_BASE, RCC_TypeDef, APB1ENR,  _BI32(RCC_APB1ENR_I2C2EN));
        CM3BBCLR(RCC_BASE, RCC_TypeDef, APB1RSTR, _BI32(RCC_APB1RSTR_I2C2RST));
    }
#endif
#if defined(I2C3_BASE)
    else if (icb->hw->icb_base == I2C3_BASE)
    {
    }
#endif
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
    i2c = (I2C_TypeDef*) icb->hw->icb_base;

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
    i2c = (I2C_TypeDef*) icb->hw->icb_base;
    i2c->CR1 |= I2C_CR1_PE;
    if (NutRegisterIrqHandler(icb->icb_sig_ev, I2cEventBusIrqHandler, icb))
        return -1;
    NutIrqEnable(icb->icb_sig_ev);
#if defined(MCU_STM32F3)
    if (NutRegisterIrqHandler(icb->icb_sig_er, I2cErrorBusIrqHandler, icb))
        return -1;
    NutIrqDisable(icb->icb_sig_er);
#endif

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
    int ret;

    if ((bus->bus_flags & I2C_BF_INITIALIZED) == 0) {
        int res;
        res = I2cBusInit(bus);
        if (res)
            return res;
    }
    icb = (STM32_I2CCB *) bus->bus_icb;
    i2c = (I2C_TypeDef*) icb->hw->icb_base;
    icb->errors = 0;
    i2c->CR1 |= I2C_CR1_NACKIE |I2C_CR1_STOPIE;
    i2c->CR2 = I2C_CR2_AUTOEND | 0*I2C_CR2_NBYTES| I2C_CR2_STOP |
        I2C_CR2_START| 0*I2C_CR2_ADD10 | 0* I2C_CR2_RD_WRN |sla<<1;
    NutEventWait(&icb->icb_queue, 10);
    if (icb->errors & I2C_ISR_NACKF)
        ret = -1;
    else
        ret = 0;
    return ret;
}

/*
F30: AF4
I2C1 SDA  PA14 PB7  PB9
I2C1_SCL  PA15 PB6  PB8
I2C1_SMBA PB5

I2C2_SDA  PA10 PF0
I2C2_SCL  PA9  PF1  PF6
I2C2_SMBA PA8  PB12

F37: AF4
I2C1 SDA  PA14 PB7  PB9
I2C1_SCL  PA15 PB6  PB8
I2C1_SMBA PB5

I2C2_SDA  PA10 PF0  PF7
I2C2_SCL  PA9  PF1  PF6
I2C2_SMBA PA8  PF2

F0: AF1
I2C1 SDA  PB7(AF1)  PB9(AF1) PA10(AF4) PF0(AF1)
I2C1_SCL  PB6(AF1)  PB8(AF1)  PA9(AF4) PF1(AF1)
I2C1_SMBA PB5(AF3)

I2C2_SDA  PB11(AF1) PB14(AF5) PA12(AF5)
I2C2_SCL  PB10(AF1) PB13(AF5) PA11(AF5)
I2C2_SMBA NA

L0: AF1
I2C1 SDA  PB7(AF1)  PB9(AF4)
I2C1_SCL  PB6(AF1)  PB8(AF4)
I2C1_SMBA PB5(AF3)

I2C2_SDA  PB11(AF6) PB14(AF5)
I2C2_SCL  PB10(AF6) PC13(AF5)
I2C2_SMBA PB12(AF5)

F7: AF4
I2C2_SDA  PB11  PF0  PH5
I2C2_SCL  PB10  PF1  PH4
I2C2_SMBA PB12  PF2  PH6

I2C3 SDA  PC9   PH8
I2C3 SCL  PA8   PH7
I2C3 SMBA PA9   PH9

I2C4 SDA  PD13  PF15 PH12
I2C4 SCL  PD12  PF13 PH11
I2C4 SMBA PD11  PF13 PH10
*/

#if   defined(MCU_STM32F7)
/*
 * F7: AF4
 * I2C1 SDA  PB7 PB9
 * I2C1_SCL  PB6 PB8
 * I2C1_SMBA PB5
 */
# if I2C1_SDA_PIN != 7 && I2C1_SDA_PIN != 9
#  warning Illegal F7 I2C1_SDA Pin
# endif

# if I2C1_SCL_PIN != 6 && I2C1_SCL_PIN != 8
#  warning Illegal F7 I2C1_SCL Pin
# endif

# if I2C1_SMBA_PIN != -1 && I2C1_SMBA_PIN != 5
#  warning Illegal F7 I2C1_SMBA Pin
# endif

/* Get the Port for the given pin */
# define I2C1_SDA_PORT NUTGPIO_PORTB
# define I2C1_SDA_AF 4
# define I2C1_SCL_PORT NUTGPIO_PORTB
# define I2C1_SCL_AF 4
# define I2C1_SMBA_PORT NUTGPIO_PORTB
# define I2C1_SMBA_AF 4

#elif defined(MCU_STM32L0)
# if I2C1_SDA_PIN != 7 && I2C1_SDA_PIN != 9
#  warning Illegal L0 I2C1_SDA Pin
# endif

# if I2C1_SCL_PIN != 6 && I2C1_SCL_PIN != 8
#  warning Illegal LO I2C1_SCL Pin
# endif

# if !defined(I2C1_SMBA_PIN)
#  define I2C1_SMBA_PIN -1
# elif I2C1_SMBA_PIN != -1 && I2C1_SMBA_PIN != 5
#  warning Illegal L0 I2C1_SMBA Pin
# endif

/* Get the Port for the given pin */
# define I2C1_SDA_PORT NUTGPIO_PORTB
# if  I2C1_SDA_PIN == 7
#  define I2C1_SDA_AF 1
# else
#  define I2C1_SDA_AF 4
# endif

# define I2C1_SCL_PORT NUTGPIO_PORTB
# if  I2C1_SCL_PIN == 6
#  define I2C1_SCL_AF 1
# else
#  define I2C1_SCL_AF 4
# endif

# define I2C1_SMBA_AF 5
# if I2C1_SMBA_PIN == 5
#  define I2C1_SMBA_PORT NUTGPIO_PORTB
# else
#  define I2C1_SMBA_PORT -1
# endif

#elif defined(MCU_STM32F0)
# if I2C1_SDA_PIN != 7 && I2C1_SDA_PIN != 9     \
    && I2C1_SDA_PIN != 10 && I2C1_SDA_PIN != 0
#  warning Illegal F0 I2C1_SDA Pin
# endif

# if I2C1_SCL_PIN != 6 && I2C1_SCL_PIN != 8     \
    && I2C1_SCL_PIN != 9 && I2C1_SCL_PIN != 1
#  warning Illegal FO I2C1_SCL Pin
# endif

# if !defined(I2C1_SMBA_PIN)
#  define I2C1_SMBA_PIN -1
# elif I2C1_SMBA_PIN != -1 && I2C1_SMBA_PIN != 5
#  warning Illegal F0 I2C1_SMBA Pin
# endif

/* Get the Port for the given pin */
# if  I2C1_SDA_PIN == 10
#  define I2C1_SDA_PORT NUTGPIO_PORTA
#  define I2C1_SDA_AF 4
# else
#  define I2C1_SDA_AF 1
#  if I2C1_SDA_PIN == 0
#   define I2C1_SDA_PORT NUTGPIO_PORTF
#  else
#   define I2C1_SDA_PORT NUTGPIO_PORTB
#  endif
# endif

# if I2C1_SCL_PIN == 9
#  define I2C1_SCL_PORT NUTGPIO_PORTA
#  define I2C1_SCL_AF 4
# else
#  define I2C1_SCL_AF 1
#  if  I2C1_SCL_PIN == 1
#   define I2C1_SCL_PORT NUTGPIO_PORTF
#  else
#   define  I2C1_SCL_PORT NUTGPIO_PORTB
#  endif
# endif

# define I2C1_SMBA_AF 3
# if I2C1_SMBA_PIN == 5
#  define I2C1_SMBA_PORT NUTGPIO_PORTB
# else
#  define I2C1_SMBA_PORT -1
# endif

#elif defined(MCU_STM32F3)
# define I2C1_SDA_AF  4
# define I2C1_SCL_AF  4
# define I2C1_SMBA_AF 4
/* Check that we got the right pin */
#if I2C1_SDA_PIN  != 14 && I2C1_SDA_PIN  != 7 && I2C1_SDA_PIN  != 9
#warning Illegal I2C1_SDA Pin
#endif

#if I2C1_SCL_PIN  != 15 && I2C1_SCL_PIN  != 6 && I2C1_SCL_PIN  != 8
#warning Illegal I2C1_SCL Pin
#endif

#if !defined(I2C1_SMBA_PIN)
#define I2C1_SMBA_PIN -1
#elif I2C1_SMBA_PIN != -1 && I2C1_SMBA_PIN != 5
#warning Illegal I2C1_SMBA Pin
#endif

/* Get the Port for the given pin */
#if I2C1_SDA_PIN == 14
#define I2C1_SDA_PORT NUTGPIO_PORTA
#elif  I2C1_SDA_PIN == 7 || I2C1_SDA_PIN == 9
#define I2C1_SDA_PORT NUTGPIO_PORTB
#endif

#if I2C1_SCL_PIN == 15
#define I2C1_SCL_PORT NUTGPIO_PORTA
#elif  I2C1_SCL_PIN == 6 || I2C1_SCL_PIN == 8
#define I2C1_SCL_PORT NUTGPIO_PORTB
#endif

#if I2C1_SMBA_PIN == 5
#define I2C1_SMBA_PORT NUTGPIO_PORTB
#else
#define I2C1_SMBA_PORT -1
#endif
#endif

#if defined(HW_I2C2_STM32V2)
# if   defined(MCU_STM32F7)
/*
 * F7: AF4
 * I2C2_SDA  PB11  PF0  PH5
 * I2C2_SCL  PB10  PF1  PH4
 * I2C2_SMBA PB12  PF2  PH6
 */
#  if I2C2_SDA_PIN != 11 && I2C2_SDA_PIN != 0 && I2C2_SDA_PIN != 5
#   warning Illegal F7 I2C2_SDA Pin
#  endif

#  if I2C2_SCL_PIN != 10 && I2C2_SCL_PIN != 1 && I2C2_SCL_PIN != 4
#   warning Illegal F7 I2C2_SCL Pin
#  endif

#  if I2C2_SMBA_PIN != -1 && I2C2_SMBA_PIN != 12 && I2C2_SMBA_PIN != 2 && I2C2_SMBA_PIN != 6
#   warning Illegal F5 I2C2_SMBA Pin
#  endif

/* Get the Port for the given pin */
#  define I2C2_SDA_AF 4
#  define I2C2_SCL_AF 4
#  define I2C2_SMBA_AF 4

#  if I2C2_SDA_PIN == 11
#   define I2C2_SDA_PORT NUTGPIO_PORTB
#  elif I2C2_SDA_PIN == 0
#   define I2C2_SDA_PORT NUTGPIO_PORTH
#  else
#   define I2C2_SDA_PORT NUTGPIO_PORTH
#  endif

#  if I2C2_SCL_PIN == 10
#   define I2C2_SCL_PORT NUTGPIO_PORTB
#  elif  I2C2_SCL_PIN == 1
#   define I2C2_SCL_PORT NUTGPIO_PORTF
#  else
#   define I2C2_SCL_PORT NUTGPIO_PORTH
#  endif

#  if I2C2_SMBA_PIN == 12
#   define I2C2_SMBA_PORT NUTGPIO_PORTB
#  elif  I2C2_SMBA_PIN == 2
#   define I2C2_SMBA_PORT NUTGPIO_PORTF
#  else
#   define I2C2_SMBA_PORT NUTGPIO_PORTH
#  endif

# elif defined(MCU_STM32L0)
#  if I2C2_SDA_PIN != 11 && I2C2_SDA_PIN != 14
#   warning Illegal L0 I2C2_SDA Pin
#  endif

#  if I2C2_SCL_PIN != 10 && I2C2_SCL_PIN != 13
#   warning Illegal L0 I2C2_SCL Pin
#  endif

#  if !defined(I2C2_SMBA_PIN)
#   define I2C2_SMBA_PIN -1
#  elif I2C2_SMBA_PIN != -1 && I2C2_SMBA_PIN != 12
#   warning Illegal L0 I2C2_SMBA Pin
#  endif

/* Get the Port for the given pin */
#  define I2C2_SDA_PORT NUTGPIO_PORTB
#  if I2C2_SDA_PIN == 11
#   define I2C2_SDA_AF 6
#  else
#   define I2C2_SDA_AF 5
#  endif

#  if I2C2_SCL_PIN == 10
#   define I2C2_SCL_PORT NUTGPIO_PORTB
#   define I2C2_SCL_AF 6
#  elif  I2C2_SCL_PIN == 13
#   define I2C2_SCL_PORT NUTGPIO_PORTC
#   define I2C2_SCL_AF 5
#  endif

#  define I2C2_SMBA_AF 5
#  if I2C2_SMBA_PIN == 12
#   define I2C2_SMBA_PORT NUTGPIO_PORTB
#  else
#   define I2C2_SMBA_PORT -1
#  endif

# elif defined(MCU_STM32F0)
#  if I2C2_SDA_PIN != 11 && I2C2_SDA_PIN != 14 &&  I2C2_SDA_PIN != 12
#   warning Illegal F0 I2C2_SDA Pin
#  endif

#  if I2C2_SCL_PIN != 10 && I2C2_SCL_PIN != 13 && I2C2_SCL_PIN != 11
#   warning Illegal F0 I2C2_SCL Pin
#  endif

#  if (I2C2_SMBA_PIN != -1)
#   warning STM32F0: Illegal SMBA pin, no SMBA pin available on I2C2
#  endif

/* Get the Port for the given pin */
#  if I2C2_SDA_PIN == 11
#   define I2C2_SDA_PORT NUTGPIO_PORTB
#   define I2C2_SDA_AF 1
#  else
#   define I2C2_SDA_AF 5
#   if I2C2_SDA_PIN == 14
#    define I2C2_SDA_PORT NUTGPIO_PORTB
#   else
#    define I2C2_SDA_PORT NUTGPIO_PORTA
#   endif
#  endif

#  if I2C2_SCL_PIN == 10
#   define I2C2_SCL_PORT NUTGPIO_PORTB
#   define I2C2_SCL_AF 1
#  else
#   define I2C2_SCL_AF 1
#   if  I2C2_SCL_PIN == 13
#    define I2C2_SCL_PORT NUTGPIO_PORTB
#   else
#    define I2C2_SCL_PORT NUTGPIO_PORTA
#   endif
#  endif

#  define I2C2_SMBA_PORT -1
#  define I2C2_SMBA_PIN  -1
#  define I2C2_SMBA_AF 17
# else /* STM32F3*/
#  define I2C2_SDA_AF  4
#  define I2C2_SCL_AF  4
#  define I2C2_SMBA_AF 4
/* Check that we got the right pin */
#if I2C2_SDA_PIN != 10 && I2C2_SDA_PIN != 0 && (defined(MCU_STM32F373_MD) && I2C2_SDA_PIN != 7)
#warning Illegal I2C2_SDA Pin
#endif

#if I2C2_SCL_PIN != 9 && I2C2_SCL_PIN != 1 && I2C2_SCL_PIN != 6
#warning Illegal I2C2_SCL Pin
#endif

#if !defined(I2C2_SMBA_PIN)
#define I2C2_SMBA_PIN -1
#elif  I2C2_SMBA_PIN != -1 && I2C2_SMBA_PIN != 8 && I2C2_SMBA_PIN != 12
#warning Illegal I2C2_SMBA Pin
/* Get the Port for the given pin */
#endif

#if I2C2_SDA_PIN == 10
#define I2C2_SDA_PORT NUTGPIO_PORTA
#elif  I2C2_SDA_PIN == 7 || I2C2_SDA_PIN == 0
#define I2C2_SDA_PORT NUTGPIO_PORTF
#endif

#if I2C2_SCL_PIN == 9
#define I2C2_SCL_PORT NUTGPIO_PORTA
#elif  I2C2_SCL_PIN == 6 || I2C2_SCL_PIN == 1
#define I2C2_SCL_PORT NUTGPIO_PORTF
#endif

#if I2C2_SMBA_PIN == 8
#define I2C2_SMBA_PORT NUTGPIO_PORTA
#elif I2C2_SMBA_PIN == 2
#define I2C2_SMBA_PORT NUTGPIO_PORTF
#else
#define I2C2_SMBA_PORT NUTGPIO_PORTF
#endif

#endif
#else
#warning Unknown family
#endif

static const STM32_I2C_HW i2c1_hw = {
    .icb_base  = I2C1_BASE,
    .sda_pin   = I2C1_SDA_PIN,
    .sda_af    = I2C1_SDA_AF,
    .sda_port  = I2C1_SDA_PORT,
    .scl_pin   = I2C1_SCL_PIN,
    .scl_af    = I2C1_SCL_AF,
    .scl_port  = I2C1_SCL_PORT,
    .smba_pin  = I2C1_SMBA_PIN,
    .smba_af   = I2C1_SMBA_AF,
    .smba_port = I2C1_SMBA_PORT,
};

static STM32_I2CCB i2c1cb = {
    .hw         = &i2c1_hw,
    .icb_sig_ev = &sig_TWI1_EV,
#if defined(MCU_STM32F3) || defined(MCU_STM32F7)
    .icb_sig_er = &sig_TWI1_ER,
#endif
};

NUTI2C_BUS i2cBus1Stm32 = {
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

#if defined(HW_I2C2_STM32V2)
static const STM32_I2C_HW i2c2_hw = {
    .icb_base  = I2C2_BASE,
    .sda_pin   = I2C2_SDA_PIN,
    .sda_af    = I2C2_SDA_AF,
    .sda_port  = I2C2_SDA_PORT,
    .scl_pin   = I2C2_SCL_PIN,
    .scl_af    = I2C2_SCL_AF,
    .scl_port  = I2C2_SCL_PORT,
    .smba_pin  = I2C2_SMBA_PIN,
    .smba_af   = I2C2_SMBA_AF,
    .smba_port = I2C2_SMBA_PORT,
};

static STM32_I2CCB i2c2cb = {
    .hw         = &i2c2_hw,
    .icb_sig_ev = &sig_TWI2_EV,
#if defined(MCU_STM32F3) || defined(MCU_STM32F7)
    .icb_sig_er = &sig_TWI2_ER,
#endif
};

NUTI2C_BUS i2cBus2Stm32 = {
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
#endif
