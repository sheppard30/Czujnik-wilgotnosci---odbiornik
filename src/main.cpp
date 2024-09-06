#include "Uart.h"

#include <util/delay.h>

int main()
{
    Uart uart;
    uart.init(MYUBRR);

    while (1)
    {
        _delay_ms(1000);
        uart.print("beel");
    }
}