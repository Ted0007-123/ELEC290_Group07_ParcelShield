#define Weight_Tolerance = 0.9
void setup() {
  // put your setup code here, to run once:
  int Max_Weight = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Max_Weight > calculate_Weight()* Weight_Tolerance && isAuthorized()){
    sound_Alarm();
  }else if(Max_Weight < calculate_Weight()){
    Max_Weight = calculate_Weight();
  }
}

int calculate_Weight(){

}
void sound_Alarm(){

}
bool isAuthorized(){

}