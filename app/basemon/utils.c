#include <stdio.h>

#include "utils.h"

/*!
 * \brief Delay by executing a given number of NOPs.
 *
 * parm nops 2,000,000 is roughly 1 second.
 */
void Delay(long nops)
{
    long i;

    for (i = 0; i < nops; i++) {
        _NOP();
    }
}

int hex2bin(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    return 0;
}

