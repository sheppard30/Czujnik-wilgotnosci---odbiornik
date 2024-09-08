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
    for (uint8_t i = 0; i < 64; i++)
    {
        debug[i] = 'p'; // Reset tablicy po wysłaniu
    }

    Uart::init(MYUBRR);
    Timer::init();

    DDRD |= (1 << PD5);

    while (1)
    {
        // Sprawdzenie, czy bufor debug jest gotowy do wysłania
        if (debugReady && !debugPrinted)
        {
            debugPrinted = true;                     // Reset flagi
            Uart::print((const uint8_t *)debug, 64); // Wyświetlenie zawartości bufora
        }
    }
}