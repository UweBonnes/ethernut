/*
 * Copyright (C) 2001-2003 by egnite Software GmbH. All rights reserved.
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
 * Revision 1.2  2004/03/16 16:48:45  haraldkipp
 * Added Jan Dubiec's H8/300 port.
 *
 * Revision 1.1.1.1  2003/05/09 14:41:52  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.8  2003/04/21 17:09:01  harald
 * *** empty log message ***
 *
 * Revision 1.7  2003/02/04 18:15:57  harald
 * Version 3 released
 *
 * Revision 1.6  2002/06/26 17:29:44  harald
 * First pre-release with 2.4 stack
 *
 */

#include <sys/thread.h>
#include <sys/timer.h>
#include <sys/event.h>
#include <sys/heap.h>
#include <sys/atom.h>

#include <sys/osdebug.h>

#if defined(__arm__) || defined(__m68k__) || defined(__H8300H__) || defined(__H8300S__)
#define ARCH_32BIT
#endif

FILE *__os_trs;
u_char __os_trf;

FILE *__heap_trs;
u_char __heap_trf;

static char *states[] = { "TRM", "RUN", "RDY", "SLP" };

#ifdef ARCH_32BIT
/*                              12345678 12345678 1234 123 12345678 12345678 12345678 1234556789 */
static prog_char qheader[] = "\nHandle   Name     Prio Sta Queue    Timer    StackPtr FreeMem\n";
#else
/*                              1234 12345678 1234 123 1234 1234 1234 12345 */
static prog_char qheader[] = "\nHndl Name     Prio Sta QUE  Timr StkP FreeMem\n";
#endif

/*!
 * \brief Dump system queue contents.
 *
 * \param stream Pointer to a previously opened stream associated to
 *               a debug device.
 * \param tdp    Pointer to the queue.
 *
 * \warning Interrupts are disabled inside this funtion.
 */
void NutDumpThreadQueue(FILE * stream, NUTTHREADINFO * tdp)
{
#ifdef ARCH_32BIT
    static prog_char fmt[] = "%08lX %-8s %4u %s %08lX %08lX %08lX %9lu %s\n";
#else
    static char fmt[] = "%04X %-8s %4u %s %04X %04X %04X %5u %s\n";
#endif

    fputs_P(qheader, stream);

    NutEnterCritical();
    if (tdp == SIGNALED)
        fputs("SIGNALED\n", stream);
    else {
        while (tdp) {
            fprintf(stream, fmt, (uptr_t) tdp, tdp->td_name, tdp->td_priority,
                    states[tdp->td_state], (uptr_t) tdp->td_queue,
                    (uptr_t) tdp->td_timer, tdp->td_sp,
                    (uptr_t) tdp->td_sp - (uptr_t) tdp->td_memory,
                    *((u_long *) tdp->td_memory) != DEADBEEF
                    && *((u_long *) (tdp->td_memory + 4)) != DEADBEEF
                    && *((u_long *) (tdp->td_memory + 8)) != DEADBEEF
                    && *((u_long *) (tdp->td_memory + 12)) !=
                    DEADBEEF ? "FAIL" : "OK");
            tdp = tdp->td_qnxt;
        }
    }
    NutExitCritical();
}

/*!
 * \brief Dump system thread list.
 *
 * \param stream Pointer to a previously opened stream associated to
 *               a debug device.
 *
 * \warning Interrupts are disabled inside this funtion.
 */
void NutDumpThreadList(FILE * stream)
{
#ifdef ARCH_32BIT
    static prog_char fmt1[] = "%08lX %-8s %4u %s %08lX %08lX %08lX %9lu %s";
    static prog_char fmt2[] = " %08lX";
#else
    static char fmt1[] = "%04X %-8s %4u %s %04X %04X %04X %5u %s";
    static char fmt2[] = " %04X";
#endif
    NUTTHREADINFO *tqp;
    NUTTHREADINFO *tdp;

    fputs_P(qheader, stream);

    NutEnterCritical();
    tdp = nutThreadList;
    while (tdp) {
        fprintf(stream, fmt1, (uptr_t) tdp, tdp->td_name, tdp->td_priority,
                states[tdp->td_state], (uptr_t) tdp->td_queue,
                (uptr_t) tdp->td_timer, tdp->td_sp,
                (uptr_t) tdp->td_sp - (uptr_t) tdp->td_memory,
                *((u_long *) tdp->td_memory) != DEADBEEF ? "FAIL" : "OK");
        if (tdp->td_queue) {
            tqp = *(NUTTHREADINFO **) (tdp->td_queue);
            if (tqp == SIGNALED)
                fputs("SIGNALED", stream);
            else {
                while (tqp) {
                    fprintf(stream, fmt2, (uptr_t) tqp);
                    tqp = tqp->td_qnxt;
                }
            }
        }
        fputc('\n', stream);
        tdp = tdp->td_next;
    }
    NutExitCritical();
}

/*!
 * \brief Dump system timer list.
 *
 * \param stream Pointer to a previously opened stream associated to
 *               a debug device.
 *
 * \warning Interrupts are disabled inside this funtion.
 */
void NutDumpTimerList(FILE * stream)
{
    static prog_char wname[] = "NutThreadWake";
    static prog_char tname[] = "NutEventTimeout";
#ifdef ARCH_32BIT
    static prog_char theader[] = "Address  Ticks  Left Callback\n";
    static prog_char fmt1[] = "%08lX%6lu%6lu ";
    static prog_char fmt2[] = "%09lX";
    static prog_char fmt3[] = "(%08lX)\n";
    static prog_char fmt4[] = " %08lX";
#else
    static prog_char theader[] = "Addr Ticks  Left Callback\n";
    static char fmt1[] = "%04X%6lu%6lu ";
    static char fmt2[] = "%05lX";
    static char fmt3[] = "(%04X)\n";
    static char fmt4[] = " %04X";
#endif
    NUTTIMERINFO *tnp;

    NutEnterCritical();
    if ((tnp = nutTimerList) != 0) {
        fputs_P(theader, stream);
        while (tnp) {
            fprintf(stream, fmt1, (uptr_t) tnp, tnp->tn_ticks,
                    tnp->tn_ticks_left);
            if (tnp->tn_callback == NutThreadWake)
                fputs_P(wname, stream);
            else if (tnp->tn_callback == NutEventTimeout)
                fputs_P(tname, stream);
            else
                fprintf(stream, fmt2,
                        (u_long) ((uptr_t) tnp->tn_callback) << 1);
            fprintf(stream, fmt3, (uptr_t) tnp->tn_arg);
            tnp = tnp->tn_next;
        }
    }
    if ((tnp = nutTimerPool) != 0) {
        fputs("Pool:", stream);
        while (tnp) {
            fprintf(stream, fmt4, (uptr_t) tnp);
            tnp = tnp->tn_next;
        }
        fputc('\n', stream);
    }
    NutExitCritical();
}

/*!
 * \brief Control OS tracing.
 *
 * \param stream Pointer to a previously opened stream associated to
 *               a debug device or null to disable trace output.
 * \param flags  Flags to enable specific traces.
 */
void NutTraceOs(FILE * stream, u_char flags)
{
    if (stream)
        __os_trs = stream;
    if (__os_trs)
        __os_trf = flags;
    else
        __os_trf = 0;
}

/*!
 * \brief Dump free node list of heap memory.
 *
 * \param stream Pointer to a previously opened stream associated to
 *               a debug device.
 */
void NutDumpHeap(FILE * stream)
{
#ifdef ARCH_32BIT
    static prog_char fmt1[] = "%08lx %9ld\n";
    static prog_char fmt2[] = "%lu counted, but %lu reported\n";
    static prog_char fmt3[] = "%lu bytes free\n";
#else
    static char fmt1[] = "%04x %5d\n";
    static char fmt2[] = "%u counted, but %u reported\n";
    static char fmt3[] = "%u bytes free\n";
#endif
    HEAPNODE *node;
    size_t sum = 0;
    size_t avail;

    fputc('\n', stream);
    for (node = heapFreeList; node; node = node->hn_next) {
        sum += node->hn_size;
        fprintf(stream, fmt1, (uptr_t) node, node->hn_size);
        /* TODO: Remove hardcoded RAMSTART and RAMEND */
        if ((uptr_t) node < 0x60 || (uptr_t) node > 0x7fff)
            break;
    }
    if ((avail = NutHeapAvailable()) != sum)
        fprintf(stream, fmt2, sum, avail);
    else
        fprintf(stream, fmt3, avail);
}

/*!
 * \brief Control dynamic memory tracing.
 *
 * \param stream Pointer to a previously opened stream or null to 
 *               disable trace output.
 * \param flags  Flags to enable specific traces.
 */
void NutTraceHeap(FILE * stream, u_char flags)
{
    if (stream)
        __heap_trs = stream;
    if (__heap_trs)
        __heap_trf = flags;
    else
        __heap_trf = 0;
}
