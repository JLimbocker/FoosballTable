int prev_state[3];
int curr_state[3];
int leftScore=0;
int rightScore=0;
String score;

void setup() {
  // put your setup code here, to run once:
  for(int i = 2; i < 5; i++){
    pinMode(i, INPUT_PULLUP);
  }
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
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
}
