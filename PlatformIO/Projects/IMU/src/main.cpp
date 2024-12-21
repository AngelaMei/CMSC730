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
float gyroX_Per_S;
float gyroAngleX;

byte GYRO_YOUT_H = 0;
byte GYRO_YOUT_L = 0;
int16_t GYRO_YOUT_RAW = 0;
float gyroY_Per_S;
float gyroAngleY;

byte GYRO_ZOUT_H = 0;
byte GYRO_ZOUT_L = 0;
int16_t GYRO_ZOUT_RAW = 0;
float gyroZ_Per_S;
float gyroAngleZ;

// Temperture
byte TEMP_OUT_H = 0;
byte TEMP_OUT_L = 0;
int16_t TEMP_OUT_RAW = 0;
float TEMP;

int address = 0x68;
float AccelSensitiveScale = 2048.0; //+-2G
float GyroSensitiveScale = 131.0;

unsigned long previousTime = 0;

float caliX = 329.56;
float caliY = 1257.35;
float caliZ = 538.38;

float calculateCalibration(int axis, int numReadings, int delayMs) {
  float totalRaw = 0.0;

  for (int i = 0; i < numReadings; i++) {
    Wire.beginTransmission(address);
    Wire.write(0x43 + axis);  // Address for X-axis (0x43), Y-axis (0x45), Z-axis (0x47)
    Wire.endTransmission();

    Wire.requestFrom(address, 2);  // Read 2 bytes for a single axis
    int rawAxis = (Wire.read() << 8) | Wire.read();

    // Accumulate raw values
    totalRaw += rawAxis;

    delay(delayMs);
  }

  // Calculate average and return as calibration value
  Serial.print("cali:");
  Serial.println(totalRaw / (float)numReadings);
  return totalRaw / (float)numReadings;
}

void setup() {
  Wire.begin(sda, scl);
  Serial.begin(9600);

  // Modify Sensor
  Wire.beginTransmission(address);
  Wire.write(0x1C);
  Wire.write(0b00011000);
  Wire.endTransmission();
  // caliX = calculateCalibration(0, 1000, 10);
  // caliY = calculateCalibration(2, 1000, 10);
  // caliZ = calculateCalibration(4, 1000, 10);
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
  gX = ACCEL_XOUT_RAW / AccelSensitiveScale;  // Assuming 16384 LSB/g for accelerometer

  ACCEL_YOUT_RAW = (ACCEL_YOUT_H << 8) | ACCEL_YOUT_L;
  gY = ACCEL_YOUT_RAW / AccelSensitiveScale;

  ACCEL_ZOUT_RAW = (ACCEL_ZOUT_H << 8) | ACCEL_ZOUT_L;
  gZ = ACCEL_ZOUT_RAW / AccelSensitiveScale;

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
  GYRO_YOUT_RAW = (GYRO_YOUT_H << 8) | GYRO_YOUT_L;
  GYRO_ZOUT_RAW = (GYRO_ZOUT_H << 8) | GYRO_ZOUT_L;

  unsigned long currentTime = millis();
  float elapsedTime = (currentTime - previousTime) / 1000.0f;
  
  gyroX_Per_S = (GYRO_XOUT_RAW - caliX) / GyroSensitiveScale;
  gyroY_Per_S = (GYRO_YOUT_RAW - caliY) / GyroSensitiveScale;
  gyroZ_Per_S = (GYRO_ZOUT_RAW - caliZ) / GyroSensitiveScale;

  gyroAngleX += gyroX_Per_S * elapsedTime;
  gyroAngleY += gyroY_Per_S * elapsedTime;
  gyroAngleZ += gyroZ_Per_S * elapsedTime;

  previousTime = currentTime;

  Serial.print(">gyroAngleX:");
  Serial.println(gyroAngleX);
  Serial.print(">gyroAngleY:");
  Serial.println(gyroAngleY);
  Serial.print(">gyroAngleZ:");
  Serial.println(gyroAngleZ);

  // Serial.print(">gyroX_Per_S:");
  // Serial.println(gyroX_Per_S);
  // Serial.print(">gyroY_Per_S:");
  // Serial.println(gyroY_Per_S);
  // Serial.print(">gyroZ_Per_S:");
  // Serial.println(gyroZ_Per_S);
  delay(10);

  // Temp
  Wire.beginTransmission(address);
  Wire.write(0x41);
  Wire.endTransmission();

  Wire.requestFrom(address, 2);
  short temperature = Wire.read() << 9 | Wire.read();
  // Serial.print(">temperature:");
  // Serial.println(temperature/333.87 + 21);
  delay(20);
}