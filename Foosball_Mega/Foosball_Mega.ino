#define DIGIT_0 0b00111111
#define DIGIT_1 0b00000110
#define DIGIT_2 0b01011011
#define DIGIT_3 0b01001111
#define DIGIT_4 0b01100110
#define DIGIT_5 0b01101101
#define DIGIT_6 0b01111101
#define DIGIT_7 0b00000111
#define DIGIT_8 0b01111111
#define DIGIT_9 0b01100111
#define DIGIT_A 0b01110111
#define DIGIT_B 0b01111111
#define DIGIT_C 0b00111001
#define DIGIT_D 0b00111111
#define DIGIT_E 0b01111001
#define DIGIT_F 0b01110001
#define SEG_A 0b00000001
#define SEG_B 0b00000010
#define SEG_C 0b00000100
#define SEG_D 0b00001000
#define SEG_E 0b00010000
#define SEG_F 0b00100000
#define SEG_G 0b01000000
#define SCORE_RED 2
#define SCORE_BLUE 3
byte digits[] = {DIGIT_0, DIGIT_1, DIGIT_2, DIGIT_3, DIGIT_4, DIGIT_5, DIGIT_6, DIGIT_7, DIGIT_8, DIGIT_9, DIGIT_A, DIGIT_B, DIGIT_C, DIGIT_D, DIGIT_E, DIGIT_F};
byte redScore = 0;
byte blueScore = 0;
//disp = 0 -> Red Display
//disp = 1 -> Blue Display
void writeDigit(int disp, byte digit)
{
  if(disp == 0)
    PORTA = digits[digit];
  else if(disp == 1)
    PORTC = digits[digit];
}

void testPattern()
{
  PORTA = 0b00000001; //SEG A
  PORTC = 0b00000001;
  delay(100);
  PORTA = 0b00000010; //SEG B
  PORTC = 0b00000010;
  delay(100);
  PORTA = 0b01000000; //SEG G
  PORTC = 0b01000000;
  delay(100);
  PORTA = 0b00010000; //SEG E
  PORTC = 0b00010000;
  delay(100);
  PORTA = 0b00001000; //SEG D
  PORTC = 0b00001000;
  delay(100);
  PORTA = 0b00000100; //SEG C
  PORTC = 0b00000100;
  delay(100);
  PORTA = 0b01000000; //SEG G
  PORTC = 0b01000000;
  delay(100);
  PORTA = 0b00100000; //SEG F
  PORTC = 0b00100000;
  delay(100);
}

void incrementBlueScore()
{
  noInterrupts();
  blueScore++;
  if(blueScore > 9)
    blueScore = 9;
  PORTA = digits[redScore];
  PORTC = digits[blueScore];
  
  delay(250); //delay to debounce switch
  interrupts();
}
void incrementRedScore()
{
  noInterrupts();
  redScore++;
  if(redScore > 9)
    redScore = 9;
  PORTA = digits[redScore];
  PORTC = digits[blueScore];
  
  delay(250); //delay to debounce switch
  interrupts();
}
void setup() {
  redScore = 0;
  blueScore = 0;
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SCORE_BLUE), incrementBlueScore, FALLING);
  attachInterrupt(digitalPinToInterrupt(SCORE_RED), incrementRedScore, FALLING);
  DDRA = 0xFF; //Set portA to digital output
  DDRC = 0xFF; //Set portC to digital output
  PORTA = 0x00; //Init output to low
  PORTC = 0x00; //Init output to low
  testPattern();
  testPattern();
  PORTA = DIGIT_0;
  PORTC = DIGIT_0;
  
}


void loop() {
  // put your main code here, to run repeatedly:
  
  //delay(1000);
  //testPattern();
}
