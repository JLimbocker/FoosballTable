//This is a working copy of scoreKeeper which will work to integrate the LED display
//functionality alongside the existing scorekeeping functions.
//last time this code was tested it worked

#include <LedControl.h>

const int datainP = 12, clkP = 11, loadP = 10; //Constants for pins. Adjust as needed.


LedControl lc = LedControl(datainP, clkP, loadP, 2); //refer to constants, 2 means 2 displays

int prev_state[3];
int curr_state[3];
int leftScore=0;
int rightScore=0;
String score;

void setup() {
  // setup the buttons
  for(int i = 2; i < 5; i++){
    pinMode(i, INPUT_PULLUP);
  }
  Serial.begin(9600);

  //Code to prep the controler and 7-seg displays
  lc.shutdown(0,false);
  lc.setIntensity(0,7);
  lc.clearDisplay(0);
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
    delay(10);
  }
  for(int i = 2; i < 5; i++){
    prev_state[i-2]=curr_state[i-2];
  }

  //code to display the scores on one 7-seg. Add a second, separate 7-seg later.
  lc.setDigit(0,0,rightScore,false);

  
  //If we were to use one driver for both displays, player 2's score would be this:
  lc.setDigit(0, 1, leftScore, false); //Make sure that the controller's constructor is
                                        //called accordingly.

 // Using a separate controller it would be:
 // lc2.setDigit(0, 0, rightScore, false); Make sure this second controller exists.
  
}
