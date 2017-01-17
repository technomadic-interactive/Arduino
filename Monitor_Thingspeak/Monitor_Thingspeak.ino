#include <Arduino.h>

#include <SPI.h>
#include <WiFi.h>
int x;
char ssid[]="Anibal Bustillo";
char pass[]="8d0iwbqey31oh";
int mon;
String stmon;
int status = WL_IDLE_STATUS;
IPAddress server(144,212,80,10);
WiFiClient client;

void setup(){
  Serial.begin(9600);
  pinMode(10, INPUT); // Setup for leads off detection LO +
  pinMode(11, INPUT); // Setup for leads off detection LO -
  x=1;
  while(x==1){
  status=WiFi.begin(ssid, pass);
  Serial.println("wifi encontrado");
  if(status != WL_CONNECTED){
    WiFiClient client;
    Serial.println("conectando");
    delay(1000);
  }
  
  else{
    Serial.println("Se tratará de conectar al servidor");
    if (client.connect(server,80)){
    Serial.print("conectado");
    x=0;
  }
  }
}
}

void loop(){
  String x="GET /update?key=46N58BVV14QWC9OT&field1=";
  if((digitalRead(10) == 1)||(digitalRead(11) == 1)){
  Serial.println('!');
  }
  else{
  Serial.println(analogRead(A0));
  mon=analogRead(A0);
  stmon=x+String(mon);

  client.println(stmon);
  }
  delay(1);
//  status=WiFi.begin(ssid, pass);
//    if(status != WL_CONNECTED){
//    WiFiClient client;
//    Serial.println("me desconecté");
//    delay(10);
//  }
}

