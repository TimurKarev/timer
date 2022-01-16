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
            digitalWrite(_heatPin, HIGH); //OFF
        }
        else
        {
            digitalWrite(_heatPin, LOW); // ON
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
        _dht = dht;
        timerMs.setPeriodMode();
        timerMs.setTime(1000);
        timerMs.start();

        pinMode(_fanPin, OUTPUT);
        digitalWrite(_fanPin, HIGH);
        pinMode(_heatPin, OUTPUT);
        digitalWrite(_heatPin, HIGH);
    }

    void tick()
    {
        if (timerMs.tick())
        {
            float t = _dht->readTemperature();
            //hum = _dht->readHumidity();
            if (t < 25)
            {
                _heatSwitch(true);
            }
            if (t > 27)
            {
                _heatSwitch(false);
            }
            // if (t > 29)
            // {
            //     _fanSwitch(true);
            // }
            // if (t < 27)
            // {
            //     _fanSwitch(false);
            // }
        }
    }
};

#endif