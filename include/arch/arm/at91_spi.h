#ifndef _ARCH_ARM_AT91_SPI_H_
#define _ARCH_ARM_AT91_SPI_H_

/*
 * Copyright (C)
 */

/*!
 * \file arch/arm/at91_spi.h
 * \brief AT91 peripherals.
 *
 * \verbatim
 *
 * $Log$
 * Revision 1.1  2006/07/21 09:03:56  haraldkipp
 * Added SPI support, kindly contributed by Andras Albert.
 *
 * Revision 1.1
 *
 *
 * \endverbatim
 */

/*!
 * \addtogroup xgNutArchArmAt91Spi
 */
/*@{*/

#if defined(SPI_BASE)
#define	SPI_CR		(SPI_BASE + 0x00)  /*!< \brief SPI Control Register Write-only. */
#define SPI_MR		(SPI_BASE + 0x04)  /*!< \brief SPI Mode Register Read/Write Reset=0x0. */
#define SPI_RDR		(SPI_BASE + 0x08)  /*!< \brief SPI Receive Data Register Read-only Reset=0x0. */
#define SPI_TDR		(SPI_BASE + 0x0C)  /*!< \brief SPI Transmit Data Register Write-only . */
#define SPI_SR		(SPI_BASE + 0x10)  /*!< \brief SPI Status Register Read-only Reset=0x000000F0. */
#define SPI_IER		(SPI_BASE + 0x14)  /*!< \brief SPI Interrupt Enable Register Write-only. */
#define SPI_IDR		(SPI_BASE + 0x18)  /*!< \brief SPI Interrupt Disable Register Write-only. */
#define SPI_IMR		(SPI_BASE + 0x1C)  /*!< \brief SPI Interrupt Mask Register Read-only Reset=0x0. */
#define SPI_CSR0	(SPI_BASE + 0x30)  /*!< \brief SPI Chip Select Register 0 Read/Write Reset=0x0. */
#define SPI_CSR1	(SPI_BASE + 0x34)  /*!< \brief SPI Chip Select Register 1 Read/Write Reset=0x0. */
#define SPI_CSR2	(SPI_BASE + 0x38)  /*!< \brief SPI Chip Select Register 2 Read/Write Reset=0x0. */
#define SPI_CSR3	(SPI_BASE + 0x3C)  /*!< \brief SPI Chip Select Register 3 Read/Write Reset=0x0. */
#endif


#if defined(SPI0_BASE)
#define	SPI0_CR		(SPI0_BASE + 0x00)  /*!< \brief SPI Control Register Write-only. */
#define SPI0_MR		(SPI0_BASE + 0x04)  /*!< \brief SPI Mode Register Read/Write Reset=0x0. */
#define SPI0_RDR	(SPI0_BASE + 0x08)  /*!< \brief SPI Receive Data Register Read-only Reset=0x0. */
#define SPI0_TDR	(SPI0_BASE + 0x0C)  /*!< \brief SPI Transmit Data Register Write-only . */
#define SPI0_SR		(SPI0_BASE + 0x10)  /*!< \brief SPI Status Register Read-only Reset=0x000000F0. */
#define SPI0_IER	(SPI0_BASE + 0x14)  /*!< \brief SPI Interrupt Enable Register Write-only. */
#define SPI0_IDR	(SPI0_BASE + 0x18)  /*!< \brief SPI Interrupt Disable Register Write-only. */
#define SPI0_IMR	(SPI0_BASE + 0x1C)  /*!< \brief SPI Interrupt Mask Register Read-only Reset=0x0. */
#define SPI0_CSR0	(SPI0_BASE + 0x30)  /*!< \brief SPI Chip Select Register 0 Read/Write Reset=0x0. */
#define SPI0_CSR1	(SPI0_BASE + 0x34)  /*!< \brief SPI Chip Select Register 1 Read/Write Reset=0x0. */
#define SPI0_CSR2	(SPI0_BASE + 0x38)  /*!< \brief SPI Chip Select Register 2 Read/Write Reset=0x0. */
#define SPI0_CSR3	(SPI0_BASE + 0x3C)  /*!< \brief SPI Chip Select Register 3 Read/Write Reset=0x0. */
#endif

#if defined(SPI1_BASE)
#define	SPI1_CR		(SPI1_BASE + 0x00)  /*!< \brief SPI Control Register Write-only. */
#define SPI1_MR		(SPI1_BASE + 0x04)  /*!< \brief SPI Mode Register Read/Write Reset=0x0. */
#define SPI1_RDR	(SPI1_BASE + 0x08)  /*!< \brief SPI Receive Data Register Read-only Reset=0x0. */
#define SPI1_TDR	(SPI1_BASE + 0x0C)  /*!< \brief SPI Transmit Data Register Write-only . */
#define SPI1_SR		(SPI1_BASE + 0x10)  /*!< \brief SPI Status Register Read-only Reset=0x000000F0. */
#define SPI1_IER	(SPI1_BASE + 0x14)  /*!< \brief SPI Interrupt Enable Register Write-only. */
#define SPI1_IDR	(SPI1_BASE + 0x18)  /*!< \brief SPI Interrupt Disable Register Write-only. */
#define SPI1_IMR	(SPI1_BASE + 0x1C)  /*!< \brief SPI Interrupt Mask Register Read-only Reset=0x0. */
#define SPI1_CSR0	(SPI1_BASE + 0x30)  /*!< \brief SPI Chip Select Register 0 Read/Write Reset=0x0. */
#define SPI1_CSR1	(SPI1_BASE + 0x34)  /*!< \brief SPI Chip Select Register 1 Read/Write Reset=0x0. */
#define SPI1_CSR2	(SPI1_BASE + 0x38)  /*!< \brief SPI Chip Select Register 2 Read/Write Reset=0x0. */
#define SPI1_CSR3	(SPI1_BASE + 0x3C)  /*!< \brief SPI Chip Select Register 3 Read/Write Reset=0x0. */
#endif

/*@} xgNutArchArmAt91Pio */

#endif                          /* _ARCH_ARM_AT91_SPI_H_ */
