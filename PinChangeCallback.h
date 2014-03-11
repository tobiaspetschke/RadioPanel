/* Small Pin Change Lib based on Arduino PCIntLib */
#ifndef __PINCHANGECALLBACK_H__
#define __PINCHANGECALLBACK_H__

#include "pins_arduino.h"

typedef void (*pinChangeFunc)(bool);

/* Mode: RISING, FALLING, CHANGE */
void PCattachInterrupt(uint8_t pin, pinChangeFunc func , int mode);
void PCdetachInterrupt(uint8_t pin);

#endif
