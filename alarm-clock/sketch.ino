#include <IRremote.h>

#define RECEIVER 26

int currentDigit = 0;
int alarmDigits[] = {0, 0, 0, 0};
bool alarmSet = false;

void setup() {
  Serial1.begin(115200);
  IrReceiver.begin(RECEIVER, ENABLE_LED_FEEDBACK);
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
}

void stopEditing() {
  alarmSet = true;
  Serial1.print("Alarm set to: ");
  for (int i = 0; i < 4; i++) {
    Serial1.print(alarmDigits[i]);
  }
  Serial1.println();
}

void switchToPreviousDigit() {
  currentDigit = (currentDigit - 1 + 4) % 4;
  Serial1.print("Switched to previous digit: ");
  Serial1.println(currentDigit);
}

void switchToNextDigit() {
  currentDigit = (currentDigit + 1) % 4;
  Serial1.print("Switched to next digit: ");
  Serial1.println(currentDigit);
}

void addToDigit() {
  alarmDigits[currentDigit] = (alarmDigits[currentDigit] + 1) % 10;
  Serial1.print("Added to digit: ");
  Serial1.println(alarmDigits[currentDigit]);
}

void substractFromDigit() {
  alarmDigits[currentDigit] = (alarmDigits[currentDigit] - 1 + 10) % 10;
  Serial1.print("Subtracted from digit: ");
  Serial1.println(alarmDigits[currentDigit]);
}

void setDigit(int digit) {
  alarmDigits[currentDigit] = digit;
  Serial1.print("Set digit to: ");
  Serial1.println(digit);
}
