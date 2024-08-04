#include <MD_MAX72xx.h>

#define VERT_PIN 28
#define HORZ_PIN 27
#define SEL_PIN  26

#define	CLK_PIN		6
#define	DATA_PIN	4
#define	CS_PIN		5

const int maxX = 7;
const int maxY = 7;

int x = 0;
int y = 0;

MD_MAX72XX mx = MD_MAX72XX(MD_MAX72XX::PAROLA_HW, CS_PIN, MAX_DEVICES);

void setup() {
  Serial1.begin(115200);

  mx.begin();
  mx.control(MD_MAX72XX::INTENSITY, MAX_INTENSITY / 2);
  mx.clear();

  pinMode(VERT_PIN, INPUT);
  pinMode(HORZ_PIN, INPUT);
  pinMode(SEL_PIN, INPUT_PULLUP);
}

// the loop function runs over and over again forever
void loop() {
  int horz = analogRead(HORZ_PIN);
  int vert = analogRead(VERT_PIN);
  if (vert < 300) {
    y = min(y + 1, maxY);
  }
  if (vert > 700) {
    y = max(y - 1, 0);
  }
  if (horz > 700) {
    x = min(x + 1, maxX);
  }
  if (horz < 300) {
    x = max(x - 1, 0);
  }
  if (digitalRead(SEL_PIN) == LOW) {
    Serial1.println("Button pressed");
  }

  drawSnake();

  Serial1.print(x);
  Serial1.println(y);
  delay(100);
}

void drawSnake(){
  mx.clear();
  mx.setPoint(y, x);
  mx.update();
}
