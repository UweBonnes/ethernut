// Trivial C++ Demo for NutOS.

#include <dev/uartavr.h>
#include <sys/version.h>
#include <inttypes.h>

#include <cpp/nutcpp.h>

extern "C" {
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

    NutRegisterDevice(&devUart0, 0, 0);
    FILE *stream = fopen("uart0", "r+");
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
}
