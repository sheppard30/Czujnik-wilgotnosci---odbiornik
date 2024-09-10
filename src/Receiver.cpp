#include "Receiver.h"
#include "Uart.h"

Receiver::Receiver()
{
    previousBit = 0;                 // Poprzedni bit
    preambleBit = 1;                 // Aktualny bit preambuly
    state = State::READING_PREAMBLE; // Aktualny stan
    bitIndex = 0;                    // Liczba zapisanych bitow i aktualny bit
    count = 0;                       // Liczba tikow
    dataAvailable = false;           // Czy dane sa dostepne do pobrania

    risingEdge = 0;
    fallingEdge = 0;

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
        state = State::READING_PREAMBLE;
        count = 0;
        dataAvailable = true;
    }
}

void Receiver::validatePreamble()
{
    if (data[0] == PREAMBLE)
    {
        state = State::READING_DATA;
        data[0] = 0;
    }
}

void Receiver::read()
{
    uint8_t currentBit = (PIND & (1 << DATA_PIN)) ? 1 : 0;

    if (previousBit == 0 && currentBit == 1)
    {
        Uart::print(fallingEdge);
        if ((fallingEdge >= 10))
        {
            Uart::print('1');
            if (state == State::READING_PREAMBLE)
            {
                data[0] = (data[0] << 1) | 1;

                validatePreamble();
            }
            else if (state == State::READING_DATA)
            {
                fillBuffer(1);
            }
        }

        fallingEdge = 0;
    }
    else if (previousBit == 1 && currentBit == 0)
    {
        Uart::print(risingEdge);
        if (risingEdge >= 10 && risingEdge <= 12)
        {
            Uart::print('0');
            if (state == State::READING_PREAMBLE)
            {
                data[0] = (data[0] << 1);
                Uart::print('V');
                validatePreamble();
            }
            else if (state == State::READING_DATA)
            {
                fillBuffer(0);
            }
        }

        risingEdge = 0;
    }

    if (currentBit)
    {
        risingEdge++;
    }
    else
    {
        fallingEdge++;
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