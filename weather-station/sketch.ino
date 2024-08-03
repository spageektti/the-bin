#include <Adafruit_NeoPixel.h>
#include <RTClib.h>
#define NEOPIXEL_PIN 5
#define BUTTON_PIN 7
#define TIMEZONE 2 // 0 = UTC | 1 = UTC+1 | -1 = UTC-1

RTC_DS1307 rtc;
Adafruit_NeoPixel pixel(1, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial1.begin(115200);

  pixel.begin();

  if (! rtc.begin()) {
    Serial1.println("RTC not found.");
    Serial1.flush();
    while (1) delay(10);
  }

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)) + TimeSpan(TIMEZONE * 3600));

  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  pixel.clear();
  
  pixel.setPixelColor(0, pixel.Color(0, 150, 0));
  pixel.show();

  delay(500);
}
