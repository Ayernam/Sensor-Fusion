#include <Wire.h>
#include <sensor_fusion.h>
#include <printf.h>
#define PWR_MGMT_1 0x6B
#define GYRO_CONFIG 0x1B
#define CONFIG 0x1A
#define INT_STATUS 0x3A
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40
#define GYRO_XOUT_H 0x43
#define GYRO_XOUT_L 0x44
#define GYRO_YOUT_H 0x45
#define GYRO_YOUT_L 0x46
#define GYRO_ZOUT_H 0x47
#define GYRO_ZOUT_L 0x48

uint8_t reader[1] = {B0};

void setup() {
  Serial.begin(9600);
  while (!Serial);
  printf_begin();

  writeReg(PWR_MGMT_1, (readRegister(PWR_MGMT_1) & B10111111) | B00000000, 1);
  writeReg(GYRO_CONFIG, (readRegister(GYRO_CONFIG) & B11100111) | B00011000, 1);
  writeReg(CONFIG, (readRegister(CONFIG) & B11111000) | B00000000, 1);
}

void loop() {
  Serial.println("hi");

  while (!(readRegister(INT_STATUS) & B00000001));

  Serial.println("Data ready");

  int accelXH = readRegister(ACCEL_XOUT_H);
  int accelXL = readRegister(ACCEL_XOUT_L);
  Serial.println(accelXH && " , " && accelXL);
}

uint8_t readRegister(uint8_t regAddress) {
  readReg(regAddress, reader, 1);
  return reader[0];
  /*
  int output = B0;
  for (int i = 0; i < 8; i++) {
    output *= 2;
    output += reader[i];
  }
  return output;
  */
}
