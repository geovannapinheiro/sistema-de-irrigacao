#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>

const char* ssid = "redeWifi";
const char* password = "**********";

int sensorPin = A0;
int waterPump = D3;

int soilWet = 300;
int soilDry = 750;

WiFiClient client;

unsigned long ChannelID = 2139052;
const char *WriteAPIKey = "0HF5EWYA8WO69QEW";

void setup() {
  Serial.begin(9600);
  pinMode(sensorPin, INPUT);
  pinMode(waterPump, OUTPUT);

  Serial.print("CONECTANDO...");
  Serial.println(ssid);
  delay(100);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("IP address");
  Serial.println(WiFi.localIP());

  ThingSpeak.begin(client);
}

void controlWaterPump(bool turnOn) {

  Serial.println();
  Serial.print("CONTROLANDO A BOMBA DE ÁGUA");
  Serial.println();

  if (turnOn) {
    Serial.println("--- LIGANDO ---");
    Serial.println();
    digitalWrite(waterPump, LOW);

  } else {
    Serial.println("--- DESLIGANDO ---");
    Serial.println();
    digitalWrite(waterPump, HIGH);
  }
}

void loop() {
  int sensorValue = analogRead(sensorPin);

  Serial.print("UMIDADE: ");
  Serial.println(sensorValue);
  Serial.println();

  if (sensorValue <= soilWet) {
    Serial.println("STATUS: solo muito molhado");
    controlWaterPump(false);
    Serial.println();
    ThingSpeak.writeField(ChannelID, 1, sensorValue, WriteAPIKey);

  } else if (sensorValue >= soilWet && sensorValue < soilDry) {
    Serial.println("STATUS: nível de umidade do solo está ok");
    controlWaterPump(false);
    Serial.println();
    ThingSpeak.writeField(ChannelID, 1, sensorValue, WriteAPIKey);

  } else {
    Serial.println("STATUS: nível de umidade do solo muito seco, hora de regar!");
    controlWaterPump(true);
    Serial.println();
    delay(5000);
    controlWaterPump(true);
    Serial.println();
    ThingSpeak.writeField(ChannelID, 1, sensorValue, WriteAPIKey);
  }

  delay(5000);
}