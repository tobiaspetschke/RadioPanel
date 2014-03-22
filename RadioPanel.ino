// RadioPanel.ino
#include "pins_arduino.h"
#include "PanelControl.h"
#include "Button.h"
#include "LED.h"
#include "SerialCommand.h"

PanelControlInterface* Controls[] =  { 
						 new Button(2,"btnPwr"),
						 new LED(6,"ledStatus"),
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

  if (!success)
  {
    Serial.println("err");
  }
}

void setup()
{  
  Serial.begin(9600);
  delay(50);
  cmdReader.addCommand("set", cmdProcessor);
  cmdReader.setDefaultHandler(cmdUnknown);

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
  	SendControlUpdates();

  //Serial.print(i, DEC);
  //Serial.print(" ");
  //Serial.println(ticktocks);
}

