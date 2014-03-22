#ifndef __LED_CONTROL_H__
#define __LED_CONTROL_H__

#if defined(ARDUINO) && ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <pins_arduino.h>
  #include <WProgram.h>
#endif

#include "PanelControl.h"

class LED: public PanelControlInterface
{
protected:
	uint8_t pwm;
	uint8_t breatheMillis;
	uint8_t minBreathePWM, maxBreathePWM;
	int8_t step, stepSize;

public:

	void setState(bool state)
	{
		setBrightness(state? 255:0);
	}

	void setBrightness(uint8_t level)
	{
		breatheMillis = 0;
		pwm = level;
		analogWrite(pin,level);
	}

	void breathe(uint8_t periodMillis, uint8_t _step)
	{
		step = stepSize = _step;
		setBrightness(minBreathePWM);
		breatheMillis=periodMillis;
	}

	LED (uint8_t _pin, char *_name)
	{
		pinMode(_pin, OUTPUT);
		breatheMillis = 0;
		minBreathePWM=10; 
		maxBreathePWM=200;
		pin = _pin;
		name = _name;
	}

	virtual void Process()
	{
		if (breatheMillis)
		{
			static long lastBreatheTime=0;
			if (millis() >= lastBreatheTime + breatheMillis)
			{				
				int16_t newPWM = pwm + step;

				if (newPWM >= maxBreathePWM)
				{
					step = -stepSize;
					pwm = maxBreathePWM;
				}else if(newPWM <= minBreathePWM)
				{
					step = +stepSize;
					pwm = minBreathePWM;
				}
				else
				{
					pwm = newPWM;
				}
				analogWrite(pin, pwm);
				lastBreatheTime = millis();

			} 
		}
	}

	virtual bool serialCmd(sCommand * pCmd)
	{ 
		bool success = false;

		if (strcmp(pCmd->property,"lvl") == 0)
		{
			setBrightness((uint8_t) atoi(pCmd->p1));
			success = true;
		}
		else if (pCmd->p2 && (strcmp(pCmd->property,"breathe") == 0))
		{
			success = true;
			breathe(atoi(pCmd->p1), atoi(pCmd->p2));
		} 

		if (!success)
		{
			Serial.println("ok");
		}

		return success;
	}
};


#endif