/****************************************************************************
*  This file is part of the AVRIDE device driver.
*
*  Copyright (c) 2002-2003 by Michael Fischer. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*  
*  1. Redistributions of source code must retain the above copyright 
*     notice, this list of conditions and the following disclaimer.
*  2. Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in the 
*     documentation and/or other materials provided with the distribution.
*  3. Neither the name of the author nor the names of its contributors may 
*     be used to endorse or promote products derived from this software 
*     without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL 
*  THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
*  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
*  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
*  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF 
*  THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
*  SUCH DAMAGE.
*
****************************************************************************
*  History:
*
*  14.12.02  mifi   First Version
*  29.12.02  mifi   Support CF-Card too .
*  01.01.03  mifi   Add support for IDELock, IDEFree.
*  08.01.03  mifi   Now support several modes, like:
*                   IDE_HARDDISK           16bit 14.7456Mhz / 2 wait states
*                   IDE_HARDDISK_7MHZ      16bit  7.3728Mhz / 2 wait states 
*                   IDE_COMPACT_FLASH      16bit 14.7456Mhz / 2 wait states
*                   MEM_8BIT_COMPACT_FLASH  8bit 14.7456Mhz / 2 wait states
*  18.01.03  mifi   Change Licence from GPL to BSD.
*  25.01.03  mifi   Fix the bug in ClearEvent.
*                   Change IDEInit and add "AutoMount", "AutoUnMount" 
*                   callback function. With these functions we are
*                   independent from the FileSystem.
*                   Now support changing of the CF-Card in a running system.
****************************************************************************/
#define __IDE_C__

#include <stddef.h>
#include <string.h>

#include <sys/timer.h>
#include <sys/thread.h>
#include <sys/event.h>
#include <sys/atom.h>
#include <sys/heap.h>
#include <dev/irqreg.h>

#include <dev/ide.h>
#include <dev/idep.h>
#include <fs/typedefs.h>

/*==========================================================*/
/*  DEFINE: All Structures and Common Constants             */
/*==========================================================*/
//
// Here are some important values.
// Change these values if you change the hardware.
//
#define IDE_IRQ                 IRQ_INT7
#define IDE_INT_RISING_EDGE     0xC0

#define CF_IRQ                  IRQ_INT6
#define CF_INT_SENS_MASK        0x30
#define CF_INT_FALLING_EDGE     0x20
#define CF_INT_RISING_EDGE      0x30

/************************************************/
#define IDE_MAX_SUPPORTED_DEVICE        1


#define CF_AVAILABLE            0
#define CF_NOT_AVAILABLE        1

#define INITTIMEOUT             10000
#define DISKTIMEOUT             10000
#define CONTROLLERTIMEOUT       200

#define IDEIn(x)                pIDE[x]
#define IDEOut(x,y)             pIDE[x] = y

//
// Drive Flags
//
#define IDE_SUPPORT_LBA         0x0001
#define IDE_SUPPORT_LBA48       0x0002
#define IDE_READY               0x8000

typedef struct _drive {
    //
    // Interface values
    //
    WORD wFlags;
    BYTE bIDEMode;
    BYTE bDevice;

#if (IDE_SUPPORT_CHS == 1)
    //
    // CHS values
    //
    WORD wCylinders;
    WORD wHeads;
    WORD wSectorsPerTrack;
    DWORD dwOneSide;
#endif

    //
    // LBA value
    //
    DWORD dwTotalSectors;

} DRIVE, *LPDRIVE;

/*==========================================================*/
/*  DEFINE: Prototypes                                      */
/*==========================================================*/

/*==========================================================*/
/*  DEFINE: Definition of all local Data                    */
/*==========================================================*/
static HANDLE hIDEEvent;
static volatile BYTE *pIDE;
static volatile BYTE gbIntStatus = 0;

static DRIVE sDrive[IDE_MAX_SUPPORTED_DEVICE];

static HANDLE hIDESemaphore;
static HANDLE hCFChangeInt;
static BYTE gbCFMountStatus = 0;

static IDE_MOUNT_FUNC *pUserMountFunc = NULL;
static IDE_MOUNT_FUNC *pUserUnMountFunc = NULL;
/*==========================================================*/
/*  DEFINE: Definition of all local Procedures              */
/*==========================================================*/
/************************************************************/
/*  IDELock                                                 */
/************************************************************/
void IDELock(void)
{
    NutEventWait(&hIDESemaphore, 0);
}

/************************************************************/
/*  IDEFree                                                 */
/************************************************************/
void IDEFree(void)
{
    NutEventPost(&hIDESemaphore);
}

/************************************************************/
/*  IDESemaInit                                             */
/************************************************************/
void IDESemaInit(void)
{
    NutEventPost(&hIDESemaphore);
}

/************************************************************/
/*  HardwareReset                                           */
/************************************************************/
static void HardwareReset(DRIVE * pDrive)
{
    if (pDrive->bIDEMode == MEM_8BIT_COMPACT_FLASH) {
        //
        // Set Reset
        //
        PORTD |= 0x20;
        _NOP();

        NutSleep(1000);

        //
        // Remove Reset
        //
        PORTD &= ~0x20;
        _NOP();

        NutSleep(1000);
    } else {                    /* HD */
        //
        // Set RESET
        //
        PORTD &= ~0x20;
        _NOP();

        NutSleep(1000);

        //
        // Remove RESET
        //
        PORTD |= 0x20;
        _NOP();

        if (pDrive->bIDEMode == IDE_COMPACT_FLASH) {
            NutSleep(1000);
        } else {
            //
            // It can takes up to 10 seconds (2.5-Inch drive) 
            // from reset high to NOT busy.
            // Take a look in the drive manual "Reset timings"
            //
            NutSleep(10000);
        }
    }
}

/************************************************************/
/*  CFInterrupt                                             */
/************************************************************/
static void CFInterrupt(void *p)
{
    p = p;

    NutEventPostAsync(&hCFChangeInt);
}

/************************************************************/
/*  IDEInterrupt                                            */
/************************************************************/
static void IDEInterrupt(void *p)
{
    p = p;

    gbIntStatus = IDEIn(STATUS_REG);

    NutEventPostAsync(&hIDEEvent);
}

/************************************************************/
/*  ClearEvent                                              */
/************************************************************/
static void ClearEvent(HANDLE * pEvent)
{
    NutEnterCritical();

    *pEvent = 0;

    NutExitCritical();
}

/************************************************************/
/*  WaitForInterrupt                                        */
/*                                                          */
/*  Return: IDE_OK, IDE_ERROR                               */
/************************************************************/
static int WaitForInterrupt(DWORD dwTimeout)
{
    int nError = IDE_OK;
    int nTimeout;

    nTimeout = NutEventWait(&hIDEEvent, dwTimeout);
    if (nTimeout == -1) {
        nError = IDE_ERROR;
    }

    return (nError);
}

#if (IDE_SUPPORT_WRITE == 1)
/************************************************************/
/*  WaitDRQ                                                 */
/*                                                          */
/*  Return: IDE_OK, IDE_ERROR                               */
/************************************************************/
static int WaitDRQ(DWORD dwTimeout)
{
    int nError = IDE_OK;
    BYTE bStatus;

    bStatus = IDEIn(STATUS_REG);
    if ((bStatus & (STATUS_BUSY | STATUS_DATA_REQUEST)) !=
        STATUS_DATA_REQUEST) {

        dwTimeout = (DWORD) (((dwTimeout * 10UL) / 625UL) + 1UL);
        dwTimeout += NutGetTickCount();

        bStatus = IDEIn(STATUS_REG);
        while ((bStatus & (STATUS_BUSY | STATUS_DATA_REQUEST)) !=
               STATUS_DATA_REQUEST) {
            if (NutGetTickCount() > dwTimeout) {
                nError = IDE_ERROR;
                break;
            }
            bStatus = IDEIn(STATUS_REG);
        }

    }

    return (nError);
}
#endif

/************************************************************/
/*  WaitNotBusy  (see ATAPI-4 r17 p223 9.6)                 */
/*                                                          */
/*  Return: IDE_OK, IDE_ERROR                               */
/************************************************************/
static int WaitNotBusy(DWORD dwTimeout)
{
    int nError = IDE_OK;
    BYTE bStatus;

    bStatus = IDEIn(STATUS_REG);
    if (bStatus & (STATUS_BUSY | STATUS_DATA_REQUEST)) {

        dwTimeout = ((dwTimeout * 10) / 625) + 1;
        dwTimeout += NutGetTickCount();

        bStatus = IDEIn(STATUS_REG);
        while (bStatus & (STATUS_BUSY | STATUS_DATA_REQUEST)) {
            if (NutGetTickCount() > dwTimeout) {
                nError = IDE_BUSY;
                break;
            }
            bStatus = IDEIn(STATUS_REG);
        }

    }

    return (nError);
}

/************************************************************/
/*  SelectDevice  (see ATAPI-4 r17 p223 9.6)                */
/*                                                          */
/*  Return: IDE_OK, IDE_ERROR                               */
/************************************************************/
static int SelectDevice(BYTE bDevice)
{
    int nError;
    int nTimeout;
    BYTE bStatus;
    BYTE bDummy;

    nError = IDE_OK;

    if (WaitNotBusy(CONTROLLERTIMEOUT) == IDE_OK) {

        IDEOut(DISK_HEAD_REG, 0xA0 + (bDevice << 4));

        //
        // Wait at least 400ns, with 14.7456Mhz we need
        // 8 nop's for 500ns
        //
        COMPRESS_DISABLE;
        _NOP();
        _NOP();
        _NOP();
        _NOP();
        _NOP();
        _NOP();
        _NOP();
        _NOP();
        COMPRESS_REENABLE;

        nError = WaitNotBusy(CONTROLLERTIMEOUT);
    } else {
        nError = IDE_BUSY;
    }

    if (nError != IDE_OK) {
        //
        // Test for special case, data available.
        // If data available, read the data!!
        //
        bStatus = IDEIn(STATUS_REG);
        if (bStatus & STATUS_DATA_REQUEST) {

            nTimeout = 512;
            while ((bStatus & STATUS_DATA_REQUEST) && (nTimeout)) {
                bDummy = IDEIn(DATA_READ_REG_LOW);
                bDummy = IDEIn(DATA_READ_REG_LOW);
                bStatus = IDEIn(STATUS_REG);
                nTimeout--;
            }
        }
    }

    return (nError);
}

/************************************************************/
/*  GetDeviceInfo                                           */
/*                                                          */
/*  PIO data in command protocol (see ATAPI-4 r17 p224 9.7) */
/*                                                          */
/*  Return: IDE_OK, IDE_ERROR                               */
/************************************************************/
static BYTE GetDeviceInfo(LPDRIVE pDrive, BYTE * pSectorBuffer)
{
    WORD i;
    int nError;
    BYTE bStatus;
    BYTE bErrorReg;
    BYTE bDevice = pDrive->bDevice;
    IDEDEVICEINFO *pInfo;

    nError = IDE_ERROR;

    if (SelectDevice(bDevice) == IDE_OK) {

        ClearEvent(&hIDEEvent);

#if 0
        IDEOut(FEATURE_REG, 0);
        IDEOut(SECTOR_COUNT_REG, 1);
        IDEOut(SECTOR_REG, 0);
        IDEOut(CYLINDER_LOW_REG, 0);
        IDEOut(CYLINDER_HIGH_REG, 0);
#endif

        IDEOut(COMMAND_REG, COMMAND_DEVICE_INFO);
        if (WaitForInterrupt(DISKTIMEOUT) == IDE_OK) {

            if (gbIntStatus & STATUS_DATA_REQUEST) {

                for (i = 0; i < IDE_SECTOR_SIZE; i = i + 2) {
                    pSectorBuffer[i] = pIDE[DATA_READ_REG_LOW];
                    if (pDrive->bIDEMode == MEM_8BIT_COMPACT_FLASH) {
                        pSectorBuffer[i + 1] = pIDE[DATA_READ_REG_LOW];
                    } else {
                        pSectorBuffer[i + 1] = pIDE[DATA_READ_REG_HIGH];
                    }
                }

                //
                // Status register is read to clear
                // pending interrupts.
                //
                bStatus = IDEIn(STATUS_REG);

                pInfo = (IDEDEVICEINFO *) pSectorBuffer;

#if (IDE_SUPPORT_CHS == 1)
                pDrive->wCylinders = (WORD) pInfo->Cylinders;
                pDrive->wHeads = (BYTE) pInfo->Heads;
                pDrive->wSectorsPerTrack = (BYTE) pInfo->SectorsPerTrack;
                pDrive->dwOneSide = pDrive->wCylinders *
                    pDrive->wSectorsPerTrack;
#endif

                if (pInfo->LBASectors > 0) {
                    pDrive->wFlags = IDE_READY | IDE_SUPPORT_LBA;
                    pDrive->dwTotalSectors = pInfo->LBASectors;
                } else {

#if (IDE_SUPPORT_CHS == 1)
                    pDrive->wFlags = IDE_READY;
                    pDrive->dwTotalSectors = pInfo->Cylinders;
                    pDrive->dwTotalSectors *= pInfo->Heads;
                    pDrive->dwTotalSectors *= pInfo->SectorsPerTrack;
#endif

                }

                nError = IDE_OK;
            }

            if (gbIntStatus & STATUS_ERROR) {
                bErrorReg = IDEIn(ERROR_REG);
            }
        }

    }

    return (nError);
}

/************************************************************/
/*  DeviceDiag                                              */
/*                                                          */
/*  NON-data command protocol (see ATAPI-4 r17 p231 9.9)    */
/*                                                          */
/*  Return: IDE_OK, IDE_ERROR                               */
/************************************************************/
static int DeviceDiag(LPDRIVE pDrive)
{
    int nError = IDE_ERROR;
    BYTE bResult;

    if (SelectDevice(0) == IDE_OK) {

#if 0
        IDEOut(FEATURE_REG, 0);
        IDEOut(SECTOR_COUNT_REG, 1);
        IDEOut(SECTOR_REG, 0);
        IDEOut(CYLINDER_LOW_REG, 0);
        IDEOut(CYLINDER_HIGH_REG, 0);
#endif

        ClearEvent(&hIDEEvent);

        IDEOut(COMMAND_REG, COMMAND_DIAG);
        if (WaitForInterrupt(INITTIMEOUT) == IDE_OK) {

            bResult = IDEIn(ERROR_REG);
            if (bResult == 0x01) {      // see ATAPI-4 r17 p72
                nError = IDE_OK;
            }

        }
    }

    return (nError);
}

/************************************************************/
/*  ReadSectors                                             */
/*                                                          */
/*  PIO data in command protocol, see ATAPI-4 r17 p224 9.7  */
/*                                                          */
/*  Return: IDE_OK, IDE_ERROR                               */
/************************************************************/
static int ReadSectors(BYTE bDevice, BYTE * pData,
                       DWORD dwStartSector, WORD wSectorCount)
{
    WORD i, x;
    int nError;
    BYTE bValue;
    BYTE bErrorReg;
    BYTE bStatus;
    LPDRIVE pDrive;

#if (IDE_SUPPORT_CHS == 1)
    WORD wCHSSector;
    WORD wCHSHead;
    WORD wCHSCylinder;
#endif

    pDrive = &sDrive[bDevice];
    nError = SelectDevice(bDevice);

#if (IDE_SUPPORT_CHS == 0)
    //
    // NO CHS support
    //
    if ((pDrive->wFlags & IDE_SUPPORT_LBA) == 0) {
        //
        // If we have a CHS device, this is an ERROR
        //
        nError = IDE_ERROR;
    }
#endif

    if (nError == IDE_OK) {

        ClearEvent(&hIDEEvent);

        if (pDrive->wFlags & IDE_SUPPORT_LBA) {
            //
            // LBA
            //
            bValue =
                (BYTE) ((bDevice << 4) | 0xE0 | (dwStartSector >> 24));
            IDEOut(DISK_HEAD_REG, bValue);
            IDEOut(LBA_16_23, (BYTE) (dwStartSector >> 16));
            IDEOut(LBA_8_15, (BYTE) (dwStartSector >> 8));
            IDEOut(LBA_0_7, (BYTE) (dwStartSector));

            IDEOut(SECTOR_COUNT_REG, (BYTE) (wSectorCount & 0xff));
            IDEOut(COMMAND_REG, COMMAND_READ_SECTORS);

#if (IDE_SUPPORT_CHS == 1)
        } else {
            //
            // CHS
            //
            wCHSSector =
                (WORD) (dwStartSector % pDrive->wSectorsPerTrack) + 1;
            wCHSHead = (WORD) (dwStartSector / pDrive->dwOneSide);
            dwStartSector = dwStartSector % pDrive->dwOneSide;
            wCHSCylinder =
                (WORD) (dwStartSector / pDrive->wSectorsPerTrack);

            wCHSHead |= (bDevice << 4) | 0xA0;

            IDEOut(DISK_HEAD_REG, (BYTE) wCHSHead);
            IDEOut(SECTOR_REG, (BYTE) wCHSSector);
            IDEOut(CYLINDER_LOW_REG, (BYTE) (wCHSCylinder & 0x00FF));
            IDEOut(CYLINDER_HIGH_REG, (BYTE) (wCHSCylinder >> 8));

            IDEOut(SECTOR_COUNT_REG, (BYTE) (wSectorCount & 0xff));
            IDEOut(COMMAND_REG, COMMAND_READ_SECTORS);
#endif
        }

        for (i = 0; i < wSectorCount; i++) {
            nError = WaitForInterrupt(DISKTIMEOUT);
            if (nError == IDE_OK) {
                bStatus = gbIntStatus;

                if (bStatus & STATUS_ERROR) {
                    bErrorReg = IDEIn(ERROR_REG);
                    nError = IDE_ERROR;
                    break;
                }

                for (x = 0; x < IDE_SECTOR_SIZE; x = x + 2) {
                    pData[x] = pIDE[DATA_READ_REG_LOW];
                    if (pDrive->bIDEMode == MEM_8BIT_COMPACT_FLASH) {
                        pData[x + 1] = pIDE[DATA_READ_REG_LOW];
                    } else {
                        pData[x + 1] = pIDE[DATA_READ_REG_HIGH];
                    }
                }

                pData += IDE_SECTOR_SIZE;

            } else {
                bStatus = IDEIn(STATUS_REG);
                bErrorReg = IDEIn(ERROR_REG);
            }
        }

        bStatus = IDEIn(STATUS_REG);

    }
    /* SelectDevice */
    return (nError);
}

#if (IDE_SUPPORT_WRITE == 1)
/************************************************************/
/*  WriteSectors                                            */
/*                                                          */
/*  PIO data out command protocol, see ATAPI-4 r17 p227 9.8 */
/*                                                          */
/*  Return: IDE_OK, IDE_ERROR                               */
/************************************************************/
static BYTE WriteSectors(WORD bDevice, BYTE * pData,
                         DWORD dwStartSector, WORD wSectorCount)
{
    WORD i;
    int x;
    int nError;
    BYTE bValue;
    BYTE bStatus;
    BYTE bErrorReg;
    LPDRIVE pDrive;

#if (IDE_SUPPORT_CHS == 1)
    WORD wCHSSector;
    WORD wCHSHead;
    WORD wCHSCylinder;
#endif

    pDrive = &sDrive[bDevice];
    nError = SelectDevice(bDevice);

#if (IDE_SUPPORT_CHS == 0)
    //
    // NO CHS support
    //
    if ((pDrive->wFlags & IDE_SUPPORT_LBA) == 0) {
        //
        // If we found a CHS device, this is an ERROR
        //
        nError = IDE_ERROR;
    }
#endif

    if (nError == IDE_OK) {

        if (pDrive->wFlags & IDE_SUPPORT_LBA) {
            //
            // LBA
            //
            bValue =
                (BYTE) ((bDevice << 4) | 0xE0 | (dwStartSector >> 24));
            IDEOut(DISK_HEAD_REG, bValue);
            IDEOut(LBA_16_23, (BYTE) (dwStartSector >> 16));
            IDEOut(LBA_8_15, (BYTE) (dwStartSector >> 8));
            IDEOut(LBA_0_7, (BYTE) (dwStartSector));

            IDEOut(SECTOR_COUNT_REG, (BYTE) (wSectorCount & 0xff));
            IDEOut(COMMAND_REG, COMMAND_WRITE_SECTORS);

#if (IDE_SUPPORT_CHS == 1)
        } else {
            //
            // CHS
            //
            wCHSSector =
                (WORD) (dwStartSector % pDrive->wSectorsPerTrack) + 1;
            wCHSHead = (WORD) (dwStartSector / pDrive->dwOneSide);
            dwStartSector = dwStartSector % pDrive->dwOneSide;
            wCHSCylinder =
                (WORD) (dwStartSector / pDrive->wSectorsPerTrack);

            wCHSHead |= (bDevice << 4) | 0xA0;

            IDEOut(DISK_HEAD_REG, (BYTE) wCHSHead);
            IDEOut(SECTOR_REG, (BYTE) wCHSSector);
            IDEOut(CYLINDER_LOW_REG, (BYTE) (wCHSCylinder & 0x00FF));
            IDEOut(CYLINDER_HIGH_REG, (BYTE) (wCHSCylinder >> 8));

            IDEOut(SECTOR_COUNT_REG, (BYTE) (wSectorCount & 0xff));
            IDEOut(COMMAND_REG, COMMAND_WRITE_SECTORS);
#endif
        }

        if (WaitDRQ(CONTROLLERTIMEOUT) == IDE_OK) {

            bStatus = IDEIn(STATUS_REG);
            for (i = 0; i < wSectorCount; i++) {

                for (x = 0; x < IDE_SECTOR_SIZE; x = x + 2) {
                    if (pDrive->bIDEMode == MEM_8BIT_COMPACT_FLASH) {
                        IDEOut(DATA_WRITE_REG_LOW, pData[x]);
                        IDEOut(DATA_WRITE_REG_LOW, pData[x + 1]);
                    } else {
                        IDEOut(DATA_WRITE_REG_HIGH, pData[x + 1]);
                        IDEOut(DATA_WRITE_REG_LOW, pData[x]);
                    }
                }

                pData += IDE_SECTOR_SIZE;

                nError = WaitForInterrupt(DISKTIMEOUT);
                if (nError == IDE_OK) {
                    bStatus = gbIntStatus;

                    if ((bStatus & STATUS_ERROR) == STATUS_ERROR) {
                        bErrorReg = IDEIn(ERROR_REG);
                        nError = IDE_ERROR;
                        break;
                    }
                } else {
                    //
                    // Error
                    //
                    nError = IDE_ERROR;
                    break;
                }
            }

        } else {
            //
            // Device: Set error status (D)
            //
            WaitForInterrupt(DISKTIMEOUT);
            bErrorReg = IDEIn(ERROR_REG);
        }                       /* endif WaitDRQ */

    }
    /* SelectDevice */
    return (nError);
}
#endif

/************************************************************/
/*  CFChange                                                */
/************************************************************/
THREAD(CFChange, arg)
{
    BYTE bNewStatus;
    BYTE *pSectorBuffer;

    while (1) {

        NutEventWaitNext(&hCFChangeInt, 0);
        //
        // Wait a while, that the cf contact is stable and not to bounce.
        //
        NutSleep(2000);

        bNewStatus = (PINE & BV(CF_IRQ));
        if (bNewStatus != gbCFMountStatus) {

            if (bNewStatus == CF_AVAILABLE) {
                gbCFMountStatus = CF_AVAILABLE;
                //
                // We must mount the new CF card.
                //
                HardwareReset(&sDrive[0]);

                pSectorBuffer = (BYTE *) NutHeapAlloc(IDE_SECTOR_SIZE);
                if (pSectorBuffer != NULL) {
                    IDEMountDevice(IDE_DRIVE_C, pSectorBuffer);
                    NutHeapFree(pSectorBuffer);

                    if (pUserMountFunc != NULL) {
                        pUserMountFunc(IDE_DRIVE_C);
                    }
                }

                NutEnterCritical();
                EICR &= ~CF_INT_SENS_MASK;
                EICR |= CF_INT_RISING_EDGE;
                NutExitCritical();

            } else {
                gbCFMountStatus = CF_NOT_AVAILABLE;
                //
                // The user has removed the CF card,
                // now UnMount the device.
                //
                IDEUnMountDevice(IDE_DRIVE_C);

                if (pUserUnMountFunc != NULL) {
                    pUserUnMountFunc(IDE_DRIVE_C);
                }

                NutEnterCritical();
                EICR &= ~CF_INT_SENS_MASK;
                EICR |= CF_INT_FALLING_EDGE;
                NutExitCritical();
            }
        }                       /* endif bValue != gbCFMountStatus */
    }                           /* end while */
}

/*==========================================================*/
/*  DEFINE: All code exported                               */
/*==========================================================*/
/************************************************************/
/*  IDEInit                                                 */
/************************************************************/
int IDEInit(int nBaseAddress,
            int nIDEMode,
            IDE_MOUNT_FUNC * pMountFunc, IDE_MOUNT_FUNC * pUnMountFunc)
{
    int i;
    int nError;
    BYTE bValue;

    pUserMountFunc = pMountFunc;
    pUserUnMountFunc = pUnMountFunc;

    //
    // With MCUCR 0xC0 and XMCRA 0x02 we set:
    //
    // Wait two cycles during read/write and 
    // wait one cycle before driving out new address.
    // This setting is for 0x1100 - 0xffff
    //
#ifdef __AVR_ATmega128__
    XMCRA = 0x02;
#endif

    for (i = 0; i < IDE_MAX_SUPPORTED_DEVICE; i++) {
        memset((BYTE *) & sDrive[i], 0x00, sizeof(DRIVE));

        sDrive[i].bDevice = (BYTE) i;
        sDrive[i].bIDEMode = (BYTE) nIDEMode;
    }

    IDESemaInit();

    pIDE = (unsigned char *) nBaseAddress;

    ClearEvent(&hIDEEvent);

    NutEnterCritical();

    //
    // Clear interrupt status
    //
    gbIntStatus = 0;

    //
    // Install IDE interrupt
    //
    nError = NutRegisterIrqHandler(&sig_INTERRUPT7, IDEInterrupt, NULL);
    if (nError == FALSE) {
        EICR |= IDE_INT_RISING_EDGE;
        sbi(EIMSK, IDE_IRQ);
    }

    switch (nIDEMode) {
    case IDE_HARDDISK:{
            break;
        }

        // 
        // If your drive does not work correct with 14.7456Mhz, 
        // try to devide the clock by 2.
        // 
        // Important: You must correct the baudrate in your app.
        // if you set 115200, you get only 57600.
        // 
    case IDE_HARDDISK_7MHZ:{
            XDIV = 0xff;
            break;
        }
    case IDE_COMPACT_FLASH:{
            break;
        }
    case MEM_8BIT_COMPACT_FLASH:{
            //
            // Now we will support to change the CF-Card in 
            // a running system, therefore we must install
            // a INT handler and some other parts :-)
            //

            //
            // Clear CF_INT_SENS_MASK.
            // 
            EICR &= ~CF_INT_SENS_MASK;

            //
            // Check the status of the compact flash.
            //
            bValue = (PINE & BV(CF_IRQ));
            if (bValue == CF_AVAILABLE) {
                gbCFMountStatus = CF_AVAILABLE;

                //
                // The rising edge between two samples 
                // of INTX generates an interrupt request.                            
                //
                EICR |= CF_INT_RISING_EDGE;
            } else {
                gbCFMountStatus = CF_NOT_AVAILABLE;

                //
                // The falling edge between two samples 
                // of INTX generates an interrupt request.                            
                //
                EICR |= CF_INT_FALLING_EDGE;
            }

            nError =
                NutRegisterIrqHandler(&sig_INTERRUPT6, CFInterrupt, NULL);
            if (nError == FALSE) {
                NutThreadCreate("cfchange", CFChange, NULL, 640);
                sbi(EIMSK, CF_IRQ);
            }

            break;
        }
    }
    NutExitCritical();

    HardwareReset(&sDrive[0]);

    nError = IDE_OK;

    return (nError);
}

/************************************************************/
/*  IDEMountDevice                                          */
/************************************************************/
int IDEMountDevice(BYTE bDevice, BYTE * pSectorBuffer)
{
    int nError;
    LPDRIVE pDrive = NULL;

    IDELock();

    if (bDevice > IDE_MAX_SUPPORTED_DEVICE) {
        nError = IDE_ERROR;
    } else {
        pDrive = &sDrive[bDevice];

        pDrive->wFlags = 0;

#if (IDE_SUPPORT_CHS == 1)
        //
        // CHS values
        //  
        pDrive->wCylinders = 0;
        pDrive->wHeads = 0;
        pDrive->wSectorsPerTrack = 0;
#endif

        //
        // LBA value
        //
        pDrive->dwTotalSectors = 0;

        nError = DeviceDiag(pDrive);
        if (nError != IDE_OK) {
            //
            // Try it again
            //
            nError = DeviceDiag(pDrive);
        }

        if (nError == IDE_OK) {
            nError = GetDeviceInfo(pDrive, pSectorBuffer);
            if (pDrive->wFlags & IDE_READY) {
                nError = IDE_OK;
            }
        }

    }

    IDEFree();

    return (nError);
}

/************************************************************/
/*  IDEUnMountDevice                                        */
/************************************************************/
int IDEUnMountDevice(BYTE bDevice)
{
    int nError = 0;
    LPDRIVE pDrive = NULL;

    IDELock();

    if (bDevice > IDE_MAX_SUPPORTED_DEVICE) {
        nError = IDE_ERROR;
    } else {
        pDrive = &sDrive[bDevice];

        pDrive->wFlags = 0;
    }

    IDEFree();

    return (nError);
}

/************************************************************/
/*  IDEGetTotalSectors                                      */
/************************************************************/
DWORD IDEGetTotalSectors(BYTE bDevice)
{
    DWORD dwTotalSectors = 0;
    LPDRIVE pDrive;

    IDELock();

    if (bDevice > IDE_MAX_SUPPORTED_DEVICE) {
        dwTotalSectors = 0;
    } else {
        pDrive = &sDrive[bDevice];
        dwTotalSectors = pDrive->dwTotalSectors;

        dwTotalSectors -= 64;
    }

    IDEFree();

    return (dwTotalSectors);
}

/************************************************************/
/*  IDEReadSectors                                          */
/************************************************************/
int IDEReadSectors(BYTE bDevice, void *pData,
                   DWORD dwStartSector, WORD wSectorCount)
{
    int nError = IDE_OK;
    WORD wReadCount;
    LPDRIVE pDrive;
    BYTE *pByte;

    IDELock();

    if (bDevice > IDE_MAX_SUPPORTED_DEVICE) {
        nError = IDE_DRIVE_NOT_FOUND;
    } else {
        pDrive = &sDrive[bDevice];
        if ((pDrive->wFlags & IDE_READY) == 0) {
            nError = IDE_DRIVE_NOT_FOUND;
        } else {
            if ((dwStartSector + wSectorCount) > pDrive->dwTotalSectors) {
                nError = IDE_PARAM_ERROR;
            }
        }
    }

    if (nError == IDE_OK) {
        pByte = (BYTE *) pData;
        while (wSectorCount > 0) {

            if (wSectorCount < 256) {
                wReadCount = wSectorCount;
            } else {
                wReadCount = 256;
            }

            nError =
                ReadSectors(bDevice, pByte, dwStartSector, wReadCount);
            if (nError != IDE_OK) {
                break;
            }

            dwStartSector += wReadCount;
            wSectorCount -= wReadCount;
            pByte += (wReadCount * IDE_SECTOR_SIZE);
        }
    }

    IDEFree();

    return (nError);
}

#if (IDE_SUPPORT_WRITE == 1)
/************************************************************/
/*  IDEWriteSectors                                         */
/************************************************************/
int IDEWriteSectors(BYTE bDevice, void *pData,
                    DWORD dwStartSector, WORD wSectorCount)
{
    int nError = IDE_OK;
    WORD wWriteCount;
    LPDRIVE pDrive = NULL;
    BYTE *pByte;

    IDELock();

    if (bDevice > IDE_MAX_SUPPORTED_DEVICE) {
        nError = IDE_DRIVE_NOT_FOUND;
    } else {
        pDrive = &sDrive[bDevice];
        if ((pDrive->wFlags & IDE_READY) == 0) {
            nError = IDE_DRIVE_NOT_FOUND;
        } else {
            if ((dwStartSector + wSectorCount) > pDrive->dwTotalSectors) {
                nError = IDE_PARAM_ERROR;
            }
        }
    }

    if (nError == IDE_OK) {
        pByte = (BYTE *) pData;
        while (wSectorCount > 0) {

            if (wSectorCount < 256) {
                wWriteCount = wSectorCount;
            } else {
                wWriteCount = 256;
            }

            nError =
                WriteSectors(bDevice, pByte, dwStartSector, wWriteCount);
            if (nError != IDE_OK) {
                break;
            }

            dwStartSector += wWriteCount;
            wSectorCount -= wWriteCount;
            pByte += (wWriteCount * IDE_SECTOR_SIZE);
        }
    }

    IDEFree();

    return (nError);
}
#endif
