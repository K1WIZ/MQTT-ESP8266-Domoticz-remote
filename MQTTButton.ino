#include <ESP8266WiFi.h>
#include <PubSubClient.h>
 
const char* ssid = "MyNetwork";
const char* password =  "P@ssw0rd";
const char* mqttServer = "my.mqtt.com";
const int mqttPort = 1883;
const char* mqttUser = "mqttuser";
const char* mqttPassword = "mqttpass";

const int tsw = 5;    // GPIO pin assigned to read toggle switch
int swstate = 0;
const int ledPin = 4;  // LED Visual Confirmation 

int id = 415;     // IDX address of Domoticz endpoint
int sw = 0;
String str1 = "{\"idx\":";
String str2 = ",\"nvalue\":";
String str3 = "}";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  pinMode(tsw, INPUT);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqttServer, mqttPort);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
 
      Serial.println("connected to MQTT broker");  
 
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
    swstate = digitalRead(tsw);  // read toggle switch to decide to send ON or OFF on power up.

    if (swstate == HIGH) {
      sw = 1;
    } else {
      sw = 0;
    }
  String payload = str1 + id + str2 + sw + str3;
  client.publish("domoticz/in", payload.c_str());
  // Provide visual confirmation command was sent when power is applied via momentary pushbutton
  digitalWrite(ledPin, HIGH);
  delay(10000);
  digitalWrite(ledPin, LOW);
}

void loop() {  
  client.loop();
}
