#include <Arduino.h>
#include "pitches.h"

const int voltagePin = 23;
const int sensingPin = 36;
const int buzzer = 16;
const int ledPin = 21; // Default LED is connected to GPIO 21

int melody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7, // First part
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0, 
  NOTE_G6, 0, 0, 0,
  
  NOTE_C7, 0, 0, NOTE_G6, 0, NOTE_E6, 0, NOTE_A6, NOTE_B6, 
  NOTE_AS6, NOTE_A6, NOTE_G6, NOTE_E7, NOTE_G7, NOTE_A7, 0, 
  NOTE_F7, NOTE_G7, 0, NOTE_E7, 0, NOTE_C7, NOTE_D7, NOTE_B6
};

int noteDurations[] = {
  8, 8, 8, 8,
  8, 8, 8, 8,
  8, 8, 8, 8,
  8, 8, 8, 8,

  8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8
};

int win[] = {
  NOTE_G5, NOTE_C6, NOTE_E6, NOTE_G6, NOTE_C7, NOTE_E7, NOTE_G7, NOTE_E7, 0,
  NOTE_GS5, NOTE_C6, NOTE_DS6, NOTE_GS6, NOTE_C7, NOTE_DS7, NOTE_GS7, NOTE_E7, 0,
  NOTE_AS5, NOTE_D6, NOTE_F6, NOTE_AS6, NOTE_D7, NOTE_F7, NOTE_AS7, NOTE_B7, NOTE_B7, NOTE_B7, NOTE_C8
};

int winDurations[] = {
  8, 8, 8, 8, 8, 8, 4, 8, 8, 
  8, 8, 8, 8, 8, 8, 4, 8, 8,
  8, 8, 8, 8, 8, 8, 4, 8, 8, 8, 2
};

int lose[] = {
  NOTE_B5, NOTE_F6, NOTE_F6, NOTE_F6, NOTE_E6, NOTE_D6,  NOTE_C5,  0
};

int loseDurations[] = {
  8, 4, 8, 4, 4, 4, 4, 8 // All notes have the same duration
};

// Function to play any melody
void playMelody(int *melody, int *noteDurations, int size) {
  for (int thisNote = 0; thisNote < size; thisNote++) {
    if (noteDurations[thisNote] > 0) { // Check to avoid division by zero
      int noteDuration = 1000 / noteDurations[thisNote];
      tone(buzzer, melody[thisNote], noteDuration);
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      noTone(buzzer);
    } else {
      Serial.print("Warning: Division by zero avoided at note ");
      Serial.println(thisNote);
    }
  }
}

void gameStart(){
  playMelody(melody, noteDurations, 40);
  Serial.println("###### Welcome to Number Guessing Game ######");
  Serial.println("###### Change the ambient light to approximate the target ######");
}

void setup(){
  pinMode(voltagePin, OUTPUT);
  pinMode(sensingPin, INPUT); //Optional
  pinMode(ledPin, OUTPUT);
  pinMode(buzzer, OUTPUT);
  
  digitalWrite(voltagePin, HIGH); //3.3V on all the time

  // Initialize LEDC
  ledcSetup(0, 5000, 8); // Channel 0, 5 kHz frequency, 8-bit resolution
  
  Serial.begin(9600);
}


bool gameStarted = false;
bool gameResult = false;
static unsigned long startTime = 0;

void loop() {
  
  noTone(buzzer);
  digitalWrite(ledPin, LOW);

  if (!gameStarted){
    gameStarted = true;
    gameResult = false;
    gameStart();

    // Step 1. Random number & game start & timer
    int target = random(255);
    Serial.print("target:");
    Serial.println(target);

    startTime = millis();

    while (millis() - startTime < 10000){
      // Step 2. Adjust buzzer
      int result = analogRead(sensingPin);
      int ledResult = (int)(result / 4095.0 * 255.0);
      analogWrite(ledPin, target);

      int difference = abs(target - ledResult);
      int pitch = NOTE_D8 - (difference * 20);
      if (pitch < NOTE_B0) { // Prevent pitch from going too low
        pitch = NOTE_B0;
      }

      tone(buzzer, pitch);

      Serial.print("difference:");
      Serial.println(difference);
      
      delay(100);
      

      if (difference <= 10){
        gameResult = true;
        break;
      }
    }

    if (!gameResult){
      Serial.println("You Lose");
      playMelody(lose, loseDurations, 8);
    } else{
      Serial.println("You Win");
      playMelody(win, winDurations, 29);
    }

    Serial.println("Press any key to restart...");
  }

  // Wait for key press to reset game
  if (Serial.available() > 0) {
    while (Serial.read() != -1) {
      // clear serial buffer
    }
    delay(500);
    gameStarted = false;
    startTime = 0;
    digitalWrite(ledPin, LOW);
  }
}