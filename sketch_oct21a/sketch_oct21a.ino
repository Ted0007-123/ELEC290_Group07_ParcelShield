#include <Keypad.h>
const int FSRPin = A0;

const int Weight_Tolerance = 0.9;
const float Vcc = 3.3;
const float knownResistance = 12000;
float fsrVoltage;
float fsrResistance;
float mass;
float A = 271.0;              
float B = -0.69;
int Max_Weight = 0;

byte pin_rows[4] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte pin_column[3] = {5, 4, 3}; //connect to the column pinouts of the keypad

char keys[4][3] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

void setup() {
}

void loop() {
  if(Max_Weight > calculate_Weight()* Weight_Tolerance){
    sound_Alarm();
  }else if(Max_Weight < calculate_Weight()){
    Max_Weight = calculate_Weight();
  }
}

float calculate_Weight(){
  fsrVoltage = (analogRead(A0)*Vcc/1023);
  fsrResistance = resistance_calculation(fsrVoltage);
  fsrResistance /= 1000; //into kilo ohms
  mass = pow((fsrResistance/A),1/B)/9.81;
}
float resistance_calculation(float voltage_in){
  return (knownResistance * (Vcc - voltage_in )/voltage_in);
}
void sound_Alarm(){

}
