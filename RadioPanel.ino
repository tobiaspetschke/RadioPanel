// RadioPanel.ino
#include "pins_arduino.h"
#include "PanelControl.h"
#include "Button.h"
#include "LED.h"
#include "SerialCommand.h"

PanelControlInterface* Controls[] =  { 
						 new Button(2,"pwr"),
						 new LED(6,"status"),
						 NULL
					 };

SerialCommand cmdReader;

void cmdUnknown(const char * cmd)
{
  Serial.print("Unknown command");
}

void cmdEcho()
{

}

void cmdSetControlState()
{
  char *arg;
  while(arg=cmdReader.next())
  {
    Serial.println(arg);
  }
}


void setup()
{  
  Serial.begin(9600);
  delay(50);
  cmdReader.addCommand("set state", cmdSetControlState);
  cmdReader.addCommand("echo", cmdEcho); 

   ((LED*)Controls[1])->setState(true);
  Serial.println("panel state 1");
}

void SendControlUpdates()
{
	uint8_t i=0;
	
	while(Controls[i])
	{
		Controls[i++]->Process();
	}
}


void loop()
{
    cmdReader.readSerial();
    delay(30);
  	SendControlUpdates();

  //Serial.print(i, DEC);
  //Serial.print(" ");
  //Serial.println(ticktocks);
}

