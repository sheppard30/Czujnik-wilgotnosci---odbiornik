#ifndef UART_H
#define UART_H

#include <avr/io.h>

#define F_CPU 8000000UL // 8 MHz
#define BAUD 4800
#define MYUBRR (F_CPU / 16 / BAUD - 1)

class Uart
{
public:
    Uart();
    void init(unsigned int ubrr);
    void print(const char *str);
    void print(uint8_t data);
};

#endif
