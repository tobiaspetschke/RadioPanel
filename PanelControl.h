#ifndef __PANELCONTROL_H__
#define __PANELCONTROL_H__

#if defined(ARDUINO) && ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <pins_arduino.h>
  #include <WProgram.h>
#endif


class PanelControlInterface
{
protected:	 
	uint8_t pin;
    char *name;

   public:

     PanelControlInterface() {};
     virtual void Process() {}
};

#endif