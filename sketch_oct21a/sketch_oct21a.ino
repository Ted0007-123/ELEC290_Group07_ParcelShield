#define FSRPin A0
#define echoPin 12
#define trigPin 13
#define BuzzPin 10

#include <PubSubClient.h>
// Fix for 'HIGH' redeclaration conflict
#undef HIGH

// Include Keypad first
#include <Keypad.h>

// Temporarily remove 'pinMode' macro to avoid conflicts with WiFiNINA
#undef pinMode

// Include WiFiNINA
#include <WiFi.h>
#define HIGH 0x1


// Redefine 'pinMode' macro for Keypad
#define pinMode(_pin, _mode) _mypinMode(_pin, _mode)


const char* ssid = "Cracker boiz";
const char* password = "Xaviers3rdNut";
const char* mqtt_server = "3.15.24.121";
WiFiClient espClient;
PubSubClient client(espClient);

const int ROWS=4;
const int COLS=3;
float Lid_Distance;
bool Authorization = false;
bool Alarm_Status1 = false;
bool Alarm_Status2 = false;
float Max_FSR=0;
float currFSR=0;
char pwd_temp[9];

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
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  reconnect();
  
}

int index=0;
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
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
        pwd_temp[index] = key;
        index++;
      }
    }
    pwd_temp[9] = NULL;
    //Test code to check password was entered properly
    for(int i; i<8; i++){
      Serial.print(pwd_temp[i]);
    }
  }

  long pwdLong = atol(pwd_temp);
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
  digitalWrite(BuzzPin, 1);
  client.publish("alerts/alarm", "Alarm Initiated");
}
void stop_Alarm(){
  digitalWrite(BuzzPin, 0);
  client.publish("alerts/alarm", "Alarm Deactivated");
}

//calculates distance of ultrasonic sensor in mm
float calculate_distance(){
  digitalWrite(trigPin, 0);
  delayMicroseconds(2);
  digitalWrite(trigPin,1); // turn on the Trigger to generate pulse
  delayMicroseconds(10);
  digitalWrite(trigPin,0);
  long duration = pulseIn(echoPin, 1);
  float distance= duration * 0.0344 / 2; // Expression to calculate
  return distance;
}

bool request_confirmation(long pwd){
  client.publish("otp/confirm", pwd);
  return(pwd==11111111);
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ArduinoClient")) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("Failed to connect. Attempting again in 5 seconds.");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.println(topic);
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println("Message: " + message);
}