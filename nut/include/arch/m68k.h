#ifndef _ARCH_M68K_H_
#define _ARCH_M68K_H_

#include <stddef.h>
#include <cfg/arch.h>

#if defined (MCU_COLDFIRE)
#include <arch/m68k/coldfire/mcf5.h>
#else
#warning "Unknown M68K MCU Family defined"
#endif

#if !defined(__BIG_ENDIAN__)
#define __BIG_ENDIAN__
#endif

#define CONST   const
#define INLINE  inline

#define PSTR(p)    (p)
#define PRG_RDB(p) (*((const char *)(p)))

#define prog_char  const char
#define PGM_P      prog_char *

#define SIGNAL(x)  __attribute__((interrupt_handler)) void x(void)

#define strlen_P(x)             strlen(x)
#define strcpy_P(x,y)           strcpy(x,y)
#define strncpy_P(x,y,z)        strncpy(x,y,z)
#define strcmp_P(x, y)          strcmp(x, y)
#define strcasecmp_P(x,y)       strcasecmp(x,y)
#define strstr_P(x,y)         	strstr(x,y)
#define memcpy_P(x,y,z)         memcpy(x,y,z)

#define outb(_reg, _val)  (*((volatile uint8_t *)(_reg)) = (_val))
#define outw(_reg, _val)  (*((volatile uint16_t *)(_reg)) = (_val))
#define outr(_reg, _val)  (*((volatile uint32_t *)(_reg)) = (_val))

#define inb(_reg)   (*((volatile uint8_t *)(_reg)))
#define inw(_reg)   (*((volatile uint16_t *)(_reg)))
#define inr(_reg)   (*((volatile uint32_t *)(_reg)))

#define _BV(bit)    (1 << (bit))

#ifndef _NOP
#define _NOP() __asm__ __volatile__ ("nop")
#endif

#endif
