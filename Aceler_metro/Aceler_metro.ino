#include <Arduino.h>

int ad=3;
int at=4
int mot=2;

int boton=5;

const int xPin = 0;
const int yPin = 1;
const int zPin = 2;
 

int minX = 267;
int maxX = 404;
int minY = 268;
int maxY = 404;
int minZ = 264;
int maxZ = 398;

double x;
double y;
double z;
void setup(){
 Serial.begin(9600);
 pinMode(boton, INPUT);
 pinMode(ad, OUTPUT);
 pinMode(at,OUTPUT);
 pinMode(13,OUTPUT);
 digitalWrite(13,HIGH);
}
void loop(){
   
   
 int xRead = analogRead(xPin);
 int yRead = analogRead(yPin);
 int zRead = analogRead(zPin);
 

 int xAng = map(xRead, minX, maxX, -90, 90);
 int yAng = map(yRead, minY, maxY, -90, 90);
 int zAng = map(zRead, minZ, maxZ, -90, 90);
 
 Serial.print(xAng*DEG_TO_RAD);
 Serial.print(",");
 Serial.print(yAng*DEG_TO_RAD);
 Serial.print(",");
 Serial.println(zAng*DEG_TO_RAD);
 
 buttonState=digitalRead(boton);
 
 if (buttonState==HIGH){
   x=0;
   y=0;
   z=0;
 }
 
 
 if (xAng<50){
   adelante();
 }
 
 if (xAng>50){
   abajo();
 }
 
}

void adelante(){
  digitalWrite(ad, LOW);
  digitalWrite(mot,HIGH);
  analogWrite(at, vel);
  delay (100);
}

void atras(){
  digitalWrite(at, LOW);
  digitalWrite(mot,HIGH);
  analogWrite(ad, vel);
  delay(100);
}
  
