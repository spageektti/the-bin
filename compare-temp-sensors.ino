#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHT_PIN A0
#define DHT_TYPE DHT22

DHT dht(DHT_PIN, DHT_TYPE);

float thermistorValtoCelsius(int thermistorVal, float BETA){
  return 1 / (log(1 / (1023.0 / thermistorVal - 1)) / BETA + 1.0 / 298.15) - 273.15;
}

void setup() {
  Serial1.begin(115200);
  Serial1.println("Hello, Raspberry Pi Pico W!");
  dht.begin();
}

void loop() {
  const float BETA = 3950; // should match the Beta Coefficient of the thermistor
  int thermistorVal = analogRead(A1);
  float thermistorCelsius = thermistorValtoCelsius(thermistorVal, BETA);

  Serial1.print("Thermistor: ");
  Serial1.println(thermistorCelsius);

  float DHTCelsius = dht.readTemperature();
  Serial1.print("DHT22: ");
  Serial1.println(DHTCelsius);

  delay(2000); // 2 seconds delay between readings
}
