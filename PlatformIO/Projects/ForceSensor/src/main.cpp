#include <Arduino.h>

// Force
const int Force1 = 32;
int fsrReading; 
float forceInNewtons;

void setup() {
  pinMode(Force1, INPUT);
  Serial.begin(115200);
}

float convertToForce(int adcValue) {
  // Map ADC value to force (adjust maxForce if needed)
  float maxForce = 98.0;  // Max force in Newtons (10kg)
  float maxAdcValue = 4095.0; // Max ADC value (12-bit resolution)
  return (adcValue / maxAdcValue) * maxForce;
}

void loop() {
  fsrReading = analogRead(Force1); // Read raw ADC value
  forceInNewtons = convertToForce(fsrReading); // Convert to Newtons

  Serial.print("Raw ADC Reading = ");
  Serial.print(fsrReading);
  Serial.print(", Force = ");
  Serial.print(forceInNewtons);
  Serial.println(" N");

  delay(500); // Wait 500ms
}
