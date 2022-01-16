#ifndef TempHumWire_h
#define TempHumWire_h

#include "Arduino.h"
#include <TimerMs.h>
#include <DHT.h>
#include <DHT_U.h>

#define _heatPin 5
#define _fanPin 6
//#define _humPin 7

class TempHumWire
{
private:
    TimerMs timerMs;

    DHT* _dht; //(WIRE_PIN, DHT11);

    void _heatSwitch(bool on)
    {
        if (!on)
        {
            digitalWrite(_heatPin, HIGH);
        }
        else
        {
            digitalWrite(_heatPin, LOW);
        }
    }

    void _fanSwitch(bool on)
    {
        if (!on)
        {
            digitalWrite(_fanPin, HIGH);
        }
        else
        {
            digitalWrite(_fanPin, LOW);
        }
    }


public:
    TempHumWire(DHT* dht)
    {
        timerMs.setPeriodMode();
        timerMs.setTime(1000);
        timerMs.start();

        pinMode(_fanPin, OUTPUT);
        pinMode(_heatPin, OUTPUT);
    }

    void tick()
    {
        float t = -1;
        float hum = -1;

        if (timerMs.tick())
        {
            t = _dht->readTemperature();
            hum = _dht->readHumidity();
            Serial.print("t- ");
            Serial.println(t);
            Serial.print("hum- ");
            Serial.println(hum);

            if (t < 25)
            {
                _heatSwitch(true);
            }
            if (t > 27)
            {
                _heatSwitch(false);
            }
            if (t > 29)
            {
                _fanSwitch(true);
            }
            if (t < 27)
            {
                _fanSwitch(false);
            }
        }
    }
};

#endif