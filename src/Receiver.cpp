#include "Receiver.h"

Receiver::Receiver()
{
    previousBit = 0;
    state = State::WAITING_FOR_PREAMBLE;
    bitIndex = 0;
    manchesterPhase = 0;

    resetBuffer();
}

void Receiver::init()
{
    DATA_DDR &= ~(1 << DATA_PIN);
}

void Receiver::resetBuffer()
{
    for (uint8_t index = 0; index < FRAME_LENGTH; ++index)
    {
        buffer[index] = 0; // Ustawiamy każdy element tablicy na 0
    }

    bitIndex = 0;
}

void Receiver::fillBuffer(uint8_t bit)
{
    // Obliczamy, w którym bajcie oraz na której pozycji w tym bajcie wstawić bit
    uint8_t byteIndex = bitIndex / 8;         // Indeks bajtu w tablicy buffer
    uint8_t bitPosition = 7 - (bitIndex % 8); // Pozycja bitu w danym bajcie

    // Wstawiamy bit na odpowiednie miejsce w bajcie
    if (bit == 1)
    {
        buffer[byteIndex] |= (1 << bitPosition); // Ustawiamy bit na 1
    }
    else
    {
        buffer[byteIndex] &= ~(1 << bitPosition); // Resetujemy bit na 0
    }

    bitIndex++;

    if (bitIndex >= FRAME_LENGTH * 8)
    {
        bitIndex = 0; // Zresetuj bitIndex, jeśli osiągnie koniec bufora

        resetBuffer();

        state = State::WAITING_FOR_PREAMBLE;
    }
}

bool Receiver::preambleDetected()
{
    uint8_t currentBit = (PIND & (1 << DATA_PIN)) ? 1 : 0;

    if (previousBit == 0 && currentBit == 1)
    {
        // Przejście z LOW do HIGH, co oznacza logiczną 1 w kodzie Manchester
        if (state == State::WAITING_FOR_PREAMBLE)
        {
            preamble = ((preamble << 1) | 1);
        }
    }
    else if (previousBit == 1 && currentBit == 0)
    {
        // Przejście z HIGH do LOW, co oznacza logiczne 0 w kodzie Manchester
        if (state == State::WAITING_FOR_PREAMBLE)
        {
            preamble = ((preamble << 1));
        }
    }

    // Zapisz aktualny stan jako poprzedni na następną iterację
    previousBit = currentBit;

    return (preamble == PREAMBLE);
}

void Receiver::read()
{
    uint8_t currentBit = (PIND & (1 << DATA_PIN)) ? 1 : 0;

    if (state == State::WAITING_FOR_PREAMBLE && preambleDetected())
    {
        state = State::READING_DATA;
        return;
    }

    if (state == State::READING_DATA)
    {
        if (currentBit)
        {
            Uart::print('1');
        }
        else
        {
            Uart::print('0');
        }
        // Dekodowanie danych po wykryciu preambuły
        if (manchesterPhase == 0)
        {
            // Pierwsza połowa bitu Manchester
            manchesterPhase = 1;
        }
        else
        {
            // Druga połowa bitu Manchester - dekodujemy bit logiczny
            if (previousBit == 0 && currentBit == 1)
            {
                fillBuffer(1); // Zapis do bufora
            }
            else if (previousBit == 1 && currentBit == 0)
            {
                fillBuffer(0); // Zapis do bufora
            }

            // Zresetuj fazę Manchester
            manchesterPhase = 0;
        }

        previousBit = currentBit;
    }
}

void Receiver::onTimerInterrupt()
{
    read();
}