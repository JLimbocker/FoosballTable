#include <LedControl.h>
#include <Adafruit_NeoPixel.h>
//Pins are as follows: RGB in digital 0, 1, 2.

Adafruit_NeoPixel redPixels = Adafruit_Neopixel(20, 0, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel bluePixels = Adafruit_Neopixel(20, 1, NEO_RGB + NEO_KHZ800);

int prev_state[3];
int curr_state[3];

/* if there's time, make each object separate
 * Adafruit_NeoPixel teamRed = Adafruit_NeoPixel(21, 1, NEO_RGB + NEO_KHZ800);
 * Adafruit_NeoPixel teamBlue = Adafruit_NeoPixel(20, 0, NEO_RGB + NEO_KHZ800);
 * */

#define COLOR_BLUE  bluePixels.Color(0, 0, 255)
#define COLOR_RED   redPixels.Color(255, 0, 0)
#define COLOR_OFF   redPixels.Color(0, 0, 0)

int dataSet[3] = {-1, -1, 0}; 
//Elements correspond to: [0]=Team scored, [1]=Team's score,[2]=VictoryWon
 
void setup() {
  // put your setup code here, to run once:
  pixels.begin();
  pixels.show();
  for(int i = 2; i < 5; i++){ //reset and scores
    pinMode(i, INPUT_PULLUP);
  }
}

//NOTICE: ALL SEQUENCES MUST COMPLETE BEFORE A NEW SEQUENCE BEGINS.
//THIS MEANS IF A GOAL IS SCORED IN THE MIDDLE OF AN IDLE SEQUENCE, IT WILL NOT CHANGE
//TO THE SCORE SEQUENCE UNTIL THE IDLE SEQUENCE COMPLETES.

void redScoreAnim(){//both goals flash red, then blue is faded while red does parades

  flash(COLOR_RED, 9); //flash 9 times
  bluePixels.setPixelColor(i, bluePixels.Color(0, 0, 100));
  for(int i = 0; i<3; i++){
    theaterChase(redPixels, COLOR_RED, 50);
  }
}

void flash(uint32_t c, uint8_t repeat){ //flashes both strips at rate of ~3 flashes per second
    for(int i = 0; i<20; i++){
      redPixels.setPixelColor(i, COLOR_RED);
      bluePixels.setPixelColor(i, COLOR_RED);
    }
    delay(167);
    for(int i = 0; i<20; i++){
     redPixels.setPixelColor(i, COLOR_DARK);
     bluePixels.setPixelColor(i, COLOR_DARK);
    }
    delay(167);
}

//Theatre-style crawling lights.
void theaterChase(Adafruit_NeoPixel strip, uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void blueScoreAnim(){//both goals flash blue, then red fades while blue does parades
  //SAME AS REDSCOREANIM BUT SWITCH RED AND BLUE
}

void idleAnim(){
  for(int i = 0; i<=255; i+=5){
    for(int j = 0; j<20; j++) redPixels.setPixelColor(j, redPixels.Color(i, 0, 0));
    for(int j = 0; j<20; j++) bluePixels.setPixelColor(j, bluePixels.Color(0, 0, i));
    delay(20);
  }
  for(int i = 255; i>=0; i-=5){
    for(int j = 0; j<20; j++) redPixels.setPixelColor(j, redPixels.Color(i, 0, 0));
    for(int j = 0; j<20; j++) bluePixels.setPixelColor(j, bluePixels.Color(0, 0, i));
    delay(20);
  }
}

void loop() {
  
  for(int i = 2; i < 5; i++){
    curr_state[i-2]=digitalRead(i);
  }
  if(curr_state[0]==LOW && curr_state[0]!=prev_state[0]){ //Triggers when reset button hit
    Serial.println("reset");
    leftScore=rightScore=hasWon=0;
    tScored = -1;
    tScore = 0;
    delay(10);
  }
  if(curr_state[1]==LOW && curr_state[1]!=prev_state[1]){
    redScoreAnim();
    delay(10);
  }
  if(curr_state[2]==LOW && curr_state[2]!=prev_state[2]){
    rightScore++;
    score = "The score is L: " + String(leftScore) + " R: " + String(rightScore);
    Serial.println(score);
    blueScoreAnim();
    delay(10);
  }
  for(int i = 2; i < 5; i++){
    prev_state[i-2]=curr_state[i-2];
  }


  delay(10);
}
