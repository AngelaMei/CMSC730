#include <Arduino.h>
#include <cstdlib>

const int vibrator1 = 23;
const int vibrator2 = 22;
const int vibrator3 = 21;
int vibrators[3];

int pwmValue1 = 0;
int pwmValue2 = 0;
int pwmValue3 = 0;

void setup() {
  vibrators[0] = vibrator1;
  vibrators[1] = vibrator2;
  vibrators[2] = vibrator3;

  pinMode(vibrator1, OUTPUT);
  pinMode(vibrator2, OUTPUT);
  pinMode(vibrator3, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int random_id = rand() % 3;
  analogWrite(vibrators[random_id], 10000);
  Serial.print(random_id);
  delay(200);
  analogWrite(vibrators[random_id], 0);
  delay(1000);

  /*
  analogWrite(vibrator1, 10000);
  delay(200);
  analogWrite(vibrator1, 0);
  delay(1000);

  analogWrite(vibrator2, 10000);
  delay(200);
  analogWrite(vibrator2, 0);
  delay(1000);

  analogWrite(vibrator3, 10000);
  delay(200);
  analogWrite(vibrator3, 0);
  delay(1000);
  */

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