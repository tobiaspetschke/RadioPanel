#ifndef __TRI_COLOUR_LED_CONTROL_H__
#define __TRI_COLOUR_LED_CONTROL_H__

#if defined(ARDUINO) && ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <pins_arduino.h>
  #include <WProgram.h>
#endif

#include "PanelControl.h"
#include "LED.h"

typedef enum LEDColour { Green = 0, Red, Yellow };

class TriColourLED: public PanelControlInterface
{
protected:
	LED * _pLedGreen;
	LED * _pLedRed;
	LEDColour _colour;
public:

	void setColour(LEDColour colour)
	{
		_colour = colour;
	}

	void setBrightness(uint8_t level)
	{
		uint8_t lvlRed = 0, lvlGreen = 0;
		switch(_colour)
		{
			case Red:
				lvlRed = level;
				break;

			case Green:
				lvlGreen = level;
				break;

			case Yellow:
				lvlGreen  = level;
				lvlRed = level;
				break;
		}
		_pLedGreen->setBrightness(lvlGreen);
		_pLedRed->setBrightness(lvlRed);
	}

	void breathe(uint8_t periodMillis, uint8_t step)
	{		
		switch (_colour)
		{
			case Red:
				_pLedGreen->setBrightness(0);
				_pLedRed->breathe(periodMillis, step);
				break;

			case Green:
				_pLedRed->setBrightness(0);
				_pLedGreen->breathe(periodMillis, step);
				break;

			case Yellow:
				_pLedGreen->breathe(periodMillis, step);
				_pLedRed->breathe(periodMillis, step);
				break;
		}
	}

	TriColourLED (uint8_t _pinGreen, uint8_t _pinRed, char *_name)
	{
		pinMode(_pinGreen, OUTPUT);
		pinMode(_pinRed, OUTPUT);
		_colour = Red;
		_pLedGreen = new LED(_pinGreen, NULL);
		_pLedRed = new LED(_pinRed, NULL);
		name = _name;
	}

	virtual ~TriColourLED()
	{
		delete(_pLedGreen);
		delete(_pLedGreen);
	}

	virtual void Process()
	{
		_pLedRed->Process();
		_pLedGreen->Process();
	}

	virtual bool serialCmd(sCommand * pCmd)
	{ 
		char * lastParam = (pCmd->p3)? (pCmd->p3) : (pCmd->p2)? pCmd->p2 : pCmd->p1;

		if (lastParam)
		{
			if (strcmp(lastParam,"red") == 0)
			{
				setColour(Red);
			}
			else if (strcmp(lastParam,"yellow") == 0)
			{
				setColour(Yellow);
			}
			else if (strcmp(lastParam,"green") == 0)
			{
				setColour(Green);
			}
		}

		if (strcmp(pCmd->property,"lvl") == 0)
		{
			setBrightness((uint8_t) atoi(pCmd->p1));
			return true;
		}
		else if (pCmd->p2 && (strcmp(pCmd->property,"breathe") == 0))
		{
			breathe(atoi(pCmd->p1), atoi(pCmd->p2));
			return true;
		} 

		return false;
	}
};


#endif