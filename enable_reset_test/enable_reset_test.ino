#include <Adafruit_NeoPixel.h>

#define BLUELIGHT 6
#define REDLIGHT 7
#define BLUEGOAL 8
#define REDGOAL 9
#define RESET 10
#define ENABLE 11
#define COLOR_BLUE blueSide.Color(0, 0, 255)
#define COLOR_RED redSide.Color(255, 0, 0)

Adafruit_NeoPixel redSide = Adafruit_NeoPixel(20, REDLIGHT, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel blueSide = Adafruit_NeoPixel(20, BLUELIGHT, NEO_GRB + NEO_KHZ800);

int pixelNum = 20; // number of LEDs on goal strip
int idleBrightness = 150; // 0-255 idle brightness setting

volatile int enableState = 0;

int maxScore = 9; // score value for win, max is 9 on single digit displays
int redScore = 0; // red score counter
int blueScore = 0; // blue score counter

void setup() {
  for (int i = 6; i < 12; i++) {
    pinMode(i, INPUT_PULLUP);
  }
  
  Serial.begin(9600);
  redSide.begin();
  blueSide.begin();
  redSide.show();
  blueSide.show();

  attachInterrupt(0, enable_ISR, FALLING);
}

void enable_ISR() {
  enableState = digitalRead(ENABLE);
}

void loop() {
  if (enableState == 1) Serial.println("scoring enabled");
  else if (enableState == 0) Serial.println("scoring disabled");
}
