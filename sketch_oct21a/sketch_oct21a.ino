const int FSRPin = A0;

const int Weight_Tolerance = 0.9;
const float Vcc = 3.3;
float fsrVoltage;
float fsrResistance;
float weight;
float A = 100.0;              
float B = -1.5;
int Max_Weight = 0;

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