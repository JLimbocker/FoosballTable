//This is a working copy of scoreKeeper which will work to integrate the LED display
//functionality alongside the existing scorekeeping functions.
//last time this code was tested it worked
/*
 * NOTE: This version of the code is made assuming the Arduino manually controls the pins. If you're using a driver, DON'T USE THIS CODE. 
 * This code is also written under the assumption that the pins are arranged as follows:
 * Display 0 for red team, SEG A-F = digital pins 0-1, 5-9 in sequential order (SEG A = pin 0, SEG F = pin 9)
 * Display 1 for blue team, SEG A-F = digital pins 10-13, analog A0-A2 in sequential order (SEG A = pin 10, SEG F = A2) 
 * */

#include <LedControl.h>
#include <Adafruit_NeoPixel.h>

//const int datainP = 12, clkP = 11, loadP = 10; //Constants for pins. Adjust as needed. ##CURRENTLY USELESS DRIVER CODE


//LedControl lc = LedControl(datainP, clkP, loadP, 2); //refer to constants, 2 means 2 displays ##CURRENTLY USELESS DRIVER CODE

int prev_state[3];
int curr_state[3];
int curr_state_ONOFF;
int leftScore=0;
int rightScore=0;
int const WINNING_SCORE=9; //set this to whatever you want, cannot be greater than 15
String score;

void setup() {
  // setup the buttons
  for(int i = 2; i < 5; i++){
    pinMode(i, INPUT_PULLUP);
  }
  pinMode(A3, INPUT_PULLUP);
  Serial.begin(9600);

  //Code to prep the controler and 7-seg displays
  //lc.shutdown(0,false);
  //lc.setIntensity(0,7);
  //lc.clearDisplay(0);
}

void intToDigWrite(int intput, int disp){ //intput is the character the display shows, disp is which display. 0 is Left and 1 is Right
  if(disp==0) //left display
    switch(intput){
      case 0: //write to all pins except pin 9 - see explanation in header
              digitalWrite(0,HIGH);
              digitalWrite(1,HIGH);
              digitalWrite(5,HIGH);
              digitalWrite(6,HIGH);
              digitalWrite(7,HIGH);
              digitalWrite(8,HIGH);
              digitalWrite(9,LOW);
              break; 
      case 1: //write to pins 1 and 5
              digitalWrite(0,LOW);
              digitalWrite(1,HIGH);
              digitalWrite(5,HIGH);
              digitalWrite(6,LOW);
              digitalWrite(7,LOW);
              digitalWrite(8,LOW);
              digitalWrite(9,LOW);
              break;
      case 2: //exclude pin 8 and pin 5
              digitalWrite(0,HIGH);
              digitalWrite(1,HIGH);
              digitalWrite(5,LOW);
              digitalWrite(6,HIGH);
              digitalWrite(7,HIGH);
              digitalWrite(8,LOW);
              digitalWrite(9,LOW);
              break;
      case 3: //exclude pins 8 and 7
              digitalWrite(0,HIGH);
              digitalWrite(1,HIGH);
              digitalWrite(5,HIGH);
              digitalWrite(6,HIGH);
              digitalWrite(7,LOW);
              digitalWrite(8,LOW);
              digitalWrite(9,HIGH);
              break;
      case 4: //exclude pins 7, 6, and 0.
              digitalWrite(0,LOW);
              digitalWrite(1,HIGH);
              digitalWrite(5,HIGH);
              digitalWrite(6,LOW);
              digitalWrite(7,LOW);
              digitalWrite(8,HIGH);
              digitalWrite(9,HIGH);
              break;
      case 5: //exclude pins 1 and 7
              digitalWrite(0,HIGH);
              digitalWrite(1,LOW);
              digitalWrite(5,HIGH);
              digitalWrite(6,HIGH);
              digitalWrite(7,LOW);
              digitalWrite(8,HIGH);
              digitalWrite(9,HIGH);
              break;
      case 6: //exclude pin 1
              digitalWrite(0,HIGH);
              digitalWrite(1,LOW);
              digitalWrite(5,HIGH);
              digitalWrite(6,HIGH);
              digitalWrite(7,HIGH);
              digitalWrite(8,HIGH);
              digitalWrite(9,HIGH);
              break;
      case 7: //write to pins 0, 1, and 5
              digitalWrite(0,HIGH);
              digitalWrite(1,HIGH);
              digitalWrite(5,HIGH);
              digitalWrite(6,LOW);
              digitalWrite(7,LOW);
              digitalWrite(8,LOW);
              digitalWrite(9,LOW);
              break;
      case 8: //write to all pins
              digitalWrite(0,HIGH);
              digitalWrite(1,HIGH);
              digitalWrite(5,HIGH);
              digitalWrite(6,HIGH);
              digitalWrite(7,HIGH);
              digitalWrite(8,HIGH);
              digitalWrite(9,HIGH);
              break;
      case 9: //excude pins 7 and 6
              digitalWrite(0,HIGH);
              digitalWrite(1,HIGH);
              digitalWrite(5,HIGH);
              digitalWrite(6,LOW);
              digitalWrite(7,LOW);
              digitalWrite(8,HIGH);
              digitalWrite(9,HIGH);
              break;
    }
  else //right display
    switch(intput){
      case 0: //exclude pin A2
              digitalWrite(10,HIGH);
              digitalWrite(11,HIGH);
              digitalWrite(12,HIGH);
              digitalWrite(13,HIGH);
              digitalWrite(A0,HIGH);
              digitalWrite(A1,HIGH);
              digitalWrite(A2,LOW);
              break;
      case 1: //write to pins 11 and 12
              digitalWrite(10,LOW);
              digitalWrite(11,HIGH);
              digitalWrite(12,HIGH);
              digitalWrite(13,LOW);
              digitalWrite(A0,LOW);
              digitalWrite(A1,LOW);
              digitalWrite(A2,LOW);
              break;
      case 2: //exclude A1 and pin 12
              digitalWrite(10,HIGH);
              digitalWrite(11,HIGH);
              digitalWrite(12,LOW);
              digitalWrite(13,HIGH);
              digitalWrite(A0,HIGH);
              digitalWrite(A1,LOW);
              digitalWrite(A2,HIGH);
              break;
      case 3: //exclude A0 and A1
              digitalWrite(10,HIGH);
              digitalWrite(11,HIGH);
              digitalWrite(12,HIGH);
              digitalWrite(13,HIGH);
              digitalWrite(A0,LOW);
              digitalWrite(A1,LOW);
              digitalWrite(A2,HIGH);
              break;
      case 4: //exclude pins 10 and 13, A0
              digitalWrite(10,LOW);
              digitalWrite(11,HIGH);
              digitalWrite(12,HIGH);
              digitalWrite(13,LOW);
              digitalWrite(A0,LOW);
              digitalWrite(A1,HIGH);
              digitalWrite(A2,HIGH);
              break;
      case 5: //exclude pin 11 and A0
              digitalWrite(10,HIGH);
              digitalWrite(11,LOW);
              digitalWrite(12,HIGH);
              digitalWrite(13,HIGH);
              digitalWrite(A0,LOW);
              digitalWrite(A1,HIGH);
              digitalWrite(A2,HIGH);
              break;
      case 6: //exclude pin 11
              digitalWrite(10,HIGH);
              digitalWrite(11,LOW);
              digitalWrite(12,HIGH);
              digitalWrite(13,HIGH);
              digitalWrite(A0,HIGH);
              digitalWrite(A1,HIGH);
              digitalWrite(A2,HIGH);
              break;
      case 7: //write to pins 10-12
              digitalWrite(10,HIGH);
              digitalWrite(11,HIGH);
              digitalWrite(12,HIGH);
              digitalWrite(13,LOW);
              digitalWrite(A0,LOW);
              digitalWrite(A1,LOW);
              digitalWrite(A2,LOW);
              break;
      case 8: //write to all pins
              digitalWrite(10,HIGH);
              digitalWrite(11,HIGH);
              digitalWrite(12,HIGH);
              digitalWrite(13,HIGH);
              digitalWrite(A0,HIGH);
              digitalWrite(A1,HIGH);
              digitalWrite(A2,HIGH);
              break;
      case 9: //exclude pin 13 and A0
              digitalWrite(10,HIGH);
              digitalWrite(11,HIGH);
              digitalWrite(12,HIGH);
              digitalWrite(13,LOW);
              digitalWrite(A0,LOW);
              digitalWrite(A1,HIGH);
              digitalWrite(A2,HIGH);
              break;
    }
}

void victoryAnim(int intput){ //intput is which team won - 0 is Red, 1 is Blue
  //use the NeoPixel libraries
  
}

void loop() {
  // put your main code here, to run repeatedly:

  //scorekeeping and such
  for(int i = 2; i < 5; i++){
    curr_state[i-2]=digitalRead(i);
  }
  if(curr_state[0]==LOW && curr_state[0]!=prev_state[0]){
    Serial.println("reset");
    leftScore=rightScore=0;
    delay(10);
  }
  if(curr_state[1]==LOW && curr_state[1]!=prev_state[1]){
    leftScore++;
    score = "The score is L: " + String(leftScore) + " R: " + String(rightScore);
    Serial.println(score);
    delay(10);
  }
  if(curr_state[2]==LOW && curr_state[2]!=prev_state[2]){
    rightScore++;
    score = "The score is L: " + String(leftScore) + " R: " + String(rightScore);
    Serial.println(score);
   // scoreAnim(1);//play a brief score animation for blue
    delay(10);
  }
  for(int i = 2; i < 5; i++){
    prev_state[i-2]=curr_state[i-2];
  }

  //code for noDriver_nov17a - manually writing to digital pins
  if(leftScore<WINNING_SCORE)
    intToDigWrite(leftScore,0); //if there is no winner, write updated score to display 0 left team
  /*else
    victoryAnim(0);*/

  if(rightScore<WINNING_SCORE)
    intToDigWrite(rightScore,1); //if there is no winner, write updated score to display 1 right team
 /* else
    victoryAnim(1);*/

  //code to display the scores on one 7-seg. Add a second, separate 7-seg later.
  //lc.setDigit(0,0,leftScore,false); //USELESS DRIVER CODE

  
  //If we were to use one driver for both displays, player 2's score would be this:
  //lc.setDigit(0, 1, rightScore, false); //Make sure that the controller's constructor is
                                        //called accordingly. USELESS DRIVER CODE

 // Using a separate controller it would be:
 // lc2.setDigit(0, 0, rightScore, false); Make sure this second controller exists.
  
}
