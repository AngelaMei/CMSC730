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

int address = 0x68;

void setup() {
  Wire.begin(sda, scl);
  Serial.begin(9600);
}

void loop() {
  // x
  Wire.beginTransmission(address);
  Wire.write(0x3B);
  Wire.endTransmission();

  Wire.requestFrom(address, 1);
  ACCEL_XOUT_H = Wire.read();

  Wire.beginTransmission(address);
  Wire.write(0x3C);
  Wire.endTransmission();

  Wire.requestFrom(address, 1);
  ACCEL_XOUT_L = Wire.read();

  ACCEL_XOUT_RAW = ACCEL_XOUT_H << 8 | ACCEL_XOUT_L;
  gX = ACCEL_XOUT_RAW / 16384.0;
  

  Serial.print(">gx:");
  Serial.println(gX);
  delay(10);

  // y
  Wire.beginTransmission(address);
  Wire.write(0x3D);
  Wire.endTransmission();

  Wire.requestFrom(address, 1);
  ACCEL_YOUT_H = Wire.read();

  Wire.beginTransmission(address);
  Wire.write(0x3E);
  Wire.endTransmission();

  Wire.requestFrom(address, 1);
  ACCEL_YOUT_L = Wire.read();

  ACCEL_YOUT_RAW = ACCEL_YOUT_H << 8 | ACCEL_YOUT_L;
  gY = ACCEL_YOUT_RAW / 16384.0;
  

  Serial.print(">gy:");
  Serial.println(gY);
  delay(10);

  // z
  Wire.beginTransmission(address);
  Wire.write(0x3F);
  Wire.endTransmission();

  Wire.requestFrom(address, 1);
  ACCEL_ZOUT_H = Wire.read();

  Wire.beginTransmission(address);
  Wire.write(0x40);
  Wire.endTransmission();

  Wire.requestFrom(address, 1);
  ACCEL_ZOUT_L = Wire.read();

  ACCEL_ZOUT_RAW = ACCEL_ZOUT_H << 8 | ACCEL_ZOUT_L;
  gZ = ACCEL_ZOUT_RAW / 16384.0;
  

  Serial.print(">gz:");
  Serial.println(gZ);
  delay(10);

}