#ifndef RECEIVER_H
#define RECEIVER_H

#define DATA_DDR DDRD
#define DATA_PORT PORTD
#define DATA_PIN PD7

#include <avr/io.h>

#define PREAMBLE 0b10101010
#define FRAME_LENGTH 4

class Receiver
{
public:
    Receiver();
    void init();
    void resetData();
    void onTimerInterrupt();
    char getIdentifier();
    uint16_t getData();
    bool isDataAvailable();

private:
    enum State
    {
        READING_PREAMBLE,
        READING_DATA
    };

    volatile uint8_t previousBit;
    volatile uint8_t bitIndex;
    volatile uint8_t preambleBit;
    volatile uint8_t count;
    volatile uint8_t data[FRAME_LENGTH];
    volatile State state;
    volatile bool dataAvailable;

    volatile uint8_t fallingEdge;
    volatile uint8_t risingEdge;

    void read();
    void fillBuffer(uint8_t bit);
    void validatePreamble();
};

#endif