#ifndef UART_H
#define UART_H

#include <avr/io.h>

#define F_CPU 8000000UL // 8 MHz
#define BAUD 9600
#define MYUBRR (F_CPU / 16 / BAUD - 1)

class Uart
{
public:
    Uart();
    static void init(unsigned int ubrr);
    static void print(uint16_t value);                      // Deklaracja dla uint16_t
    static void print(const char *str);                     // Deklaracja dla stringa
    static void print(uint8_t data);                        // Deklaracja dla uint8_t
    static void print(const uint8_t *data, uint8_t length); // Deklaracja dla tablicy bajtów
    static void print(char data);
};

#endif
