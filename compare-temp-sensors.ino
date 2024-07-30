// Now turn this trash into treasure!
#include <dht.h>

dht DHT;

float termsistorValtoCelsius(int thermsistorVal, float BETA){
  return 1 / (log(1 / (1023. / thermsistorVal - 1)) / BETA + 1.0 / 298.15) - 273.15;
}

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(115200);
  Serial1.println("Hello, Raspberry Pi Pico W!");
}
void loop() {
  // put your main code here, to run repeatedly:
  // thermsistor
  const float BETA = 3950; // should match the Beta Coefficient of the thermistor
  int thermsistorVal = analogRead(A0);
  float thermsistorCelsius = termsistorValtoCelsius(thermsistorVal, BETA);

  Serial1.print("thermsistor:");
  Serial1.println(thermsistorCelsius);

  DHT.read22(1);
  float DHTCelsius = DHT.temprerature;
  Serial1.print("DHT22:");
  Serial1.println(DHTCelsius);

  delay(1); // this speeds up the simulation
}
