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
   bool currentState, newState;
   uint8_t pin;
   char *name;

   void pinChangeCallback(bool state)
   {
     volatile static long timeLastPress=0;
     if (millis() - timeLastPress > DEBOUNCE_TIMEOUT_MS)
     {
      
      timeLastPress = millis();
     }
   };

   Button (uint8_t _pin, char *_name): pin(_pin), name(_name)
   {
      pinMode(pin, INPUT_PULLUP);
      currentState = newState = (bool) digitalRead(pin);
      //PCattachInterrupt(pin, &Button::pinChangeCallback, CHANGE);
   };   
  
   char *getName()
   {
     return name;
   }


};

#endif