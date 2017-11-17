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

int accelBias[3], gyroBias[3];
int sampleNum;
uint8_t reader[1] = {B0};

void setup() {
  Serial.begin(9600);
  while (!Serial);
  printf_begin();

  //configure accelerometer and gyroscope
  writeReg(PWR_MGMT_1, (readRegister(PWR_MGMT_1) & B10111111) | B00000000, 1);
  writeReg(GYRO_CONFIG, (readRegister(GYRO_CONFIG) & B11100111) | B00011000, 1);
  writeReg(CONFIG, (readRegister(CONFIG) & B11111000) | B00000000, 1);
}

void loop() {
  //Serial.println("test");

  //Wait for data to be sent
  while (!(readRegister(INT_STATUS) & B00000001));
  Serial.println("Data ready");

  //find biases of acceleromter and gyroscope by 100 samples
  findBiases(100);
}

void findBiases(int sampleNum) {
  int accelBiasSum[3], gyroBiasSum[3];

  for (int i = 0; i < sampleNum; i++) {
    while (!(readRegister(INT_STATUS) & B00000001));

    accelBiasSum[0] += (readRegister(ACCEL_XOUT_H) + readRegister(ACCEL_XOUT_L)) / 2;
    accelBiasSum[1] += (readRegister(ACCEL_YOUT_H) + readRegister(ACCEL_YOUT_L)) / 2;
    accelBiasSum[2] += (readRegister(ACCEL_ZOUT_H) + readRegister(ACCEL_ZOUT_L)) / 2 - 1;

    gyroBiasSum[0] += (readRegister(GYRO_XOUT_H) + readRegister(GYRO_XOUT_L)) / 2;
    gyroBiasSum[1] += (readRegister(GYRO_YOUT_H) + readRegister(GYRO_YOUT_L)) / 2;
    gyroBiasSum[2] += (readRegister(GYRO_ZOUT_H) + readRegister(GYRO_ZOUT_L)) / 2;
  }

  for (int i = 0; i < 3; i++) {
    accelBias[i] = accelBiasSum[i] / sampleNum;
    gyroBias[i] = gyroBiasSum[i] / sampleNum;
  }
}

uint8_t readRegister(uint8_t regAddress) {
  readReg(regAddress, reader, 1);
  return reader[0];
}
