#include <LiquidCrystal.h>

const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int buttonPin = 8;
const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;
const int encoderPinA = 12;
const int encoderPinB = 13;

int carPosition = 1;
int obstaclePositions[3] = {15, 15, 15}; // positions of obstacles
int obstacleRows[3] = {0, 0, 0}; // rows of obstacles
int numObstacles = 1; // number of obstacles
bool gameOver = false;
unsigned long lastMoveTime = 0;
const unsigned long moveInterval = 100;

int lastEncoderA = LOW;
int lastEncoderB = LOW;

void setup() {
  lcd.begin(16, 2);

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);

  lcd.setCursor(0, 0);
  lcd.print("Car Driving Game");
  delay(2000);
  lcd.clear();

  setLEDColor(0, 255, 0);

  lcd.setCursor(0, 1);
  lcd.print(">");

  numObstacles = random(1, 3);

  int lastPosition = 8;
  for (int i = 0; i < numObstacles; i++) {
    obstaclePositions[i] = random(5, lastPosition + 1);
    lastPosition = obstaclePositions[i] - 4;
    obstacleRows[i] = random(0, 2);
  }
}

void loop() {
  if (gameOver) {
    return;
  }

  if (millis() - lastMoveTime >= moveInterval) {
    lastMoveTime = millis();
    moveObstacles();
  }

  int buttonState = digitalRead(buttonPin);
  if (buttonState == LOW) {
    changeCarPosition();
    delay(200);
  }

  int encoderA = digitalRead(encoderPinA);
  int encoderB = digitalRead(encoderPinB);

  if (encoderA != lastEncoderA) {
    if (encoderB != encoderA) {
      changeCarPosition();
    }
    lastEncoderA = encoderA;
  }

  for (int i = 0; i < numObstacles; i++) {
    if (obstaclePositions[i] == 0 && carPosition == obstacleRows[i]) {
      gameOver = true;
      setLEDColor(255, 0, 0);
      lcd.setCursor(0, 1);
      lcd.print("Game Over!      ");
    }
  }
}

void moveObstacles() {
  lcd.clear();

  for (int i = 0; i < numObstacles; i++) {
    if (obstaclePositions[i] >= 0 && obstaclePositions[i] < 16) {
      lcd.setCursor(obstaclePositions[i], obstacleRows[i]);
      lcd.print("*");
    }

    if (obstaclePositions[i] > 0) {
      obstaclePositions[i]--;
    } else {
      obstaclePositions[i] = random(15, 17);
      obstacleRows[i] = random(0, 2);
    }
  }

  lcd.setCursor(0, carPosition);
  lcd.print(">");

  for (int i = 0; i < numObstacles; i++) {
    if (obstaclePositions[i] == 0 && carPosition == obstacleRows[i]) {
      gameOver = true;
      setLEDColor(255, 0, 0);
      lcd.setCursor(0, 1);
      lcd.print("Game Over!      ");
    }
  }
}

void changeCarPosition() {
  lcd.setCursor(0, carPosition);
  lcd.print(" ");
  carPosition = (carPosition == 1) ? 0 : 1;
  lcd.setCursor(0, carPosition);
  lcd.print(">");
}

void setLEDColor(int red, int green, int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}