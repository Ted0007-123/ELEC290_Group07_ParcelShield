#define int FSRPin = A0;
#define int echoPin = 1;
#define int trigPin = 2;

const int Tolerance = 0.9;
const float Vcc = 3.3;
float fsrVoltage;
float fsrResistance;
float weight;
float A = 100.0;              
float B = -1.5;
int Max_Weight;
int Lid_Distance;
bool Authorization = false;
//Master Password (Ted's birth date)
const long Password_Master = 20001012;

//To manage password as Linkedlist, define node
typedef struct PasswordNode{
  long Password;
  struct PasswordNode* next;
}PasswordNode;

//Random 8 Digit Number generator
long num_Randoms() {
    long num = 0;
    for(int i =0; i < 8; i++) {
        num = num * 10 + rand() % 10;
    }
    return num;
}

//Initiate password list
PasswordNode* password_list_head = NULL;

//Create password of 8 digit random number, push it to head of the list
long createPassword(){
  long pwd = num_Randoms();
  PasswordNode* new_password = (PasswordNode*)malloc(sizeof(PasswordNode));
  new_password -> Password = pwd;
  new_password -> next = password_list_head;
  password_list_head = new_password;
  return pwd;
}

//Take in a head reference, the only case for us will be password_list_head, and a password value
bool is_Password_remove(struct PasswordNode** head_ref, long pwd){
  //if the list is empty, return false
  if(*head_ref==NULL){
    return false;
  }
  //if password of the head node is equal to the input password, remove the node
  if((*head_ref) -> Password == pwd){
    struct PasswordNode* temp = *head_ref;
    *head_ref = (*head_ref)-> next;
    free(temp);
    return true;
  }
  //if the password of head node does not match, move onto next node and repeat same process
  return is_Password_remove(&((*head_ref)->next), pwd);
}

int calculate_Weight(){
  int FSRVal_Analog = analogRead(FSRPin);
  FSRVoltage = (FSRVal_Analog/1023.0)*Vcc;
  if(FSRVoltage > 0){
    R_FSR = R_known * (Vcc - FSRVoltage) / FSRVoltage;
    weight = A*pow(R_FSR, B)
  }
  return weight;
}
void sound_Alarm(){

}

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Lid_distance = calculate_distance();
  Max_Weight  = calculate_Weight();
}

//calculates distance of ultrasonic sensor in mm
int calculate_distance(){
  digitalWrite(trigPin, Low);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH); // turn on the Trigger to generate pulse
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  long duration = pulseIn(echoPin, HIGH);
  float distance= duration * 0.00344 / 2; // Expression to calculate
}

void loop() {
  if(Max_Weight > calculate_Weight()* Tolerance){
    sound_Alarm();
  }else if(Max_Weight < calculate_Weight()){
    Max_Weight = calculate_Weight();
  }
  if(Lid_Distance > calculate_distance()){
    Lid_Distance = calculate_distance();
  }else if(Lid_distance < calculate_distance()*Tolerance && Authorization == false){
    sound_Alarm();
  }
}