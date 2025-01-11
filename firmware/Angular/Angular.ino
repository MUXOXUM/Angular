#include <Wire.h>
#include <GyverGFX.h>
#include <GyverMAX7219.h>
#include "ADXL345.h"

const uint8_t CS_PIN = 5;

MAX7219<1, 1, CS_PIN> mtrx;  // одна матрица (1x1), UNO / Nano (CLK - D13, DI - D11, CS - любой пин)

ADXL345 accelerometer;

// Калькулятор для дисплея - https://jorydotcom.github.io/matrix-emoji/
const uint8_t snake[] PROGMEM = { 0x00, 0x70, 0x52, 0x50, 0x52, 0x52, 0x1e, 0x00 };
const uint8_t setting[] PROGMEM = { 0x18, 0x7e, 0x7e, 0xe7, 0xe7, 0x7e, 0x7e, 0x18 };
const uint8_t logo[] PROGMEM = { 0xd0, 0xc4, 0x2a, 0x84, 0x0e, 0xe4, 0xff, 0xff };
const uint8_t sand[] PROGMEM = { 0x00, 0x08, 0x00, 0x04, 0x1e, 0x7f, 0xff, 0xff };
const uint8_t error[] PROGMEM = { 0x00, 0x77, 0x45, 0x77, 0x46, 0x75, 0x00, 0x00 };
const uint8_t ornament_1[] PROGMEM = { 0xa5, 0x3c, 0xe7, 0x42, 0x42, 0xe7, 0x3c, 0xa5 };
const uint8_t ornament_2[] PROGMEM = { 0xff, 0xc3, 0xa5, 0x99, 0x99, 0xa5, 0xc3, 0xff };

const uint8_t arrow_up[] PROGMEM = { 0x18, 0x3c, 0x7e, 0xff, 0x18, 0x18, 0x18, 0x18 };
const uint8_t arrow_down[] PROGMEM = { 0x18, 0x18, 0x18, 0x18, 0xff, 0x7e, 0x3c, 0x18 };
const uint8_t arrow_left[] PROGMEM = { 0x10, 0x30, 0x70, 0xff, 0xff, 0x70, 0x30, 0x10 };
const uint8_t arrow_right[] PROGMEM = { 0x08, 0x0c, 0x0e, 0xff, 0xff, 0x0e, 0x0c, 0x08 };
const uint8_t arrow_ret[] PROGMEM = { 0x00, 0x00, 0x38, 0x44, 0x44, 0x4e, 0x04, 0x00 };

void setup() {
  Serial.begin(9600);

  mtrx.begin();         // запускаем
  mtrx.setBright(0);    // яркость 0..15
  mtrx.setRotation(2);  // можно повернуть 0..3, по 90 град по часовой стрелке
  mtrx.clear();

  Wire.begin();
  if (accelerometer.init()) {
    Serial.println("ADXL345 initialization successful!");
  } else {
    Serial.println("ADXL345 initialization failed!");
  }

  mtrx.drawBitmap(0, 0, logo, 8, 8);
  mtrx.update();
  delay(2000);
}

void loop() {
  //Чтение данных с акселерометра
  accelerometer.read();
  float x = accelerometer.getX();
  float y = accelerometer.getY();
  float z = accelerometer.getZ();

  mtrx.clear();

  // Распознавание жестов
  mtrx.drawBitmap(0, 0, ornament_2, 8, 8);
  if (z < -0.6) {  // Перевернут дисплей
    mtrx.drawBitmap(0, 0, arrow_ret, 8, 8);
  } else if (x > 0.6) {  // Наклон вправо
    mtrx.drawBitmap(0, 0, arrow_right, 8, 8);
  } else if (x < -0.6) {  // Наклон влево
    mtrx.drawBitmap(0, 0, arrow_left, 8, 8);
  } else if (y > 0.6) {  // Наклон вперед
    mtrx.drawBitmap(0, 0, arrow_up, 8, 8);
  } else if (y < -0.6) {  // Наклон назад
    mtrx.drawBitmap(0, 0, arrow_down, 8, 8);
  }
  mtrx.update();
  delay(100);


  // Serial.print("X:");
  // Serial.print(x);
  // Serial.print("\tY:");
  // Serial.print(y);
  // Serial.print("\tZ:");
  // Serial.println(z);


  // mtrx.clear();
  // mtrx.drawBitmap(0, 0, arrow_right, 8, 8);
  // mtrx.update();
  // delay(2000);

  // mtrx.clear();
  // mtrx.drawBitmap(0, 0, arrow_left, 8, 8);
  // mtrx.update();
  // delay(2000);

  // mtrx.clear();
  // mtrx.drawBitmap(0, 0, arrow_up, 8, 8);
  // mtrx.update();
  // delay(2000);

  // mtrx.clear();
  // mtrx.drawBitmap(0, 0, arrow_down, 8, 8);
  // mtrx.update();
  // delay(5000);
}
