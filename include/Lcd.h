#ifndef DEBUG_H
#define DEBUG_h

#include "StringUtils.h"
#include <lcd_hd44780.h>

class Lcd
{
public:
    Lcd();
    void print(char c);
    void print(char *str);
    void print(uint16_t number);
    void goToRow(uint8_t row);
    void reset();

private:
    void init();
};

#endif