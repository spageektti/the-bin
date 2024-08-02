#include <Adafruit_NeoPixel.h>
#define PIN 12
#define NUMPIXELS 1
#define MOTION_SENSOR 18
#define RELAY 14
#define BUTTON 2
#define DELAY 10000

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial1.begin(115200);

  pixels.begin();

  pinMode(MOTION_SENSOR, INPUT);
  pinMode(RELAY, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
}

void loop() {
  pixels.clear();
  if(digitalRead(MOTION_SENSOR)){
  pixels.setPixelColor(0, pixels.Color(0, 150, 0));
  pixels.show();
  digitalWrite(RELAY, HIGH);
  Serial1.println("Active");

  // need to do it this way instead of delay() bc of the button
  unsigned long startMillis = millis();
  while (millis() - startMillis < DELAY) {
    if (digitalRead(BUTTON) == LOW) {
      Serial1.println("Button pressed");
      
      break;
      }
      delay(100);
    }
  } else{
    pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // can change the color to red (0, 0, 150) or disable the red (0, 0, 0)
    pixels.show();
    digitalWrite(RELAY, LOW);
    Serial1.println("Not active");
  }
  
  
  delay(500);
}
