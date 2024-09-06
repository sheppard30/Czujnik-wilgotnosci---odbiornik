#include "Receiver.h"

void Receiver::init()
{
    DATA_DDR &= ~(1 << DATA_PIN);
}