// RadioPanel.ino
#include "pins_arduino.h"
#include "PanelControl.h"
#include "Button.h"
#include "LED.h"
#include "TriColourLED.h"
#include "SerialCommand.h"

PanelControlInterface* Controls[] =  { 
						 new Button(2,"btnPwr"),
						 /*new LED(13,"ledStatus"),*/
             new TriColourLED(6,5,"tri"),
						 NULL
					 };

SerialCommand cmdReader;


void cmdUnknown(const char * cmd)
{
  Serial.println("err unknown command");
}

void cmdProcessor()
{
  bool success = false;
  sCommand cmd;
  cmd.ctrlName = cmdReader.next();
  cmd.property = cmdReader.next();
  cmd.p1 = cmdReader.next();
  cmd.p2 = cmdReader.next();
  cmd.p3 = cmdReader.next();
  
  if (cmd.ctrlName && cmd.property && cmd.p1)
  {
    uint8_t i = 0;
    while(Controls[i])
    {
      if (strcmp(cmd.ctrlName, Controls[i]->getName()) == 0)
      {
        success = Controls[i]->serialCmd(&cmd);        
        break;
      }
      i++;
    }
  }

  Serial.println(success? "ok" : "err");
}

void setup()
{  
  pinMode(13,INPUT);
  Serial.begin(9600);
  delay(50);
  cmdReader.clearBuffer();
  cmdReader.addCommand("set", cmdProcessor);
  cmdReader.setDefaultHandler(cmdUnknown);
  ((TriColourLED*)Controls[1])->setColour(Yellow);
  ((TriColourLED*)Controls[1])->setBrightness(255);
  ((TriColourLED*)Controls[1])->breathe(50,3);
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
  	SendControlUpdates();

  //Serial.print(i, DEC);
  //Serial.print(" ");
  //Serial.println(ticktocks);
}

