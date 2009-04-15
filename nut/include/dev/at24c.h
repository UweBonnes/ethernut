#ifndef _DEV_AT24C_H_
#define _DEV_AT24C_H_

#include <sys/types.h>
#include <stdint.h>

#define I2C_SLA_AT24C	0x50

struct at24c{
   uint8_t 	SlaveAddress;	/* Slave address on the bus */
   uint8_t	IAddrW;			/* With of slaves address register in bytes */
   uint8_t 	PageSize;   	/* Size of pages that can be written at once */
   uint8_t 	NumOfPage;  	/* Number of pages of previous defined size */
   uint32_t EepromSize;		/* Size of the eeprom memory in total */
   uint8_t  Timeout;		/* Timeout of internal programming in ms*/
//   unsigned int EepromName;
};

__BEGIN_DECLS
/* Prototypes */
extern int At24cWrite( struct at24c *at24cs, uint8_t *buffer, uint32_t len, uint32_t addr);
extern int At24cRead( struct at24c *at24cs, uint8_t *buffer, uint32_t len, uint32_t addr );
__END_DECLS
/* End of prototypes */
#endif
