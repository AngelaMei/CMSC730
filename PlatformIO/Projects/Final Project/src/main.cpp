#include <Arduino.h>

const int vibrator1 = 23;
const int vibrator2 = 22;
const int vibrator3 = 21;

int pwmValue1 = 0;
int pwmValue2 = 0;
int pwmValue3 = 0;

void setup() {
  pinMode(vibrator1, OUTPUT);
  pinMode(vibrator2, OUTPUT);
  pinMode(vibrator3, OUTPUT);
  Serial.begin(9600);
}

void loop() {

  analogWrite(vibrator1, 3);
  Serial.print("3");
  delay(1000);
  analogWrite(vibrator1, 0);
  delay(1000);
  analogWrite(vibrator1, 20);
  Serial.print("20");
  delay(1000);
  analogWrite(vibrator1, 0);
  delay(1000);
  analogWrite(vibrator1, 25);
  Serial.print("25");
  delay(1000);
  analogWrite(vibrator1, 0);
  delay(1000);
  analogWrite(vibrator1, 45);
  Serial.print("45");
  delay(1000);
  analogWrite(vibrator1, 0);
  delay(1000);
  analogWrite(vibrator1, 60);
  Serial.print("60");
  delay(1000);
  analogWrite(vibrator1, 0);
  delay(1000);
  analogWrite(vibrator2, 100);
  Serial.print("100");
  delay(1000);
  analogWrite(vibrator2, 0);
  delay(1000);
  analogWrite(vibrator3, 500);
  Serial.print("500");
  delay(1000);
  analogWrite(vibrator3, 0);
  delay(1000);
  analogWrite(vibrator3, 1000);
  Serial.print("1000");
  delay(1000);
  analogWrite(vibrator3, 0);
  delay(1000);
  analogWrite(vibrator3, 1500);
  Serial.print("1500");
  delay(1000);
  analogWrite(vibrator3, 0);
  delay(1000);
  analogWrite(vibrator3, 2000);
  Serial.print("2000");
  delay(1000);
  analogWrite(vibrator3, 0);
  delay(1000);
  analogWrite(vibrator3, 3000);
  Serial.print("3000");
  delay(1000);
  analogWrite(vibrator3, 0);
  delay(1000);





  // for (int i = 0; i < 1000; i+=100) {
  //   pwmValue1 = i;
  //   analogWrite(vibrator1, pwmValue1);
  // }

  // for (int i = 0; i < 5000; i+=100) {
  //   pwmValue2 = i * 2; // Vibrator 2 will vibrate at twice the frequency
  //   analogWrite(vibrator2, pwmValue2);
  //   delay(100);
  // }

  //   for (int i = 0; i < 10000; i+=100) {
  //   pwmValue2 = i * 3; // Vibrator 2 will vibrate at twice the frequency
  //   analogWrite(vibrator3, pwmValue3);
  //   delay(100);
  // }

}