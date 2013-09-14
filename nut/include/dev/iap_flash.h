#ifndef _INCLUDE_IAP_FLASH_H
#define _INCLUDE__IAP_FLASH_H
#include <cfg/memory.h>

typedef enum
{
    FLASH_ERASE_ALWAYS      = 0,   /*!< Always erase whole memory block before writing */
    FLASH_ERASE_FIRST_TOUCH = 1,   /*!< Only erase whole memory block with first write to that block*/
    FLASH_ERASE_NEVER       = 2,   /*!< Write without erasing */
} FLASH_ERASE_MODE;

/*!
 * brief FLASH status returns of any operation.
 */
typedef enum
{
    FLASH_BUSY          =  1,   /*!< Flash operation pending */
    FLASH_COMPLETE      =  0,   /*!< Flash operation successfull completed */
    FLASH_ERROR_PG      = -1,   /*!< Flash programming failed */
    FLASH_ERROR_WRP     = -2,   /*!< Flash write protected */
    FLASH_LOCKED        = -3,   /*!< FLASH is locked, unlocking failed */
    FLASH_TIMEOUT       = -4,   /*!< Flash operation timed out */
    FLASH_BOUNDARY      = -5,   /*!< Flash write crosses page/sector border */
    FLASH_COMPARE       = -6,   /*!< FLASH compare mismatch */
    FLASH_OUT_OF_MEMORY = -7,   /*!< Intermediate buffer allocation failed*/
    FLASH_ERR_CONF_LAYOUT = -8,   /*!< CONF_PAGE has unexpected layout*/
    FLASH_NOT_IMPLEMENTED = -9,   /*!< Intermediate buffer allocation failed*/
} FLASH_Status;

#if defined(IAP_FLASH)
extern FLASH_Status  IapFlashWrite( void* dst, void* src, size_t len, FLASH_ERASE_MODE mode);
extern FLASH_Status IapFlashWriteProtect(void *dst, size_t len, int ena);
#else
FLASH_Status  IapFlashWrite( void* dst, void* src, size_t len, FLASH_ERASE_MODE mode)
{
    return FLASH_NOT_IMPLEMENTED;
}
FLASH_Status  IapFlashWriteProtect(void *dst, size_t len, int ena)
{
    return FLASH_NOT_IMPLEMENTED;
}
#endif
#endif
