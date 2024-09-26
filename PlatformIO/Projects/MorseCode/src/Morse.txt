#include <Arduino.h>

const int ledPin = 23; // Default LED is connected to GPIO 23

const char *morseCode[] = {
    "-----",".----","t---","...--","....-", 
    ".....","-....","--...","---..","----."
};

void morseCodeConverter(char c){
  if (c >= '0' && c <= '9')
  {
    int index = c - '0'; //Why it need to sub 0
    for (int i = 0; morseCode[index][i] != '\0'; i++){
      if (morseCode[index][i] == '.'){
        digitalWrite(ledPin, HIGH);
        delay(100);
        Serial.print(morseCode[index][i]);
      } else if (morseCode[index][i] == '-')
      {
        digitalWrite(ledPin, HIGH);
        delay(300);
        Serial.print(morseCode[index][i]);
      }
      digitalWrite(ledPin, LOW);
      delay(100);
    } 
    delay(300);
  }
}

void setup(){
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
  // Read the incoming byte
  char incomingByte = Serial.read();
  Serial.print("You entered: ");
  Serial.println(incomingByte);
  morseCodeConverter(incomingByte);
  }
}