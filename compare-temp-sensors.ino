#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Pin definitions
#define DHT_PIN A0
#define DHT_TYPE DHT22
#define ONE_WIRE_BUS 15
#define THERMISTOR_PIN A1
#define BUTTON_PIN 11
#define NUM_LEDS 10

// Button and LED bar
volatile int displayMode = 0; // 0 = Thermistor, 1 = DHT22, 2 = DS18B20
const int ledPins[NUM_LEDS] = {3, 4, 5, 6, 7, 8, 9, 10, 12, 13};
void handleButtonPress() {
  displayMode = (displayMode + 1) % 3;
}
void displayTemperatureOnLEDBarGraph(float temperature) {
  // Map temperature range to number of LEDs lit
  int numLedsLit = map(temperature, 0, 50, 0, NUM_LEDS); // Adjust range as needed

  // Update LED bar graph
  for (int i = 0; i < NUM_LEDS; i++) {
    if (i < numLedsLit) {
      digitalWrite(ledPins[i], HIGH);
    } else {
      digitalWrite(ledPins[i], LOW);
    }
  }
}

// DHT setup
DHT dht(DHT_PIN, DHT_TYPE);

// OneWire and DS18B20 setup
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

float thermistorValtoCelsius(int thermistorVal, float BETA) {
  return 1 / (log(1 / (1023.0 / thermistorVal - 1)) / BETA + 1.0 / 298.15) - 273.15;
}

void setup() {
  // Initialize Serial1 communication
  Serial1.begin(115200);
  Serial1.println("Hello, Raspberry Pi Pico W!");

  // Initialize DHT sensor
  dht.begin();

  // Initialize DS18B20 sensor
  pinMode(ONE_WIRE_BUS, INPUT_PULLUP);
  sensors.begin();

  // Initialize LED Bar pins
  for (int i = 0; i < NUM_LEDS; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  // Initialize button
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleButtonPress, FALLING);
}

void loop() {
  // Thermistor reading
  const float BETA = 3950; // Beta coefficient of the thermistor
  int thermistorVal = analogRead(THERMISTOR_PIN);
  float thermistorCelsius = thermistorValtoCelsius(thermistorVal, BETA);

  Serial1.print("Thermistor: ");
  Serial1.println(thermistorCelsius);

  // DHT22 reading
  float dhtCelsius = dht.readTemperature();
  Serial1.print("DHT22: ");
  Serial1.println(dhtCelsius);

  // DS18B20 reading
  sensors.requestTemperatures();
  float ds18b20Celsius = sensors.getTempCByIndex(0);
  Serial1.print("DS18B20: ");
  Serial1.println(ds18b20Celsius);

  // Get the temperature to display
  float temperatureToDisplay;
  switch (displayMode) {
    case 0:
      temperatureToDisplay = thermistorCelsius;
      Serial1.print("Displaying Thermistor Temperature: ");
      break;
    case 1:
      temperatureToDisplay = dhtCelsius;
      Serial1.print("Displaying DHT22 Temperature: ");
      break;
    case 2:
      temperatureToDisplay = ds18b20Celsius;
      Serial1.print("Displaying DS18B20 Temperature: ");
      break;
  }

  // Update LED bar graph
  displayTemperatureOnLEDBarGraph(temperatureToDisplay);

  // Delay before next reading
  delay(100); // 2 seconds delay between readings
}
