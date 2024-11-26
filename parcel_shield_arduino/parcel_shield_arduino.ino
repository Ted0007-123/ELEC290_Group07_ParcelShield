#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Cracker boiz";
const char* password = "Xaviers3rdNut";
const char* mqtt_server = "3.15.24.121";

WiFiClient espClient;
PubSubClient client(espClient);

const int fsrPin = A0; // FSR sensor connected to analog pin A0
const int threshold = 900; // Define the threshold value

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  client.setServer(mqtt_server, 1883);
  reconnect();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  int fsrValue = analogRead(fsrPin);
  Serial.print("FSR Value: ");
  Serial.println(fsrValue);

  if (fsrValue > threshold) {
    client.publish("alerts/fsr", "present"); // Publish to the "fsr/alert" topic
    delay(5000); // Don't overflood the broker
  }
  else {
    client.publish("fsr/alert", "not present");
    delay(5000); // Don't overflood the broker
  }
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("arduino_client")) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("Failed to connect. Attempting again in 5 seconds.");
      delay(5000);
    }
  }
}