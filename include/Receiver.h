#ifndef RECEIVER_H
#define RECEIVER_H

#define DATA_DDR DDRD
#define DATA_PORT PORTD
#define DATA_PIN PD6

#include <avr/io.h>

#define PREAMBLE 0b10101010
#define FRAME_LENGTH 4
#define T 0.5

class Receiver
{
public:
    Receiver();
    void init();
    void resetData();
    void onTimerInterrupt();
    uint8_t getIdentifier();
    uint16_t getData();
    bool isDataAvailable();

private:
    enum State
    {
        NONE,
        READING_PREAMBLE,
        READING_DATA
    };

    volatile uint8_t previousBit;
    volatile uint8_t bitIndex;
    volatile uint8_t t;
    volatile uint8_t data[FRAME_LENGTH];
    volatile State state;
    volatile bool dataAvailable;

    void read();
    void fillBuffer(uint8_t bit);
};

#endif