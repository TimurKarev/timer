#ifndef LightPin_h
#define LightPin_h

#include "Arduino.h"
#include <TimerMs.h>

class LightPin
{
private:
    int _pin;

    bool isStartPeriod;
    bool lightOn;

    uint32_t lightOffTime;
    uint32_t lightOnTime;

    TimerMs timerMs;


    void switchLight()
    {
        if (isStartPeriod)
        {
            isStartPeriod = false;
        }
        lightOn = !lightOn;

        if (lightOn) {
            timerMs.setTime(lightOnTime);
        } else {
            timerMs.setTime(lightOffTime);
        }
        timerMs.setTime(perTime);
            digitalWrite(_pin, HIGH);
            digitalWrite(LED_BUILTIN, HIGH);
        } else {
            digitalWrite(_pin, LOW);
            digitalWrite(LED_BUILTIN, LOW);
        }
    }

public:
    LightPin(int pin, bool light, uint32_t startTime, uint32_t offTime, uint32_t onTime)
    {
        _pin = pin;
        pinMode(_pin, OUTPUT);
        pinMode(LED_BUILTIN, OUTPUT);

        isStartPeriod = true;
        lightOn = light;
        lightOffTime = offTime;
        lightOnTime = onTime;
        timerMs.setPeriodMode();
        timerMs.start();
        
        timerMs.setTime(startTime);
        setPin();
    }

    void tick() {
        if (timerMs.tick()) {
            switchLight();
        }
    }
};

#endif