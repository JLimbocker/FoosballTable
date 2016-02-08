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

int enable = 0;
int oldEnable = 0;
int state = 0;

int reset = 0;

int maxScore = 9; // score value for win, max is 9 on single digit displays
int redScore = 0; // red score counter
int blueScore = 0; // blue score counter

void setup() {
  // initialize pin modes
  for (int i = 6; i < 12; i++) {
    pinMode(i, INPUT_PULLUP);
  }
  
  // initialize serial connection, turn on LEDs
  Serial.begin(9600);
  redSide.begin();
  blueSide.begin();
  redSide.show();
  blueSide.show();
}

// flash lights when scored on
// input params:
//  @repeat - how many times to flash
//  @wait - flash length/pause length between flashes
//  @side - red(0) or blue(1)
void flash(int repeat, int wait, int side) {
  for(int i=0; i<repeat; i++) {
    for(int j=0; j<pixelNum; j++) {
      switch (side) {
        case 0: // flash red
          redSide.setBrightness(255);
          redSide.setPixelColor(j, COLOR_RED);
          break;
        case 1: // flash blue
          blueSide.setBrightness(255);
          blueSide.setPixelColor(j, COLOR_BLUE);
          break;
        default: // default to off
          redSide.setPixelColor(j, 0, 0, 0);
          blueSide.setPixelColor(j, 0, 0, 0);
          break;
      }
    }
    delay(wait);
    // go off before next flash
    for(int k=0; k<pixelNum; k++) {
      redSide.setPixelColor(i, 0, 0, 0);
      blueSide.setPixelColor(i, 0, 0, 0);
    }
    delay(wait);
  }
}

// light is dimmed but constant on
void idle(int brightness) {
  redSide.setBrightness(brightness);
  blueSide.setBrightness(brightness);
}

// returns to dim setting from bright
void returnToIdle(int side) {
  switch (side) {
    case 0:
      for(int i=255; i>idleBrightness; i--) {
        redSide.setBrightness(i);
        delay(5);
      }
      break;
    case 1:
      for(int j=255; j>idleBrightness; j--) {
        blueSide.setBrightness(j);
        delay(5);
      }
      break;
  }
}

// winner light animation - random flashing colors
void winner(int side) {
  switch (side) {
    case 0:
      redSide.setBrightness(255);
      rainbowCycle(0,0);
      break;
    case 1:
      blueSide.setBrightness(255);
      rainbowCycle(0,1);
      break;
  }
}

// loser light animation - slow breathing
void loser(int side) {
  switch (side) {
    case 0:
      redSide.setBrightness(idleBrightness);
      break;
    case 1:
      blueSide.setBrightness(idleBrightness);
      break;
  }
}

// check reset press, button resets the score
void checkReset(int repeat, int wait) {
  int reset = digitalRead(RESET);
  if (reset == 1) {
    redScore = 0;
    blueScore = 0;
    // flash white to signify reset
    for(int i=0; i<repeat; i++) {
      for(int j=0; j<pixelNum; j++) {
        redSide.setBrightness(255);
        redSide.setPixelColor(j, redSide.Color(255,255,255));
        blueSide.setBrightness(255);
        blueSide.setPixelColor(j, blueSide.Color(255,255,255));
      }
      delay(wait);
      // go off before next flash
      for(int k=0; k<pixelNum; k++) {
        redSide.setPixelColor(i, 0, 0, 0);
        blueSide.setPixelColor(i, 0, 0, 0);
      }
      delay(wait);
    }
    // set to red and blue ready to start game
    delay(wait);
    for(int j=0; j<pixelNum; j++) {
      redSide.setPixelColor(j, COLOR_RED);
      blueSide.setPixelColor(j, COLOR_BLUE);
    }
    returnToIdle(0);
    returnToIdle(1);
  }
}

// check enable on/off, button press will toggle the enable setting
bool checkEnable() {
  int enable = digitalRead(ENABLE);
  if (enable && !oldEnable) { // same as if (enable == high && oldEnable == low)
    // new button press
    if (state == 0) { // if state is off, turn it on
      state = 1;
    } else { // if state is on, turn it off
      state = 0;
    }
    oldEnable = 1;
  } else if (!enable && oldEnable) { // same as if (enable == low && oldEnable == high)
    // button was released
    oldEnable = 0;
  }
    
  if (state == 1) return true; // scoring enabled
  else if (state == 0) return false; // scoring disabled
}

bool redTriggered() {
  int redTrigger = digitalRead(REDGOAL);
  if (redTrigger == 1) return true;
  else return false;
}

bool blueTriggered() {
  int blueTrigger = digitalRead(BLUEGOAL);
  if (blueTrigger == 1) return true;
  else return false;
}

// goal lights will idle unless scored upon
void loop() {
  if (checkEnable()) {
    checkReset(3, 200); // check for reset, reset scores if button is pressed
    
    idle(idleBrightness); // idle brightness constant in both goals if no interrupt
  
    if(redTriggered()) { // red side has been scored on
      redScore++;
      if (redScore >= maxScore) { // check for red win 
        winner(0);
        loser(1);
      } else {
        flash(3, 200, 0); // repeat 3x, 200ms delay, red side
        delay(500);
        returnToIdle(0);
      }
    } else if(blueTriggered()) { // blue side has been scored on
      blueScore++;
      if (blueScore >= maxScore) { // check for blue win
        winner(1);
        loser(0);
      } else {
        flash(3, 200, 1); // repeat 3x, 200ms delay, red side
        delay(500);
        returnToIdle(1);
      }
    } else idle(idleBrightness);
  } else {
    // set both side LED strips to off
    for(int k=0; k<pixelNum; k++) {
      redSide.setPixelColor(k, 0, 0, 0);
      blueSide.setPixelColor(k, 0, 0, 0);
    }
  } 
}

// Rainbow equally distributed throughout
void rainbowCycle(uint8_t wait, int side) {
  uint16_t i, j;
  switch (side) {
    case 0:
      for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
        for(i=0; i< redSide.numPixels(); i++) {
          redSide.setPixelColor(i, redWheel(((i * 256 / redSide.numPixels()) + j) & 255));
        }
        redSide.show();
        delay(wait);
      }
      break;
    case 1:
      for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
        for(i=0; i< blueSide.numPixels(); i++) {
          blueSide.setPixelColor(i, blueWheel(((i * 256 / blueSide.numPixels()) + j) & 255));
        }
        blueSide.show();
        delay(wait);
      }
      break;
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t redWheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return redSide.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return redSide.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return redSide.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t blueWheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return blueSide.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return blueSide.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return blueSide.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
