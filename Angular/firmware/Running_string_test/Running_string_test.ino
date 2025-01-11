// пример RunningGFX
// RunningGFX работает с классом, который наследует GyverGFX. Например библиотека GyverMAX7219
#include <GyverMAX7219.h>

#include <RunningGFX.h>

MAX7219<1, 1, 5> mtrx;  // одна матрица (1х1), пин CS на D5
RunningGFX run1(&mtrx);
RunningGFX run2(&mtrx);
const char pstr_g[] PROGMEM = "Global pgm string";
String str_g = "123";



void setup() {
  mtrx.begin();       // запускаем
  mtrx.setBright(0);  // яркость 0..15
  mtrx.setRotation(2); // можно повернуть 0..3, по 90 град по часовой стрелке

  //run1.setText("16");  // строковая константа
  run1.setText(str_g); // глобальная стринга
  run1.setSpeed(15);
  run1.setWindow(0, 16, 0);
  run1.start();

  run2.setText_P(pstr_g); // глобальная PGM строка
  run2.setSpeed(5);
  run2.setWindow(0, 7, 0);
  run2.start();
}

void loop() {
  //run1.tick();
  run2.tick();
}