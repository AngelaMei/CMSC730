#include <Arduino.h>
#include <Wire.h> // Wire library

const int sda = 21; //Data Pin
const int scl = 22; //Clock Pin

int latchPin = 18;
int clockPin = 5;
int dataPin = 19;

// IMU
int address = 0x68;
float AccelSensitiveScale = 2048.0; //+-2G

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


// Shift
byte mybyte = 0b00001100;
byte led_data = 0;

void updateShiftRegister(){
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, led_data);
  digitalWrite(latchPin, HIGH);
}

void setup() {
  Wire.begin(sda, scl);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  Serial.begin(9600);
}


void loop() {
  updateShiftRegister();

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
  
  ACCEL_YOUT_RAW = (ACCEL_YOUT_H << 8) | ACCEL_YOUT_L;
  gY = ACCEL_YOUT_RAW / AccelSensitiveScale;

  Serial.print(">gy:");
  Serial.println(gY*100);

  // Map gY (-1 to 1) to LED range (0 to 8)
  int ledCount = map(gY * 100, -800, 800, 0, 8); // Use scaling for precision

  // Update LED pattern
  led_data = 0; // Reset LEDs
  for (int i = 0; i < ledCount; i++) {
    led_data |= (1 << i); // Turn on LEDs progressively
  }

  updateShiftRegister();

  delay(100);
}