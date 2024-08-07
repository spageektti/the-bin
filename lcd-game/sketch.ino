#include <LiquidCrystal.h>

const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int buttonPin = 8;
const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;

int carPosition = 1; // 1 for bottom row
int obstaclePosition = 15; // obstacle starts from the rightmost position
int obstacleRow = 0; // 0 for top row, 1 for bottom row
bool gameOver = false;
unsigned long lastMoveTime = 0;
const unsigned long moveInterval = 100;

void setup() {
  lcd.begin(16, 2);

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  lcd.setCursor(0, 0);
  lcd.print("Car Driving Game");
  delay(2000);
  lcd.clear();

  setLEDColor(0, 255, 0);

  lcd.setCursor(0, 1);
  lcd.print(">");
}

void loop() {
  if (gameOver) {
    return;
  }

  if (millis() - lastMoveTime >= moveInterval) {
    lastMoveTime = millis();
    moveObstacle();
  }

  int buttonState = digitalRead(buttonPin);
  if (buttonState == LOW) {
    changeCarPosition();
    delay(200);
  }

  if (obstaclePosition == 0 && carPosition == obstacleRow) {
    gameOver = true;
    setLEDColor(255, 0, 0);
    lcd.setCursor(0, 1);
    lcd.print("Game Over!      ");
  }
}

void moveObstacle() {
  lcd.setCursor(obstaclePosition, obstacleRow);
  lcd.print(" ");

  if (obstaclePosition > 0) {
    obstaclePosition--;
  } else {
    obstaclePosition = 15;
    obstacleRow = random(0, 2);
  }

  lcd.setCursor(obstaclePosition, obstacleRow);
  lcd.print("*");

  lcd.setCursor(0, carPosition);
  lcd.print(">");
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
