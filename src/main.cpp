#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "Timer.h"
#include "Receiver.h"
#include "Uart.h"
#include "Sensors.h"
#include "Button.h"
// #include "Debug.h"

Receiver receiver;
Sensors sensors;
Button button;

ISR(TIMER0_COMPA_vect)
{
    receiver.onTimerInterrupt();
}

ISR(INT0_vect)
{
    button.onInterrupt();
}

void switchSensor()
{
    sensors.switchSensor();
}

int main()
{
    Uart::init(MYUBRR);
    Timer::init();

    button.onPress(switchSensor);

    while (1)
    {
        if (receiver.isDataAvailable())
        {
            char id = receiver.getIdentifier();
            uint8_t huminity = (receiver.getData() / MAX_HUMINITY) * 100;

            sensors.addOrUpdateSensor(id, huminity);

            receiver.resetData();
        }
    }
}