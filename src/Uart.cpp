#include "Uart.h"

Uart::Uart() {}

void Uart::init(unsigned int ubrr)
{
    // Ustawienie baud rate
    UBRRH = (unsigned char)(ubrr >> 8);
    UBRRL = (unsigned char)ubrr;

    // Włączenie transmisji (TX) i odbioru (RX)
    UCSRB = (1 << TXEN) | (1 << RXEN);

    // Ustawienie formatu ramki: 8-bit danych, 1 bit stopu
    UCSRC = (1 << UCSZ1) | (1 << UCSZ0); // Brak bitu URSEL w ATtiny2313A
}

void Uart::print(const char *str)
{
    while (*str)
    {
        print(*str);
        str++;
    }
}

void Uart::print(uint8_t data)
{
    // Czekaj aż bufor nadawczy będzie pusty
    while (!(UCSRA & (1 << UDRE)))
        ;

    // Wyślij dane
    UDR = data;
}