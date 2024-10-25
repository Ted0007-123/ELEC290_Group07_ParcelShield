const int FSRPin = A0;

const int Weight_Tolerance = 0.9;
const float Vcc = 3.3;
float fsrVoltage;
float fsrResistance;
float weight;
float A = 100.0;              
float B = -1.5;
int Max_Weight = 0;
const long Password_Master = 20001012;

typedef struct PasswordNode{
  long Password;
  struct PasswordNode* next;
}PasswordNode;

PasswordNode* password_list_head = NULL;


long createPassword(){
  long pwd = random(99999999);
  PasswordNode* new_password = (PasswordNode*)malloc(sizeof(PasswordNode));
  new_password -> Password = pwd;
  new_password -> next = password_list_head;
  password_list_head = new_password;
  return pwd;
}

int is_Password_remove(struct PasswordNode** head_ref, long pwd){
  if(*head_ref==NULL){
    return 0;
  }
  if((*head_ref) -> Pwassword == pwd){
    struct PasswordNode* temp = *head_ref;
    *head_ref = (*head_ref)-> next;
    free(temp);
    return 1;
  }
  is_Password_remove(&((*head_ref)->next), pwd);
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