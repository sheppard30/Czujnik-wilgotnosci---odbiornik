#include "Receiver.h"

// Globalne zmienne debugowania
volatile uint8_t debug[64];      // Definicja globalnej tablicy debug
volatile uint8_t debugIndex = 0; // Indeks do bufora
volatile uint8_t debugReady = 0; // Flaga sygnalizująca gotowość bufora
volatile bool debugPrinted = false;

void debugValue(uint8_t value)
{
    if (!debugReady)
    {
        debug[debugIndex] = value;
        debugIndex++;
    }

    if (debugIndex >= 64)
    {
        debugReady = true;
    }
}

void debugValue(uint8_t value, uint8_t length)
{
    if (!debugReady)
    {
        debug[debugIndex] = value;
        debugIndex++;
    }

    if (debugIndex >= length)
    {
        debugReady = true;
    }
}

Receiver::Receiver()
{
    previousBit = 0;
    state = State::NONE;
    bitIndex = 0;
    t = 0;

    resetData();
}

void Receiver::init()
{
    DATA_DDR &= ~(1 << DATA_PIN);
}

void Receiver::resetData()
{
}

void Receiver::fillBuffer(uint8_t bit)
{
    // Obliczamy, w którym bajcie oraz na której pozycji w tym bajcie wstawić bit
    uint8_t byteIndex = (bitIndex / 8) + 1;   // Indeks bajtu w tablicy buffer
    uint8_t bitPosition = 7 - (bitIndex % 8); // Pozycja bitu w danym bajcie

    // Wstawiamy bit na odpowiednie miejsce w bajcie
    if (bit == 1)
    {
        data[byteIndex] |= (1 << bitPosition); // Ustawiamy bit na 1
    }
    else
    {
        data[byteIndex] &= ~(1 << bitPosition); // Resetujemy bit na 0
    }

    bitIndex++;

    if (bitIndex >= (FRAME_LENGTH - 1) * 8)
    {
        bitIndex = 0; // Zresetuj bitIndex, jeśli osiągnie koniec bufora
        debugValue(data[2], 1);
        resetData();

        state = State::NONE;
    }
}

void Receiver::read()
{
    uint8_t currentBit = (PIND & (1 << DATA_PIN)) ? 1 : 0;

    if (previousBit == 0 && currentBit == 1)
    {
        if (state == State::NONE)
        {
            state = State::READING_PREAMBLE;
            t = 0;
        }

        if (t >= 10 || t == 0)
        {
            if (state == State::READING_PREAMBLE)
            {
                data[0] = (data[0] << 1) | 1;
                t = 0;
            }
            else if (state == State::READING_DATA)
            {
                debugValue('1');
                fillBuffer(1);
            }
        }
    }
    else if (previousBit == 1 && currentBit == 0)
    {
        if (t >= 10)
        {
            if (state == State::READING_PREAMBLE)
            {
                data[0] = (data[0] << 1);
                t = 0;
            }
            else if (state == State::READING_DATA)
            {
                debugValue('0');
                fillBuffer(0);
            }
        }
    }

    if (state == State::READING_PREAMBLE)
    {
        t++;

        if (data[0] == PREAMBLE)
        {
            state = State::READING_DATA;
        }
    }

    if (state == State::READING_DATA)
    {
        t++;
    }

    previousBit = currentBit;
}

void Receiver::onTimerInterrupt()
{
    read();
}