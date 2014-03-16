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
	uint16_t breatheMillis;
	uint8_t minBreathePWM, maxBreathePWM;
	int8_t step, stepSize;
	uint8_t pin;
    char *name;


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

	void breathe(uint16_t periodMillis, uint8_t _step)
	{
		step = stepSize = _step;
		setBrightness(minBreathePWM);
		breatheMillis=periodMillis;
	}

	LED (uint8_t _pin, char *_name): pin(_pin), name(_name)
	{
		pinMode(_pin, OUTPUT);
		breatheMillis = 0;
		minBreathePWM=10; 
		maxBreathePWM=200;
	}

	void Process()
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
};

#endif