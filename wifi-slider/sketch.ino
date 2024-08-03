#include <Servo.h>
#include <WiFi.h>

#define SLIDER_PIN A0
#define SERVO_PIN 15

int sliderVal, servoVal, LEDBarVal;

Servo servo;

const int LEDBarPins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

const char* ssid = "Wokwi-GUEST";
const char* password = "";

WiFiServer server(80);

void setup() {
  Serial1.begin(115200);
  servo.attach(SERVO_PIN);

  for (int i = 0; i < 10; i++) {
    pinMode(LEDBarPins[i], OUTPUT);
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial1.print(".");
  }
  
  Serial1.println("Wi-Fi connected");
  Serial1.print("IP Address: ");
  Serial1.println(WiFi.localIP());

  server.begin();
}

void loop() {
  sliderVal = analogRead(SLIDER_PIN);
  servoVal = map(sliderVal, 0, 1023, 0, 180);
  LEDBarVal = map(sliderVal, 0, 1023, 0, 10);

  servo.write(servoVal);

  for (int i = 0; i < 10; i++) {
    if (i < LEDBarVal) {
      digitalWrite(LEDBarPins[i], HIGH);
    } else {
      digitalWrite(LEDBarPins[i], LOW);
    }
  }

  WiFiClient client = server.available();
  if (client) {
    Serial1.println("New Client connected");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial1.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.print("Slider Value: ");
            client.print(sliderVal);
            client.println("<br>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
    Serial1.println("Client disconnected");
  }

  delay(100);
}
