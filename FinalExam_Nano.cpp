#include <Arduino.h>
#include <Wire.h> 

int latchPin = 5;  // Latch pin of 74HC595 is connected to Digital pin 5
int clockPin = 6; // Clock pin of 74HC595 is connected to Digital pin 6
int dataPin = 4;  // Data pin of 74HC595 is connected to Digital pin 4

// Shift Register
byte mybyte = 0b00001100;
byte led_data = 0;
int ledCount = 8;

void updateShiftRegister(){
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, led_data);
  digitalWrite(latchPin, HIGH);
}

void setup(){
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT); 
  pinMode(clockPin, OUTPUT)
}


void loop() {
  updateShiftRegister();

  led_data = 0;

  for (int i = 0; i < ledCount; i+=2) {
    led_data |= (1 << i);
  }

  updateShiftRegister();
  delay(1000);

  led_data = 0;

  for (int i = 1; i < ledCount; i+=2) {
    led_data |= (1 << i);
  }
  
  updateShiftRegister();
  delay(1000);
}