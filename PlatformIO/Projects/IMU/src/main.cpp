#include <Arduino.h>
#include <Wire.h> // Wire library

const int sda = 21; //Data Pin
const int scl = 22; //Clock Pin

byte ACCEL_XOUT_H = 0;
byte ACCEL_XOUT_L = 0;
int16_t ACCEL_XOUT_RAW = 0;
float gX;

byte ACCEL_YOUT_H = 0;
byte ACCEL_YOUT_L = 0;
int16_t ACCEL_YOUT_RAW = 0;
float gY;

byte ACCEL_ZOUT_H = 0;
byte ACCEL_ZOUT_L = 0;
int16_t ACCEL_ZOUT_RAW = 0;
float gZ;

// Gyro
byte GYRO_XOUT_H = 0;
byte GYRO_XOUT_L = 0;
int16_t GYRO_XOUT_RAW = 0;
float gyroX;

byte GYRO_YOUT_H = 0;
byte GYRO_YOUT_L = 0;
int16_t GYRO_YOUT_RAW = 0;
float gyroY;

byte GYRO_ZOUT_H = 0;
byte GYRO_ZOUT_L = 0;
int16_t GYRO_ZOUT_RAW = 0;
float gyroZ;


int address = 0x68;

void setup() {
  Wire.begin(sda, scl);
  Serial.begin(9600);
}

void loop() {
  // Read all accelerometer data in one transmission
  Wire.beginTransmission(address);
  Wire.write(0x3B);  // Start with accelerometer X-axis register address
  Wire.endTransmission();

  Wire.requestFrom(address, 6);  // Read 6 bytes for all axes

  ACCEL_XOUT_H = Wire.read();
  ACCEL_XOUT_L = Wire.read();
  ACCEL_YOUT_H = Wire.read();
  ACCEL_YOUT_L = Wire.read();
  ACCEL_ZOUT_H = Wire.read();
  ACCEL_ZOUT_L = Wire.read();
  
  ACCEL_XOUT_RAW = (ACCEL_XOUT_H << 8) | ACCEL_XOUT_L;
  gX = ACCEL_XOUT_RAW / 16384.0;  // Assuming 16384 LSB/g for accelerometer

  ACCEL_YOUT_RAW = (ACCEL_YOUT_H << 8) | ACCEL_YOUT_L;
  gY = ACCEL_YOUT_RAW / 16384.0;

  ACCEL_ZOUT_RAW = (ACCEL_ZOUT_H << 8) | ACCEL_ZOUT_L;
  gZ = ACCEL_ZOUT_RAW / 16384.0;

  Serial.print(">gx:");
  Serial.println(gX);
  Serial.print(">gy:");
  Serial.println(gY);
  Serial.print(">gz:");
  Serial.println(gZ);
  delay(10);

  // Gyro
  Wire.beginTransmission(address);
  Wire.write(0x43);  // Start with accelerometer X-axis register address
  Wire.endTransmission();

  Wire.requestFrom(address, 6);  // Read 6 bytes for all axes

  GYRO_XOUT_H = Wire.read();
  GYRO_XOUT_L = Wire.read();
  GYRO_YOUT_H = Wire.read();
  GYRO_YOUT_L = Wire.read();
  GYRO_ZOUT_H = Wire.read();
  GYRO_ZOUT_L = Wire.read();
  
  GYRO_XOUT_RAW = (GYRO_XOUT_H << 8) | GYRO_XOUT_L;
  gyroX = GYRO_XOUT_RAW / 16384.0;  // Assuming 16384 LSB/g for accelerometer

  GYRO_YOUT_RAW = (GYRO_YOUT_H << 8) | GYRO_YOUT_L;
  gyroY = GYRO_YOUT_RAW / 16384.0;

  GYRO_ZOUT_RAW = (GYRO_ZOUT_H << 8) | GYRO_ZOUT_L;
  gyroZ = GYRO_ZOUT_RAW / 16384.0;

  Serial.print(">gyrox:");
  Serial.println(gyroX);
  Serial.print(">gyroy:");
  Serial.println(gyroY);
  Serial.print(">gyroz:");
  Serial.println(gyroZ);
  delay(10);
}

