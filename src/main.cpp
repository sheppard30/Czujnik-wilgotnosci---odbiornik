#include <util/delay.h>
#include <avr/pgmspace.h>

#include "Timer.h"
#include "Receiver.h"
#include "Lcd.h"
#include "Uart.h"
// #include "Debug.h"

Receiver receiver;

#include <avr/interrupt.h>

ISR(TIMER0_COMPA_vect)
{
    receiver.onTimerInterrupt();
}

int main()
{
    Uart::init(MYUBRR);
    Timer::init();
    Lcd lcd;

    while (1)
    {
        if (receiver.isDataAvailable())
        {
            char sensorLabel[] = "Czujnik: ";
            char huminityLabel[] = "Wilgotnosc: ";

            lcd.reset();
            lcd.print(sensorLabel);
            lcd.print(receiver.getIdentifier());
            lcd.goToRow(1);
            lcd.print(huminityLabel);
            lcd.print(receiver.getData());

            receiver.resetData();
        }
    }
}