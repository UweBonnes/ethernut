#ifndef EEPROM_H
#define EEPROM_H

#include <stdint.h>
#include <dev/at24c.h>
void EEInit( void );
int EEReadData( uint16_t addr, void *data, uint16_t len );
int EEWriteData( uint16_t addr, void *data, uint16_t len );

#endif