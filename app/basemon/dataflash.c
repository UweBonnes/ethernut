#include "dataflash.h"

/*!
 * \brief Exchange SPI byte.
 *
 * Inlined?
 */
static u_char SpiByte(u_char c)
{
    outb(SPDR, c);
    while ((inb(SPSR) & 0x80) == 0);

    return inb(SPDR);
}

/*!
 * \brief Init SPI interface.
 *
 * Should be more general (Dataflash, VS1001 etc.)
 * Dataflash requires SPI mode 3 (enable flash when SCK is high)
 *
 * - PB1(SCK) -> Mem-SCK
 * - PB2(MOSI) -> Mem-SI
 * - PB3(MISO) <- Mem-SO
 * - PB4 -> Mem-CS (through inverter)
 *
 */
static void SpiInit(void)
{
    /*
     * Init SS pin. When configured as input, we will lose master
     * mode, if this pin goes low. Thus we switch on the pull-up.
     */
    if(bit_is_clear(DDRB, 0)) {
        sbi(PORTB, 0);
    }

    /* Set SCK output. */
    sbi(DDRB, 1);
    /* Set MOSI output. */
    sbi(DDRB, 2);
    /* Enable MISO pullup. */
    sbi(PORTB, 3);

    /* Set double speed. */
    outb(SPSR, _BV(SPI2X));

    /* Enable SPI master mode 3, fosc/2. */
    outb(SPCR, _BV(SPE) | _BV(MSTR) | _BV(CPHA) | _BV(CPOL));

    /* Clean-up the status. */
    outb(SPSR, inb(SPSR));
    inb(SPDR);
}

/*!
 * \brief Read memory chip status.
 */
static u_char SpiMemStatus(void)
{
    u_char rc;

    sbi(SPIMEM_CS_PORT, SPIMEM_CS_BIT);
    SpiByte(0x57);
    rc = SpiByte(0);
    cbi(SPIMEM_CS_PORT, SPIMEM_CS_BIT);

    return rc;
}

static int SpiMemInit(u_short *pages, u_short *pagesize)
{
    u_char fs;
    
    /* Init SPI memory chip select. */
    cbi(SPIMEM_CS_PORT, SPIMEM_CS_BIT);
    sbi(SPIMEM_CS_DDR, SPIMEM_CS_BIT);
    
    /* Initialize the SPI interface. */
    SpiInit();
    
    /* Read the status register for a rudimentary check. */
    fs = SpiMemStatus();
    if(fs & 0x80) {
        fs = (fs >> 2) & 0x0F;
        *pagesize = 264;
        if(fs == 3) {
            *pages = 512;
            return 0;
        }
        else if(fs == 5) {
            *pages = 1024;
            return 0;
        }
        else if(fs == 7) {
            *pages = 2048;
            return 0;
        }
    }
    return -1;
}

/*
 * \return Number of bytes.
 */
long SpiMemTest(void)
{
    u_short pages = 0;
    u_short pagesize = 0;

    SpiMemInit(&pages, &pagesize);

    /* Disable SPI */
    outb(SPCR, 0);

    return (long)pages * (long)pagesize;
}
