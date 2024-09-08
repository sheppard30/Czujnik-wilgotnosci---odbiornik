#include "Timer.h"

void Timer::init()
{
    // Ustawienie trybu CTC (Clear Timer on Compare Match)
    TCCR0A = (1 << WGM01); // WGM01 = 1 ustawia tryb CTC

    // Ustawienie preskalera na 64
    TCCR0B = (1 << CS01) | (1 << CS00); // Preskaler 64

    // Ustawienie wartości porównawczej (OCR0A) na 61, aby uzyskać nieco poniżej 0,5 ms
    OCR0A = 12;

    // Włączenie przerwań od porównania (przerwanie dla OCR0A)
    TIMSK = (1 << OCIE0A);

    // Włączenie globalnych przerwań
    sei();
}