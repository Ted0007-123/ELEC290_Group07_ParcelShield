#define FSRPin A0
#define echoPin 12
#define trigPin 13
#define BuzzPin 10
#include <Keypad.h>

const int ROWS=4;
const int COLS=3;
float Lid_Distance;
bool Authorization = false;
bool Alarm_Status1 = false;
bool Alarm_Status2 = false;
float Max_FSR=0;
float currFSR=0;
char password[9];

char keys[ROWS][COLS] = {
    { '1','2','3' },
    { '4','5','6' },
    { '7','8','9' },
    { '*','0','#' }
};
byte pin_rows[ROWS] = {4,9,8,6}; //connect to the row pinouts of the keypad
byte pin_column[COLS] = {5,3,7}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROWS, COLS );

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(BuzzPin, OUTPUT);
  pinMode(FSRPin, INPUT);
  Lid_Distance = 100;
  Serial.begin(9600);
}

int index=0;
void loop() {
  //Get a single digit to see if * is a input
  char key = keypad.getKey();
  //If * is entered, start getting user password
  if(key == '*'){
    index = 0;
    Serial.println("Start Entering Password");
    while(index<8){
      key = keypad.getKey();
      if(isDigit(key)){
        Serial.print('*');
        password[index] = key;
        index++;
      }
    }
    password[9] = NULL;
    //Test code to check password was entered properly
    for(int i; i<8; i++){
      Serial.print(password[i]);
    }
  }

  long pwdLong = atol(password);
  Authorization = request_confirmation(pwdLong);

  //Update current values of FSR reading and distance between ultrasonic sensor and the lid
  currFSR = analogRead(FSRPin);

  Alarm_Status1 = (calculate_distance() > 10 && !Authorization);
  Max_FSR = (Max_FSR < currFSR) ? currFSR : Max_FSR;
  Alarm_Status2 = (Max_FSR * 0.9 > currFSR);
  (Alarm_Status1 || Alarm_Status2) ? sound_Alarm() : stop_Alarm();
}

void sound_Alarm(){
  // Serial.println("Alarm Initiated");
  digitalWrite(BuzzPin, HIGH);
}
void stop_Alarm(){
  digitalWrite(BuzzPin, LOW);
}

//calculates distance of ultrasonic sensor in mm
float calculate_distance(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH); // turn on the Trigger to generate pulse
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  long duration = pulseIn(echoPin, HIGH);
  float distance= duration * 0.0344 / 2; // Expression to calculate
  return distance;
}

bool request_confirmation(long pwd){
  return(pwd==11111111);
}