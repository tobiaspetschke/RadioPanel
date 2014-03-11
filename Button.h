#ifndef __BUTTON_H__
#define __BUTTON_H__

#if defined(ARDUINO) && ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <pins_arduino.h>
  #include <WProgram.h>
#endif

#include "PinChangeCallback.h"

#define DEBOUNCE_TIMEOUT_MS 20

class Button /*: public CallBackInterface */
{
 public:
   bool state;
   bool uint8_t newState;
   uint8_t pin;
   char *name;

   void pinChangeCallback(bool state)
   {
     volatile static long timeLastPress=0;
     if (millis() - timeLastPress > DEAD_MILLIS_AFTER_PRESS)
     {
      
      timeLastPress = millis();
     }
   };

   button (uint8_t _pin, char *_name): pin(_pin), name(_name)
   {
      pinMode(pin, INPUT_PULLUP);
      state = lastState = (bool) digitalRead(pin);
      PCintPort::attachInterrupt(pin, &pinChangeCallback, CHANGE);
   };   
  
   char *getName()
   {
     return name;
   }


};

#endif