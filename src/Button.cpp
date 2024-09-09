#include "Button.h"

Button::Button()
{
    // Ustawienie pinu jako wejście
    DDRD &= ~(1 << BUTTON_PIN);
    // Włączenie pull-up (jeśli nie ma zewnętrznego rezystora pull-up)
    PORTD |= (1 << BUTTON_PIN);

    // Ustawienie przerwania na dowolną zmianę stanu
    EICRA |= (1 << ISC00); // ISC00 = 1 i ISC01 = 0 oznacza dowolną zmianę stanu
    EICRA &= ~(1 << ISC01);

    // Włączenie przerwania INT0
    EIMSK |= (1 << INT0);

    // Włączenie globalnych przerwań
    sei();

    // Na początku brak callbacku
    onPressFunc = nullptr;
}

void Button::onPress(CallbackFunction func)
{
    onPressFunc = func;
}

// Funkcja wywoływana w przerwaniu
void Button::onInterrupt()
{
    if (onPressFunc != nullptr && !(PIND & (1 << PD2)))
    {
        onPressFunc(); // Wywołanie przypisanego callbacku
    }
}