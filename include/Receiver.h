#ifndef RECEIVER_H
#define RECEIVER_H

#define DATA_DDR DDRD
#define DATA_PORT PORTD
#define DATA_PIN PD6

#include <avr/io.h>
#include "Uart.h"

#define PREAMBLE 0b10101010
#define FRAME_LENGTH 3

class Receiver
{
public:
    Receiver();
    void init();
    void onTimerInterrupt();
    uint8_t preamble;
    uint8_t buffer[FRAME_LENGTH];

private:
    enum State
    {
        WAITING_FOR_PREAMBLE,
        READING_DATA
    };

    uint8_t previousBit;
    State state;

    uint8_t bitIndex;
    uint8_t manchesterPhase;

    void read();
    void resetBuffer();
    bool preambleDetected();
    void fillBuffer(uint8_t bit);
};

#endif