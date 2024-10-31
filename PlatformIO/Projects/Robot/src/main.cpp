#include <Arduino.h>

int IN1 = 22;
int IN2 = 23;

void setup() {
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
}

void loop() {
  if (Serial.available()) {
    char incomingByte = Serial.read();
    Serial.print("You entered: ");
    Serial.println(incomingByte);

    // Use single equals for comparison
    if (incomingByte == 'l') {
      analogWrite(IN1, 255);
      analogWrite(IN2, 0);  // Turn left
    } else if (incomingByte == 'r') {
      analogWrite(IN1, 0);
      analogWrite(IN2, 255); // Turn right
    } else {
      // Stop the motor (both pins LOW)
      analogWrite(IN1, 0);
      analogWrite(IN2, 0);
    }
  }
}