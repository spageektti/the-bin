#include <Adafruit_NeoPixel.h>
#include <RTClib.h>
#include <LiquidCrystal.h>

#define NEOPIXEL_PIN 12
#define BUTTON_PIN 7
#define TIMEZONE 2 // 0 = UTC | 1 = UTC+1 | -1 = UTC-1

RTC_DS1307 rtc;
Adafruit_NeoPixel pixel(1, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
LiquidCrystal lcd(16, 17, 18, 19, 20, 21);

int currentScreen = 0; // 0 = home screen | 1 = DHT screen | 2 = weather screen
bool lastButtonState = HIGH;

void setup() {
  Serial1.begin(115200);

  pixel.begin();
  lcd.begin(16, 2);

  if (!rtc.begin()) {
    Serial1.println("RTC not found.");
    Serial1.flush();
    abort();
  }

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)) + TimeSpan(TIMEZONE * 3600));

  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  pixel.clear();
  pixel.setPixelColor(0, pixel.Color(0, 150, 0));
  pixel.show();

  displayScreen();

  bool buttonState = digitalRead(BUTTON_PIN);
  if (buttonState == LOW && lastButtonState == HIGH) {
    delay(50); // Debounce delay
    if (digitalRead(BUTTON_PIN) == LOW) {
      currentScreen = (currentScreen + 1) % 3;
      while(digitalRead(BUTTON_PIN) == LOW); // wait for button release
    }
  }
  lastButtonState = buttonState;

  delay(500);
}

void displayScreen() {
  switch (currentScreen) {
    case 0:
      displayHomeScreen();
      break;
    case 1:
      displayDHTScreen();
      break;
    case 2:
      displayWeatherScreen();
      break;
  }
}

void displayHomeScreen() {
  lcd.clear();
  DateTime now = rtc.now();

  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  lcd.print(now.hour());
  lcd.print(":");
  lcd.print(now.minute());
  lcd.print(":");
  lcd.print(now.second());

  lcd.setCursor(0, 1);
  lcd.print("Date: ");
  lcd.print(now.day());
  lcd.print("/");
  lcd.print(now.month());
  lcd.print("/");
  lcd.print(now.year());
}

void displayDHTScreen() {
  lcd.clear();

  // replace with actual DHT readings
  float temperature = 25.0;
  float humidity = 50.0;

  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(humidity);
  lcd.print(" %");
}

void displayWeatherScreen() {
  lcd.clear();

  // replace with actual weather API data
  String weather = "Sunny";
  float temperature = 28.0;

  lcd.setCursor(0, 0);
  lcd.print("Weather: ");
  lcd.print(weather);

  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print(" C");
}
