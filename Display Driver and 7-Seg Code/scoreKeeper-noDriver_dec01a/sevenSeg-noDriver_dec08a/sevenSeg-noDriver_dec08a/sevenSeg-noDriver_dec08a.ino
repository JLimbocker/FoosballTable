//This version of scoreKeeper handles only scorekeeping and notifies the LED control of
//changes in score. 
/* 
 * 
 * 
 * NOTE: This version of the code is made assuming the Arduino manually controls the pins. If you're using a driver, DON'T USE THIS CODE. 
 * This code is also written under the assumption that the pins are arranged as follows:
 * Display 0 for red team, SEG A-F = digital pins 0-1, 5-9 in sequential order (SEG A = pin 0, SEG F = pin 9)
 * Display 1 for blue team, SEG A-F = digital pins 10-13, analog A0-A2 in sequential order (SEG A = pin 10, SEG F = A2)
 * WARNING: Blue team's display was installed upside-down. The pin sequence must be inverted.
 * */

#include <LedControl.h>
#include <Adafruit_NeoPixel.h>

//const int datainP = 12, clkP = 11, loadP = 10; //Constants for pins. Adjust as needed. ##CURRENTLY USELESS DRIVER CODE


//LedControl lc = LedControl(datainP, clkP, loadP, 2); //refer to constants, 2 means 2 displays ##CURRENTLY USELESS DRIVER CODE

bool keepScore = true; //toggles scorekeeping

//int prev_state[3];
int redPS = 0, bluePS = 0, resPS = 0, scoPS = 0; //previous states for all buttons
//int curr_state[3];
int redCS = 0, blueCS = 0, resCS = 0, scoCS = 0; //current states for all buttons
int redScore=0;
int blueScore=0;
/*int tScored = -1;//which team scored this cycle. if none scored, -1
int tScore = 0;//the score of the team which just scored
int hasWon = 0;//if a team has won, 1. else, 0. is read as a boolean*/
int const WINNING_SCORE=9; //set this to whatever you want, don't make it more than 9
String score;

void setup() {
  // setup the buttons
  /*for(int i = 2; i < 5; i++){ //reset and scores
    pinMode(i, INPUT_PULLUP);
  }*/
  pinMode(A0, INPUT_PULLUP); //Red Team bumper
  pinMode(A1, INPUT_PULLUP); //Blue Team bumper
  pinMode(A2, INPUT_PULLUP); //Reset Button
  pinMode(A3, INPUT_PULLUP); //Scoring Toggle
  Serial.begin(9600);
  
  //Code to prep the controller and 7-seg displays
  //lc.shutdown(0,false);
  //lc.setIntensity(0,7);
  //lc.clearDisplay(0);
}


void intToDigWrite(int intput, int disp){ //intput is the character the display shows, disp is which display. 0 is Red and 1 is Blue
  if(disp==0) //left display
    switch(intput){
      case 0: //see explanation in header
              digitalWrite(0,HIGH);
              digitalWrite(1,HIGH);
              digitalWrite(2,HIGH);
              digitalWrite(3,HIGH);
              digitalWrite(4,HIGH);
              digitalWrite(5,HIGH);
              digitalWrite(6,LOW);
              break; 
      case 1: //write to pins 1 and 5
              digitalWrite(0,LOW);
              digitalWrite(1,HIGH);
              digitalWrite(2,HIGH);
              digitalWrite(3,LOW);
              digitalWrite(4,LOW);
              digitalWrite(5,LOW);
              digitalWrite(6,LOW);
              break;
      case 2: //exclude pin 8 and pin 5
              digitalWrite(0,HIGH);
              digitalWrite(1,HIGH);
              digitalWrite(2,LOW);
              digitalWrite(3,HIGH);
              digitalWrite(4,HIGH);
              digitalWrite(5,LOW);
              digitalWrite(6,LOW);
              break;
      case 3: //exclude pins 8 and 7
              digitalWrite(0,HIGH);
              digitalWrite(1,HIGH);
              digitalWrite(2,HIGH);
              digitalWrite(3,HIGH);
              digitalWrite(4,LOW);
              digitalWrite(5,LOW);
              digitalWrite(6,HIGH);
              break;
      case 4: //exclude pins 7, 6, and 0.
              digitalWrite(0,LOW);
              digitalWrite(1,HIGH);
              digitalWrite(2,HIGH);
              digitalWrite(3,LOW);
              digitalWrite(4,LOW);
              digitalWrite(5,HIGH);
              digitalWrite(6,HIGH);
              break;
      case 5: //exclude pins 1 and 7
              digitalWrite(0,HIGH);
              digitalWrite(1,LOW);
              digitalWrite(2,HIGH);
              digitalWrite(3,HIGH);
              digitalWrite(4,LOW);
              digitalWrite(5,HIGH);
              digitalWrite(6,HIGH);
              break;
      case 6: //exclude pin 1
              digitalWrite(0,HIGH);
              digitalWrite(1,LOW);
              digitalWrite(2,HIGH);
              digitalWrite(3,HIGH);
              digitalWrite(4,HIGH);
              digitalWrite(5,HIGH);
              digitalWrite(6,HIGH);
              break;
      case 7: //write to pins 0, 1, and 5
              digitalWrite(0,HIGH);
              digitalWrite(1,HIGH);
              digitalWrite(2,HIGH);
              digitalWrite(3,LOW);
              digitalWrite(4,LOW);
              digitalWrite(5,LOW);
              digitalWrite(6,LOW);
              break;
      case 8: //write to all pins
              digitalWrite(0,HIGH);
              digitalWrite(1,HIGH);
              digitalWrite(2,HIGH);
              digitalWrite(3,HIGH);
              digitalWrite(4,HIGH);
              digitalWrite(5,HIGH);
              digitalWrite(6,HIGH);
              break;
      case 9: //excude pins 7 and 6
              digitalWrite(0,HIGH);
              digitalWrite(1,HIGH);
              digitalWrite(2,HIGH);
              digitalWrite(3,LOW);
              digitalWrite(4,LOW);
              digitalWrite(5,HIGH);
              digitalWrite(6,HIGH);
              break;
       default:   digitalWrite(0,LOW); //if there's something wrong it will display X
                  digitalWrite(1,HIGH);
                  digitalWrite(2,HIGH);
                  digitalWrite(3,LOW);
                  digitalWrite(4,HIGH);
                  digitalWrite(5,HIGH);
                  digitalWrite(6,HIGH);
                  break;
    }
  else //right display
    switch(intput){
      case 0: //write to all pins except pin 9 - see explanation in header
              digitalWrite(10,HIGH);
              digitalWrite(11,HIGH);
              digitalWrite(12,HIGH);
              digitalWrite(7,HIGH);
              digitalWrite(8,HIGH);
              digitalWrite(9,HIGH);
              digitalWrite(13,LOW);
              break; 
      case 1: //write to pins 1 and 5
              digitalWrite(10,LOW);
              digitalWrite(11,HIGH);
              digitalWrite(12,HIGH);
              digitalWrite(7,LOW);
              digitalWrite(8,LOW);
              digitalWrite(9,LOW);
              digitalWrite(13,LOW);
              break;
      case 2: //exclude pin 8 and pin 5
              digitalWrite(10,HIGH);
              digitalWrite(11,HIGH);
              digitalWrite(12,LOW);
              digitalWrite(7,HIGH);
              digitalWrite(8,HIGH);
              digitalWrite(9,LOW);
              digitalWrite(13,LOW);
              break;
      case 3: //exclude pins 8 and 7
              digitalWrite(10,HIGH);
              digitalWrite(11,HIGH);
              digitalWrite(12,HIGH);
              digitalWrite(7,HIGH);
              digitalWrite(8,LOW);
              digitalWrite(9,LOW);
              digitalWrite(13,HIGH);
              break;
      case 4: //exclude pins 7, 6, and 0.
              digitalWrite(10,LOW);
              digitalWrite(11,HIGH);
              digitalWrite(12,HIGH);
              digitalWrite(7,LOW);
              digitalWrite(8,LOW);
              digitalWrite(9,HIGH);
              digitalWrite(13,HIGH);
              break;
      case 5: //exclude pins 1 and 7
              digitalWrite(10,HIGH);
              digitalWrite(11,LOW);
              digitalWrite(12,HIGH);
              digitalWrite(7,HIGH);
              digitalWrite(8,LOW);
              digitalWrite(9,HIGH);
              digitalWrite(13,HIGH);
              break;
      case 6: //exclude pin 1
              digitalWrite(10,HIGH);
              digitalWrite(11,LOW);
              digitalWrite(12,HIGH);
              digitalWrite(7,HIGH);
              digitalWrite(8,HIGH);
              digitalWrite(9,HIGH);
              digitalWrite(13,HIGH);
              break;
      case 7: //write to pins 0, 1, and 5
              digitalWrite(10,HIGH);
              digitalWrite(11,HIGH);
              digitalWrite(12,HIGH);
              digitalWrite(7,LOW);
              digitalWrite(8,LOW);
              digitalWrite(9,LOW);
              digitalWrite(13,LOW);
              break;
      case 8: //write to all pins
              digitalWrite(10,HIGH);
              digitalWrite(11,HIGH);
              digitalWrite(22,HIGH);
              digitalWrite(7,HIGH);
              digitalWrite(8,HIGH);
              digitalWrite(9,HIGH);
              digitalWrite(13,HIGH);
              break;
      case 9: //excude pins 7 and 6
              digitalWrite(10,HIGH);
              digitalWrite(11,HIGH);
              digitalWrite(12,HIGH);
              digitalWrite(7,LOW);
              digitalWrite(8,LOW);
              digitalWrite(9,HIGH);
              digitalWrite(13,HIGH);
              break;
       default:   digitalWrite(10,LOW); //if there's something wrong it will display X
                  digitalWrite(11,HIGH);
                  digitalWrite(12,HIGH);
                  digitalWrite(7,LOW);
                  digitalWrite(8,HIGH);
                  digitalWrite(9,HIGH);
                  digitalWrite(13,HIGH);
                  break;
    }
}

void reportToLED(int team, int score, int victory){ //not used in current version
  int dataOut[3] = {team, score, victory};

  //give dataOut to other arduino somehow. Digital write?
}

void checkScorekeeping(){ //NOTICE: IT WILL SAVE DATA IF SCOREKEEPING IS DISABLED.
  scoCS = digitalRead(A3);
  if(scoCS==LOW && scoCS != scoPS){
      if(keepScore) keepScore = false;
      else keepScore = true;
    }
}


void checkButtons(){ //Checks all the buttons. Made to streamline loop code.
  redCS = digitalRead(A0);
  blueCS = digitalRead(A1);
  resCS = digitalRead(A2);
  if(resCS==LOW && resCS!=resPS){
    Serial.println("reset");
    redScore=blueScore=0;
    delay(10);
  }
  if(redCS==LOW && redCS!=redPS){
    redScore++;
    Serial.println("The score is R: " + String(redScore) + " B: " + String(blueScore));
    delay(2000); //This prevents ball spamming
  }
  if(blueCS==LOW && blueCS!=bluePS){
    blueScore++;
    Serial.println("The score is R: " + String(redScore) + " B: " + String(blueScore));
    delay(2000); //This prevents ball spamming
  }
  redPS = redCS; 
  bluePS = blueCS;
  resPS = resCS;
}

void loop() {
  // put your main code here, to run repeatedly:
  checkScorekeeping();
  if(keepScore) //this controls scoring being off or on
  {
    checkButtons();
    intToDigWrite(redScore,0);
    intToDigWrite(blueScore,1);
    delay(10);
 }
}
