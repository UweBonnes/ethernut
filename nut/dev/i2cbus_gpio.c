/*
 * Copyright (C) 2013 Uwe Bonnes(bon@elelktron.ikp.physik.tu-darmstadt.de
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
 * \file dev/i2cbus_gpio.c
 * \brief I2C bus driver for GPIO.
 *
 * This driver is in an early stage and has been tested on STM32 only.
 *
 * It is intended that this driver replaces the current GPIO TWI driver,
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
 * \brief I2C bus driver for GPIO hardware.
 *
 * This is an polling driver, which supports master mode only.
 */

#include <dev/irqreg.h>
#include <sys/nutdebug.h>
#include <sys/timer.h>
#include <sys/event.h>

#include <stdlib.h>

#include <dev/gpio.h>
#include <dev/i2cbus_gpio.h>
#include <cfg/twi.h>
/*!
 * \addtogroup xgI2cBusGPIO
 */
/*@{*/

/*!
 * \brief Local data of the GPIO TWI bus driver.
 */
typedef struct _GPIO_TWICB {
    /*! \brief SDA_PORT. */
    nutgpio_port_t sda_port;
    /*! \brief SDA_PIN. */
    nutgpio_pin_t sda_pin;
    /*! \brief SCL_PORT. */
    nutgpio_port_t scl_port;
    /*! \brief SCL_PIN. */
    nutgpio_pin_t scl_pin;
    /*! \brief Delay Unit. */
    unsigned int delay_unit;
} GPIO_TWICB;

/*
 * Falling edge on the data line while the clock line is high indicates
 * a start condition.
 *
 * Entry: SCL any, SDA any
 * Exit: SCL low, SDA low
 */
static void TwStart(GPIO_TWICB* icb)
{
    GpioPinRelease(icb->sda_port, icb->sda_pin);
    GpioPinSetHigh(icb->sda_port, icb->sda_pin);
    NutMicroDelay (icb->delay_unit);
    GpioPinRelease(icb->scl_port, icb->scl_pin);
    GpioPinSetHigh(icb->scl_port, icb->scl_pin);
    NutMicroDelay (icb->delay_unit);
    GpioPinSetLow (icb->sda_port, icb->sda_pin);
    GpioPinDrive  (icb->sda_port, icb->sda_pin);
    NutMicroDelay (icb->delay_unit);
    GpioPinSetLow (icb->scl_port, icb->scl_pin);
    GpioPinDrive  (icb->scl_port, icb->scl_pin);
    NutMicroDelay (icb->delay_unit);
}

/*
 * Rising edge on the data line while the clock line is high indicates
 * a stop condition.
 *
 * Entry: SCL low, SDA any
 * Exit: SCL high, SDA high
 */
static void TwStop(GPIO_TWICB* icb)
{
    GpioPinSetLow (icb->sda_port, icb->sda_pin);
    GpioPinDrive  (icb->sda_port, icb->sda_pin);
    NutMicroDelay (icb->delay_unit);
    GpioPinRelease(icb->scl_port, icb->scl_pin);
    GpioPinSetHigh(icb->scl_port, icb->scl_pin);
    NutMicroDelay (2 * icb->delay_unit);
    GpioPinRelease(icb->sda_port, icb->sda_pin);
    GpioPinSetHigh(icb->sda_port, icb->sda_pin);
    NutMicroDelay (8 * icb->delay_unit);
}

/*
 * Toggles out a single byte in master mode.
 *
 * Entry: SCL low, SDA any
 * Exit: SCL low, SDA high
 */
static int TwPut(GPIO_TWICB* icb, uint8_t octet)
{
    int i;

    for (i = 0x80; i; i >>= 1) {
        /* Set the data bit. */
        if (octet & i) {
            GpioPinRelease(icb->sda_port, icb->sda_pin);
            GpioPinSetHigh(icb->sda_port, icb->sda_pin);
        } else {
            GpioPinSetLow (icb->sda_port, icb->sda_pin);
            GpioPinDrive  (icb->sda_port, icb->sda_pin);
        }
        /* Wait for data to stabelize. */
        NutMicroDelay (1 * icb->delay_unit);
        /* Toggle the clock. */
        GpioPinRelease(icb->scl_port, icb->scl_pin);
        GpioPinSetHigh(icb->scl_port, icb->scl_pin);
        NutMicroDelay (2 * icb->delay_unit);
        GpioPinSetLow (icb->scl_port, icb->scl_pin);
        GpioPinDrive  (icb->scl_port, icb->scl_pin);
        NutMicroDelay (1 * icb->delay_unit);
    }

    /* Set data line high to receive the ACK bit. */
    GpioPinRelease(icb->sda_port, icb->sda_pin);
    GpioPinSetHigh(icb->sda_port, icb->sda_pin);

    /* ACK should appear shortly after the clock's rising edge. */
    GpioPinRelease(icb->scl_port, icb->scl_pin);
    GpioPinSetHigh(icb->scl_port, icb->scl_pin);
    NutMicroDelay (2 * icb->delay_unit);
    if (GpioPinGet(icb->sda_port, icb->sda_pin)) {
        i = -1;
    } else {
        i = 0;
    }
    GpioPinSetLow (icb->scl_port, icb->scl_pin);
    GpioPinDrive  (icb->scl_port, icb->scl_pin);

    return i;
}

/*
 * Toggles in a single byte in master mode.
 *
 * Entry: SCL low, SDA any
 * Exit: SCL low, SDA high
 */
static uint8_t TwGet(GPIO_TWICB* icb)
{
    uint8_t rc = 0;
    int i;

    /* SDA is input. */
    GpioPinRelease(icb->sda_port, icb->sda_pin);
    GpioPinSetHigh(icb->sda_port, icb->sda_pin);
    NutMicroDelay (1 * icb->delay_unit);
    for (i = 0x80; i; i >>= 1) {
        NutMicroDelay (1 * icb->delay_unit);
        /* Data should appear shortly after the clock's rising edge. */
        GpioPinRelease(icb->scl_port, icb->scl_pin);
        GpioPinSetHigh(icb->scl_port, icb->scl_pin);
        NutMicroDelay (2 * icb->delay_unit);
        /* SDA read. */
        if (GpioPinGet(icb->sda_port, icb->sda_pin)) {
            rc |= i;
        }
        GpioPinSetLow (icb->scl_port, icb->scl_pin);
        GpioPinDrive  (icb->scl_port, icb->scl_pin);
    }
    return rc;
}

#if 0
/*
 * Toggles out an acknowledge bit in master mode.
 *
 * Entry: SCL low, SDA any
 * Exit: SCL low, SDA high
 */
static void TwAck(GPIO_TWICB* icb)
{
    GpioPinSetLow (icb->sda_port, icb->sda_pin);
    GpioPinDrive  (icb->sda_port, icb->sda_pin);
    NutMicroDelay (icb->delay_unit);
    GpioPinRelease(icb->scl_port, icb->scl_pin);
    GpioPinSetHigh(icb->scl_port, icb->scl_pin);
    NutMicroDelay (2 * icb->delay_unit);
    GpioPinSetLow (icb->scl_port, icb->scl_pin);
    GpioPinDrive  (icb->scl_port, icb->scl_pin);
    NutMicroDelay (1 * icb->delay_unit);
    GpioPinRelease(icb->sda_port, icb->sda_pin);
    GpioPinSetHigh(icb->sda_port, icb->sda_pin);
}
#endif

/*!
 * \brief I2C bus transfer (GPIO TWI implementation).
 *
 * This function is called by the platform independent code via the
 * NUTI2C_BUS::bus_tran function pointer.
 */
static int TwiBusTran(NUTI2C_SLAVE *slave, NUTI2C_MSG *msg)
{
    NUTI2C_BUS *bus;
    GPIO_TWICB *icb;
    int i, rc = -1;

    bus = slave->slave_bus;
    icb = (GPIO_TWICB *) bus->bus_icb;

    msg->msg_widx = 0;
    msg->msg_ridx = 0;
    /*
     * Process I2C read operation.
     */
    TwStart(icb);
    rc = TwPut(icb, slave->slave_address);
    if (rc)
        return rc;
    if (msg->msg_wlen) {
        for (i = 0; i < msg->msg_wlen; i++) {
            rc = TwPut(icb, msg->msg_wdat[i]);
            if (rc)
                return rc;
            msg->msg_widx++;
        }
    }
    if (msg->msg_rsiz) {
        for (i = 0; i < msg->msg_rsiz; i++) {
            msg->msg_rdat[i] = TwGet(icb);
            /* Check for NACK?*/
            msg->msg_ridx++;
        }
    }
    TwStop(icb);

    return msg->msg_ridx;
}

/*!
 * \brief Configure the I2C bus controller (GPIO TWI implementation).
 *
 * This function is called by the platform independent code via the
 * NUTI2C_BUS::bus_conf function pointer. Most implementations will
 * also call this function during initialization to set the
 * default configuration.
 *
 * Right now only the bus clock rate is configurable.
 */
static int TwiBusConf(NUTI2C_BUS *bus)
{
    GPIO_TWICB *icb;
    long rate;

    /* Check parameters. */
    NUTASSERT(bus != NULL);
    NUTASSERT(bus->bus_icb != NULL);
    icb = (GPIO_TWICB *) bus->bus_icb;

    /* Get requested rate or use the default. */
    rate = bus->bus_rate;
    if (rate == 0) {
        rate = 100000L;
    }
    if (rate > 400000) {
        /* Speed out of range */
        return -1;
    }
    icb->delay_unit = 250000/rate;
    if (icb->delay_unit == 0)
        icb->delay_unit = 1;

    return 0;
}

/*!
 * \brief Initialize the I2C bus controller (GPIO implementation).
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
static int TwiBusInit(NUTI2C_BUS *bus)
{
    GPIO_TWICB *icb;

    icb = (GPIO_TWICB *) bus->bus_icb;

    if(icb->sda_port == -1 || icb->sda_pin == -1 ||
       icb->scl_port == -1 || icb->scl_pin == -1)
        return 0;

    /* Try to configure the bus and register the IRQ Handler */
    if (TwiBusConf(bus)) {
        return -1;
    }
    /* TODO: Check for errors*/
    GpioPinRelease(icb->sda_port, icb->sda_pin);
    GpioPinRelease(icb->scl_port, icb->scl_pin);
    GpioPinConfigSet(icb->sda_port, icb->sda_pin,
                     GPIO_CFG_OUTPUT | GPIO_CFG_MULTIDRIVE | GPIO_CFG_PULLUP);
    GpioPinConfigSet(icb->scl_port, icb->scl_pin,
                     GPIO_CFG_OUTPUT | GPIO_CFG_MULTIDRIVE | GPIO_CFG_PULLUP);

    return 0;
}

/*!
 * \brief Probe the I2C bus for a specified slave address (GPIO implementation).
 *
 * This function is called by the platform independent code via the
 * NUTI2C_BUS::bus_probe function pointer. This may happen even if no
 * slave device had been attached to the bus and thus without any
 * previous call to NUTI2C_BUS::bus_init. However, in that case
 * NUTI2C_BUS::bus_configure will have been called.
 */
static int TwiBusProbe(NUTI2C_BUS *bus, int sla)
{
    int rc = -1;
    GPIO_TWICB *icb;

    icb = (GPIO_TWICB *) bus->bus_icb;
    if ((bus->bus_flags & I2C_BF_INITIALIZED) == 0) {
        int res;
        res = TwiBusInit(bus);
        if (res)
            return res;
    }
    TwStart(icb);
    rc = TwPut(icb, (sla<<1));
    TwStop(icb);

    return rc;
}

static GPIO_TWICB twi0cb = {
#if defined(GPIO0_SDA_PORT) && defined(GPIO0_SDA_PIN) \
    && defined(GPIO0_SCL_PORT) && defined(GPIO0_SCL_PIN)
    GPIO0_SDA_PORT,      /* SDA PORT as NutGpioPortX */
    GPIO0_SDA_PIN,       /* SDA Pin number*/
    GPIO0_SCL_PORT,      /* SCL PORT as NutGpioPortX */
    GPIO0_SCL_PIN,       /* SCL Pin number*/
#else
    (nutgpio_port_t) -1, /* */
    (nutgpio_pin_t)  -1, /* */
    (nutgpio_port_t) -1, /* */
    (nutgpio_pin_t)  -1, /* */
#endif
    0                 /* Delay unit in us*/
};


NUTI2C_BUS i2cBus0Gpio = {
    &twi0cb,    /* bus_icb */
    TwiBusInit, /* bus_init */
    TwiBusConf, /* bus_configure */
    TwiBusProbe,/* bus_probe */
    TwiBusTran, /* bus_transceive */
    100,        /* bus_timeout */
    0,          /* bus_rate */
    0,          /* bus_flags */
    NULL        /* bus_mutex */
};
