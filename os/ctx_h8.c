/*
 * Copyright (C) 2001-2004 by egnite Software GmbH. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor the names of
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY EGNITE SOFTWARE GMBH AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL EGNITE
 * SOFTWARE GMBH OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * For additional information see http://www.ethernut.de/
 *
 */

/*
 * $Log$
 * Revision 1.1  2004/02/01 18:49:48  haraldkipp
 * Added CPU family support
 *
 */

/*!
 * \brief H8/300 GCC context switch frame layout.
 *
 * This is the layout of the stack after a thread's context has been 
 * switched-out.
 */
typedef struct {
    u_long  csf_er0;
    u_long  csf_er1;
    u_long  csf_er2;
    u_long  csf_er3;
    u_long  csf_er4;
    u_long  csf_er5;
    u_long  csf_er6;
    u_short csf_ccr;
    u_long  csf_pc;
} SWITCHFRAME;

/*!
 * \brief H8/300 GCC thread entry frame layout.
 *
 * This is the stack layout being build to enter a new thread.
 */
typedef struct {
    u_long cef_er0;
    u_long cef_er1;
    u_long cef_er2;
    u_long cef_er3;
    u_long cef_er4;
    u_long cef_er5;
    u_long cef_er6;
    u_long cef_ccr_pc;
} ENTERFRAME;


/*
* Get the current Stack Pointer
*/
static u_long SysGetCurSP(void)
{
     __asm__ __volatile__("MOV.L   ER7, ER0");
}

static OS_STK* g_ulSysStackPointer;

static void SysSetSP(void)
{
     __asm__ __volatile__("MOV.L   @__$g_ulSysStackPointer:24, ER7;
}


static void NutThreadEntry(void)
{
     __asm__ __volatile__("POP.L       ER0");
     __asm__ __volatile__("POP.L       ER1");
     __asm__ __volatile__("POP.L       ER2");
     __asm__ __volatile__("POP.L       ER3");
     __asm__ __volatile__("POP.L       ER4");
     __asm__ __volatile__("POP.L       ER5");
     __asm__ __volatile__("POP.L       ER6"); /* Restore task registers */
     __asm__ __volatile__("RTE");             /* ccr<-@sp+, pc-<-@sp+ */
}

/*!
 * \brief Switch to another thread.
 *
 * Stop the current thread, saving its context. Then start the
 * one with the highest priority, which is ready to run.
 *
 * Application programs typically do not call this function.
 *
 * \note CPU interrupts must be disabled before calling this function.
 *
 */
static inline void SysPushAll(void) __attribute__ ((naked));
{
        __asm__ __volatile__("STC.W       CCR, @-ER7");
        __asm__ __volatile__("PUSH.L      ER6");
        __asm__ __volatile__("PUSH.L      ER5");
        __asm__ __volatile__("PUSH.L      ER4");
        __asm__ __volatile__("PUSH.L      ER3");
        __asm__ __volatile__("PUSH.L      ER2");
        __asm__ __volatile__("PUSH.L      ER1");
        __asm__ __volatile__("PUSH.L      ER0");
}

static inline void SysPopAll(void) __attribute__ ((naked));
{
        __asm__ __volatile__("POP.L       ER0");
        __asm__ __volatile__("POP.L       ER1");
        __asm__ __volatile__("POP.L       ER2");
        __asm__ __volatile__("POP.L       ER3");
        __asm__ __volatile__("POP.L       ER4");
        __asm__ __volatile__("POP.L       ER5");
        __asm__ __volatile__("POP.L       ER6");
        __asm__ __volatile__("LDC.W       @ER7+, CCR");
}

//#pragma     noregsave     (NutThreadSwitch)
void NutThreadSwitch(void) __attribute__ ((naked));
{
    SysPushAll();

    runningThread->td_sp = (OS_STK*) SysGetCurSP();
    runningThread = runQueue;
    runningThread->td_state = TDS_RUNNING;

    g_ulSysStackPointer = runningThread->td_sp;
    SysSetSP();

    SysPopAll();
}

//#pragma     noregsave     (NutThreadSwith_Jmp)
void NutThreadSwith_Jmp(void)  __attribute__ ((naked));
{
    /*
     * Reload CPU registers from the thread in front
     * of the run queue.
     */
    runningThread = runQueue;
    runningThread->td_state = TDS_RUNNING;

    g_ulSysStackPointer = runningThread->td_sp;
    SysSetSP();

    SysPopAll();

}

/*!
 * \brief Create a new thread.
 *
 * If the current thread's priority is lower or equal than the default
 * priority (64), then the current thread is stopped and the new one
 * is started.
 *
 * \param name      String containing the symbolic name of the new thread,
 *                  up to 8 characters long.
 * \param fn        The thread's entry point, typically created by the
 *                  THREAD macro.
 * \param arg       Argument pointer passed to the new thread.
 * \param stackSize Number of bytes of the stack space allocated for
 *                  the new thread.
 *
 * \return Pointer to the NUTTHREADINFO structure or 0 to indicate an
 *         error.
 */
HANDLE NutThreadCreate(char * name, void (*fn) (void *), void *arg, SIZE stackSize)
{
    u_char          * threadMem;
    SWITCHFRAME     * sf;
    ENTERFRAME      * ef;
    NUTTHREADINFO   * td;
    u_short         yreg;
    OS_CPU_SR       cpu_sr;

    NutEnterCritical();
    //paddr = (u_long) fn;       // *KU* fn doesn't contain the entry address of the thread!
    /*
     * Allocate stack and thread info structure in one block.
     */

    /*
     * align memory boundary
     */
    if (0 != (stackSize % MEM_ALIGNMENT))
    {
        stackSize += MEM_ALIGNMENT;
        stackSize &= ~((SIZE)MEM_ALIGNMENT-1);
    }

    if ((threadMem = NutHeapAlloc(stackSize + sizeof(NUTTHREADINFO))) == 0)
        return 0;

    td = (NUTTHREADINFO *) ((PTRINT)threadMem + stackSize);
    ef = (ENTERFRAME *)    ((PTRINT)td - sizeof(ENTERFRAME));
    sf = (SWITCHFRAME *)   ((PTRINT)ef - sizeof(SWITCHFRAME));


    memcpy(td->td_name, name, sizeof(td->td_name) - 1);
    td->td_sp = (OS_STK*) sf;
    td->td_memory = threadMem;

    *((u_long *) threadMem)         = DEADBEEF;
    *((u_long *) (threadMem + 4))   = DEADBEEF;
    *((u_long *) (threadMem + 8))   = DEADBEEF;
    *((u_long *) (threadMem + 12))  = DEADBEEF;
    td->td_priority = 64;

    /*
     * Setup entry frame to simulate C function entry.
     */

    ef->cef_ccr_pc  = (u_long)fn;
    ef->cef_er6     = 0x66666666;
    ef->cef_er5     = 0x55555555;
    ef->cef_er4     = 0x44444444;
    ef->cef_er3     = 0x33333333;
    ef->cef_er2     = 0x22222222;
    ef->cef_er1     = 0x11111111;
    ef->cef_er0     = (u_long)arg;

    sf->csf_pc  = (PTRINT) NutThreadEntry;

    /*
     * Insert into the thread list and the run queue.
     */
//    NutEnterCritical();
    td->td_next = nutThreadList;
    nutThreadList = td;
    td->td_state = TDS_READY;
    td->td_timer = 0;
    td->td_queue = 0;
#ifdef NUTDEBUG
    if (__os_trf)
        fprintf(__os_trs, "Cre<%04x>", (u_int) td);
#endif
    NutThreadAddPriQueue(td, (NUTTHREADINFO **) & runQueue);
#ifdef NUTDEBUG
    if (__os_trf)
        NutDumpThreadList(__os_trs);
    //NutDumpThreadQueue(__os_trs, runQueue);
#endif

    /*
     * If no thread is active, switch to new thread.
     */
    if (runningThread == 0)
    {
        NutThreadSwith_Jmp();
    }


    /*
     * If current context is not in front of
     * the run queue (highest priority), then
     * switch to the thread in front.
     */
    if (runningThread != runQueue) {
        runningThread->td_state = TDS_READY;
#ifdef NUTDEBUG
        if (__os_trf)
            fprintf(__os_trs, "New<%04x %04x>", (u_int) runningThread,
                    (u_int) runQueue);
#endif
        NutThreadSwitch();
    }
    NutExitCritical();

    return td;
}
