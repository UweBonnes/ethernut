#include <sys/atom.h>
#include <dev/irqreg.h>

void CallHandler(IRQ_HANDLER * irh)
{
    irh->ir_count++;
    if (irh->ir_handler)
        (irh->ir_handler) (irh->ir_arg);
}

/*!
 * \brief Register an interrupt handler.
 *
 * This function is typically called by device drivers.
 *
 * \param irq     Interrupt number to be associated with this handler.
 * \param handler This routine will be called by Nut/OS, when the
 *                specified interrupt occurs.
 * \param arg     Argument to be passed to the interrupt handler.
 *
 * \return 0 on success, -1 otherwise.
 */
int NutRegisterIrqHandler(IRQ_HANDLER * irq, void (*handler) (void *),
                          void *arg)
{
    NutEnterCritical();

    irq->ir_arg = arg;
    irq->ir_handler = handler;

    NutExitCritical();

    return 0;
}
