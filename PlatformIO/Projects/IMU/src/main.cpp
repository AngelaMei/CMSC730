#include <Arduino.h>
#include <Wire.h> // Wire library

const int sda = 21; //Data Pin
const int scl = 22; //Clock Pin

byte ACCEL_XOUT_H = 0;
byte ACCEL_XOUT_L = 0;
int16_t ACCEL_XOUT_RAW = 0;
float gx;

int address = 0x68;

void setup() {
  Wire.begin(sda, scl);
  Serial.begin(9600);
}

void loop() {
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
  gx = ACCEL_XOUT_RAW / 16384.0;
  

  Serial.print(">gx:");
  Serial.println(gx);
  delay(10);
}