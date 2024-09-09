#include "Uart.h"

#include <util/delay.h>

#include "Timer.h"
#include "Receiver.h"
#include "Lcd.h"
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
    Lcd lcd;

    lcd.setCursor(0, 0);
    lcd.print("Wartosc:");

    while (1)
    {
        if (receiver.isDataAvailable())
        {
            // lcd.setCursor(0, 0);                 // Ustawienie kursora na 1. linii
            // lcd.print("Wartosc:");               // Wyświetlenie tekstu
            // lcd.setCursor(0, 1);                 // Ustawienie kursora na 2. linii
            // lcd.printNumber(receiver.getData()); // Wyświetlenie liczby
            receiver.resetData();
        }

        if (debugReady && !debugPrinted)
        {
            debugPrinted = true; // Reset flagi
            // Uart::print((const uint8_t *)debugBuffer, 64);
        }
    }
}