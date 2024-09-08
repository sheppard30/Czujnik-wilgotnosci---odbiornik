#include "Uart.h"

#include <util/delay.h>

#include "Timer.h"
#include "Receiver.h"
#include "Debug.h"
#include "Uart.h"

Receiver receiver;

ISR(TIMER0_COMPA_vect)
{
    receiver.onTimerInterrupt();
}

int main()
{
    Uart::init(MYUBRR);
    Timer::init();

    DDRD |= (1 << PD5);

    while (1)
    {
        if (receiver.isDataAvailable())
        {
            Uart::print(receiver.getData());
            receiver.resetData();
        }

        if (debugReady && !debugPrinted)
        {
            debugPrinted = true; // Reset flagi
            // Uart::print((const uint8_t *)debugBuffer, 64);
        }
    }
}