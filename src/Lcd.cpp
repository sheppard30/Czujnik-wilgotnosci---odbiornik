#include "Lcd.h"

Lcd::Lcd()
{
    init();
}

void Lcd::init()
{
    // Ustawienie odpowiednich pinów jako wyjścia
    DDRD |= (1 << LCD_RS) | (1 << LCD_EN);
    DDRB |= (1 << LCD_D4) | (1 << LCD_D5) | (1 << LCD_D6) | (1 << LCD_D7);

    // Inicjalizacja wyświetlacza w trybie 4-bitowym
    _delay_ms(50); // Zastąpienie _delay_ms
    sendNibble(0x03);
    _delay_ms(5);
    sendNibble(0x03);
    _delay_us(100); // Zastąpienie _delay_us
    sendNibble(0x03);
    sendNibble(0x02);

    // Konfiguracja wyświetlacza
    sendCommand(0x28); // Tryb 4-bitowy, 2 linie, 5x8 znaków
    sendCommand(0x0C); // Włącz wyświetlacz, bez kursora
    sendCommand(0x06); // Inkrementacja kursora
    clear();           // Wyczyść wyświetlacz
}

void Lcd::clear()
{
    sendCommand(0x01); // Komenda do czyszczenia ekranu
    _delay_ms(2);      // Zastąpienie _delay_ms
}

// Ustaw kursor na określonej pozycji
void Lcd::setCursor(uint8_t col, uint8_t row)
{
    uint8_t row_offsets[] = {0x00, 0x40};
    sendCommand(0x80 | (col + row_offsets[row]));
}

// Wyświetl tekst
void Lcd::print(const char *str)
{
    while (*str)
    {
        sendData(*str++);
    }
}

// Wyświetl liczbę 16-bitową
void Lcd::printNumber(uint16_t number)
{
    char buffer[6]; // Bufor na 5 cyfr + znak końca stringa
    uint8_t i = 0;

    if (number == 0)
    {
        buffer[i++] = '0';
    }
    else
    {
        while (number > 0)
        {
            buffer[i++] = (number % 10) + '0'; // Pobieranie ostatniej cyfry liczby
            number /= 10;                      // Przesuwanie liczby o jedną cyfrę
        }
    }

    buffer[i] = '\0'; // Dodaj terminator stringa

    reverseString(buffer, i); // Odwróć bufor, ponieważ liczby są w odwrotnej kolejności
    print(buffer);            // Wyświetl liczby na LCD
}

// Odwracanie stringa, ponieważ liczby były wstawiane w odwrotnej kolejności
void Lcd::reverseString(char *str, uint8_t length)
{
    uint8_t start = 0;
    uint8_t end = length - 1;
    while (start < end)
    {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

// Wysyłanie półbajtu (nibble) danych
void Lcd::sendNibble(uint8_t data)
{
    if (data & 0x01)
        PORTB |= (1 << LCD_D4);
    else
        PORTB &= ~(1 << LCD_D4);
    if (data & 0x02)
        PORTB |= (1 << LCD_D5);
    else
        PORTB &= ~(1 << LCD_D5);
    if (data & 0x04)
        PORTB |= (1 << LCD_D6);
    else
        PORTB &= ~(1 << LCD_D6);
    if (data & 0x08)
        PORTB |= (1 << LCD_D7);
    else
        PORTB &= ~(1 << LCD_D7);

    // Impuls na pinie EN
    PORTD |= (1 << LCD_EN);
    _delay_us(1); // Zastąpienie _delay_us
    PORTD &= ~(1 << LCD_EN);
}

// Wysyłanie komendy
void Lcd::sendCommand(uint8_t cmd)
{
    PORTD &= ~(1 << LCD_RS); // RS = 0 (komenda)
    sendNibble(cmd >> 4);    // Wyslij górną część
    sendNibble(cmd);         // Wyslij dolną część
    _delay_ms(2);            // Zastąpienie _delay_ms
}

// Wysyłanie danych
void Lcd::sendData(uint8_t data)
{
    PORTD |= (1 << LCD_RS); // RS = 1 (dane)
    sendNibble(data >> 4);  // Wyslij górną część
    sendNibble(data);       // Wyslij dolną część
    _delay_ms(2);           // Zastąpienie _delay_ms
}
