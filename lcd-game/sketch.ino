#include <LiquidCrystal.h>

const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int buttonPin = 8;
const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;

int carPosition = 0; // 0 for left, 1 for right
bool gameOver = false;

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

  int buttonState = digitalRead(buttonPin);
  if (buttonState == LOW) {
    changeCarPosition();
    delay(200);
  }
}

void changeCarPosition() {
  carPosition = (carPosition == 0) ? 1 : 0;
  lcd.setCursor(carPosition == 0 ? 0 : 15, 1);
  lcd.print(">");
  lcd.setCursor(carPosition == 0 ? 15 : 0, 1);
  lcd.print(" ");
}

void setLEDColor(int red, int green, int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}
