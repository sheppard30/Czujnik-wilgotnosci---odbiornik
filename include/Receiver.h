#ifndef RECEIVER_H
#define RECEIVER_H

#define DATA_DDR DDRD
#define DATA_PORT PORTD
#define DATA_PIN PD6

#include <avr/io.h>
#include "Uart.h"

#define PREAMBLE 0b10101010
#define FRAME_LENGTH 3
#define T 0.5

extern volatile uint8_t debug[64];
extern volatile uint8_t debugIndex;
extern volatile uint8_t debugReady;
extern volatile bool debugPrinted;

class Receiver
{
public:
    Receiver();
    void init();
    void onTimerInterrupt();

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
    volatile uint8_t phase;
    volatile State state;

    volatile uint8_t buffer[FRAME_LENGTH];

    void read();
    void resetBuffer();
    bool preambleDetected();
    void fillBuffer(uint8_t bit);
};

#endif