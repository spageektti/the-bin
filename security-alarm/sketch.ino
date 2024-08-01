#include <Servo.h>
#include <Keypad.h>

#define MOTION_SENSOR_PIN 22
#define MOTOR_PIN 15
#define TRIG_PIN 3
#define ECHO_PIN 2
#define BUZZER_PIN 26

#define ROTATION_LEFT 90
#define ROTATION_RIGHT 90
#define DISTANCE 100 // if distance lower than this the alarm will start
#define MOTOR_DELAY 20 // milliseconds
#define MOTOR_STEP 10
#define TIME_BETWEEN_SCANS 1000

Servo servo;
int position = ROTATION_LEFT;

const byte ROWS = 4; 
const byte COLS = 4; 
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {6, 7, 8, 9}; 
byte colPins[COLS] = {21, 20, 19, 18}; 

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

String inputPIN;
const String correctPIN = "1A2B3C4D"; // very secure PIN btw
bool alarmActive = false;

void checkPIRSensor(){
  bool motionDetected = digitalRead(MOTION_SENSOR_PIN);
  Serial1.print("Motion detected? ");
  Serial1.println(motionDetected);
  if(motionDetected){
    Serial1.println("Alarm!");
    alarmActive = true;
  }
}

void checkDistanceSensor(){
  long duration;
  int distance;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);

  distance = duration * 0.034 / 2; // speed of sound wave divided by 2 (go and back)

  Serial1.print("Distance: ");
  Serial1.println(distance);

  if (distance < DISTANCE) {
    Serial1.println("Alarm!");
    alarmActive = true;
  }
}

void triggerAlarm(){
  while(alarmActive){
    digitalWrite(BUZZER_PIN, HIGH);
    char key = keypad.getKey();
    if (key){
      Serial1.println(key);
      if (key == '#') {
        if (inputPIN == correctPIN) {
          alarmActive = false;
          inputPIN = "";
          Serial1.println("Alarm deactivated.");
        } else {
          inputPIN = "";
          Serial1.println("Incorrect PIN. Try again.");
        }
      } else {
        inputPIN += key;
      }
    }
  }
  digitalWrite(BUZZER_PIN, LOW);
}

void setup() {
  Serial1.begin(115200);
  Serial1.println("Hello, Raspberry Pi Pico W!");

  pinMode(MOTION_SENSOR_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  servo.attach(MOTOR_PIN);
}

void loop() {
  for(position = ROTATION_LEFT; position >= 0; position -= MOTOR_STEP){
    checkDistanceSensor();
    checkPIRSensor();
    servo.write(position);
    delay(MOTOR_DELAY);
     if (alarmActive) {
      triggerAlarm();
    }
  }
  for(position = 0; position <= ROTATION_LEFT + ROTATION_RIGHT; position += MOTOR_STEP){
    checkDistanceSensor();
    checkPIRSensor();
    servo.write(position);
    delay(MOTOR_DELAY);
     if (alarmActive) {
      triggerAlarm();
    }
  }
  for(position = ROTATION_LEFT + ROTATION_RIGHT; position >= ROTATION_LEFT; position -= MOTOR_STEP){
    checkDistanceSensor();
    checkPIRSensor();
    servo.write(position);
    delay(MOTOR_DELAY);
     if (alarmActive) {
      triggerAlarm();
    }
  }
  delay(TIME_BETWEEN_SCANS);
}
