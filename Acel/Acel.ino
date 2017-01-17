#include <Arduino.h>

#include <Wire.h>
#include "MMA7660.h"
MMA7660 accelemeter;
float xpas = 0.0;
float ypas = 0.0;
float zpas = 0.0;
float fx, fy, fz;
float cont = 0.5;
int p=0;
int r=0;
void setup()
{
  accelemeter.init();  
  Serial.begin(9600);
}
void loop()
{
  int8_t x;
  int8_t y;
  int8_t z;
  float ax,ay,az;
  accelemeter.getXYZ(&x,&y,&z);
  
  accelemeter.getAcceleration(&ax,&ay,&az);
//  Serial.print("x");
//  Serial.print(ax);
//  Serial.print("g   ");
//  Serial.print("y");
//  Serial.print(ay);
//  Serial.print("g   ");
//  Serial.print("z");
//  Serial.print(az);
//  Serial.println("g   ");
  fx= ax-xpas;
  fy= ay-ypas;
  fz= az-zpas;
  delay(1);
  xpas=ax;
  ypas=ay;
  zpas=az;
  if (fx>cont || fy>cont || fz>cont){
    p=p+1;
  }
  r=r+1;
  if (r==60){
    r=0;
    p=0;
  }
  if (p==17){
    Serial.println("Necesitas un doctor");
    delay(10000);
  }
  else{
   Serial.println("Todo está bien");
  }
//  Serial.print("xp");
//  Serial.print(fx);
//  Serial.print("g   ");
//  Serial.print("yp");
//  Serial.print(fy);
//  Serial.print("g   ");
//  Serial.print("zp");
//  Serial.print(fz);
//  Serial.println("g   ");
  delay(20);

}


