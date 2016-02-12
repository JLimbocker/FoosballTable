#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define __DEBUG__
//
//  Segment Reference
//        a
//      _____
//  f  /    / b
//    /____/ 
// e /  g /   c
//  /____/
//     d 
//
#define DIGIT_0 0b00111111 //0x3F
#define DIGIT_1 0b00000110 //0x06
#define DIGIT_2 0b01011011 //0x5B
#define DIGIT_3 0b01001111 //0x4F
#define DIGIT_4 0b01100110 //0x66
#define DIGIT_5 0b01101101 //0x6D
#define DIGIT_6 0b01111101 //0x7D
#define DIGIT_7 0b00000111 //0x07
#define DIGIT_8 0b01111111 //0x7F
#define DIGIT_9 0b01100111 //0x67
#define DIGIT_A 0b01110111 //0x77
#define DIGIT_B 0b01111111 //0x7F
#define DIGIT_C 0b00111001 //0x39
#define DIGIT_D 0b00111111 //0x3F
#define DIGIT_E 0b01111001 //0x79
#define DIGIT_F 0b01110001 //0x71
#define SEG_A   0b00000001 //0x01
#define SEG_B   0b00000010 //0x02
#define SEG_C   0b00000100 //0x04
#define SEG_D   0b00001000 //0x08
#define SEG_E   0b00010000 //0X10
#define SEG_F   0b00100000 //0X20
#define SEG_G   0b01000000 //0X40
#define OFF     0b00000000 //0x00
#define SCORE_RED 2
#define SCORE_BLUE 18
#define RESET 21
#define ENABLE 20
#define REDLIGHT 4
#define BLUELIGHT 5
#define COLOR_BLUE blueSide.Color(0, 0, 255)
#define COLOR_RED redSide.Color(255, 0, 0)
enum colors { RED, BLUE, BOTH, WHITE, BLACK };

byte digits[] = {DIGIT_0, DIGIT_1, DIGIT_2, DIGIT_3, DIGIT_4, DIGIT_5, DIGIT_6, DIGIT_7, DIGIT_8, DIGIT_9, DIGIT_A, DIGIT_B, DIGIT_C, DIGIT_D, DIGIT_E, DIGIT_F};
byte redScore = 0;
byte blueScore = 0;
byte redHold = 0;
byte blueHold = 0;
byte enbl = 0;
byte rst = 0;
bool scoreToggle = 1;
bool gameWon = false;

Adafruit_NeoPixel redSide = Adafruit_NeoPixel(21, REDLIGHT, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel blueSide = Adafruit_NeoPixel(20, BLUELIGHT, NEO_GRB + NEO_KHZ800);
int pixelNum = 21; // number of LEDs on goal strip
int idleBrightness = 50; // 0-255 idle brightness setting

//disp = 0 -> Red Display
//disp = 1 -> Blue Display
//void writeDigit(int disp, byte digit)
//{
//  if(disp == 0)
//    PORTA = digits[digit];
//  else if(disp == 1)
//    PORTC = digits[digit];
//}

void testPattern()
{
  int d = 5000;
//  #ifdef __DEBUG__
//    Serial.println("Test Pattern");
//  #endif
  PORTA = 0b00000001; //SEG A
  PORTC = 0b00000001;
  delay(d);
  PORTA = 0b00000010; //SEG B
  PORTC = 0b00000010;
  delay(d);
  PORTA = 0b01000000; //SEG G
  PORTC = 0b01000000;
  delay(d);
  PORTA = 0b00010000; //SEG E
  PORTC = 0b00010000;
  delay(d);
  PORTA = 0b00001000; //SEG D
  PORTC = 0b00001000;
  delay(d);
  PORTA = 0b00000100; //SEG C
  PORTC = 0b00000100;
  delay(d);
  PORTA = 0b01000000; //SEG G
  PORTC = 0b01000000;
  delay(d);
  PORTA = 0b00100000; //SEG F
  PORTC = 0b00100000;
  delay(d);
  #ifdef __DEBUG__
    Serial.println("Test Complete");
  #endif
}

void blueScored()
{
  blueHold++;
  //if(blueHold % 2 == 1)
  {
    if(blueScore < 9) 
      blueScore++;
    flash(2, 100, RED); // red goal was scored on
    idle(idleBrightness);
    PORTC = digits[blueScore];
    #ifdef __DEBUG__
      Serial.println("B Sc " + String(blueScore));
    #endif

    if(blueScore >= 9 && gameWon == false)
    {
      winner(BLUE);
      redSide.setBrightness(idleBrightness);
      blueScore = 9;
      PORTC = digits[blueScore];
    } else if(blueScore >= 9 && gameWon == true) {
      #ifdef __DEBUG__
        Serial.println("Game won by Blue!");
      #endif
    }
  }
  
  delay(50); //delay to debounce switch
  #ifdef __DEBUG__
    Serial.println("ISR Complete");
  #endif
}

void redScored()
{
  redHold++;
  //if(redHold%2 == 1)
  {
    if(redScore < 9) redScore++;
    flash(2, 100, BLUE);
    idle(idleBrightness);
    PORTA = digits[redScore];
    #ifdef __DEBUG__
      Serial.println("R Sc " + String(redScore));
    #endif
    
    if(redScore >= 9 && gameWon == false)
    {
      winner(RED);
      blueSide.setBrightness(idleBrightness);
      redScore = 9;
      PORTA = digits[redScore];
    } else if(redScore >= 9 && gameWon == true){
      #ifdef __DEBUG__
        Serial.println("Game won by Red!");
      #endif
    }
  }
  delay(50);//delay to debounce switch
 // #ifdef __DEBUG__
 //   Serial.println("ISR Complete");
 // #endif
}

void enableScore()
{
  enbl++;
  if(enbl%2 == 1)
  {
    if(scoreToggle == 1)
    {
      scoreToggle = 0; // if scoring is on, turn it off
      PORTA = OFF;
      PORTC = OFF;
      for(int i=0; i<pixelNum; i++)
      {
        redSide.setPixelColor(i, 0, 0, 0);
        blueSide.setPixelColor(i, 0, 0, 0);
      }
      redSide.show();
      blueSide.show();
    }
    else if(scoreToggle == 0)
    {
      scoreToggle = 1; // if scoring is off, turn it on
      redScore = 0;
      blueScore = 0;
      testPattern();
      testPattern();
      flash(2, 100, WHITE); // white on both sides
      PORTA = DIGIT_0;
      PORTC = DIGIT_0;
      idle(idleBrightness);
    }
    
    #ifdef __DEBUG__
      //Serial.println("Sc Enable");
      if(scoreToggle == 1) Serial.println("Scoring On");
      else Serial.println("Scoring Off");
    #endif
  }
  
  delay(20000); //delay to debounce switch
}

void resetScore()
{
  rst++;
  if(rst%2 == 1)
  {
    redScore = 0;
    blueScore = 0;
    PORTC = digits[redScore];
    PORTA = digits[blueScore];
    flash(2, 100, WHITE); // white on both sides
    #ifdef __DEBUG__
      Serial.println("Sc Reset");
    #endif
  }
  delay(20000); //delay to debounce switch
}

// flash lights when scored on
void flash(byte repeat, int wait, byte side) {
  for(int i=0; i<repeat; i++) {
    for(int j=0; j<pixelNum; j++) {
      switch (side) {
        case RED: // flash red
          redSide.setBrightness(255);
          redSide.setPixelColor(j, COLOR_RED);
          break;
        case BLUE: // flash blue
          blueSide.setBrightness(255);
          blueSide.setPixelColor(j, COLOR_BLUE);
          break;
        case BOTH: // flash red and blue
          redSide.setBrightness(255);
          redSide.setPixelColor(j, COLOR_RED);
          blueSide.setBrightness(255);
          blueSide.setPixelColor(j, COLOR_BLUE);
          break;
        case WHITE: // flash both white
          redSide.setBrightness(255);
          redSide.setPixelColor(j, 255, 255, 255);
          blueSide.setBrightness(255);
          blueSide.setPixelColor(j, 255, 255, 255);
          break;
        default: // default to off
          redSide.setPixelColor(j, 0, 0, 0);
          blueSide.setPixelColor(j, 0, 0, 0);
          break;
      }
    }
    redSide.show();
    blueSide.show();
    delay(wait);
    // go off before next flash
    for(byte k=0; k<pixelNum; k++) {
      switch(side) {
        case RED:
          redSide.setPixelColor(k, 0, 0, 0);
          break;
        case BLUE:
          blueSide.setPixelColor(k, 0, 0, 0);
          break;
        default: // default to off
          redSide.setPixelColor(k, 0, 0, 0);
          blueSide.setPixelColor(k, 0, 0, 0);
          break;
      }
    }
    redSide.show();
    blueSide.show();
    delay(wait);
  }
}

// light is dimmed but constant on
void idle(int brightness) {
  for(int i=0; i<pixelNum; i++) {
    redSide.setPixelColor(i, COLOR_RED);
    blueSide.setPixelColor(i, COLOR_BLUE);
  }
  redSide.setBrightness(brightness);
  blueSide.setBrightness(brightness);
  redSide.show();
  blueSide.show();
}

// Rainbow equally distributed throughout
void rainbowCycle(uint8_t wait, byte side) {
  uint16_t i, j;
  switch (side) {
    case RED:
      for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
        for(i=0; i< redSide.numPixels(); i++) {
          redSide.setPixelColor(i, Wheel(((i * 256 / redSide.numPixels()) + j) & 255, RED));
        }
        redSide.show();
        delay(wait);
      }
      break;
    case BLUE:
      for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
        for(i=0; i< blueSide.numPixels(); i++) {
          blueSide.setPixelColor(i, Wheel(((i * 256 / blueSide.numPixels()) + j) & 255, BLUE));
        }
        blueSide.show();
        delay(wait);
      }
      break;
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos, byte side) {
  WheelPos = 255 - WheelPos;
  switch(side) {
    case RED:
      if(WheelPos < 85) {
        return redSide.Color(255 - WheelPos * 3, 0, WheelPos * 3);
      }
      if(WheelPos < 170) {
        WheelPos -= 85;
        return redSide.Color(0, WheelPos * 3, 255 - WheelPos * 3);
      }
      WheelPos -= 170;
      return redSide.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
      break;
    case BLUE:
      if(WheelPos < 85) {
        return blueSide.Color(255 - WheelPos * 3, 0, WheelPos * 3);
      }
      if(WheelPos < 170) {
        WheelPos -= 85;
        return blueSide.Color(0, WheelPos * 3, 255 - WheelPos * 3);
      }
      WheelPos -= 170;
      return blueSide.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
      break;
  }
}

// winning light anim
void winner(byte side)
{
  #ifdef __DEBUG__
    Serial.println("Winner is " + String(side));
  #endif
  int t = millis();
  switch(side) {
    case RED:
      redSide.setBrightness(255);
      while (millis()-t <= 10000) {
        rainbowCycle(0, RED);
        redSide.show();
      }
      resetScore();
      break;
    case BLUE:
      blueSide.setBrightness(255);
      while (millis()-t <= 10000) {
        rainbowCycle(0, BLUE);
        blueSide.show();
      }
      resetScore();
      break;
  }
  gameWon = true;
}

void setup() 
{
  redScore = 0;
  blueScore = 0;
  redHold = 0;
  blueHold = 0;
  enbl = 0;
  rst = 0;
  scoreToggle = 1;
  gameWon = false;
  noInterrupts();
  redScore = 0;
  blueScore = 0;
  #ifdef __DEBUG__
    Serial.begin(9600);
  #endif
  // initialize pin modes
  for (int i = 2; i < 8; i++) {
    pinMode(i, INPUT_PULLUP);
  }
  pinMode(18, INPUT_PULLUP);

  // turn on goal LEDs
  redSide.begin();
  blueSide.begin();

  DDRA = 0xFF; //Set portA to digital output
  DDRC = 0xFF; //Set portC to digital output
  PORTA = 0x00; //Init output to low
  PORTC = 0x00; //Init output to low
  testPattern();
  testPattern();
  flash(2, 100, WHITE); // white on both sides
  PORTA = DIGIT_0;
  PORTC = DIGIT_0;
  idle(idleBrightness);
  
  #ifdef __DEBUG__
    Serial.println("Setup Complete");
  #endif
    
  attachInterrupt(digitalPinToInterrupt(SCORE_BLUE), redScored, RISING);
  attachInterrupt(digitalPinToInterrupt(SCORE_RED), blueScored, RISING);
  attachInterrupt(digitalPinToInterrupt(RESET), resetScore, RISING);
  attachInterrupt(digitalPinToInterrupt(ENABLE), enableScore, RISING);
  interrupts();
  
}

void loop() 
{
  
}

