// Detect motion using PIR sensor and Ultrasonic Distance Sensor. The Ultrasonic one can rotate 360 or 180 degrees.
#define MOTION_SENSOR_PIN 22
#define MOTOR_PIN 15
#define TRIG_PIN 3
#define ECHO_PIN 2
#define ROTATION_LEFT 90
#define ROTATION_RIGHT 90
#define DISTANCE = 100 // if distance lower than this the alarm will start
#define MOTOR_DELAY 20 // miliseconds
#define MOTOR_STEP 10

#include <Servo.h>
Servo servo;

int position = ROTATION_LEFT;

void checkPIRSensor(){
  bool motionDetected = digitalRead(MOTION_SENSOR_PIN);
  Serial1.println(motionDetected);
}
void checkDistanceSensor(){
  
}
void setup() {
  // put your setup code here, to run once:
  Serial1.begin(115200);
  Serial1.println("Hello, Raspberry Pi Pico W!");

  pinMode(MOTION_SENSOR_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  servo.attach(MOTOR_PIN);
}
void loop() {
  checkPIRSensor();
  for(position = ROTATION_LEFT; position >= 0; position -= MOTOR_STEP){
    checkDistanceSensor();
    servo.write(position);
    delay(MOTOR_DELAY);
  }
  for(position; position <= ROTATION_LEFT + ROTATION_RIGHT; position += MOTOR_STEP){
    checkDistanceSensor();
    servo.write(position);
    delay(MOTOR_DELAY);
  }
  for(position; position >= ROTATION_LEFT; position -= MOTOR_STEP){
    checkDistanceSensor();
    servo.write(position);
    delay(MOTOR_DELAY);
  }
  checkPIRSensor();
  delay(1000);
}
