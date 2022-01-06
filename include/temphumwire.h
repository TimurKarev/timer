#ifndef TempHumWire_h
#define TempHumWire_h

#include "Arduino.h"
#include <TimerMs.h>
#include <DHT.h>
#include <DHT_U.h>

#define WIRE_PIN 2
#define _heatPin 5
#define _fanPin 6
#define _humPin 7

class TempHumWire
{
private:
    TimerMs timerMs;

    DHT dht; //(WIRE_PIN, DHT11);

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

    void _humSwitch(bool on)
    {
        if (!on)
        {
            digitalWrite(_humPin, HIGH);
        }
        else
        {
            digitalWrite(_humPin, LOW);
        }
    }

public:
    TempHumWire() : dht(WIRE_PIN, DHT11)
    {
        timerMs.setPeriodMode();
        timerMs.setTime(1000);
        timerMs.start();
        dht.begin();

        pinMode(WIRE_PIN, OUTPUT);
        pinMode(_humPin, OUTPUT);
        pinMode(_fanPin, OUTPUT);
        pinMode(_heatPin, OUTPUT);
    }

    void tick()
    {
        float t = -1;
        float hum = -1;

        if (timerMs.tick())
        {
            t = dht.readTemperature();
            hum = dht.readHumidity();
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
            if (hum < 65)
            {
                _humSwitch(true);
            }
            if (hum > 80)
            {
                _humSwitch(false);
            }
        }
    }
};

#endif