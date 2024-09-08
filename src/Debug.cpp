#include "Debug.h"

// Globalne zmienne debugowania
volatile uint8_t debugBuffer[64] = {0, 0, 0}; // Definicja globalnej tablicy debug
volatile uint8_t debugIndex = 0;              // Indeks do bufora
volatile uint8_t debugReady = 0;              // Flaga sygnalizująca gotowość bufora
volatile bool debugPrinted = false;

void debug(uint8_t value)
{
    if (!debugReady)
    {
        debugBuffer[debugIndex] = value;
        debugIndex++;
    }

    if (debugIndex >= 64)
    {
        debugReady = true;
    }
}

void debug(uint8_t value, uint8_t length)
{
    if (!debugReady)
    {
        debugBuffer[debugIndex] = value;
        debugIndex++;
    }

    if (debugIndex >= length)
    {
        debugReady = true;
    }
}