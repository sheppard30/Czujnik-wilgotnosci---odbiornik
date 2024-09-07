#include "Uart.h"

#include <util/delay.h>

#include "Timer.h"
#include "Receiver.h"

Receiver receiver;

ISR(TIMER0_COMPA_vect)
{
    receiver.onTimerInterrupt();
}

int main()
{
    Uart::init(MYUBRR);
    Timer::init();

    while (1)
    {
    }
}