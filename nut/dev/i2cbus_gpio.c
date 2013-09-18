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

#if defined(GPIO0_SDA_PORT) && defined(GPIO0_SDA_PIN)
#undef GPIO_ID
#define GPIO_ID GPIO0_SDA_PORT
#include <cfg/arch/piotran.h>
static INLINE void I2C_SDA_INIT(void) { GPIO_INIT(GPIO0_SDA_PIN); GPIO_PULLUP_ON(GPIO0_SDA_PIN); }
static INLINE void I2C_SDA_LO(void) { GPIO_SET_LO(GPIO0_SDA_PIN); GPIO_OUTPUT(GPIO0_SDA_PIN); }
static INLINE void I2C_SDA_HI(void) { GPIO_INPUT(GPIO0_SDA_PIN); GPIO_SET_HI(GPIO0_SDA_PIN); }
static INLINE int I2C_SDA_GET(void) { return GPIO_GET(GPIO0_SDA_PIN); }
#else
#define I2C_SDA_INIT()
#define I2C_SDA_LO()
#define I2C_SDA_HI()
#define I2C_SDA_GET() 0
#endif

#if defined(GPIO0_SCL_PORT) && defined(GPIO0_SCL_PIN)
#undef GPIO_ID
#define GPIO_ID GPIO0_SCL_PORT
#include <cfg/arch/piotran.h>
static INLINE void I2C_SCL_INIT(void) { GPIO_INIT(GPIO0_SCL_PIN); GPIO_PULLUP_ON(GPIO0_SCL_PIN); }
static INLINE void I2C_SCL_LO(void) { GPIO_SET_LO(GPIO0_SCL_PIN); GPIO_OUTPUT(GPIO0_SCL_PIN);}
static INLINE void I2C_SCL_HI(void) { GPIO_INPUT(GPIO0_SCL_PIN); GPIO_SET_HI(GPIO0_SCL_PIN); }
static INLINE int I2C_SCL_GET(void) { return GPIO_GET(GPIO0_SCL_PIN); }
#else
#define I2C_SCL_INIT()
#define I2C_SCL_LO()
#define I2C_SCL_HI()
#define I2C_SCL_GET() 0
#endif

/*!
 * \brief Local data of the GPIO TWI bus driver.
 */
typedef struct _GPIO_TWICB {
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
    I2C_SDA_HI();
    NutMicroDelay (icb->delay_unit);
    I2C_SCL_HI();
    NutMicroDelay (icb->delay_unit);
    I2C_SDA_LO();
    NutMicroDelay (icb->delay_unit);
    I2C_SCL_LO();
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
    I2C_SDA_LO();
    NutMicroDelay (icb->delay_unit);
    I2C_SCL_HI();
    NutMicroDelay (2 * icb->delay_unit);
    I2C_SDA_HI();
    NutMicroDelay (8 * icb->delay_unit);
}

/*
 * Toggles out a single byte in master mode.
 *
 * Entry: SCL low, SDA any
 * Exit: SCL low, SDA high
 *
 * Change SDA only when SCL is low!
 * Sample SDA short before setting SCL low!
 */
static int TwPut(GPIO_TWICB* icb, uint8_t octet)
{
    int i;

    for (i = 0x80; i; i >>= 1) {
        /* Set the data bit. */
        if (octet & i) {
            I2C_SDA_HI();
        } else {
            I2C_SDA_LO();
        }
        /* Wait for data to stabilize. */
        NutMicroDelay (2 * icb->delay_unit);
        /* Toggle the clock. */
        I2C_SCL_HI();
        NutMicroDelay (2 * icb->delay_unit);
        while(I2C_SCL_GET() == 0)
        {
            /* Clock stretching*/
            NutMicroDelay (2 * icb->delay_unit);
        }
        I2C_SCL_LO();
    }

    /* Release data line to receive the ACK bit. */
    I2C_SDA_HI();
    NutMicroDelay (2 * icb->delay_unit);
    I2C_SCL_HI();
    NutMicroDelay (2 * icb->delay_unit);
    if (I2C_SDA_GET()) {
        i = -1;
    } else {
        i = 0;
    }
    I2C_SCL_LO();
    NutMicroDelay (2 * icb->delay_unit);

    return i;
}

/*
 * Toggles in a single byte in master mode.
 *
 * Entry: SCL low, SDA any
 * Exit: SCL low, SDA low with ack set, high else
 *
 * Change SDA only when SCL is low!
 * Sample SDA short before setting SCL low!
 */
static uint8_t TwGet(GPIO_TWICB* icb, uint8_t ack)
{
    uint8_t rc = 0;
    int i;

    /* SDA is input. */
    I2C_SDA_HI();
    NutMicroDelay (1 * icb->delay_unit);
    for (i = 0x80; i; i >>= 1) {
        NutMicroDelay (2 * icb->delay_unit);
        I2C_SCL_HI();
        NutMicroDelay (2 * icb->delay_unit);
        while(I2C_SCL_GET() == 0)
        {
            /* Clock stretching*/
            NutMicroDelay (2 * icb->delay_unit);
        }
        if (I2C_SDA_GET()) {
            rc |= i;
        }
        I2C_SCL_LO();
    }
    if (ack)
    {
        /* Master sets acknowledge */
        I2C_SDA_LO();
    }
    NutMicroDelay (2 * icb->delay_unit);
   I2C_SCL_HI();
    NutMicroDelay (2 * icb->delay_unit);
    I2C_SCL_LO();
    NutMicroDelay (2 * icb->delay_unit);
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
    I2C_SDA_LO();
    NutMicroDelay (icb->delay_unit);
    I2C_SCL_HI();
    NutMicroDelay (2 * icb->delay_unit);
    I2C_SCL_LO();
    NutMicroDelay (1 * icb->delay_unit);
    I2C_SDA_HI();
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
    int i, rc = 0;

    bus = slave->slave_bus;
    icb = (GPIO_TWICB *) bus->bus_icb;

    msg->msg_widx = 0;
    msg->msg_ridx = 0;
    if (msg->msg_wlen)
    {
        /*
         * Process I2C write operation.
         */
        TwStart(icb);
        rc = TwPut(icb, (slave->slave_address << 1));
        if (rc == 0)
        {
            for (i = 0; i < msg->msg_wlen; i++)
            {
                rc = TwPut(icb, msg->msg_wdat[i]);
                if (rc)
                    break;
                msg->msg_widx++;
            }
        }
    }
    if ((rc == 0) && msg->msg_rsiz)
    {
        TwStart(icb);
        rc = TwPut(icb, (slave->slave_address << 1)|1);
        if (rc == 0)
        {
            for (i = 0; i < msg->msg_rsiz; i++)
            {
                msg->msg_rdat[i] = TwGet(icb, i < (msg->msg_rsiz-1));
                msg->msg_ridx++;
            }
        }
    }
    TwStop(icb);
    if (rc)
        msg->msg_ridx = rc;
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

#if !defined(GPIO0_SDA_PORT) || !defined(GPIO0_SDA_PIN) || \
    !defined(GPIO0_SCL_PORT) || !defined(GPIO0_SCL_PIN)
    return -1;
#endif
    /* Try to configure the bus and register the IRQ Handler */
    if (TwiBusConf(bus)) {
        return -1;
    }
    I2C_SDA_INIT();
    I2C_SCL_INIT();
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
