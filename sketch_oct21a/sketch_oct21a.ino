const int FSRPin = A0;

const int Weight_Tolerance = 0.9;
const float Vcc = 3.3;
float fsrVoltage;
float fsrResistance;
float weight;
float A = 100.0;              
float B = -1.5;
int Max_Weight = 0;
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
int is_Password_remove(struct PasswordNode** head_ref, long pwd){
  //if the list is empty, return false
  if(*head_ref==NULL){
    return 0;
  }
  //if password of the head node is equal to the input password, remove the node
  if((*head_ref) -> Password == pwd){
    struct PasswordNode* temp = *head_ref;
    *head_ref = (*head_ref)-> next;
    free(temp);
    return 1;
  }
  //if the password of head node does not match, move onto next node and repeat same process
  return is_Password_remove(&((*head_ref)->next), pwd);
}

void setup() {
}

void loop() {
  if(Max_Weight > calculate_Weight()* Weight_Tolerance){
    sound_Alarm();
  }else if(Max_Weight < calculate_Weight()){
    Max_Weight = calculate_Weight();
  }
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