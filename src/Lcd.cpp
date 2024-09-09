#include "Lcd.h"
#include <lcd_hd44780.c>

Lcd::Lcd()
{
    lcd_init(LCD_DISP_ON);
}

void Lcd::print(char c)
{
    lcd_putc(c);
}

void Lcd::print(char *str)
{
    lcd_puts(str);
}

void Lcd::print(uint8_t number)
{
    char buffer[6];
    intToString(number, buffer);

    print(buffer);
}

void Lcd::print(uint16_t number)
{
    char buffer[6];
    intToString(number, buffer);

    print(buffer);
}

void Lcd::goToRow(uint8_t row)
{
    lcd_gotoxy(0, row);
}

void Lcd::reset()
{
    lcd_clrscr();
}
