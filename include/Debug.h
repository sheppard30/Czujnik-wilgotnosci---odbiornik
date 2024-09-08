#ifndef DEBUG_H
#define DEBUG_h

#include <avr/io.h>

extern volatile uint8_t debugBuffer[64];
extern volatile uint8_t debugIndex;
extern volatile uint8_t debugReady;
extern volatile bool debugPrinted;
extern void debug(uint8_t value);
extern void debug(uint8_t value, uint8_t length);

#endif