#include <Arduino.h>
#include <Wire.h>

const int sda = 21; //Data Pin
const int scl = 22; //Clock Pin

// Temperture
int address = 0x48;

void setup() {
  Wire.begin(sda, scl);
  Serial.begin(9600);
}

void loop() {
  Wire.beginTransmission(address);
  Wire.write(0x00);
  Wire.endTransmission(false);

  Wire.requestFrom(address, 1);
  Serial.print(">temperature:");

  int8_t temperature;
  if (Wire.available()){
        temperature = Wire.read();
        Serial.println(temperature);
        Wire.endTransmission();
    }

  delay(2000);
}
