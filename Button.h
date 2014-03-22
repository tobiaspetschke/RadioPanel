#ifndef __BUTTON_CONTROL_H__
#define __BUTTON_CONTROL_H__

#if defined(ARDUINO) && ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <pins_arduino.h>
  #include <WProgram.h>
#endif

#include "ooPinChangeInt.h"
#include "PanelControl.h"

#define DEBOUNCE_TIMEOUT_MS 10

class Button : public PinChangeCallBackInterface, public PanelControlInterface
{
protected:
 volatile long timeLastPress;

 public:
   bool currentState;
   volatile bool newState;

   void SendState()
   {
    Serial.print("btn ");
    Serial.print(name);
    Serial.print(" state ");
    Serial.println(currentState);
   }

   virtual void Process()
   {
      noInterrupts();
      if (newState != currentState)
      {
        if (millis() - DEBOUNCE_TIMEOUT_MS >= timeLastPress)
        {
          currentState = newState;
          SendState();
        }
      }
      interrupts();
   }

   void pinChanged(bool state)
   {
     state = ! state; // low means button is pushed
     timeLastPress = millis();
     if (state != currentState)
     {
        newState = state;
     }
   };

   Button (uint8_t _pin, char *_name)
   {
      pinMode(pin, INPUT_PULLUP);
      newState = !digitalRead(pin);
      currentState = newState;
      timeLastPress=0;
      pin = _pin;
      name = _name;
      PCintPort::attachInterrupt(pin, this, CHANGE);
   };   

};

#endif