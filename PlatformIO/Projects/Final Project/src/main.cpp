#include <Arduino.h>
#include <Wire.h>

const int greenLed = 15;
const int redLed = 2;

// IMU
int address = 0x68;
const int sda = 21; //Data Pin
const int scl = 22; //Clock Pin

struct IMUData {
  byte high;
  byte low;
  float g;
};

IMUData accelX, accelY, accelZ;
IMUData gyroX, gyroY, gyroZ;

// Vibration Motor
const int vibrator1 = 19; // Left
const int vibrator2 = 18; // Right
const int vibrator3 = 17; // Front
const int vibrator4 = 16; // Back

// Define the dance move dictionary
const char* dance_moves[][6] = {
  {"L", "L", "R", "F", "R", "F"},
  {"L", "L", "R", "R", "F", "R"}
};

const int num_moves = 2; // Number of dance sequences


void readAccelerometerData() {
  // Read all accelerometer data in one transmission
  Wire.beginTransmission(address);
  Wire.write(0x3B);  // Start with accelerometer X-axis register address
  Wire.endTransmission();

  Wire.requestFrom(address, 6);  // Read 6 bytes for all axes

  accelX.high = Wire.read();
  accelX.low = Wire.read();
  accelY.high = Wire.read();
  accelY.low = Wire.read();
  accelZ.high = Wire.read();
  accelZ.low = Wire.read();
  
  // Calculate g-forces for accelerometer
  accelX.g = static_cast<int16_t>((accelX.high << 8) | accelX.low) / 16384.0;
  accelY.g = static_cast<int16_t>((accelX.high << 8) | accelX.low) / 16384.0;
  accelZ.g = static_cast<int16_t>((accelX.high << 8) | accelX.low) / 16384.0;

  Serial.print(">accelX:");
  Serial.println(accelX.g);
  Serial.print(">accelY:");
  Serial.println(accelY.g);
  Serial.print(">accelZ:");
  Serial.println(accelZ.g);
  delay(10);

  // Gyro
  Wire.beginTransmission(address);
  Wire.write(0x43);  // Start with accelerometer X-axis register address
  Wire.endTransmission();

  Wire.requestFrom(address, 6);  // Read 6 bytes for all axes

  gyroX.high = Wire.read();
  gyroX.low = Wire.read();
  gyroY.high = Wire.read();
  gyroY.low = Wire.read();
  gyroZ.high = Wire.read();
  gyroZ.low = Wire.read();
  
  // Calculate angular velocities for gyro (modify if needed)
  gyroX.g = static_cast<int16_t>((gyroX.high << 8) | gyroX.low) / 16384.0; // Assuming same conversion for gyro
  gyroY.g = static_cast<int16_t>((gyroY.high << 8) | gyroY.low) / 16384.0;
  gyroZ.g = static_cast<int16_t>((gyroZ.high << 8) | gyroZ.low) / 16384.0;

  Serial.print(">gyrox:");
  Serial.println(gyroX.g);
  Serial.print(">gyroy:");
  Serial.println(gyroY.g);
  Serial.print(">gyroz:");
  Serial.println(gyroZ.g);
  delay(10);
}

// Function to activate the corresponding motor
void activateMotor(char move) {
  switch (move) {
    case 'L':
      analogWrite(vibrator1, 10000); // Full speed
      Serial.println("Left");
      break;
    case 'R':
      analogWrite(vibrator2, 10000); // Full speed
      Serial.println("Right");
      break;
    case 'F':
      analogWrite(vibrator3, 10000); // Full speed
      Serial.println("Front");
      break;
    case 'B':
      analogWrite(vibrator4, 10000); // Full speed
      Serial.println("Back");
      break;
  }
}

void stopAllMotors() {
  analogWrite(vibrator1, 0);
  analogWrite(vibrator2, 0);
  analogWrite(vibrator3, 0);
  analogWrite(vibrator4, 0);
}

bool checkMovement(char move){
    readAccelerometerData(); 
    float threshold = 0.1;

    switch (move) {
      case 'L':
        return gyroX.g < -threshold;  // Left movement
      case 'R':
        return gyroX.g > threshold;   // Right movement
      case 'F':
        return gyroY.g > threshold;   // Forward movement
      case 'B':
        return gyroY.g < -threshold;  // Backward movement
    }
}

void setup(){
  Wire.begin(sda, scl);

  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);

  pinMode(vibrator1, OUTPUT);
  pinMode(vibrator2, OUTPUT);
  pinMode(vibrator3, OUTPUT);
  pinMode(vibrator4, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  for (int dance_move_index = 0; dance_move_index < num_moves; dance_move_index++) {
  Serial.print("Playing Dance Sequence ");
  Serial.println(dance_move_index + 1);

  // Iterate through each step in the dance move sequence
    for (int i = 0; dance_moves[dance_move_index][i][0] != '\0'; i++) {
      char move = dance_moves[dance_move_index][i][0];

      // Vibrate the corresponding motor based on the move
      activateMotor(move);

       // Wait until the user performs the correct movement
      while (!checkMovement(move)) {
        Serial.println("Incorrect movement!");
        digitalWrite(redLed, HIGH);  // Light up red LED
        delay(500);                  // Give time for user correction
        digitalWrite(redLed, LOW);
      }

      // If movement is correct, light up green LED
      Serial.println("Correct movement!");
      digitalWrite(greenLed, HIGH);
      delay(300);  // Keep green LED on for 300ms
      digitalWrite(greenLed, LOW);


      // Turn off all vibration motors
      stopAllMotors();

      // Delay between moves (adjust as needed)
      delay(500);
    }

    // Add a pause between dance sequences (optional)
    delay(1000);
  }
}