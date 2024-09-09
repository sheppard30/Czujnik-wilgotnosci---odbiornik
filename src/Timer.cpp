#include "Timer.h"

void Timer::init()
{
    // Ustawienie trybu CTC (Clear Timer on Compare Match)
    TCCR0A = (1 << WGM01); // WGM01 = 1 ustawia tryb CTC

    // Ustawienie preskalera na 64
    TCCR0B = (1 << CS01) | (1 << CS00); // Preskaler 64

    // Ustawienie wartości porównawczej (OCR0A) na 24, aby uzyskać nieco 0,2 ms
    OCR0A = 24;

    // Włączenie przerwań od porównania (przerwanie dla OCR0A)
    TIMSK0 = (1 << OCIE0A); // Zmieniono TIMSK na TIMSK0, specyficzny dla ATmega328P

    // Włączenie globalnych przerwań
    sei();
}
