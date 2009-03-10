// Trivial C++ Demo for NutOS.

/*!
 * $Log: cppdemo.cc,v $
 * Revision 1.4  2007/04/12 09:15:39  haraldkipp
 * Now compiles on avr-gcc 4.1.1.
 *
 * Revision 1.3  2006/07/10 14:24:11  haraldkipp
 * Header files replaced by platform independent variants.
 * Contributed by Matthias Wilde.
 *
 * Revision 1.2  2005/08/02 17:46:44  haraldkipp
 * Major API documentation update.
 *
 */

/*!
 * \example cppdemo/cppdemo.cc
 *
 * This sample demonstrates the usage of Nut/OS with C++.
 *
 * You should carefully think about using C++ with tiny embedded systems.
 * This sample just proofs, that it basically works.
 */

#include <cpp/nutcpp.h>

extern "C" {
#include <dev/board.h>
#include <sys/version.h>
#include <inttypes.h>
#include <io.h>
#include <stdio.h>
}



template<class tp_type> class TemplateCounter
{
protected:
    tp_type m_value;

public:
    tp_type value() const { return m_value; }
    void inc() { m_value++; }
    void dec() { m_value--; }
    void set(const tp_type &newValue) { m_value = newValue; }
};



class Counter: public TemplateCounter<uint8_t>
{
public:
    void print(FILE *stream);

    Counter(uint8_t initValue=10);
};


void Counter::print(FILE* stream)
{
    fprintf(stream, "\nCounter value = %i\n", value());
}


Counter::Counter(uint8_t initValue)
{
    m_value = initValue;
}



int main(void) {
    u_long baud = 115200;

    NutRegisterDevice(&DEV_UART0, 0, 0);
    FILE *stream = fopen(DEV_UART0_NAME, "r+");
    _ioctl(_fileno(stream), UART_SETSPEED, &baud);

    fprintf(stream, "\n\nC++ Demo on Nut/OS %s ready.\n", NutVersionString());

    Counter counter;
    counter.print(stream);

    for (;;) {
        char c;
        fread(&c, sizeof(c), 1, stream);

        switch (c) {
        case '+':
            counter.inc();
            counter.print(stream);
            break;
        case '-':
            counter.dec();
            counter.print(stream);
            break;
        case 'r':
            counter.set(0);
            counter.print(stream);
            break;
        default:
            fprintf(stream, "Unknown command.\n");
        }
    }
    return 0;
}


