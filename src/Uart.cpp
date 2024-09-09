#include "Uart.h"

Uart::Uart() {}

void Uart::init(unsigned int ubrr)
{
    // Ustawienie baud rate
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;

    // Włączenie transmisji (TX) i odbioru (RX)
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);

    // Ustawienie formatu ramki: 8-bit danych, 1 bit stopu
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Konfiguracja ramki
}

void Uart::print(uint16_t value)
{
    char buffer[6]; // Bufor na maksymalnie 5 cyfr uint16_t + null terminator
    uint8_t i = 0;

    // Jeśli wartość wynosi 0, od razu wyświetlamy '0'
    if (value == 0)
    {
        print('0');
        return;
    }

    // Wyciąganie cyfr i zapisywanie ich do bufora
    while (value > 0)
    {
        buffer[i++] = (value % 10) + '0'; // Zamiana cyfry na znak ASCII
        value /= 10;
    }

    // Teraz drukujemy cyfry w odwrotnej kolejności
    while (i > 0)
    {
        print(buffer[--i]);
    }
}

void Uart::print(char data)
{
    print(static_cast<uint8_t>(data)); // Wywołanie funkcji dla uint8_t
}

void Uart::print(const char *str)
{
    while (*str)
    {
        print(*str); // Teraz wywołuje `print(char)`
        str++;
    }
}

void Uart::print(uint8_t data)
{
    // Czekaj aż bufor nadawczy będzie pusty
    while (!(UCSR0A & (1 << UDRE0)))
        ;

    // Wyślij dane
    UDR0 = data;
}

void Uart::print(const uint8_t *data, uint8_t length)
{
    // Iterujemy przez każdy element w tablicy danych
    for (uint8_t i = 0; i < length; ++i)
    {
        print(data[i]); // Wywołanie istniejącej metody dla każdego bajtu
    }
}
