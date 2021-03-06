#include <Arduino.h>
#include <TimerMs.h>
#include "lightpin.h"
#include "temphumwire.h"

//#define DHT_PIN 2
#define LIGHT1PIN 4

#define DHT_TICK_PERIOD 5000
                         
#define LIGHT1_ON_PERIOD 72000000 // 20 часов
#define LIGHT1_OFF_PERIOD 14400000 // 4 часа
#define LIGHT1_START_PERIOD 18000000


bool isStartDelayPass = false;
int time = 0;

// (период, мс), (0 не запущен / 1 запущен), (режим: 0 период / 1 таймер)
TimerMs strDelTimer(5000, 1, 1);

// Установка таймера освещения
LightPin lightPin(4, true, LIGHT1_START_PERIOD, LIGHT1_OFF_PERIOD, LIGHT1_ON_PERIOD);
TempHumWire tempHumWire = TempHumWire();


void start_delay_pass()
{
  isStartDelayPass = true;
  Serial.println("Start delay callback");
}


void setup()
{
  Serial.begin(9600);

  strDelTimer.setTimerMode();
  void (*start_delay_pass_ptr)() = &start_delay_pass;
  strDelTimer.attach(start_delay_pass_ptr);

  // light1Timer.setPeriodMode();
  // void (*light1_task_ptr)() = &light1_task;
  // light1Timer.attach(light1_task_ptr);

  // tempHumTimer.setPeriodMode();
  // void (*temp_hum_task_ptr)() = &temp_hum_task;
  // tempHumTimer.attach(temp_hum_task_ptr);
}

void loop()
{
  if (!isStartDelayPass)
  {
    strDelTimer.tick();
    return;
  }

  lightPin.tick();
  tempHumWire.tick();
}