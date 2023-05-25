#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>


const char* ssid = "Bento23";
const char* password = "Bento23@";
WiFiClient client;
unsigned long ChannelID = 2154237;
const char *WriteAPIKey =  "BFCP7RGFZZP31MBJ";

int Relay =  D3;
int sensor = A0, soil;
  
void setup() {
  pinMode(Relay, OUTPUT);
  Serial.begin(9600);
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


void loop() {
  soil = analogRead(A0);
  soil = map(soil, 1023, 460, 0, 100);
  Serial.println("UMIDADE:");
  Serial.println(soil);
  ThingSpeak.writeField(ChannelID, 1, soil, WriteAPIKey);
  if (soil < 20){
    Serial.println("Regando...");
    Serial.println("");
    digitalWrite(Relay, LOW);
    
  } else {
    Serial.println("Bomba desativada.");
    Serial.println("");
    digitalWrite(Relay, HIGH);
  }
  delay(3000);
}




