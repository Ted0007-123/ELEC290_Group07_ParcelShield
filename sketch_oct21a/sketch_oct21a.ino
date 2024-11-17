#define int FSRPin = A0;
#define int echoPin = 12;
#define int trigPin = 13;
#include <Keypad.h>
#include <ctype.h>

const byte ROWS=4;
const byte COLS=3;

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
char numberKeys[ROWS][COLS] = {
    { '1','2','3' },
    { '4','5','6' },
    { '7','8','9' },
    { ' ','0','#' }
};
boolean alpha = false;
byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 7, 6}; //connect to the column pinouts of the keypad
Keypad numpad( makeKeymap(numberKeys), rowPins, colPins, sizeof(rowPins), sizeof(colPins) );
Keypad ltrpad( makeKeymap(alphaKeys), rowPins, colPins, sizeof(rowPins), sizeof(colPins) );

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
void keypadEvent_ltr(KeypadEvent key) {
    // in here when in alpha mode.
    kpadState = ltrpad.getState( );
    swOnState( key );
} // end ltrs keypad events

void keypadEvent_num( KeypadEvent key ) {
    // in here when using number keypad
    kpadState = numpad.getState( );
    swOnState( key );
} // end numbers keypad events

void swOnState( char key ) {
    switch( kpadState ) {
        case PRESSED:
            if (isalpha(key)) {              // This is a letter key so we're using the letter keymap.
                if (physKey != key) {        // New key so start with the first of 3 characters.
                    pressCount = 0;
                    virtKey = key;
                    physKey = key;
                }
                else {                       // Pressed the same key again...
                    virtKey++;                   // so select the next character on that key.
                    pressCount++;                // Tracks how many times we press the same key.
                }
                    if (pressCount > 2) {    // Last character reached so cycle back to start.
                        pressCount = 0;
                        virtKey = key;
                    }
                    Serial.print(virtKey);   // Used for testing.
                }
                if (isdigit(key) || key == ' ' || key == '.')
                    Serial.print(key);
                if (key == '#')
                    Serial.println();
                break;

        case HOLD:
            if (key == '#')  {               // Toggle between keymaps.
                if (alpha == true)  {        // We are currently using a keymap with letters
                    alpha = false;           // Now we want a keymap with numbers.
                    digitalWrite(ledPin, LOW);
                }
                else  {                      // We are currently using a keymap with numbers
                    alpha = true;            // Now we want a keymap with letters.
                }
            }
            else  {                          // Some key other than '#' was pressed.
                buildStr[buildCount++] = (isalpha(key)) ? virtKey : key;
                buildStr[buildCount] = '\0';
                Serial.println();
                Serial.println(buildStr);
            }
            break;

        case RELEASED:
            if (buildCount >= sizeof(buildStr))  buildCount = 0;  // Our string is full. Start fresh.
            break;
    }  // end switch-case
}// end switch on state function

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Lid_distance = calculate_distance();
  Max_Weight  = calculate_Weight();
  ltrpad.begin( makeKeymap(alphaKeys) );
  numpad.begin( makeKeymap(numberKeys) );
  ltrpad.setHoldTime(500);                   // Default is 1000mS
  numpad.addEventListener(keypadEvent_num);  // Add an event listener.
  numpad.setHoldTime(500);     

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