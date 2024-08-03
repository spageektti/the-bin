#include <IRremote.h>
#include <TM1637.h>
#include <RTClib.h>

#define RECEIVER 12
#define CLK 2
#define DIO 3
#define ALARM_BUTTON_PIN 28
#define BUZZER_PIN 27

TM1637 tm(CLK, DIO);
RTC_DS1307 rtc;

int currentDigit = 0;
int alarmDigits[] = {0, 0, 0, 0};
bool alarmSet = false;
bool displayTimeFlag = true;
bool alarmRinging = false;
const int timezoneOffset = 2; // offset from UTC, default 2 because I'm from Poland

void setup() {
  Serial1.begin(115200);

  IrReceiver.begin(RECEIVER, ENABLE_LED_FEEDBACK);

  tm.init();
  tm.set(BRIGHT_TYPICAL);

  if (! rtc.begin()) {
    Serial1.println("No RTC");
    Serial1.flush();
    while (1) delay(10);
  }

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)) + TimeSpan(timezoneOffset * 3600));

  pinMode(ALARM_BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  if (IrReceiver.decode()) {
    switch (IrReceiver.decodedIRData.command) {
      case 168:
        stopEditing();
        break;
      case 224:
        switchToPreviousDigit();
        break;
      case 144:
        switchToNextDigit();
        break;
      case 2:
        addToDigit();
        break;
      case 152:
        substractFromDigit();
        break;
      case 104:
        setDigit(0);
        break;
      case 48:
        setDigit(1);
        break;
      case 24:
        setDigit(2);
        break;
      case 122:
        setDigit(3);
        break;
      case 16:
        setDigit(4);
        break;
      case 56:
        setDigit(5);
        break;
      case 90:
        setDigit(6);
        break;
      case 66:
        setDigit(7);
        break;
      case 74:
        setDigit(8);
        break;
      case 82:
        setDigit(9);
        break;
      default:
        Serial1.print("Other signal ");
        Serial1.println(IrReceiver.decodedIRData.command);
    }
    IrReceiver.resume(); // Receive the next value
  }

  if (alarmSet && !alarmRinging) {
    checkAlarm();
  }

  if (digitalRead(ALARM_BUTTON_PIN) == LOW) {
    stopAlarm();
    alarmSet = false;
  }

  if (displayTimeFlag) {
    displayTime();
  } else {
    displayOnScreen();
  }
}

void stopEditing() {
  alarmSet = true;
  Serial1.print("Alarm set to: ");
  for (int i = 0; i < 4; i++) {
    if (i == 2) Serial1.print(":");
    Serial1.print(alarmDigits[i]);
  }
  Serial1.println();
  displayTimeFlag = true;
}

void switchToPreviousDigit() {
  currentDigit = (currentDigit - 1 + 4) % 4;
  Serial1.print("Switched to previous digit: ");
  Serial1.println(currentDigit);
  displayTimeFlag = false;
}

void switchToNextDigit() {
  currentDigit = (currentDigit + 1) % 4;
  Serial1.print("Switched to next digit: ");
  Serial1.println(currentDigit);
  displayTimeFlag = false;
}

void addToDigit() {
  alarmDigits[currentDigit] = (alarmDigits[currentDigit] + 1) % 10;
  Serial1.print("Added to digit: ");
  Serial1.println(alarmDigits[currentDigit]);
  displayTimeFlag = false;
}

void substractFromDigit() {
  alarmDigits[currentDigit] = (alarmDigits[currentDigit] - 1 + 10) % 10;
  Serial1.print("Subtracted from digit: ");
  Serial1.println(alarmDigits[currentDigit]);
  displayTimeFlag = false;
}

void setDigit(int digit) {
  alarmDigits[currentDigit] = digit;
  Serial1.print("Set digit to: ");
  Serial1.println(digit);
  displayTimeFlag = false;
}

void displayOnScreen() {
  for (int i = 0; i < 4; i++) {
    tm.display(i, alarmDigits[i]);
  }
}

void displayTime() {
  DateTime now = rtc.now();

  tm.display(0, now.hour() / 10);
  tm.display(1, now.hour() % 10);

  tm.display(2, now.minute() / 10);
  tm.display(3, now.minute() % 10);
}

void checkAlarm() {
  DateTime now = rtc.now();
  int currentHour = now.hour();
  int currentMinute = now.minute();

  int alarmHour = alarmDigits[0] * 10 + alarmDigits[1];
  int alarmMinute = alarmDigits[2] * 10 + alarmDigits[3];

  if (currentHour == alarmHour && currentMinute == alarmMinute) {
    startAlarm();
  }
}

void startAlarm() {
  alarmRinging = true;
  Serial1.println("Alarm ringing!");
  digitalWrite(BUZZER_PIN, HIGH);
}

void stopAlarm() {
  if (alarmRinging) {
    alarmRinging = false;
    Serial1.println("Alarm stopped");
    digitalWrite(BUZZER_PIN, LOW);
  }
}
