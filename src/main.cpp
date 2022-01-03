#include <Arduino.h>
#include <TimerMs.h>
#include "lightpin.h"
#include <DHT.h>
#include <DHT_U.h>

#define DHT_PIN 2
#define LIGHT1PIN 4

#define DHT_TICK_PERIOD 5000

#define LIGHT1_ON_PERIOD 60000
#define LIGHT1_OFF_PERIOD 1000
#define LIGHT1_START_PERIOD 10000

bool islight1StartPeriod = true;
bool isLight1OnState = false;

bool isStartDelayPass = false;
int time = 0;

// (период, мс), (0 не запущен / 1 запущен), (режим: 0 период / 1 таймер)
TimerMs strDelTimer(5000, 1, 1);
TimerMs light1Timer(LIGHT1_START_PERIOD, 1, 0);
TimerMs tempHumTimer(DHT_TICK_PERIOD, 1, 0);

// Установка таймера освещения
LightPin lightPin(4, false, LIGHT1_START_PERIOD, LIGHT1_OFF_PERIOD, LIGHT1_ON_PERIOD);

DHT_Unified dht(DHT_PIN, DHT11);

void start_delay_pass()
{
  isStartDelayPass = true;
  Serial.println("Start delay callback");
}

void light1_update_hardware()
{
  if (isLight1OnState)
  {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else
  {
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void light1_switch_state()
{
  if (islight1StartPeriod)
  {
    islight1StartPeriod = false;
    Serial.println("Start light time off");
  }
  if (isLight1OnState)
  {
    light1Timer.setTime(LIGHT1_OFF_PERIOD);
    Serial.println("light off");
  }
  else
  {
    light1Timer.setTime(LIGHT1_ON_PERIOD);
    Serial.println("light ON");
  }
  isLight1OnState = !isLight1OnState;
}
void light1_task()
{
  Serial.print("islight1StartPeriod- ");
  Serial.println(islight1StartPeriod);
  Serial.print("isLight1OnState- ");
  Serial.println(isLight1OnState);
  light1_switch_state();
  light1_update_hardware();
}

void temp_hum_task()
{
  sensors_event_t event;

  dht.temperature().getEvent(&event);
  // Serial.print("Ta - ");
  if (!isnan(event.temperature))
  {
    // Serial.println(event.temperature);
  }
  else
  {
    // Serial.println("Error");
  }

  dht.humidity().getEvent(&event);
  // Serial.print("Hum - ");
  if (!isnan(event.relative_humidity))
  {
    // Serial.println(event.relative_humidity);
  }
  else
  {
    Serial.println("Error");
  }
}

void setup()
{
  Serial.begin(9600);

  dht.begin();

  pinMode(LED_BUILTIN, OUTPUT);

  strDelTimer.setTimerMode();
  void (*start_delay_pass_ptr)() = &start_delay_pass;
  strDelTimer.attach(start_delay_pass_ptr);

  // light1Timer.setPeriodMode();
  // void (*light1_task_ptr)() = &light1_task;
  // light1Timer.attach(light1_task_ptr);

  tempHumTimer.setPeriodMode();
  void (*temp_hum_task_ptr)() = &temp_hum_task;
  tempHumTimer.attach(temp_hum_task_ptr);
}

void loop()
{
  if (!isStartDelayPass)
  {
    strDelTimer.tick();
    return;
  }

  light1Timer.tick();
  tempHumTimer.tick();
  lightPin.tick();
}