
/*--------------------------*/
/* ARM Mode and Status Bits */
/*--------------------------*/

#define     ARM_MODE_USER       0x10
#define     ARM_MODE_FIQ        0x11
#define     ARM_MODE_IRQ        0x12
#define     ARM_MODE_SVC        0x13
#define     ARM_MODE_ABORT      0x17
#define     ARM_MODE_UNDEF      0x1B
#define     ARM_MODE_SYS        0x1F

#define     I_BIT               0x80
#define     F_BIT               0x40
#define     T_BIT               0x20



/* EBI base address. */
#define EBI_BASE    0xFFE00000

/* Chip select registers. */
#define EBI_CSR(i)  (EBI_BASE + i * 4)

/* Remap control registers. */
#define EBI_RCR     (EBI_BASE + 0x20)

/* Memory control registers. */
#define EBI_MCR     (EBI_BASE + 0x24)


/*--------------------------------*/
/* EBI_CSR: Chip Select Registers */
/*--------------------------------*/

#define EBI_DBW             0x3         /* Data Bus Width */
#define EBI_DBW_16          0x1         /* 16-bit Data Bus */
#define EBI_DBW_8           0x2         /* 8-bit Data Bus */

#define EBI_NWS             0x1C        /* Number of Wait States */
#define EBI_NWS_1           0x0         /* 1 wait state */
#define EBI_NWS_2           0x4         /* 2 wait state */
#define EBI_NWS_3           0x8         /* 3 wait state */
#define EBI_NWS_4           0xC         /* 4 wait state */
#define EBI_NWS_5           0x10        /* 5 wait state */
#define EBI_NWS_6           0x14        /* 6 wait state */
#define EBI_NWS_7           0x18        /* 7 wait state */
#define EBI_NWS_8           0x1C        /* 8 wait state */

#define EBI_WSE             0x20        /* Wait State Enable */

#define EBI_PAGES           0x180       /* Page size */
#define EBI_PAGES_1M        0x0         /* 1 Mbytes */
#define EBI_PAGES_4M        0x80        /* 4 Mbytes */
#define EBI_PAGES_16M       0x100       /* 16 Mbytes */
#define EBI_PAGES_64M       0x180       /* 64 Mbytes */

#define EBI_TDF             0xE00       /* Number of Data Float Output Time Clock Cycle */
#define EBI_TDF_0           0x0         /* 0 cycles added */
#define EBI_TDF_1           0x200       /* 1 cycles added */
#define EBI_TDF_2           0x400       /* 2 cycles added */
#define EBI_TDF_3           0x600       /* 3 cycles added */
#define EBI_TDF_4           0x800       /* 4 cycles added */
#define EBI_TDF_5           0xA00       /* 5 cycles added */
#define EBI_TDF_6           0xC00       /* 6 cycles added */
#define EBI_TDF_7           0xE00       /* 7 cycles added */

#define EBI_BAT             0x1000      /* Byte Access Type */
#define EBI_BAT_BYTE_WRITE  0x0         /* Byte Write Access Type */
#define EBI_BAT_BYTE_SELECT 0x1000      /* Byte Select Access Type */

#define EBI_CSEN            0x2000      /* Chip Select Enable */

#define EBI_BA              0xFFF00000

/*---------------------------------*/
/* EBI_RCR: Remap Control Register */
/*---------------------------------*/

#define EBI_RCB             0x1         /* Remap Command Bit */

/*----------------------------------*/
/* EBI_MCR: Memory Control Register */
/*----------------------------------*/

#define EBI_ALE             0x7         /* Address Line Enable */
#define EBI_ALE_16M         0x0         /* Bank Size 16Mbytes */
#define EBI_ALE_8M          0x4         /* Bank Size 16Mbytes */
#define EBI_ALE_4M          0x5         /* Bank Size 16Mbytes */
#define EBI_ALE_2M          0x6         /* Bank Size 16Mbytes */
#define EBI_ALE_1M          0x7         /* Bank Size 16Mbytes */

#define EBI_DRP             0x10        /* Data Read Protocol */
#define EBI_DRP_STANDARD    0x0         /* Standard Read Protocol */
#define EBI_DRP_EARLY       0x10        /* Early Read Protocol */


/* SF base address. */
#define SF_BASE     0xFFF00000

/* Chip ID registers. */
#define SF_CIDR     (SF_BASE)

/* Chip ID extension registers. */
#define SF_EXID     (SF_BASE + 0x04)

/* Reset status registers. */
#define SF_RSR      (SF_BASE + 0x08)

/* Memory mode registers. */
#define SF_MMR      (SF_BASE + 0x0C)

/* Protect mode registers. */
#define SF_PMR      (SF_BASE + 0x18)


/* USART1 base address. */
#define USART1_BASE 0xFFFCC000

/* Control register. */
#define US1_CR      (USART1_BASE)

/* Mode register. */
#define US1_MR      (USART1_BASE + 0x04)

/* Interrupt enable register. */
#define US1_IER     (USART1_BASE + 0x08)

/* Interrupt disable register. */
#define US1_IDR     (USART1_BASE + 0x0C)

/* Interrupt mask register. */
#define US1_IMR     (USART1_BASE + 0x10)

/* Channel status register. */
#define US1_CSR     (USART1_BASE + 0x14)

/* Receiver holding register. */
#define US1_RHR     (USART1_BASE + 0x18)

/* Transmitter holding register. */
#define US1_THR     (USART1_BASE + 0x1C)

/* Baud rate generator register. */
#define US1_BRGR    (USART1_BASE + 0x20)

/* Receiver timeout register. */
#define US1_RTOR    (USART1_BASE + 0x24)

/* Transmitter time guard register. */
#define US1_TTGR    (USART1_BASE + 0x28)

/* Receive pointer register. */
#define US1_RPR     (USART1_BASE + 0x30)

/* Receive counter register. */
#define US1_RCR     (USART1_BASE + 0x34)

/* Transmit pointer register. */
#define US1_TPR     (USART1_BASE + 0x38)

/* Transmit counter register. */
#define US1_TCR     (USART1_BASE + 0x3C)



/* USART0 base address. */
#define USART0_BASE 0xFFFD0000

/* Control register. */
#define US0_CR      (USART0_BASE)

/* Mode register. */
#define US0_MR      (USART0_BASE + 0x04)

/* Interrupt enable register. */
#define US0_IER     (USART0_BASE + 0x08)

/* Interrupt disable register. */
#define US0_IDR     (USART0_BASE + 0x0C)

/* Interrupt mask register. */
#define US0_IMR     (USART0_BASE + 0x10)

/* Channel status register. */
#define US0_CSR     (USART0_BASE + 0x14)

/* Receiver holding register. */
#define US0_RHR     (USART0_BASE + 0x18)

/* Transmitter holding register. */
#define US0_THR     (USART0_BASE + 0x1C)

/* Baud rate generator register. */
#define US0_BRGR    (USART0_BASE + 0x20)

/* Receiver timeout register. */
#define US0_RTOR    (USART0_BASE + 0x24)

/* Transmitter time guard register. */
#define US0_TTGR    (USART0_BASE + 0x28)

/* Receive pointer register. */
#define US0_RPR     (USART0_BASE + 0x30)

/* Receive counter register. */
#define US0_RCR     (USART0_BASE + 0x34)

/* Transmit pointer register. */
#define US0_TPR     (USART0_BASE + 0x38)

/* Transmit counter register. */
#define US0_TCR     (USART0_BASE + 0x3C)

/*--------------------------*/
/* US_CR : Control Register */
/*--------------------------*/

#define US_RSTRX                0x0004      /* Reset Receiver */
#define US_RSTTX                0x0008      /* Reset Transmitter */
#define US_RXEN                 0x0010      /* Receiver Enable */
#define US_RXDIS                0x0020      /* Receiver Disable */
#define US_TXEN                 0x0040      /* Transmitter Enable */
#define US_TXDIS                0x0080      /* Transmitter Disable */
#define US_RSTSTA               0x0100      /* Reset Status Bits */
#define US_STTBRK               0x0200      /* Start Break */
#define US_STPBRK               0x0400      /* Stop Break */
#define US_STTTO                0x0800      /* Start Time-out */
#define US_SENDA                0x1000      /* Send Address */

/*-----------------------*/
/* US_MR : Mode Register */
/*-----------------------*/

#define US_CLKS                 0x0030      /* Clock Selection */
#define US_CLKS_MCK             0x00        /* Master Clock */
#define US_CLKS_MCK8            0x10        /* Master Clock divided by 8 */
#define US_CLKS_SCK             0x20        /* External Clock */
#define US_CLKS_SLCK            0x30        /* Slow Clock */

#define US_CHRL                 0x00C0      /* Byte Length */
#define US_CHRL_5               0x00        /* 5 bits */
#define US_CHRL_6               0x40        /* 6 bits */
#define US_CHRL_7               0x80        /* 7 bits */
#define US_CHRL_8               0xC0        /* 8 bits */

#define US_SYNC                 0x0100      /* Synchronous Mode Enable */

#define US_PAR                  0x0E00      /* Parity Mode */
#define US_PAR_EVEN             0x00        /* Even Parity */
#define US_PAR_ODD              0x200       /* Odd Parity */
#define US_PAR_SPACE            0x400       /* Space Parity to 0 */
#define US_PAR_MARK             0x600       /* Marked Parity to 1 */
#define US_PAR_NO               0x800       /* No Parity */
#define US_PAR_MULTIDROP        0xC00       /* Multi-drop Mode */

#define US_NBSTOP               0x3000      /* Stop Bit Number */
#define US_NBSTOP_1             0x0000      /* 1 Stop Bit */
#define US_NBSTOP_1_5           0x1000      /* 1.5 Stop Bits */
#define US_NBSTOP_2             0x2000      /* 2 Stop Bits */

#define US_CHMODE                   0xC000  /* Channel Mode */
#define US_CHMODE_NORMAL            0x0000  /* Normal Mode */
#define US_CHMODE_AUTOMATIC_ECHO    0x4000  /* Automatic Echo */
#define US_CHMODE_LOCAL_LOOPBACK    0x8000  /* Local Loopback */
#define US_CHMODE_REMOTE_LOOPBACK   0xC000  /* Remote Loopback */

#define US_MODE9                0x20000     /* 9 Bit Mode */

#define US_CLKO                 0x40000     /* Baud Rate Output Enable */

/*---------------------------------------------------------------*/
/* US_IER, US_IDR, US_IMR, US_IMR: Status and Interrupt Register */
/*---------------------------------------------------------------*/

#define US_RXRDY            0x1       /* Receiver Ready */
#define US_TXRDY            0x2       /* Transmitter Ready */
#define US_RXBRK            0x4       /* Receiver Break */
#define US_ENDRX            0x8       /* End of Receiver PDC Transfer */
#define US_ENDTX            0x10       /* End of Transmitter PDC Transfer */
#define US_OVRE             0x20       /* Overrun Error */
#define US_FRAME            0x40       /* Framing Error */
#define US_PARE             0x80       /* Parity Error */
#define US_TIMEOUT          0x100       /* Receiver Timeout */
#define US_TXEMPTY          0x200       /* Transmitter Empty */

#define US_MASK_IRQ_TX      (US_TXRDY | US_ENDTX | US_TXEMPTY)
#define US_MASK_IRQ_RX      (US_RXRDY | US_ENDRX | US_TIMEOUT)
#define US_MASK_IRQ_ERROR   (US_PARE | US_FRAME | US_OVRE | US_RXBRK)


/* TC base address. */
#define TC_BASE     0xFFFE0000

/* Channel control register. */
#define TC0_CCR     (TC_BASE)

/* Channel mode register. */
#define TC0_CMR     (TC_BASE + 0x04)

/* Counter value. */
#define TC0_CV      (TC_BASE + 0x10)

/* Register A. */
#define TC0_RA      (TC_BASE + 0x14)

/* Register B. */
#define TC0_RB      (TC_BASE + 0x18)

/* Register C. */
#define TC0_RC      (TC_BASE + 0x1C)

/* Status register. */
#define TC0_SR      (TC_BASE + 0x20)

/* Interrupt enable register. */
#define TC0_IER     (TC_BASE + 0x24)

/* Interrupt disable register. */
#define TC0_IDR     (TC_BASE + 0x28)

/* Interrupt mask register. */
#define TC0_IMR     (TC_BASE + 0x2C)


/* Channel control register. */
#define TC1_CCR     (TC_BASE + 0x40)

/* Channel mode register. */
#define TC1_CMR     (TC_BASE + 0x44)

/* Counter value. */
#define TC1_CV      (TC_BASE + 0x50)

/* Register A. */
#define TC1_RA      (TC_BASE + 0x54)

/* Register B. */
#define TC1_RB      (TC_BASE + 0x58)

/* Register C. */
#define TC1_RC      (TC_BASE + 0x5C)

/* Status register. */
#define TC1_SR      (TC_BASE + 0x60)

/* Interrupt enable register. */
#define TC1_IER     (TC_BASE + 0x64)

/* Interrupt disable register. */
#define TC1_IDR     (TC_BASE + 0x68)

/* Interrupt mask register. */
#define TC1_IMR     (TC_BASE + 0x6C)


/* Channel control register. */
#define TC2_CCR     (TC_BASE + 0x80)

/* Channel mode register. */
#define TC2_CMR     (TC_BASE + 0x84)

/* Counter value. */
#define TC2_CV      (TC_BASE + 0x90)

/* Register A. */
#define TC2_RA      (TC_BASE + 0x94)

/* Register B. */
#define TC2_RB      (TC_BASE + 0x98)

/* Register C. */
#define TC2_RC      (TC_BASE + 0x9C)

/* Status register. */
#define TC2_SR      (TC_BASE + 0xA0)

/* Interrupt enable register. */
#define TC2_IER     (TC_BASE + 0xA4)

/* Interrupt disable register. */
#define TC2_IDR     (TC_BASE + 0xA8)

/* Interrupt mask register. */
#define TC2_IMR     (TC_BASE + 0xAC)

/* Block control register. */
#define TC_BCR      (TC_BASE + 0xC0)

/* Block mode register. */
#define TC_BMR      (TC_BASE + 0xC4)


/* PIO base address. */
#define PIO_BASE    0xFFFF0000

/* PIO enable register. */
#define PIO_PER     (PIO_BASE)

/* PIO disable register. */
#define PIO_PDR     (PIO_BASE + 0x04)

/* PIO status register. */
#define PIO_PSR     (PIO_BASE + 0x08)

/* Output enable register . */
#define PIO_OER     (PIO_BASE + 0x10)

/* Output disable register. */
#define PIO_ODR     (PIO_BASE + 0x14)

/* Output status register. */
#define PIO_OSR     (PIO_BASE + 0x18)

/* Input filter enable register */
#define PIO_IFER    (PIO_BASE + 0x20)

/* Input filter disable register */
#define PIO_IFDR    (PIO_BASE + 0x24)

/* Input filter status register. */
#define PIO_IFSR    (PIO_BASE + 0x28)

/* Set output data register. */
#define PIO_SODR    (PIO_BASE + 0x30)

/* Clear output data register. */
#define PIO_CODR    (PIO_BASE + 0x34)

/* Output data status register. */
#define PIO_ODSR    (PIO_BASE + 0x38)

/* Pin data status register. */
#define PIO_PDSR    (PIO_BASE + 0x3C)

/* Interrupt enable register. */
#define PIO_IER     (PIO_BASE + 0x40)

/* Interrupt disable register. */
#define PIO_IDR     (PIO_BASE + 0x44)

/* Interrupt mask register. */
#define PIO_IMR     (PIO_BASE + 0x48)

/* Interrupt status register. */
#define PIO_ISR     (PIO_BASE + 0x4C)



/* PS base address. */
#define PS_BASE     0xFFFF4000

/* Control register. */
#define PS_CR       (PS_BASE)

/* Peripheral clock enable register. */
#define PS_PCER     (PS_BASE + 0x04)

/* Peripheral clock disable register. */
#define PS_PCDR     (PS_BASE + 0x08)

/* Peripheral clock status register. */
#define PS_PCSR     (PS_BASE + 0x0C)



/* WD base address. */
#define WD_BASE     0xFFFF8000

/* Overflow mode register. */
#define WD_OMR      (WD_BASE + 0x00)

/* Clock mode register. */
#define WD_CMR      (WD_BASE + 0x04)

/* Control register. */
#define WD_CR       (WD_BASE + 0x08)

/* Status register. */
#define WD_SR       (WD_BASE + 0x0C)



/* AIC base address. */
#define AIC_BASE    0xFFFFF000

/* Source mode register. */
#define AIC_SMR(i)  (AIC_BASE + i * 4)

/* Source vector register. */
#define AIC_SVR(i)  (AIC_BASE + 0x80 + i * 4)

/* IRQ vector register. */
#define AIC_IVR     (AIC_BASE + 0x100)

/* FIQ vector register. */
#define AIC_FVR     (AIC_BASE + 0x104)

/* Interrupt status register. */
#define AIC_ISR     (AIC_BASE + 0x108)

/* Interrupt pending register. */
#define AIC_IPR     (AIC_BASE + 0x10C)

/* Interrupt mask register. */
#define AIC_IMR     (AIC_BASE + 0x110)

/* Core interrupt status register. */
#define AIC_CISR    (AIC_BASE + 0x114)

/* Interrupt enable command register. */
#define AIC_IECR    (AIC_BASE + 0x120)

/* Interrupt disable command register. */
#define AIC_IDCR    (AIC_BASE + 0x124)

/* Interrupt clear command register. */
#define AIC_ICCR    (AIC_BASE + 0x128)

/* Interrupt set command register. */
#define AIC_ISCR    (AIC_BASE + 0x12C)

/* End of interrupt command register. */
#define AIC_EOICR   (AIC_BASE + 0x130)

/* Spurious vector register. */
#define AIC_SPU     (AIC_BASE + 0x134)

/*--------------------------------------------*/
/* AIC_SMR[]: Interrupt Source Mode Registers */
/*--------------------------------------------*/

#define AIC_PRIOR                       0x07    /* Priority */

#define AIC_SRCTYPE                     0x60    /* Source Type Definition */

/* Internal Interrupts */
#define AIC_SRCTYPE_INT_LEVEL_SENSITIVE 0x00    /* Level Sensitive */
#define AIC_SRCTYPE_INT_EDGE_TRIGGERED  0x20    /* Edge Triggered */

/* External Interrupts */
#define AIC_SRCTYPE_EXT_LOW_LEVEL       0x00    /* Low Level */
#define AIC_SRCTYPE_EXT_NEGATIVE_EDGE   0x20    /* Negative Edge */
#define AIC_SRCTYPE_EXT_HIGH_LEVEL      0x40    /* High Level */
#define AIC_SRCTYPE_EXT_POSITIVE_EDGE   0x60    /* Positive Edge */

/*------------------------------------*/
/* AIC_ISR: Interrupt Status Register */
/*------------------------------------*/

#define AIC_IRQID                       0x1F    /* Current source interrupt */

/*------------------------------------------*/
/* AIC_CISR: Interrupt Core Status Register */
/*------------------------------------------*/

#define AIC_NFIQ                        0x01    /* Core FIQ Status */
#define AIC_NIRQ                        0x02    /* Core IRQ Status */


/*--------------------------------------------------------*/
/* TC_CCR: Timer Counter Control Register Bits Definition */
/*--------------------------------------------------------*/
#define TC_CLKEN            0x1
#define TC_CLKDIS           0x2
#define TC_SWTRG            0x4

/*---------------------------------------------------------------*/
/* TC_CMR: Timer Counter Channel Mode Register Bits Definition   */
/*---------------------------------------------------------------*/

/*-----------------*/
/* Clock Selection */
/*-----------------*/
#define TC_CLKS                  0x7
#define TC_CLKS_MCK2             0x0
#define TC_CLKS_MCK8             0x1
#define TC_CLKS_MCK32            0x2
#define TC_CLKS_MCK128           0x3
#define TC_CLKS_MCK1024          0x4

#define TC_CLKS_SLCK             0x4

#define TC_CLKS_XC0              0x5
#define TC_CLKS_XC1              0x6
#define TC_CLKS_XC2              0x7


/*-----------------*/
/* Clock Inversion */
/*-----------------*/
#define TC_CLKI             0x8

/*------------------------*/
/* Burst Signal Selection */
/*------------------------*/
#define TC_BURST            0x30
#define TC_BURST_NONE       0x0
#define TC_BUSRT_XC0        0x10
#define TC_BURST_XC1        0x20
#define TC_BURST_XC2        0x30

/*------------------------------------------------------*/
/* Capture Mode : Counter Clock Stopped with RB Loading */
/*------------------------------------------------------*/
#define TC_LDBSTOP          0x40

/*-------------------------------------------------------*/
/* Waveform Mode : Counter Clock Stopped with RC Compare */
/*-------------------------------------------------------*/
#define TC_CPCSTOP          0x40

/*-------------------------------------------------------*/
/* Capture Mode : Counter Clock Disabled with RB Loading */
/*--------------------------------------------------------*/
#define TC_LDBDIS           0x80

/*--------------------------------------------------------*/
/* Waveform Mode : Counter Clock Disabled with RC Compare */
/*--------------------------------------------------------*/
#define TC_CPCDIS           0x80

/*------------------------------------------------*/
/* Capture Mode : External Trigger Edge Selection */
/*------------------------------------------------*/
#define TC_ETRGEDG                  0x300
#define TC_ETRGEDG_EDGE_NONE        0x0
#define TC_ETRGEDG_RISING_EDGE      0x100
#define TC_ETRGEDG_FALLING_EDGE     0x200
#define TC_ETRGEDG_BOTH_EDGE        0x300

/*-----------------------------------------------*/
/* Waveform Mode : External Event Edge Selection */
/*-----------------------------------------------*/
#define TC_EEVTEDG                  0x300
#define TC_EEVTEDG_EDGE_NONE        0x0
#define TC_EEVTEDG_RISING_EDGE      0x100
#define TC_EEVTEDG_FALLING_EDGE     0x200
#define TC_EEVTEDG_BOTH_EDGE        0x300

/*--------------------------------------------------------*/
/* Capture Mode : TIOA or TIOB External Trigger Selection */
/*--------------------------------------------------------*/
#define TC_ABETRG                   0x400
#define TC_ABETRG_TIOB              0x0
#define TC_ABETRG_TIOA              0x400

/*------------------------------------------*/
/* Waveform Mode : External Event Selection */
/*------------------------------------------*/
#define TC_EEVT                     0xC00
#define TC_EEVT_TIOB                0x0
#define TC_EEVT_XC0                 0x400
#define TC_EEVT_XC1                 0x800
#define TC_EEVT_XC2                 0xC00

/*--------------------------------------------------*/
/* Waveform Mode : Enable Trigger on External Event */
/*--------------------------------------------------*/
#define TC_ENETRG                   0x1000

/*----------------------------------*/
/* RC Compare Enable Trigger Enable */
/*----------------------------------*/
#define TC_CPCTRG                   0x4000

/*----------------*/
/* Mode Selection */
/*----------------*/
#define TC_WAVE                     0x8000
#define TC_CAPT                     0x0

/*-------------------------------------*/
/* Capture Mode : RA Loading Selection */
/*-------------------------------------*/
#define TC_LDRA                     0x30000
#define TC_LDRA_EDGE_NONE           0x0
#define TC_LDRA_RISING_EDGE         0x10000
#define TC_LDRA_FALLING_EDGE        0x20000
#define TC_LDRA_BOTH_EDGE           0x30000

/*-------------------------------------------*/
/* Waveform Mode : RA Compare Effect on TIOA */
/*-------------------------------------------*/
#define TC_ACPA                     0x30000
#define TC_ACPA_OUTPUT_NONE         0x0
#define TC_ACPA_SET_OUTPUT          0x10000
#define TC_ACPA_CLEAR_OUTPUT        0x20000
#define TC_ACPA_TOGGLE_OUTPUT       0x30000

/*-------------------------------------*/
/* Capture Mode : RB Loading Selection */
/*-------------------------------------*/
#define TC_LDRB                     0xC0000
#define TC_LDRB_EDGE_NONE           0x0
#define TC_LDRB_RISING_EDGE         0x40000
#define TC_LDRB_FALLING_EDGE        0x80000
#define TC_LDRB_BOTH_EDGE           0xC0000

/*-------------------------------------------*/
/* Waveform Mode : RC Compare Effect on TIOA */
/*-------------------------------------------*/
#define TC_ACPC                     0xC0000
#define TC_ACPC_OUTPUT_NONE         0x0
#define TC_ACPC_SET_OUTPUT          0x40000
#define TC_ACPC_CLEAR_OUTPUT        0x80000
#define TC_ACPC_TOGGLE_OUTPUT       0xC0000

/*-----------------------------------------------*/
/* Waveform Mode : External Event Effect on TIOA */
/*-----------------------------------------------*/
#define TC_AEEVT                    0x300000
#define TC_AEEVT_OUTPUT_NONE        0x0
#define TC_AEEVT_SET_OUTPUT         0x100000
#define TC_AEEVT_CLEAR_OUTPUT       0x200000
#define TC_AEEVT_TOGGLE_OUTPUT      0x300000

/*-------------------------------------------------*/
/* Waveform Mode : Software Trigger Effect on TIOA */
/*-------------------------------------------------*/
#define TC_ASWTRG                   0xC00000
#define TC_ASWTRG_OUTPUT_NONE       0x0
#define TC_ASWTRG_SET_OUTPUT        0x400000
#define TC_ASWTRG_CLEAR_OUTPUT      0x800000
#define TC_ASWTRG_TOGGLE_OUTPUT     0xC00000

/*-------------------------------------------*/
/* Waveform Mode : RB Compare Effect on TIOB */
/*-------------------------------------------*/
#define TC_BCPB                     0x1000000
#define TC_BCPB_OUTPUT_NONE         0x0
#define TC_BCPB_SET_OUTPUT          0x1000000
#define TC_BCPB_CLEAR_OUTPUT        0x2000000
#define TC_BCPB_TOGGLE_OUTPUT       0x3000000

/*-------------------------------------------*/
/* Waveform Mode : RC Compare Effect on TIOB */
/*-------------------------------------------*/
#define TC_BCPC                     0xC000000
#define TC_BCPC_OUTPUT_NONE         0x0
#define TC_BCPC_SET_OUTPUT          0x4000000
#define TC_BCPC_CLEAR_OUTPUT        0x8000000
#define TC_BCPC_TOGGLE_OUTPUT       0xC000000

/*-----------------------------------------------*/
/* Waveform Mode : External Event Effect on TIOB */
/*-----------------------------------------------*/
#define TC_BEEVT                    0x30000000      //* bit 29-28
#define TC_BEEVT_OUTPUT_NONE        0x0
#define TC_BEEVT_SET_OUTPUT         0x10000000      //* bit 29-28  01
#define TC_BEEVT_CLEAR_OUTPUT       0x20000000      //* bit 29-28  10
#define TC_BEEVT_TOGGLE_OUTPUT      0x30000000      //* bit 29-28  11

/*- -----------------------------------------------*/
/* Waveform Mode : Software Trigger Effect on TIOB */
/*-------------------------------------------------*/
#define TC_BSWTRG                   0xC0000000
#define TC_BSWTRG_OUTPUT_NONE       0x0
#define TC_BSWTRG_SET_OUTPUT        0x40000000
#define TC_BSWTRG_CLEAR_OUTPUT      0x80000000
#define TC_BSWTRG_TOGGLE_OUTPUT     0xC0000000

/*------------------------------------------------------*/
/* TC_SR: Timer Counter Status Register Bits Definition */
/*------------------------------------------------------*/
#define TC_COVFS            0x1         /* Counter Overflow Status */
#define TC_LOVRS            0x2         /* Load Overrun Status */
#define TC_CPAS             0x4         /* RA Compare Status */
#define TC_CPBS             0x8         /* RB Compare Status */
#define TC_CPCS             0x10        /* RC Compare Status */
#define TC_LDRAS            0x20        /* RA Loading Status */
#define TC_LDRBS            0x40        /* RB Loading Status */
#define TC_ETRGS            0x80        /* External Trigger Status */
#define TC_CLKSTA           0x10000     /* Clock Status */
#define TC_MTIOA            0x20000     /* TIOA Mirror */
#define TC_MTIOB            0x40000     /* TIOB Status */

/*--------------------------------------------------------------*/
/* TC_BCR: Timer Counter Block Control Register Bits Definition */
/*--------------------------------------------------------------*/
#define TC_SYNC             0x1         /* Synchronisation Trigger */

/*------------------------------------------------------------*/
/*  TC_BMR: Timer Counter Block Mode Register Bits Definition */
/*------------------------------------------------------------*/
#define TC_TC0XC0S          0x3        /* External Clock Signal 0 Selection */
#define TC_TCLK0XC0         0x0
#define TC_NONEXC0          0x1
#define TC_TIOA1XC0         0x2
#define TC_TIOA2XC0         0x3

#define TC_TC1XC1S          0xC        /* External Clock Signal 1 Selection */
#define TC_TCLK1XC1         0x0
#define TC_NONEXC1          0x4
#define TC_TIOA0XC1         0x8
#define TC_TIOA2XC1         0xC

#define TC_TC2XC2S          0x30       /* External Clock Signal 2 Selection */
#define TC_TCLK2XC2         0x0
#define TC_NONEXC2          0x10
#define TC_TIOA0XC2         0x20
#define TC_TIOA1XC2         0x30

/*-------------------------------------*/
/* Peripheral Identifier and Interrupt */
/*-------------------------------------*/

/*
 * Interrupt sources.
 */
#define FIQ_ID          0       /* Fast Interrupt */
#define SWIRQ_ID        1       /* Soft Interrupt (generated by the AIC) */
#define US0_ID          2       /* USART Channel 0 interrupt */
#define US1_ID          3       /* USART Channel 1 interrupt */
#define TC0_ID          4       /* Timer Channel 0 interrupt */
#define TC1_ID          5       /* Timer Channel 1 interrupt */
#define TC2_ID          6       /* Timer Channel 2 interrupt */
#define WDI_ID          7       /* Watchdog interrupt */
#define PIO_ID          8       /* Parallel I/O Controller A interrupt */
#define IRQ0_ID         16      /* External interrupt 0 */
#define IRQ1_ID         17      /* External interrupt 1 */
#define IRQ2_ID         18      /* External interrupt 2 */


#define outb(_reg, _val)  (*((volatile unsigned char *)(_reg)) = (_val))
#define outw(_reg, _val)  (*((volatile unsigned short *)(_reg)) = (_val))
#define outr(_reg, _val)  (*((volatile unsigned int *)(_reg)) = (_val))

#define inb(_reg)   (*((volatile unsigned char *)(_reg)))
#define inw(_reg)   (*((volatile unsigned short *)(_reg)))
#define inr(_reg)   (*((volatile unsigned int *)(_reg)))

#define _BV(bit)    (1 << bit)

#define IRQ_ENTRY() \
    asm volatile("sub   lr, lr,#4"          "\n\t"  /* Adjust LR */ \
                 "stmfd sp!,{r0-r12,lr}"    "\n\t"  /* Save registers on IRQ stack. */ \
                 "mrs   r1, spsr"           "\n\t"  /* Save SPSR */ \
                 "stmfd sp!,{r1}"           "\n\t")

#define IRQ_EXIT() \
    asm volatile("ldmfd sp!, {r1}"          "\n\t"  /* Restore SPSR */ \
                 "msr   spsr_c, r1"         "\n\t"  /* */ \
                 "ldr   r0, =0xFFFFF000"    "\n\t"  /* End of interrupt. */ \
                 "str   r0, [r0, #0x130]"   "\n\t"  /* */ \
                 "ldmfd sp!, {r0-r12, pc}^" "\n\t") /* Restore registers and return. */
