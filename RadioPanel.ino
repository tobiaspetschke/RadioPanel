// RadioPanel.ino
#include "pins_arduino.h"
#include "PinChangeCallback.h"

void setup()
{
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  delay(1000);
  PCattachInterrupt(2, 0, CHANGE);
  PCdetachInterrupt(2);
}

void loop()
{
  
  delay(10);
  //Serial.print(i, DEC);
  //Serial.print(" ");
  //Serial.println(ticktocks);
}

