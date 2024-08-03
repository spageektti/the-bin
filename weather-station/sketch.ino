#include <Adafruit_NeoPixel.h>
#include <RTClib.h>
#include <LiquidCrystal.h>
#include <DHT.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define NEOPIXEL_PIN 12
#define BUTTON_PIN 7
#define DHT_PIN 8
#define DHT_TYPE DHT22
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""
#define TIMEZONE 2 // 0 = UTC | 1 = UTC+1 | -1 = UTC-1

#define RED_PIN 11
#define GREEN_PIN 14
#define BLUE_PIN 15

RTC_DS1307 rtc;
Adafruit_NeoPixel pixel(1, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
LiquidCrystal lcd(21, 20, 19, 18, 17, 16);
DHT dht(DHT_PIN, DHT_TYPE);

int currentScreen = 0; // 0 = home screen | 1 = DHT screen | 2 = weather screen
bool lastButtonState = HIGH;

void setup() {
  Serial1.begin(115200);

  pixel.begin();
  lcd.begin(16, 2);
  dht.begin();

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  if (!rtc.begin()) {
    Serial1.println("RTC not found.");
    Serial1.flush();
    abort();
  }

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)) + TimeSpan(TIMEZONE * 3600));

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial1.println("Connecting to WiFi...");
  }
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
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  uint32_t color = mapTemperatureToColor(temperature);
  pixel.clear();
  pixel.setPixelColor(0, color);
  pixel.show();

  setRGBLedColor(humidity);

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
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("https://api.open-meteo.com/v1/forecast?latitude=52.52&longitude=13.41&current_weather=true");
    int httpCode = http.GET();

    if (httpCode > 0) {
      String payload = http.getString();
      const size_t capacity = 1024;
      DynamicJsonDocument doc(capacity);
      DeserializationError error = deserializeJson(doc, payload);

      if (!error) {
        String weather = doc["current_weather"]["weathercode"];
        float temperature = doc["current_weather"]["temperature"];

        lcd.setCursor(0, 0);
        lcd.print("Weather: ");
        lcd.print(weather);

        lcd.setCursor(0, 1);
        lcd.print("Temp: ");
        lcd.print(temperature);
        lcd.print(" C");
      } else {
        lcd.setCursor(0, 0);
        lcd.print("Parse error");
      }
    } else {
      lcd.setCursor(0, 0);
      lcd.print("HTTP error");
    }
    http.end();
  } else {
    lcd.setCursor(0, 0);
    lcd.print("WiFi not");
    lcd.setCursor(0, 1);
    lcd.print("connected");
  }
}


uint32_t mapTemperatureToColor(float temperature) {
  int red = map(temperature, 0, 40, 0, 255);
  int blue = map(temperature, 0, 40, 255, 0);
  return pixel.Color(red, 0, blue);
}

void setRGBLedColor(float humidity) {
  int green = map(humidity, 0, 100, 0, 255);
  int red = map(humidity, 0, 100, 255, 0);
  int blue = map(humidity, 0, 100, 0, 255);

  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
}
