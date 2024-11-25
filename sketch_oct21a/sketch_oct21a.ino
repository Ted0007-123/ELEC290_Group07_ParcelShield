#define FSRPin A0
#define echoPin 12
#define trigPin 13
#define BuzzPin 10
#include <Keypad.h>
#include <ctype.h>

const byte ROWS=4;
const byte COLS=3;
float Lid_Distance;
float curr_Distance;
bool Authorization = false;
float Max_FSR=0;
float currFSR=0;

// char numberKeys[ROWS][COLS] = {
//     { '1','2','3' },
//     { '4','5','6' },
//     { '7','8','9' },
//     { ' ','0','#' }
// };
// boolean alpha = false;
// byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
// byte colPins[COLS] = {8, 7, 6}; //connect to the column pinouts of the keypad
// Keypad numpad( makeKeymap(numberKeys), rowPins, colPins, sizeof(rowPins), sizeof(colPins) );
// Keypad ltrpad( makeKeymap(alphaKeys), rowPins, colPins, sizeof(rowPins), sizeof(colPins) );

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(BuzzPin, OUTPUT);
  pinMode(FSRPin, INPUT);
  Lid_Distance = 100;

  // ltrpad.begin( makeKeymap(alphaKeys) );
  // // numpad.begin( makeKeymap(numberKeys) );
  // ltrpad.setHoldTime(500);                   // Default is 1000mS
  // numpad.addEventListener(keypadEvent_num);  // Add an event listener.
  // numpad.setHoldTime(500);     
  Serial.begin(9600);
}

void loop() {
  delay(500);
  // currFSR = analogRead(FSRPin);
  curr_Distance = calculate_distance();
  if(Lid_Distance > curr_Distance || Max_FSR <= currFSR){
    Lid_Distance = curr_Distance;
    Max_FSR = currFSR;
    stop_Alarm();
  }else if(Lid_Distance < curr_Distance*0.9){
    sound_Alarm();
  }
  Serial.print("Lid distance:     ");
  Serial.println(Lid_Distance);
  Serial.print("current distance: ");
  Serial.println(calculate_distance());


  // if(){
  //   stop_Alarm();
  // }else if(Max_FSR*0.9 > currFSR){
  //   sound_Alarm();
  // }

  // Serial.print("Max Weight       ");
  // Serial.println(Max_FSR);  
  // Serial.print("current Weight   ");
  // Serial.println(currFSR);
  delay(500);
}

void sound_Alarm(){
  // Serial.println("Alarm Initiated");
  digitalWrite(BuzzPin, HIGH);
}
void stop_Alarm(){
  digitalWrite(BuzzPin, LOW);
}

// void keypadEvent_ltr(KeypadEvent key) {
//     // in here when in alpha mode.
//     kpadState = ltrpad.getState();
//     swOnState( key );
// } // end ltrs keypad events

// void keypadEvent_num( KeypadEvent key ) {
//     // in here when using number keypad
//     kpadState = numpad.getState( );
//     swOnState( key );
// } // end numbers keypad events

// void swOnState( char key ) {
//     switch( kpadState ) {
//         case PRESSED:
//             if (isalpha(key)) {              // This is a letter key so we're using the letter keymap.
//                 if (physKey != key) {        // New key so start with the first of 3 characters.
//                     pressCount = 0;
//                     virtKey = key;
//                     physKey = key;
//                 }
//                 else {                       // Pressed the same key again...
//                     virtKey++;                   // so select the next character on that key.
//                     pressCount++;                // Tracks how many times we press the same key.
//                 }
//                     if (pressCount > 2) {    // Last character reached so cycle back to start.
//                         pressCount = 0;
//                         virtKey = key;
//                     }
//                     Serial.print(virtKey);   // Used for testing.
//                 }
//                 if (isdigit(key) || key == ' ' || key == '.')
//                     Serial.print(key);
//                 if (key == '#')
//                     Serial.println();
//                 break;

//         case HOLD:
//             if (key == '#')  {               // Toggle between keymaps.
//                 if (alpha == true)  {        // We are currently using a keymap with letters
//                     alpha = false;           // Now we want a keymap with numbers.
//                     digitalWrite(ledPin, LOW);
//                 }
//                 else  {                      // We are currently using a keymap with numbers
//                     alpha = true;            // Now we want a keymap with letters.
//                 }
//             }
//             else  {                          // Some key other than '#' was pressed.
//                 buildStr[buildCount++] = (isalpha(key)) ? virtKey : key;
//                 buildStr[buildCount] = '\0';
//                 Serial.println();
//                 Serial.println(buildStr);
//             }
//             break;

//         case RELEASED:
//             if (buildCount >= sizeof(buildStr))  buildCount = 0;  // Our string is full. Start fresh.
//             break;
//     }  // end switch-case
// }// end switch on state function



//calculates distance of ultrasonic sensor in mm
float calculate_distance(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH); // turn on the Trigger to generate pulse
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  long duration = pulseIn(echoPin, HIGH);
  float distance= duration * 0.00344 / 2; // Expression to calculate
  return distance;
}

