#ifndef RECEIVER_H
#define RECEIVER_H

#define DATA_DDR DDRD
#define DATA_PORT PORTD
#define DATA_PIN PD6

#include <avr/io.h>

class Receiver
{
private:
public:
    void init();
    void onTimerInterrupt();
};

#endif