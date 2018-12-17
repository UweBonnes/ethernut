/*
 * Copyright (C) 2013-2018 Uwe Bonnes(bon@elektron.ikp.physik.tu-darmstadt.de
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
 * \brief I2C bus driver for I2C hardware in STM32F0/F3/F7 and L0/4.
 *
 * L0/F0 don't have a seperate Error interrupt.
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
 * This is an interrupt driver, which supports master mode only.
 * No support for FM+
 * No error handling (ERRIE not enabled)
 */

#include <dev/irqreg.h>
#include <sys/nutdebug.h>
#include <sys/timer.h>
#include <sys/event.h>

#include <stdlib.h>

#include <cfg/arch.h>
#include <arch/cm3.h>
#include <dev/gpio.h>
#include <cfg/twi.h>
#include <arch/cm3/stm/stm32_gpio.h>
#include PINMUXHEADER
#include <arch/cm3/stm/stm32_irqreg.h>
#include <dev/i2cbus.h>
#include <cfg/devices.h>
/*!
 * \addtogroup xgI2cBusSTM32
 */
/*@{*/

/*!
 * \brief Constant local data of the STM32 I2C hardware.
 */
typedef struct _STM32_I2C_HW{
    /*! \brief SDA_PIN. */
    const nutgpio_t sda;
     /*! \brief SDA Pinmux. */
    const uint8_t sda_af;
    /*! \brief SCL_PIN. */
    const nutgpio_t scl;
     /*! \brief SCL PinMux. */
    const uint8_t scl_af;
    /*! \brief SMBA_PIN. */
    const nutgpio_t smba;
    /*! \brief SMBA PinMux. */
    const uint8_t smba_af;
    /*! \brief Enable Register. */
    volatile uint32_t * const enable_reg;
    /*! \brief Enable mask. */
    const uint32_t enable_mask;
    /*! \brief Reset Register. */
    volatile uint32_t *const reset_reg;
    /*! \brief reset mask. */
    const uint32_t reset_mask;
}STM32_I2C_HW;

/*!
 * \brief Local data of the STM32 I2C bus driver.
 */
typedef struct _STM32_I2CCB {
    const STM32_I2C_HW *hw;
    I2C_TypeDef *icb_i2c;
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
    uint32_t cr2;

    cr2 = icb->icb_i2c->CR2;

    /* TX parts*/
    if ((icb->icb_i2c->ISR & I2C_ISR_TCR) && !(cr2 & I2C_CR2_RD_WRN))
    {
        uint32_t txbytes_left = msg->msg_wlen - msg->msg_widx;
        if (txbytes_left <= 0xff)
        {
            cr2 &= ~(I2C_CR2_NBYTES | I2C_CR2_RELOAD);
            cr2 |= txbytes_left << 16;
        }
        /* else I2C_CR2_NBYTES and I2C_CR2_RELOAD already set*/
        icb->icb_i2c->CR2 = cr2; /* Write to NBYTES clears TCR*/
    }
    if ((icb->icb_i2c->ISR & I2C_ISR_TC) && !(cr2 & I2C_CR2_RD_WRN))
    {
        icb->icb_i2c->CR1 &= ~I2C_CR1_TXIE;
        cr2 &= ~I2C_CR2_NBYTES;
        if (msg->msg_rsiz == 0) /* No read transaction, stop after write*/
        {
            icb->icb_i2c->CR2 = cr2 | I2C_CR2_STOP  ;
        }
        else
        {
            icb->icb_i2c->CR1 |= I2C_CR1_RXIE;
            cr2 |= I2C_CR2_RD_WRN | msg->msg_rsiz << 16;
            icb->icb_i2c->CR2 = cr2 | I2C_CR2_START  ;
        }
    }
    if ((icb->icb_i2c->ISR & I2C_ISR_TXIS) && !(cr2 & I2C_CR2_RD_WRN))
    {
        icb->icb_i2c->TXDR = msg->msg_wdat[msg->msg_widx];
        msg->msg_widx++;
    }
    /* RX parts*/
    if ((icb->icb_i2c->ISR & I2C_ISR_TCR) && (cr2 & I2C_CR2_RD_WRN))
    {
        uint32_t rxbytes_left = msg->msg_rsiz - msg->msg_ridx;
        if (rxbytes_left <= 0xff)
        {
            cr2 &= ~(I2C_CR2_NBYTES | I2C_CR2_RELOAD);
            cr2 |= rxbytes_left << 16;
        }
        /* else I2C_CR2_NBYTES and I2C_CR2_RELOAD already set*/
        icb->icb_i2c->CR2 = cr2; /* Write to NBYTES clears TCR*/
    }
    if ((icb->icb_i2c->ISR & I2C_ISR_TC) && (cr2 & I2C_CR2_RD_WRN))
    {
        icb->icb_i2c->CR1 &= ~(I2C_CR1_TCIE | I2C_CR1_RXIE);
        icb->icb_i2c->CR2 = cr2 | I2C_CR2_STOP  ;
    }
    if ((icb->icb_i2c->ISR & I2C_ISR_RXNE) && (cr2 & I2C_CR2_RD_WRN))
    {
        msg->msg_rdat[msg->msg_ridx] = icb->icb_i2c->RXDR;
        msg->msg_ridx++;
        if (msg->msg_ridx == msg->msg_rsiz) {
            /* No more RX Interrupts*/
            icb->icb_i2c->CR1 &= ~I2C_CR1_RXIE;
            /* Generate Stop at end of byte */
            icb->icb_i2c->CR2 |=  I2C_CR2_STOP;
        }
    }
    if (icb->icb_i2c->ISR & I2C_ISR_STOPF)
    {
        icb->icb_i2c->CR1 &= ~I2C_CR1_STOPIE;
        icb->icb_i2c->ICR |=  I2C_ICR_STOPCF;
        NutEventPostFromIrq(&icb->icb_queue);
    }
    if (icb->icb_i2c->ISR & I2C_ISR_NACKF)
    {
        /* Save error, but only exit through STOPF */
        icb->errors |= I2C_ISR_NACKF;
        icb->icb_i2c->CR1 &= ~I2C_CR1_NACKIE;
        icb->icb_i2c->ICR |=  I2C_ICR_NACKCF;
    }
#if !defined(MCU_STM32F0) && !defined(MCU_STM32L0)

}

/*
 * STM32V2 I2C Error interrupt function.
 */
static void I2cErrorBusIrqHandler(void *arg)
{
    STM32_I2CCB *icb = (STM32_I2CCB *) arg;

    /* Fixme: More error handling! */
#endif
    if (icb->icb_i2c->ISR & I2C_ISR_ALERT)
        icb->icb_i2c->ICR |= I2C_ICR_ALERTCF;
    if (icb->icb_i2c->ISR & I2C_ISR_PECERR)
        icb->icb_i2c->ICR |= I2C_ICR_PECCF;
    if (icb->icb_i2c->ISR & I2C_ISR_OVR)
        icb->icb_i2c->ICR |= I2C_ICR_OVRCF;
    if (icb->icb_i2c->ISR & I2C_ISR_ARLO)
        icb->icb_i2c->ICR |= I2C_ICR_ARLOCF;
    if (icb->icb_i2c->ISR & I2C_ISR_BERR)
        icb->icb_i2c->ICR |= I2C_ICR_BERRCF;
    if (icb->icb_i2c->ISR & I2C_ISR_TIMEOUT)
        icb->icb_i2c->ICR |= I2C_ICR_TIMOUTCF;
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
    uint32_t cr2;
    int rc = 0;

    bus = slave->slave_bus;
    NUTASSERT(bus != NULL);
    NUTASSERT(bus->bus_icb != NULL);
    icb = (STM32_I2CCB *) bus->bus_icb;
    icb->icb_msg = msg;
    icb->errors = 0;
    cr2 = icb->icb_i2c->CR2;
    cr2 &= 0xf8000000; /* Clean out */
    cr2 |= slave->slave_address << 1;
    msg->msg_widx = 0;
    msg->msg_ridx = 0;
    /* are there bytes to write? */
    if (msg->msg_wlen)
    {
        icb->icb_i2c->CR1 |= I2C_CR1_TXIE | I2C_CR1_TCIE | I2C_CR1_STOPIE | I2C_CR1_NACKIE;
        if (msg->msg_wlen > 0xff)
            cr2 |= I2C_CR2_NBYTES | I2C_CR2_RELOAD;
        else
            cr2 |= msg->msg_wlen << 16;
    }
    else if (msg->msg_rsiz)
    {
        icb->icb_i2c->CR1 |= I2C_CR1_RXIE | I2C_CR1_STOPIE  | I2C_CR1_NACKIE;
        if (msg->msg_rsiz > 0xff)
            cr2 |= I2C_CR2_RD_WRN | I2C_CR2_NBYTES | I2C_CR2_RELOAD;
        else
            cr2 |= I2C_CR2_RD_WRN | msg->msg_rsiz << 16;
    }
    icb->icb_i2c->CR2 = cr2 | I2C_CR2_START;
    rc = NutEventWait(&icb->icb_queue, slave->slave_timeout);
    if ((icb->errors) || (rc))
        msg->msg_ridx = -1;
    return msg->msg_ridx;
}

static int checkpin_and_config(STM32_I2CCB *icb)
{
    if (!icb) {
        return -1;
    }
    Stm32GpioConfigSet(
        icb->hw->sda, GPIO_CFG_OUTPUT| GPIO_CFG_PERIPHAL|
        GPIO_CFG_MULTIDRIVE| GPIO_CFG_PULLUP | GPIO_CFG_SPEED_FAST,
        icb->hw->sda_af);
    Stm32GpioConfigSet(
        icb->hw->scl, GPIO_CFG_OUTPUT| GPIO_CFG_PERIPHAL|
        GPIO_CFG_MULTIDRIVE| GPIO_CFG_PULLUP | GPIO_CFG_SPEED_FAST,
        icb->hw->scl_af);
    Stm32GpioConfigSet(
        icb->hw->smba, GPIO_CFG_OUTPUT | GPIO_CFG_PERIPHAL|
        GPIO_CFG_MULTIDRIVE| GPIO_CFG_PULLUP |GPIO_CFG_SPEED_FAST,
        icb->hw->smba_af);
    *icb->hw->reset_reg  |=  icb->hw->reset_mask;
    *icb->hw->enable_reg |=  icb->hw->enable_mask;
    *icb->hw->reset_reg  &= ~icb->hw->reset_mask;
    /* HSI is default clock.*/
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

    /* Check parameters. */
    NUTASSERT(bus != NULL);
    NUTASSERT(bus->bus_icb != NULL);
    icb = (STM32_I2CCB *) bus->bus_icb;

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
    if ((icb->icb_i2c->TIMINGR & 0xf0ffffff) != timing)
    {
        /* Disable I2C and set new timing */
        while(icb->icb_i2c->CR1 & I2C_CR1_PE)
            icb->icb_i2c->CR1 &= ~I2C_CR1_PE;
        icb->icb_i2c->TIMINGR = timing;
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
    STM32_I2CCB *icb;

    icb = (STM32_I2CCB *) bus->bus_icb;

    if (checkpin_and_config(icb))
        return -1;
    /* Try to configure the bus*/
    if (I2cBusConf(bus)) {
        return -1;
    }
    icb->icb_i2c->CR1 |= I2C_CR1_PE;
    if (NutRegisterIrqHandler(icb->icb_sig_ev, I2cEventBusIrqHandler, icb))
        return -1;
    NutIrqEnable(icb->icb_sig_ev);
#if !defined(MCU_STM32F0) && !defined(MCU_STM32L0)
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
    int ret;

    if ((bus->bus_flags & I2C_BF_INITIALIZED) == 0) {
        int res;
        res = I2cBusInit(bus);
        if (res)
            return res;
    }
    icb = (STM32_I2CCB *) bus->bus_icb;
    icb->errors = 0;
    icb->icb_i2c->CR1 |= I2C_CR1_NACKIE |I2C_CR1_STOPIE;
    icb->icb_i2c->CR2 = I2C_CR2_AUTOEND | 0*I2C_CR2_NBYTES| I2C_CR2_STOP |
        I2C_CR2_START| 0*I2C_CR2_ADD10 | 0* I2C_CR2_RD_WRN |sla<<1;
    NutEventWait(&icb->icb_queue, 10);
    if (icb->errors & I2C_ISR_NACKF)
        ret = -1;
    else
        ret = 0;
    return ret;
}

#define I2C1_SDA_AF  PINMUX(I2C1_SDA, I2C1_SDA_FUNC)

#if  I2C1_SDA_AF == AF_NO_SUCH_PINFUNC
# warning BAD I2C1_SDA assignment
#endif
#define I2C1_SCL_AF  PINMUX(I2C1_SCL, I2C1_SCL_FUNC)
#if  I2C1_SCL_AF == AF_NO_SUCH_PINFUNC
# warning BAD I2C1_SCL assignment
#endif
#define I2C1_SMBA_AF PINMUX(I2C1_SMBA, I2C1_SMBA_FUNC)
#if  I2C1_SMBA_AF == AF_NO_SUCH_PINFUNC
# warning BAD I2C1_SMBA assignment
#endif

static const STM32_I2C_HW i2c1_hw = {
    .sda       = I2C1_SDA,
    .scl       = I2C1_SCL,
    .smba      = I2C1_SMBA,
    .sda_af    = I2C1_SDA_AF,
    .scl_af    = I2C1_SCL_AF,
    .smba_af   = I2C1_SMBA_AF,
#if defined(RCC_APB1ENR1_I2C1EN)
    .enable_reg  = &RCC->APB1ENR1,
    .enable_mask = RCC_APB1ENR1_I2C1EN,
    .reset_reg   = &RCC->APB1RSTR1,
    .reset_mask  = RCC_APB1RSTR1_I2C1RST,
#else
    .enable_reg  = &RCC->APB1ENR,
    .enable_mask = RCC_APB1ENR_I2C1EN,
    .reset_reg   = &RCC->APB1RSTR,
    .reset_mask  = RCC_APB1RSTR_I2C1RST,
#endif
};

static STM32_I2CCB i2c1cb = {
    .hw         = &i2c1_hw,
    .icb_i2c    = I2C1,
    .icb_sig_ev = &sig_TWI1_EV,
#if !defined(MCU_STM32F0) && !defined(MCU_STM32L0)
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

#if defined(HW_I2C2_STM32)

# define I2C2_SDA_AF  PINMUX(I2C2_SDA, I2C2_SDA_FUNC)
# if  I2C2_SDA_AF == AF_NO_SUCH_PINFUNC
#  warning BAD I2C2_SDA assignment
# endif
# define I2C2_SCL_AF  PINMUX(I2C2_SCL, I2C2_SCL_FUNC)
# if  I2C2_SCL_AF == AF_NO_SUCH_PINFUNC
#  warning BAD I2C2_SCL assignment
# endif
# if defined( I2C2_SMBA_FUNC)
#  define I2C2_SMBA_AF PINMUX(I2C2_SMBA, I2C2_SMBA_FUNC)
#  if  I2C2_SMBA_AF == AF_NO_SUCH_PINFUNC
#   warning BAD I2C2_SMBA assignment
#  endif
# endif

static const STM32_I2C_HW i2c2_hw = {
    .sda       = I2C2_SDA,
    .scl       = I2C2_SCL,
    .smba      = I2C2_SMBA,
    .sda_af    = I2C2_SDA_AF,
    .scl_af    = I2C2_SCL_AF,
# if defined( I2C2_SMBA_FUNC)
    /* e.g. STM32F091 has no I2C2_SMBA pin. */
    .smba_af   = I2C2_SMBA_AF,
# endif
#if defined(RCC_APB1ENR1_I2C2EN)
    .enable_reg  = &RCC->APB1ENR1,
    .enable_mask = RCC_APB1ENR1_I2C2EN,
    .reset_reg   = &RCC->APB1RSTR1,
    .reset_mask  = RCC_APB1RSTR1_I2C2RST,
#else
    .enable_reg  = &RCC->APB1ENR,
    .enable_mask = RCC_APB1ENR_I2C2EN,
    .reset_reg   = &RCC->APB1RSTR,
    .reset_mask  = RCC_APB1RSTR_I2C2RST,
#endif
};

static STM32_I2CCB i2c2cb = {
    .hw         = &i2c2_hw,
    .icb_i2c    = I2C2,
    .icb_sig_ev = &sig_TWI2_EV,
# if !defined(MCU_STM32F0) && !defined(MCU_STM32L0)
    .icb_sig_er = &sig_TWI2_ER,
# endif
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

#if defined(HW_I2C3_STM32)

# define I2C3_SDA_AF  PINMUX(I2C3_SDA, I2C3_SDA_FUNC)
# if  I2C3_SDA_AF == AF_NO_SUCH_PINFUNC
#  warning BAD I2C3_SDA assignment
# endif
# define I2C3_SCL_AF  PINMUX(I2C3_SCL, I2C3_SCL_FUNC)
# if  I2C3_SCL_AF == AF_NO_SUCH_PINFUNC
#  warning BAD I2C3_SCL assignment
# endif
# if defined( I2C3_SMBA_FUNC)
#  define I2C3_SMBA_AF PINMUX(I2C3_SMBA, I2C3_SMBA_FUNC)
#  if  I2C3_SMBA_AF == AF_NO_SUCH_PINFUNC
#   warning BAD I2C3_SMBA assignment
#  endif
# endif

static const STM32_I2C_HW i2c3_hw = {
    .sda       = I2C3_SDA,
    .scl       = I2C3_SCL,
    .smba      = I2C3_SMBA,
    .sda_af    = I2C3_SDA_AF,
    .scl_af    = I2C3_SCL_AF,
# if defined( I2C3_SMBA_FUNC)
    /* e.g. STM32F091 has no I2C3_SMBA pin. */
    .smba_af   = I2C3_SMBA_AF,
# endif
#if defined(RCC_APB1ENR1_I2C3EN)
/* STM32L4 */
    .enable_reg  = &RCC->APB1ENR1,
    .enable_mask = RCC_APB1ENR1_I2C3EN,
    .reset_reg   = &RCC->APB1RSTR1,
    .reset_mask  = RCC_APB1RSTR1_I2C3RST,
#elif defined(RCC_APB1LENR_I2C3EN)
/* H7*/
    .enable_reg  = &RCC->APB1LENR,
    .enable_mask = RCC_APB1LENR_I2C3EN,
    .reset_reg   = &RCC->APB1LRSTR,
    .reset_mask  = RCC_APB1LRSTR_I2C3RST,
#else
    .enable_reg  = &RCC->APB1ENR,
    .enable_mask = RCC_APB1ENR_I2C3EN,
    .reset_reg   = &RCC->APB1RSTR,
    .reset_mask  = RCC_APB1RSTR_I2C3RST,
#endif
};

static STM32_I2CCB i2c3cb = {
    .hw         = &i2c3_hw,
    .icb_i2c    = I2C3,
    .icb_sig_ev = &sig_TWI3_EV,
# if !defined(MCU_STM32F0) && !defined(MCU_STM32L0)
    .icb_sig_er = &sig_TWI3_ER,
# endif
};

NUTI2C_BUS i2cBus3Stm32 = {
    &i2c3cb,    /* bus_icb */
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

#if defined(HW_I2C4_STM32)

# define I2C4_SDA_AF  PINMUX(I2C4_SDA, I2C4_SDA_FUNC)
# if  I2C4_SDA_AF == AF_NO_SUCH_PINFUNC
#  warning BAD I2C4_SDA assignment
# endif
# define I2C4_SCL_AF  PINMUX(I2C4_SCL, I2C4_SCL_FUNC)
# if  I2C4_SCL_AF == AF_NO_SUCH_PINFUNC
#  warning BAD I2C4_SCL assignment
# endif
# if defined( I2C4_SMBA_FUNC)
#  define I2C4_SMBA_AF PINMUX(I2C4_SMBA, I2C4_SMBA_FUNC)
#  if  I2C4_SMBA_AF == AF_NO_SUCH_PINFUNC
#   warning BAD I2C4_SMBA assignment
#  endif
# endif

static const STM32_I2C_HW i2c4_hw = {
    .sda       = I2C4_SDA,
    .scl       = I2C4_SCL,
    .smba      = I2C4_SMBA,
    .sda_af    = I2C4_SDA_AF,
    .scl_af    = I2C4_SCL_AF,
# if defined( I2C4_SMBA_FUNC)
    /* e.g. STM32F091 has no I2C4_SMBA pin. */
    .smba_af   = I2C4_SMBA_AF,
# endif
#if defined(RCC_APB1ENR_I2C4EN)
/* STM32F76*/
    .enable_reg  = &RCC->APB1ENR,
    .enable_mask = RCC_APB1ENR_I2C4EN,
    .reset_reg   = &RCC->APB1RSTR,
    .reset_mask  = RCC_APB1RSTR_I2C4RST,
#elif defined(RCC_APB1EN2_I2C4EN)
 /* STM32L4*/
    .enable_reg  = &RCC->APB1ENR2,
    .enable_mask = RCC_APB1ENR2_I2C4EN,
    .reset_reg   = &RCC->APB1RSTR2,
    .reset_mask  = RCC_APB1RSTR2_I2C4RST,
#elif defined(RCC_APB4ENR_I2C4EN)
 /* STM32H7*/
    .enable_reg  = &RCC->APB4ENR,
    .enable_mask = RCC_APB4ENR_I2C4EN,
    .reset_reg   = &RCC->APB4RSTR,
    .reset_mask  = RCC_APB4RSTR_I2C4RST,
#endif
};

static STM32_I2CCB i2c4cb = {
    .hw         = &i2c4_hw,
    .icb_i2c    = I2C4,
    .icb_sig_ev = &sig_TWI4_EV,
# if !defined(MCU_STM32F0) && !defined(MCU_STM32L0)
    .icb_sig_er = &sig_TWI4_ER,
# endif
};

NUTI2C_BUS i2cBus4Stm32 = {
    &i2c4cb,    /* bus_icb */
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
