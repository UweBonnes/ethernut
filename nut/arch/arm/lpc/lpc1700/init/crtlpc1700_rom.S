        .thumb
        .syntax unified

/*
 * Section 0: Vector table
 */
        .section .init0
        .global __vectors
__vectors:
        .word   __stack         /* Initial stack pointer value */
        .word   _start + 1          /* 1: Reset */
        .word   __nmi           /* 2: Non-maskable interrupt */
        .word   __hard_fault    /* 3: Hard fault */
        .word   __mpu_fault     /* 4: Memory protection fault */
        .word   __bus_fault     /* 5: Bus fault */
        .word   __usage_fault   /* 6: Usage fault */
        .word   0
        .word   0
        .word   0
        .word   0
        .word   __sv_call       /* 11: Supervisor call */
        .word   __debug_mon     /* 12: Debug monitor */
        .word   0
        .word   __pend_sv       /* 14: Supervisor request */
        .word   __sys_tick      /* 15: System timer zero */
        .word   __wdt_irq       /* 16: Watchdog */
        .word   __timer0_irq    /* 17: Timer 0 */
        .word   __timer1_irq    /* 18: Timer 1 */
        .word   __timer2_irq    /* 19: Timer 2 */
        .word   __timer3_irq    /* 20: Timer 3 */
        .word   __uart0_irq     /* 21: UART 0 */
        .word   __uart1_irq     /* 22: UART 1 */
        .word   __uart2_irq     /* 23: UART 2 */
        .word   __uart3_irq     /* 24: UART 3 */
        .word   __pwm1_irq      /* 25: PWM 1 */
        .word   __i2c0_irq      /* 26: I2C 0 */
        .word   __i2c1_irq      /* 27: I2C 1 */
        .word   __i2c2_irq      /* 28: I2C 2 */
        .word   __spi_irq       /* 29: SPI */
        .word   __ssp0_irq      /* 30: SSP 0 */
        .word   __ssp1_irq      /* 31: SSP 1 */
        .word   __pll0_irq      /* 32: PLL 0 */
        .word   __rtc_irq       /* 33: RTC */
        .word   __eint0         /* 34: External interrupt 0 */
        .word   __eint1         /* 35: External interrupt 1 */
        .word   __eint2         /* 36: External interrupt 2 */
        .word   __eint3         /* 37: External interrupt 3 */
        .word   __adc_irq       /* 38: A/D converter */
        .word   __bod_irq       /* 39: Brown-out detector */
        .word   __usb_irq       /* 40: USB */
        .word   __can_irq       /* 41: CAN */
        .word   __gpdma_irq     /* 42: General purpose DMA */
        .word   __i2s_irq       /* 43: I2S */
        .word   __ethernet_irq  /* 44: Ethernet */
        .word   __rit_irq       /* 45: Repetitive interrupt timer */
        .word   __pwm_irq       /* 46: Motor control PWM */
        .word   __qe_irq        /* 47: Quadrature Encoder */
        .word   __pll1_irq      /* 48: PLL 1 */
        .word   __usb_act_irq   /* 49: USB activity */
        .word   __can_act_irq   /* 50: CAN activity */

        .weak   __nmi
        .set    __nmi, __xcpt_dummy
        .weak   __hard_fault
        .set    __hard_fault, __xcpt_dummy
        .weak   __mpu_fault
        .set    __mpu_fault, __xcpt_dummy
        .weak   __bus_fault
        .set    __bus_fault, __xcpt_dummy
        .weak   __usage_fault
        .set    __usage_fault, __xcpt_dummy
        .weak   __sv_call
        .set    __sv_call, __xcpt_dummy
        .weak   __debug_mon
        .set    __debug_mon, __xcpt_dummy
        .weak   __pend_sv
        .set    __pend_sv, __xcpt_dummy
        .weak   __sys_tick
        .set    __sys_tick, __xcpt_dummy
        .weak   __wdt_irq
        .set    __wdt_irq, __xcpt_dummy
        .weak   __timer0_irq
        .set    __timer0_irq, __xcpt_dummy
        .weak   __timer1_irq
        .set    __timer1_irq, __xcpt_dummy
        .weak   __timer2_irq
        .set    __timer2_irq, __xcpt_dummy
        .weak   __timer3_irq
        .set    __timer3_irq, __xcpt_dummy
        .weak   __uart0_irq
        .set    __uart0_irq, __xcpt_dummy
        .weak   __uart1_irq
        .set    __uart1_irq, __xcpt_dummy
        .weak   __uart2_irq
        .set    __uart2_irq, __xcpt_dummy
        .weak   __uart3_irq
        .set    __uart3_irq, __xcpt_dummy
        .weak   __pwm1_irq
        .set    __pwm1_irq, __xcpt_dummy
        .weak   __i2c0_irq
        .set    __i2c0_irq, __xcpt_dummy
        .weak   __i2c1_irq
        .set    __i2c1_irq, __xcpt_dummy
        .weak   __i2c2_irq
        .set    __i2c2_irq, __xcpt_dummy
        .weak   __spi_irq
        .set    __spi_irq, __xcpt_dummy
        .weak   __ssp0_irq
        .set    __ssp0_irq, __xcpt_dummy
        .weak   __ssp1_irq
        .set    __ssp1_irq, __xcpt_dummy
        .weak   __pll0_irq
        .set    __pll0_irq, __xcpt_dummy
        .weak   __rtc_irq
        .set    __rtc_irq, __xcpt_dummy
        .weak   __eint0
        .set    __eint0, __xcpt_dummy
        .weak   __eint1
        .set    __eint1, __xcpt_dummy
        .weak   __eint2
        .set    __eint2, __xcpt_dummy
        .weak   __eint3
        .set    __eint3, __xcpt_dummy
        .weak   __adc_irq
        .set    __adc_irq, __xcpt_dummy
        .weak   __bod_irq
        .set    __bod_irq, __xcpt_dummy
        .weak   __usb_irq
        .set    __usb_irq, __xcpt_dummy
        .weak   __can_irq
        .set    __can_irq, __xcpt_dummy
        .weak   __gpdma_irq
        .set    __gpdma_irq, __xcpt_dummy
        .weak   __i2s_irq
        .set    __i2s_irq, __xcpt_dummy
        .weak   __ethernet_irq
        .set    __ethernet_irq, __xcpt_dummy
        .weak   __rit_irq
        .set    __rit_irq, __xcpt_dummy
        .weak   __pwm_irq
        .set    __pwm_irq, __xcpt_dummy
        .weak   __qe_irq
        .set    __qe_irq, __xcpt_dummy
        .weak   __pll1_irq
        .set    __pll1_irq, __xcpt_dummy
        .weak   __usb_act_irq
        .set    __usb_act_irq, __xcpt_dummy
        .weak   __can_act_irq
        .set    __can_act_irq, __xcpt_dummy

__xcpt_dummy:
        b       .


/*
 * Section 4: Clear bss and copy data.
 */
        .section .init4
@        .thumb_func
@        .globl  _start
@        .type   _start, %function
_start:
@        .fnstart
        /* Copy data section */
        ldr     r1, =__data_load_start
        ldr     r2, =__data_start
        ldr     r3, =__data_end
1:      cmp     r2, r3
        ldr	r0, [r1], #4
        str	r0, [r2], #4
        blo     1b

        /* Clear bss section  */
        mov     r0, #0
        ldr     r1, =__bss_start
        ldr     r2, =__bss_end
2:      cmp     r1, r2
        str	r0, [r1], #4
        blo     2b

        ldr     r0, =NutInit
        bx      r0

        .pool
@        .cantunwind
@        .fnend
@        .size   _start, . - _start

        .end
