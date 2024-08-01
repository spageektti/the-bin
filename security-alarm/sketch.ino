// Detect motion using PIR sensor and Ultrasonic Distance Sensor. The Ultrasonic one can rotate 360 or 180 degrees.
#define MOTION_SENSOR_PIN 22
#define MOTOR_PIN 15
#define TRIG_PIN 3
#define ECHO_PIN 2
#define ROTATION_LEFT 90
#define ROTATION_RIGHT 90
#define DISTANCE = 100 // if distance lower than this the alarm will start

#include <Servo.h>

Servo servo;

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(115200);
  Serial1.println("Hello, Raspberry Pi Pico W!");

  pinMode(MOTION_SENSOR_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}
void loop() {

  bool motionDetected = digitalRead(MOTION_SENSOR_PIN);
  Serial1.println(motionDetected);
  // put your main code here, to run repeatedly:
  delay(1000);
}
