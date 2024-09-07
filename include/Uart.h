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
    static void print(const char *str);
    static void print(uint8_t data);
    static void print(const uint8_t *data, uint8_t length);
};

#endif
