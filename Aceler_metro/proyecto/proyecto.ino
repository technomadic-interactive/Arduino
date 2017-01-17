#include <Arduino.h>

const int xPin = 0;
const int yPin = 1;
const int zPin = 2;

int ena= 2;
int ina= 3;
int inb= 4;
 
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
 pinMode(ena, OUTPUT);
 pinMode(ina, OUTPUT);
 pinMode(inb, OUTPUT);
 Serial.begin(9600);
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



if (xAng<10 && xAng>-10){
  alto();
  delay(50);
}
 
if (xAng>10){
  digitalWrite(ena, HIGH);
  digitalWrite(ina, HIGH);
  digitalWrite(inb, LOW);
  delay(80);
  Serial.println("      ");
  Serial.println("Velocidad Angular = 6 rev/s        Velocidad tangencial = .16 m/s");
 }
 
if (xAng <-10){
  digitalWrite(ena, HIGH);
  digitalWrite(ina, LOW);
  digitalWrite(inb, LOW);
  delay(50);
  digitalWrite(ena, HIGH);
  digitalWrite(ina, 50);
  analogWrite(inb, LOW);
  delay(50);
  Serial.println("      ");
  Serial.println("Velocidad Angular = 2 rev/s        Velocidad tangencial = .053m/s");
 }
 
if (yAng>20){
   abajo();
   delay(50);
 }
 

}

void vel1(){
  digitalWrite(ena, HIGH);
  digitalWrite(ina, LOW);
  digitalWrite(inb, HIGH);
}

void vel2(){
  digitalWrite(ena, HIGH);
  digitalWrite(ina, LOW);
  analogWrite(inb, 170);
}

void abajo(){
  digitalWrite(ena, HIGH);
  digitalWrite(ina, LOW);
  digitalWrite(inb, HIGH);

}

void alto(){
  digitalWrite(ena, LOW);
  digitalWrite(ina, LOW);
  digitalWrite(inb, LOW);
}
