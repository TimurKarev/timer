#include "DHT.h"
#include <DHT_U.h>   // когда на чистом си собираешь еще и такое надо ставить
#define DHTPIN 2 // Тот самый номер пина, о котором упоминалось выше
// Одна из следующих строк закоментирована. Снимите комментарий, если подключаете датчик DHT11 к arduino
//DHT dht(DHTPIN, DHT22); //Инициация датчика
DHT dht(DHTPIN, DHT11);
#define PIN_Heating 5
#define PIN_Fan 6
#define PIN_Vaporizer_power 7
#define PIN_Vaporizer_on 8
boolean flag = true;
const int ledPin = 4;
int State = LOW;
unsigned long previousMillis;
const long interval_1 = 64800000; //Выкл
const long interval_2 = 21600000; //Вкл
void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(PIN_Heating, OUTPUT); // Объявляем пин реле как выход
  pinMode(PIN_Fan, OUTPUT);
  pinMode(PIN_Vaporizer_power, OUTPUT);
  pinMode(PIN_Vaporizer_on, OUTPUT);
  pinMode(ledPin, OUTPUT);
}
void loop() {
  delay(2000); // 2 секунды задержки
  float h = dht.readHumidity(); //Измеряем влажность
  float t = dht.readTemperature(); //Измеряем температуру
  if (isnan(h) || isnan(t)) {  // Проверка. Если не удается считать показания, выводится «Ошибка считывания», и программа завершает работу
    Serial.println("Ошибка считывания");
    return;
  }
  {
    if ((t) < 25) {
      digitalWrite(PIN_Heating, HIGH); //У нас подключено так, что включается при помощи LOW
    }
    if ((t) > 27) {
      digitalWrite(PIN_Heating, LOW);  // а в эмуляторе ты подключил по другому, поэтому там работает а тут нет
    }
    //Вытяжка
    if ((t) > 29 || (h) > 95) {
      digitalWrite(PIN_Fan, HIGH);
    }
    if ((t) < 29 && (h) < 90) {  // сравнение это &&, одно & это операция побитного И
      digitalWrite(PIN_Fan, LOW);
    }
    //Испаритель питание
    if ((h) < 75) {
      digitalWrite(PIN_Vaporizer_power, HIGH);
    }
    if ((h) > 80) {
      digitalWrite(PIN_Vaporizer_power, LOW);
      delay(500);
    }
  }
  //Испаритель управление
  if (digitalRead(PIN_Vaporizer_power) && flag) {
    flag = !flag;
    digitalWrite(PIN_Vaporizer_on, HIGH);
    delay(500);
    digitalWrite(PIN_Vaporizer_on, LOW);
  }
  if (!digitalRead(PIN_Vaporizer_power) && !flag) {
    flag = !flag;
  }
  unsigned long currentMillis = millis ( );
  if ( State == LOW ) {
    if ( currentMillis - previousMillis >= interval_1 ) {
      previousMillis = currentMillis;
      State = HIGH;
      digitalWrite (ledPin, State );
    }
  }
  if ( State == HIGH ) {
    if ( currentMillis - previousMillis >= interval_2 ) {
      previousMillis = currentMillis;
      State = LOW;
      digitalWrite ( ledPin, State );
    }
  }

  Serial.print("Влажность: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Температура: ");
  Serial.print(t);
  Serial.println(" *C "); //Вывод показателей на экран
}