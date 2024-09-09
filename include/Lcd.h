#ifndef LCD_H
#define LCD_H

#include <avr/io.h>
#include <util/delay.h>

#define LCD_RS PD2
#define LCD_EN PD3
#define LCD_D4 PB0
#define LCD_D5 PB1
#define LCD_D6 PB2
#define LCD_D7 PB3

#include "Timer.h"

class Lcd
{
public:
    Lcd();
    void init();
    void clear();
    void setCursor(uint8_t col, uint8_t row);
    void print(const char *str);
    void printNumber(uint16_t number);

private:
    void reverseString(char *str, uint8_t length);
    void sendData(uint8_t data);
    void sendCommand(uint8_t cmd);
    void sendNibble(uint8_t data);
};

#endif