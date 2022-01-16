#ifndef Humiditer_h
#define Humiditer_h

#include <DHT.h>
#include <DHT_U.h>
#include <TimerMs.h>

class Humiditer
{

private:
    const int vccPin = 7; //
    const int ctrPin = 8; //TODO: insert right pin

    const int delayBeforeCtrOn = 1000;
    const int impulseTimeOn = 1000;

    const int perTime = 1000;
    const int mimHumVal = 65;
    const int maxHumVal = 80;

    DHT *_dht;
    TimerMs tickTimer;

public:
    Humiditer(DHT *dht)
    {
        _dht = dht;
        tickTimer.setPeriodMode();
        tickTimer.setTime(perTime);
        tickTimer.start();

        pinMode(vccPin, OUTPUT);
        digitalWrite(vccPin, HIGH);

        pinMode(ctrPin, OUTPUT);
        digitalWrite(ctrPin, HIGH);
    }

    void tick()
    {
        if (tickTimer.tick())
        {
            float hum = _dht->readHumidity();

            if (hum < mimHumVal && !isVccOn())
            {
                _onHumiditer();
            }

            if (hum > maxHumVal && isVccOn())
            {
                _offHumiditer();
            }
        }
    }

private:
    bool isVccOn()
    {
        if (digitalRead(vccPin) == HIGH)
        {
            return false;
        }
        return true;
    }

    void _onHumiditer()
    {
        digitalWrite(vccPin, LOW);
        delay(delayBeforeCtrOn);
        digitalWrite(ctrPin, LOW);
        delay(impulseTimeOn);
        digitalWrite(ctrPin, HIGH);
    }

    void _offHumiditer()
    {
        digitalWrite(vccPin, HIGH);
    }
};

#endif