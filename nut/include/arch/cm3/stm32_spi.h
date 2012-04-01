#ifndef _STM32_SPI_H_
#define _STM32_SPI_H_

static int Stm32SpiSetup(NUTSPINODE * node);
static void Stm32SpiEnable(SPI_TypeDef* SPIx);
static void Stm32SpiDisable(SPI_TypeDef* SPIx);
static void Stm32SpiHardwareConfig(SPI_TypeDef* SPIx);
static int Stm32SpiBusNodeInit(NUTSPINODE * node);
static int Stm32SpiBusTransfer(NUTSPINODE * node, CONST void *txbuf, void *rxbuf, int xlen);

#endif
