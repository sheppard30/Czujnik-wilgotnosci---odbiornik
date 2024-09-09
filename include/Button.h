#ifndef BUTTON_H
#define BUTTON_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define BUTTON_PIN PD2

class Button
{
public:
    typedef void (*CallbackFunction)();

    Button();
    void onInterrupt();
    void onPress(CallbackFunction func);

private:
    CallbackFunction onPressFunc;
};

#endif