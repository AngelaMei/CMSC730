#include <Arduino.h>

int pos = 0;
int servoPin = 13;
int triggerPin = 5;
int echoPin = 18;

void distance(){
  long duration = 0, distance = -1;
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);

  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) * 0.0343;

  if(distance >= 20){
    Serial.println("Out of Range");
  } else {
    Serial.print(distance);
    Serial.println(" cm");
  }

}

void angle(int a){

  int pulseWidth = map(a, 0, 180, 500, 2600);
  digitalWrite(servoPin, HIGH);
  delayMicroseconds(pulseWidth);
  digitalWrite(servoPin, LOW);
  delayMicroseconds(20000 - pulseWidth); // 20ms 
}

void setup() {
  pinMode(servoPin, OUTPUT);
  pinMode(triggerPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600);

}

void loop() {

  for(pos = 0; pos<180; pos+=1){
    angle(pos);
    delay(15);
    distance();
  }
  delay(300);

  for(pos = 180; pos>0; pos-=1){
    angle(pos);
    delay(15);
    distance();
  }
  delay(300);
}




// Using Library
// #include <ESP32Servo.h>

// int pos = 0;
// int servoPin = 13;
// Servo myservo;


// void setup() {
//   myservo.attach(servoPin, 500, 2600);
// }

// void loop() {
//   for(pos = 0; pos<180; pos+=1){
//     myservo.write(pos);
//     delay(15);
//   }

//   delay(500);

//   for(pos = 180; pos>0; pos-=1){
//     myservo.write(pos);
//     delay(15);
//   }

//   delay(500);
// }