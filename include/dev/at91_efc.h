#ifndef _DEV_AT91_EFC_H_
#define _DEV_AT91_EFC_H_

#include <sys/types.h>

/*!
 * \brief Load data from MCU on-chip flash.
 *
 * \return 0 on success, -1 otherwise.
 */
extern int OnChipFlashLoad(u_long addr, void *buff, size_t siz);

/*!
 * \brief Save data to MCU on-chip flash.
 *
 * \return 0 on success, -1 otherwise.
 */
extern int OnChipFlashSave(u_long addr, CONST void *buff, size_t len);

extern int FlashAreaIsLock(u_long addr);

extern int LockFlashArea(u_long addr);

extern int UnlockFlashArea(u_long addr);

#endif
