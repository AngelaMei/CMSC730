#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>


// I2C
#define SSD1306_I2C_ADDRESS 0x3C // SSD1306 address
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define BUFFER_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT / 8)
uint8_t buffer[BUFFER_SIZE]; // Screen buffer
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

// IMU
const int sda = 21;
const int scl = 22;
int address = 0x68;
float AccelSensitiveScale = 2048.0; //+-2G

struct IMUData {
  byte high;
  byte low;
  float g;
  int16_t raw;
};

IMUData accelX, accelY, accelZ;

// Joystick
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
int threshold = 4000;
const int joyUpPin = 39;
const int joyDownPin = 36;
const int joyCenterPin = 35;
int difficultyLevel = 1;

// Game
int playerX, playerY;
int targetX, targetY;
int score = 0;
unsigned long startTime;
bool gamestatus = false;
float elapsedTime;

void readIMUData() {
  Wire.beginTransmission(address);
  Wire.write(0x3B);
  Wire.endTransmission();

  Wire.requestFrom(address, 6);

  accelX.high = Wire.read();
  accelX.low = Wire.read();
  accelY.high = Wire.read();
  accelY.low = Wire.read();
  accelZ.high = Wire.read();
  accelZ.low = Wire.read();
  
  // Calculate g-forces for accelerometer
  accelX.g = static_cast<int16_t>((accelX.high << 8) | accelX.low) / AccelSensitiveScale;
  accelY.g = static_cast<int16_t>((accelY.high << 8) | accelY.low) / AccelSensitiveScale;
  accelZ.g = static_cast<int16_t>((accelZ.high << 8) | accelZ.low) / AccelSensitiveScale;

//   Serial.print(">accelX:");
//   Serial.println(accelX.g);
//   Serial.print(">accelY:");
//   Serial.println(accelY.g);
//   Serial.print(">accelZ:");
//   Serial.println(accelZ.g);
  delay(10);
}

void updatePlayerPosition() {
  readIMUData();

  // Update player position based on acceleration
  playerX -= (int)(accelY.g * 100); // Adjust sensitivity as needed
  playerY -= (int)(accelX.g * 100);

  // Constrain player position to screen boundaries
  playerX = constrain(playerX, 0, SCREEN_WIDTH - 1);
  playerY = constrain(playerY, 0, SCREEN_HEIGHT - 1);
}

void generateTarget() {
  // Generate random target position
  targetX = random(SCREEN_WIDTH);
  targetY = random(SCREEN_HEIGHT);
}

void checkCollision() {
  if (abs(playerX - targetX) < 5 && abs(playerY - targetY) < 5) {
    score++;
    generateTarget();
  }
}

void updateDisplay() {
  display.clearDisplay();

  // Draw player and target
  display.drawPixel(playerX, playerY, WHITE);
  display.drawPixel(targetX, targetY, WHITE);

  // Display score and time
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Score: ");
  display.print(score);
  display.setCursor(0, 10);
  display.print("Time: ");
  display.print((millis() - startTime) / 1000);
  elapsedTime = (millis() - startTime) / 1000;

  // Update the display
  display.display();
}

void resetGame() {
  score = 0;
  startTime = millis();
  generateTarget();
}

void gameOver(bool win) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);

    if (win) {
        display.setCursor(0, 10);
        display.print("You Win!");
    } else {
        display.print("Game Over");
    }
    display.setCursor(0, 30);
    display.print("You use: ");
    display.print(elapsedTime);
    display.print("s");
    display.display();
  // Reset the game after a delay
  delay(3000);
  resetGame();
}

void startgame(int level){
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 10);
    display.print("Game Start");
    display.display();
    delay(1500);

    if (level == 1){
        display.clearDisplay();
        display.setTextSize(2);
        display.setCursor(0, 10);
        display.print("Score 10");
        display.setCursor(0, 30);
        display.print("in 30 sec");
        display.display();
        delay(1500);

        while(!gamestatus){
            updatePlayerPosition();
            checkCollision();
            updateDisplay();

            if (score >= 10) {
                gamestatus = true;
                gameOver(true); // Player wins

                break;
            } else if ((millis() - startTime) >= 30000) { // 30 seconds
                gameOver(false); // Player loses
                gamestatus = true;
                break;
            }
        }
    } else if (level == 2){
        display.clearDisplay();
        display.setTextSize(2);
        display.setCursor(0, 10);
        display.print("Score 10");
        display.setCursor(0, 30);
        display.print("in 20 sec");
        display.display();
        delay(1500);
        
        while(!gamestatus){
            updatePlayerPosition();
            checkCollision();
            updateDisplay();

            if (score >= 10) {
                gamestatus = true;
                gameOver(true); // Player wins
                break;
            } else if ((millis() - startTime) >= 20000) { // 20 seconds
                gameOver(false); // Player loses
                gamestatus = true;
                break;
            }
        }
    }
    
}


void setup(){
    Serial.begin(9600);

    // I2C
    Wire.begin(21, 22); // SDA, SCL pins
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();

    // Display the welcome message
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.print("Welcome to the game!");
    display.display();
    delay(3000);

    // IMU
    Wire.begin(sda, scl);
    Wire.beginTransmission(address);
    Wire.write(0x1C);
    Wire.write(0b00011000);
    Wire.endTransmission();

    // Joystick
    pinMode(joyUpPin, INPUT);
    pinMode(joyDownPin, INPUT);
    pinMode(joyCenterPin, INPUT);

    // Game
    playerX = SCREEN_WIDTH / 2;
    playerY = SCREEN_HEIGHT / 2;
    generateTarget();
    startTime = millis();
}

void loop() {
    // Read joystick input with debouncing
    int upValue = analogRead(joyUpPin);
    int downValue = analogRead(joyDownPin);
    int centerValue = analogRead(joyCenterPin);
    //   Serial.println("centerValue");
    //   Serial.print(centerValue);

    if (millis() - lastDebounceTime > debounceDelay) {
        lastDebounceTime = millis();

        if (centerValue > threshold){
            gamestatus = false;
            while (!gamestatus){
                startgame(difficultyLevel);
            }
        } else {
            if (upValue > threshold) {
                difficultyLevel++;
                if (difficultyLevel > 2) {
                difficultyLevel = 2;
                }
            } else if (downValue > threshold) {
                difficultyLevel--;
                if (difficultyLevel < 1) {
                difficultyLevel = 1;
                }
            }
        }

        // Display the current difficulty level
        display.clearDisplay();
        display.setTextSize(2);
        display.setCursor(0, 10);
        display.print("Level: ");
        display.setCursor(0, 30);
        display.print(difficultyLevel);
        display.display();
  }
}