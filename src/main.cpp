#include <util/delay.h>
#include <avr/pgmspace.h>

#include "Timer.h"
#include "Receiver.h"
// #include "Debug.h"
#include "StringUtils.h"

#include <lcd.h>
#include <lcd.c>

Receiver receiver;

#include <avr/interrupt.h>

ISR(TIMER0_COMPA_vect)
{
    receiver.onTimerInterrupt();
}

int main()
{
    // Uart::init(MYUBRR);
    Timer::init();

    lcd_init(LCD_DISP_ON);
    char huminityString[4];

    while (1)
    {
        if (receiver.isDataAvailable())
        {
            char sensorLabel[] = "Czujnik: ";
            char huminityLabel[] = "Wilgotnosc: ";

            intToString(receiver.getData(), huminityString);
            lcd_clrscr();
            lcd_puts(sensorLabel);
            lcd_putc(receiver.getIdentifier());
            lcd_gotoxy(0, 1);
            lcd_puts(huminityLabel);
            lcd_puts(huminityString);
            receiver.resetData();
        }
    }
}