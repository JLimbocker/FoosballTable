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

//#include <LedControl.h>
//#include <Adafruit_NeoPixel.h>

//const int datainP = 12, clkP = 11, loadP = 10; //Constants for pins. Adjust as needed. ##CURRENTLY USELESS DRIVER CODE

#define DISP1_A 7
#define DISP1_B 8
#define DISP1_C 9
#define DISP1_D 10
#define DISP1_E 11
#define DISP1_F 12
#define DISP1_G 13

#define DISP2_E 2
#define DISP2_D 3
#define DISP2_C 4
#define DISP2_A 5
#define DISP2_B A3
#define DISP2_G A4
#define DISP2_F A5

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
  pinMode(A3, OUTPUT); //Scoring Toggle
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  Serial.println("Ready");

  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  digitalWrite(A4, LOW);
  digitalWrite(A5, LOW);
  digitalWrite(A3, LOW);
  
  //Code to prep the controller and 7-seg displays
  //lc.shutdown(0,false);
  //lc.setIntensity(0,7);
  //lc.clearDisplay(0);
}


void intToDigWrite(int intput, int disp){ //intput is the character the display shows, disp is which display. 0 is Red and 1 is Blue
  if(disp==1) //left display
    switch(intput){
      case 0: //see explanation in header
              digitalWrite(DISP1_A,HIGH);
              digitalWrite(DISP1_B,HIGH);
              digitalWrite(DISP1_C,HIGH);
              digitalWrite(DISP1_D,HIGH);
              digitalWrite(DISP1_E,HIGH);
              digitalWrite(DISP1_F,HIGH);
              digitalWrite(DISP1_G,LOW);
              break; 
      case 1: //write to pins 1 and 5
              digitalWrite(DISP1_A,LOW);
              digitalWrite(DISP1_B,HIGH);
              digitalWrite(DISP1_C,HIGH);
              digitalWrite(DISP1_D,LOW);
              digitalWrite(DISP1_E,LOW);
              digitalWrite(DISP1_F,LOW);
              digitalWrite(DISP1_G,LOW);
              break;
      case 2: //exclude pin 8 and pin 5
              digitalWrite(DISP1_A,HIGH);
              digitalWrite(DISP1_B,HIGH);
              digitalWrite(DISP1_C,LOW);
              digitalWrite(DISP1_D,HIGH);
              digitalWrite(DISP1_E,HIGH);
              digitalWrite(DISP1_F,LOW);
              digitalWrite(DISP1_G,HIGH);
              break;
      case 3: //exclude pins 8 and 7
              digitalWrite(DISP1_A,HIGH);
              digitalWrite(DISP1_B,HIGH);
              digitalWrite(DISP1_C,HIGH);
              digitalWrite(DISP1_D,HIGH);
              digitalWrite(DISP1_E,LOW);
              digitalWrite(DISP1_F,LOW);
              digitalWrite(DISP1_G,HIGH);
              break;
      case 4: //exclude pins 7, 6, and 0.
              digitalWrite(DISP1_A,LOW);
              digitalWrite(DISP1_B,HIGH);
              digitalWrite(DISP1_C,HIGH);
              digitalWrite(DISP1_D,LOW);
              digitalWrite(DISP1_E,LOW);
              digitalWrite(DISP1_F,HIGH);
              digitalWrite(DISP1_G,HIGH);
              break;
      case 5: //exclude pins 1 and 7
              digitalWrite(DISP1_A,HIGH);
              digitalWrite(DISP1_B,LOW);
              digitalWrite(DISP1_C,HIGH);
              digitalWrite(DISP1_D,HIGH);
              digitalWrite(DISP1_E,LOW);
              digitalWrite(DISP1_F,HIGH);
              digitalWrite(DISP1_G,HIGH);
              break;
      case 6: //exclude pin 1
              digitalWrite(DISP1_A,HIGH);
              digitalWrite(DISP1_B,LOW);
              digitalWrite(DISP1_C,HIGH);
              digitalWrite(DISP1_D,HIGH);
              digitalWrite(DISP1_E,HIGH);
              digitalWrite(DISP1_F,HIGH);
              digitalWrite(DISP1_G,HIGH);
              break;
      case 7: //write to pins 0, 1, and 5
              digitalWrite(DISP1_A,HIGH);
              digitalWrite(DISP1_B,HIGH);
              digitalWrite(DISP1_C,HIGH);
              digitalWrite(DISP1_D,LOW);
              digitalWrite(DISP1_E,LOW);
              digitalWrite(DISP1_F,LOW);
              digitalWrite(DISP1_G,LOW);
              break;
      case 8: //write to all pins
              digitalWrite(DISP1_A,HIGH);
              digitalWrite(DISP1_B,HIGH);
              digitalWrite(DISP1_C,HIGH);
              digitalWrite(DISP1_D,HIGH);
              digitalWrite(DISP1_E,HIGH);
              digitalWrite(DISP1_F,HIGH);
              digitalWrite(DISP1_G,HIGH);
              break;
      case 9: //excude pins 7 and 6
              digitalWrite(DISP1_A,HIGH);
              digitalWrite(DISP1_B,HIGH);
              digitalWrite(DISP1_C,HIGH);
              digitalWrite(DISP1_D,LOW);
              digitalWrite(DISP1_E,LOW);
              digitalWrite(DISP1_F,HIGH);
              digitalWrite(DISP1_G,HIGH);
              break;
       default:   digitalWrite(DISP1_A,LOW); //if there's something wrong it will display X
                  digitalWrite(DISP1_B,HIGH);
                  digitalWrite(DISP1_C,HIGH);
                  digitalWrite(DISP1_D,LOW);
                  digitalWrite(DISP1_E,HIGH);
                  digitalWrite(DISP1_F,HIGH);
                  digitalWrite(DISP1_G,HIGH);
                  break;
    }
  else //right display
    switch(intput){
      case 0: //write to all pins except pin 9 - see explanation in header
              digitalWrite(DISP2_A,HIGH);
              digitalWrite(DISP2_B,HIGH);
              digitalWrite(DISP2_C,HIGH);
              digitalWrite(DISP2_D,HIGH);
              digitalWrite(DISP2_E,HIGH);
              digitalWrite(DISP2_F,HIGH);
              digitalWrite(DISP2_G,LOW);
              break; 
      case 1: //write to pins 1 and 5
              digitalWrite(DISP2_A,LOW);
              digitalWrite(DISP2_B,HIGH);
              digitalWrite(DISP2_C,HIGH);
              digitalWrite(DISP2_D,LOW);
              digitalWrite(DISP2_E,LOW);
              digitalWrite(DISP2_F,LOW);
              digitalWrite(DISP2_G,LOW);
              break;
      case 2: //exclude pin 8 and pin 5
              digitalWrite(DISP2_A,HIGH);
              digitalWrite(DISP2_B,HIGH);
              digitalWrite(DISP2_C,LOW);
              digitalWrite(DISP2_D,HIGH);
              digitalWrite(DISP2_E,HIGH);
              digitalWrite(DISP2_F,LOW);
              digitalWrite(DISP2_G,HIGH);
              break;
      case 3: //exclude pins 8 and 7
              digitalWrite(DISP2_A,HIGH);
              digitalWrite(DISP2_B,HIGH);
              digitalWrite(DISP2_C,HIGH);
              digitalWrite(DISP2_D,HIGH);
              digitalWrite(DISP2_E,LOW);
              digitalWrite(DISP2_F,LOW);
              digitalWrite(DISP2_G,HIGH);
              break;
      case 4: //exclude pins 7, 6, and 0.
              digitalWrite(DISP2_A,LOW);
              digitalWrite(DISP2_B,HIGH);
              digitalWrite(DISP2_C,HIGH);
              digitalWrite(DISP2_D,LOW);
              digitalWrite(DISP2_E,LOW);
              digitalWrite(DISP2_F,HIGH);
              digitalWrite(DISP2_G,HIGH);
              break;
      case 5: //exclude pins 1 and 7
              digitalWrite(DISP2_A,HIGH);
              digitalWrite(DISP2_B,LOW);
              digitalWrite(DISP2_C,HIGH);
              digitalWrite(DISP2_D,HIGH);
              digitalWrite(DISP2_E,LOW);
              digitalWrite(DISP2_F,HIGH);
              digitalWrite(DISP2_G,HIGH);
              break;
      case 6: //exclude pin 1
              digitalWrite(DISP2_A,HIGH);
              digitalWrite(DISP2_B,LOW);
              digitalWrite(DISP2_C,HIGH);
              digitalWrite(DISP2_D,HIGH);
              digitalWrite(DISP2_E,HIGH);
              digitalWrite(DISP2_F,HIGH);
              digitalWrite(DISP2_G,HIGH);
              break;
      case 7: //write to pins 0, 1, and 5
              digitalWrite(DISP2_A,HIGH);
              digitalWrite(DISP2_B,HIGH);
              digitalWrite(DISP2_C,HIGH);
              digitalWrite(DISP2_D,LOW);
              digitalWrite(DISP2_E,LOW);
              digitalWrite(DISP2_F,LOW);
              digitalWrite(DISP2_G,LOW);
              break;
      case 8: //write to all pins
              digitalWrite(DISP2_A,HIGH);
              digitalWrite(DISP2_B,HIGH);
              digitalWrite(DISP2_C,HIGH);
              digitalWrite(DISP2_D,HIGH);
              digitalWrite(DISP2_E,HIGH);
              digitalWrite(DISP2_F,HIGH);
              digitalWrite(DISP2_G,HIGH);
              break;
      case 9: //excude pins 7 and 6
              digitalWrite(DISP2_A,HIGH);
              digitalWrite(DISP2_B,HIGH);
              digitalWrite(DISP2_C,HIGH);
              digitalWrite(DISP2_D,LOW);
              digitalWrite(DISP2_E,LOW);
              digitalWrite(DISP2_F,HIGH);
              digitalWrite(DISP2_G,HIGH);
              break;
       default:   digitalWrite(DISP2_A,LOW); //if there's something wrong it will display X
                  digitalWrite(DISP2_B,HIGH);
                  digitalWrite(DISP2_C,HIGH);
                  digitalWrite(DISP2_D,LOW);
                  digitalWrite(DISP2_E,HIGH);
                  digitalWrite(DISP2_F,HIGH);
                  digitalWrite(DISP2_G,HIGH);
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
      keepScore = !keepScore;
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

void testPattern(int disp)
{
  if(disp == 0)
  {
    digitalWrite(DISP1_A, HIGH);
    delay(300);
    digitalWrite(DISP1_A, LOW);
    digitalWrite(DISP1_B, HIGH);
    delay(300);
    digitalWrite(DISP1_B, LOW);
    digitalWrite(DISP1_C, HIGH);
    delay(300);
    digitalWrite(DISP1_C, LOW);
    digitalWrite(DISP1_D, HIGH);
    delay(300);
    digitalWrite(DISP1_D, LOW);
    digitalWrite(DISP1_E, HIGH);
    delay(300);
    digitalWrite(DISP1_E, LOW);
    digitalWrite(DISP1_F, HIGH);
    delay(300);
    digitalWrite(DISP1_F, LOW);
    digitalWrite(DISP1_G, HIGH);
    delay(300);
    digitalWrite(DISP1_G, LOW);
  }
  else if(disp == 1)
  {
    digitalWrite(DISP2_A, HIGH);
    delay(300);
    digitalWrite(DISP2_A, LOW);
    digitalWrite(DISP2_B, HIGH);
    delay(300);
    digitalWrite(DISP2_B, LOW);
    digitalWrite(DISP2_C, HIGH);
    delay(300);
    digitalWrite(DISP2_C, LOW);
    digitalWrite(DISP2_D, HIGH);
    delay(300);
    digitalWrite(DISP2_D, LOW);
    digitalWrite(DISP2_E, HIGH);
    delay(300);
    digitalWrite(DISP2_E, LOW);
    digitalWrite(DISP2_F, HIGH);
    delay(300);
    digitalWrite(DISP2_F, LOW);
    digitalWrite(DISP2_G, HIGH);
    delay(300);
    digitalWrite(DISP2_G, LOW);
    
  }
}
void loop() {
  // put your main code here, to run repeatedly:
  //checkScorekeeping();
  //if(keepScore) //this controls scoring being off or on
  {
    //checkButtons();
    //intToDigWrite(redScore,0);
    //intToDigWrite(blueScore,1);
    //delay(500);
    //testPattern(1);
    //testPattern(0);
    for(int i = 0; i <= 9; i++)
    {
      delay(250);
      intToDigWrite(i, 0);
      intToDigWrite(i, 1);
    }
 }
}
