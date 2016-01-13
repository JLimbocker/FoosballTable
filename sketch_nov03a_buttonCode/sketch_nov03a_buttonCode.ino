int const LIMSWITCH1 = 2, LIMSWITCH2 = 3; //change these to whatever port the lim switches occupy
int pstatus1 = -1, pstatus2 = -1;

void setup() {
  // put your setup code here, to run once:
  pinMode(LIMSWITCH1, INPUT);
  pinMode(LIMSWITCH2, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int limS1_status, limS2_status; //these will hold the active statuses of the lim switches

  limS1_status = digitalRead(LIMSWITCH1);//get statuses
  limS2_status = digitalRead(LIMSWITCH2);
  
  if(limS1_status!=pstatus1){
    if(limS1_status==HIGH) 
      Serial.println("LimS1 is active");
    else 
      Serial.println("LimS1 is inactive");
  };
  limS1_status = pstatus1;

  if(limS2_status!=pstatus2){
    if(limS2_status==HIGH) Serial.println("LimS2 is active");
    else Serial.println("LimS2 is inactive");
  }
  limS2_status = pstatus2;
}
