#include "Receiver.h"

Receiver::Receiver()
{
    previousBit = 0;
    state = State::NONE;
    bitIndex = 0;
    t = 0;
    dataAvailable = false;

    resetData();
}

void Receiver::init()
{
    DATA_DDR &= ~(1 << DATA_PIN);
}

void Receiver::resetData()
{
    dataAvailable = false;

    for (uint8_t index = 0; index < FRAME_LENGTH; index++)
    {
        data[index] = 0; // Ustaw każdy element tablicy na 0
    }
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
        state = State::NONE;
        dataAvailable = true;
    }
}

void Receiver::read()
{
    uint8_t currentBit = (PIND & (1 << DATA_PIN)) ? 1 : 0;

    // TODO: przeniesc
    if (state == State::NONE)
    {
        resetData();
        state = State::READING_PREAMBLE;
        t = 0;
    }

    if (previousBit == 0 && currentBit == 1)
    {
        if (t >= 10 || t == 0)
        {
            if (state == State::READING_PREAMBLE)
            {
                data[0] = (data[0] << 1) | 1;
            }
            else if (state == State::READING_DATA)
            {
                fillBuffer(1);
            }
            t = 0;
        }
    }
    else if (previousBit == 1 && currentBit == 0)
    {
        if (t >= 10)
        {
            if (state == State::READING_PREAMBLE)
            {
                data[0] = (data[0] << 1);
            }
            else if (state == State::READING_DATA)
            {
                fillBuffer(0);
            }
            t = 0;
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
    else if (state == State::READING_DATA)
    {
        t++;
    }

    previousBit = currentBit;
}

char Receiver::getIdentifier()
{
    return data[1];
}

uint16_t Receiver::getData()
{
    // Łączy data[2] i data[3] w jedną 16-bitową liczbę
    return (static_cast<uint16_t>(data[2]) << 8) | data[3];
}

bool Receiver::isDataAvailable()
{
    return dataAvailable;
}

void Receiver::onTimerInterrupt()
{
    read();
}