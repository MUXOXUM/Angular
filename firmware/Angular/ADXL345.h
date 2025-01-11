#ifndef ADXL345_H
#define ADXL345_H

#include <Wire.h>

// ADXL345 I2C адрес
const uint8_t ADXL345_ADDRESS = (0xA6 >> 1);  // 7-битный адрес устройства

// Регистры чипа
const uint8_t DEV_ID = 0x00;          // Device ID (0x01 to 0x1C do not access)
const uint8_t THRESH_TAP = 0x1D;      // Tap threshold
const uint8_t OFS_X = 0x1E;           // X-axis offset
const uint8_t OFS_Y = 0x1F;           // Y-axis offset
const uint8_t OFS_Z = 0x20;           // Z-axis offset
const uint8_t DUR = 0x21;             // Tap duration
const uint8_t LATENT = 0x22;          // Tap latency
const uint8_t WINDOW = 0x23;          // Tap window
const uint8_t THRESH_ACT = 0x24;      // Activity threshold
const uint8_t THRESH_INACT = 0x25;    // Inactivity threshold
const uint8_t TIME_INACT = 0x26;      // Inactivity time
const uint8_t ACT_INACT_CTL = 0x27;   // Axis enable control for activity and inactivity detection
const uint8_t THRESH_FF = 0x28;       // Free-fall threshold
const uint8_t TIME_FF = 0x29;         // Free-fall time
const uint8_t TAP_AXES = 0x2A;        // Axis control for single tap/double tap
const uint8_t ACT_TAP_STATUS = 0x2B;  // Source of single tap/double tap
const uint8_t BW_RATE = 0x2C;         // Data rate and power mode control
const uint8_t POWER_CTL = 0x2D;       // Power-saving features control
const uint8_t INT_ENABLE = 0x2E;      // Interrupt enable control
const uint8_t INT_MAP = 0x2F;         // Interrupt mapping control
const uint8_t INT_SOURCE = 0x30;      // Source of interrupts
const uint8_t DATA_FORMAT = 0x31;     // Data format control
const uint8_t DATA_X0 = 0x32;         // X-Axis Data 0
const uint8_t DATA_X1 = 0x33;         // X-Axis Data 1
const uint8_t DATA_Y0 = 0x34;         // Y-Axis Data 0
const uint8_t DATA_Y1 = 0x35;         // Y-Axis Data 1
const uint8_t DATA_Z0 = 0x36;         // Z-Axis Data 0
const uint8_t DATA_Z1 = 0x37;         // Z-Axis Data 1
const uint8_t FIFO_CTL = 0x38;        // FIFO control
const uint8_t FIFO_STATUS = 0x39;     // FIFO status


#define SCALE_FACTOR (3.9 / 1000)  // 3.9mg/LSB for all g-ranges, full resolution

class ADXL345 {
public:
  // Конструктор
  ADXL345() {
    for (uint8_t i = 0; i < 3; ++i) {
      accelerometer_data[i] = 0;
    }
  }

  // Инициализация устройства
  bool init() {
    i2c_write(DATA_FORMAT, 0x0B);  // Устанавливаем режим 13-битного режима с ±16g
    i2c_write(POWER_CTL, 0x08);    // Выбираем режим измерения
    i2c_write(OFS_X, 0x00);        // Задаем поправку значений по осям
    i2c_write(OFS_Y, 0x00);
    i2c_write(OFS_Z, 0x00);

    // Проверка успешной инициализации
    uint8_t data = 0;
    i2c_read(DEV_ID, 1, &data);
    if (data == 0xE5) {  // неизменный ID ADXL345
      return true;
    } else {
      return false;
    }
  }

  // Чтение данных с акселерометра
  void read() {
    uint8_t bytes[6];
    memset(bytes, 0, 6);  // присваеваем всем элементам нули

    i2c_read(DATA_X0, 6, bytes);  // Чтение 6 байт данных с DATA_X0 по DATA_Z1

    // Распаковка данных
    for (uint8_t i = 0; i < 3; ++i) {
      accelerometer_data[i] = (int16_t)bytes[2 * i] + (((int16_t)bytes[2 * i + 1]) << 8);
    }
  }

  // Получение значения по оси X
  float getX() {
    return float(accelerometer_data[0]) * SCALE_FACTOR;
  }

  // Получение значения по оси Y
  float getY() {
    return float(accelerometer_data[1]) * SCALE_FACTOR;
  }

  // Получение значения по оси Z
  float getZ() {
    return float(accelerometer_data[2]) * SCALE_FACTOR;
  }

private:
  // Массив для хранения данных акселерометра
  int16_t accelerometer_data[3];

  // Вспомогательная функция для записи данных в ADXL345
  void i2c_write(uint8_t reg, uint8_t data) {
    Wire.beginTransmission(ADXL345_ADDRESS);
    Wire.write(reg);
    Wire.write(data);
    Wire.endTransmission();
  }

  // Вспомогательная функция для чтения данных из ADXL345
  void i2c_read(uint8_t reg, uint8_t count, uint8_t* data) {
    uint8_t i = 0;
    Wire.beginTransmission(ADXL345_ADDRESS);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.beginTransmission(ADXL345_ADDRESS);
    Wire.requestFrom(ADXL345_ADDRESS, count);
    while (Wire.available()) {
      data[i] = Wire.read();
      i++;
    }
    Wire.endTransmission();
  }
};

#endif  // ADXL345_H