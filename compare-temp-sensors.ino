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

  // Delay before next reading
  delay(2000); // 2 seconds delay between readings
}
