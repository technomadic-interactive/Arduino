#include <Arduino.h>

int S0 = 8;//pinB
int S1 = 9;//pinA
int S2 = 12;//pinE
int S3 = 11;//pinF
int taosOutPin = 10;//pinC
int LED = 13;//pinD

#include <Keypad.h>
#include <Wire.h>
#include "rgb_lcd.h"

//Display
rgb_lcd lcd;
const int colorR = 255;
const int colorG = 255;
const int colorB = 255;
//Teclado
const byte filas =4;
const byte columnas=4;
int motor=7;

char teclado[filas][columnas]={
  {'1','4','7','*'},
  {'2','5','8','0'},
  {'3','6','9','#'},
  {'A','B','C','D'}
  };
byte filasPines[filas] = {28,26,24,22};
byte columnasPines[columnas ] = {36 ,34 ,32 ,30};
Keypad datos = Keypad(makeKeymap (teclado),filasPines,columnasPines,filas,columnas);

//botones
int blue=50;
int red=51;
int green=52;
int ResetInicio=0;
int azul;
int rojo;
int verde;
int x=0;
int a;
int b;
int c;
int d;
char car1;
char car2;
int cuentas;
int cazul;
int crojo;
int cverde;


void setup() {
pinMode(motor, OUTPUT);
pinMode(blue,INPUT);
pinMode(green, INPUT);
pinMode(red,INPUT);
lcd.begin(16, 2);
lcd.setRGB(colorR, colorG, colorB);
lcd.setCursor(0,0);
lcd.print("Bienvenido");
delay(3000);
lcd.clear();
TCS3200setup();
Serial.begin(115200);
Serial.print("\n\n\nready\n\n\n");
delay(100);
}

void loop() {
  digitalWrite(motor, HIGH);
  lcd.setCursor(0,0);
  lcd.print("Selec. Boton");

  azul=digitalRead(blue);
  rojo=digitalRead(red);
  verde=digitalRead(green);

  if (azul==HIGH){
    lcd.setRGB(0,0,255);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Cantidad. Azules");
    Serial.println(x);
    cuenta();
    x=0;
    Serial.print("Cantidad de Azules:   ");
    Serial.println(cuentas);
    lcd.clear();  
    cazul=cuentas;
  }
  if (rojo==HIGH){
    lcd.setRGB(255,0,0);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Cantidad. Rojos");
    Serial.println(x);
    cuenta();
    x=0;
    Serial.print("Cantidad de Rojos:   ");
    Serial.println(cuentas);
    lcd.clear();  
    crojo=cuentas;
  }
  if (verde==HIGH){
    lcd.setRGB(0,255,0);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Cantidad. Verdes");
    Serial.println(x);
    cuenta();
    x=0;
    Serial.print("Cantidad de Verdes:   ");
    Serial.println(cuentas);
    lcd.clear();  
    cverde=cuentas;
  }
  scolor();
}

void cuenta(){
  cuentas=0;
   while (x<2){
      char car=datos.getKey();
      if (car){
        switch (car){
          case 'A':
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("No Se Aceptan");
            lcd.setCursor(0,1);
            lcd.print("Letras!");
            x=3;
            delay(1000);
            break;
          case 'B':
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("No Se Aceptan");
            lcd.setCursor(0,1);
            lcd.print("Letras!");
            delay(1000);
            x=3;
            break;
          case 'C':
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("No Se Aceptan");
            lcd.setCursor(0,1);
            lcd.print("Letras!");
            delay(1000);
            x=3;
            break;
          case 'D':
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("No Se Aceptan");
            lcd.setCursor(0,1);
            lcd.print("Letras!");
            x=3;
            delay(1000);
            break;
          case '*':
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("No Se Aceptan");
            lcd.setCursor(0,1);
            lcd.print("Letras!");
            delay(1000);
            break;
          case '#':
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("No Se Aceptan");
            lcd.setCursor(0,1);
            lcd.print("Letras!");
            x=3;
            delay(1000);
            break; 
          default:
            lcd.setCursor(x,1);
            lcd.print(car);
            if (x==0){
              car1=car;
              a=car;
              b=car-48;
            }
            if (x==1){
              car2=car;
              c=car;
              d=car-48;
              cuentas=(b*10)+d;
            }
            x++;
            break;
        }
      }
      delay(50);
    }
    if (cuentas<=20){
      while(x==2){
        lcd.setCursor(0,0);
        lcd.print("Seguro?         ");
        lcd.setCursor(0,1);
        lcd.print(car1);
        lcd.setCursor(1,1);
        lcd.print(car2);
        char cara=datos.getKey();
        if (cara){
          if (cara=='A'){
            Serial.println("cont");
            lcd.clear();
            lcd.setCursor(0,0);
            //lcd.print("Continuara...");
            //delay(3000);
            x=0;
          }
          else if (cara=='D'){
            Serial.println("reinicio");
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Reiniciando");
            x=3;
            delay(1500);
          }
          else{
            Serial.println("fallo");
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Intenta de Nuevo!!");
            delay(1500);
          }
        }
      }
  }
  else {
    Serial.println("Muy Grande");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Menos de 20!");
    cuentas=0;
    delay(1500);
  }
}

int detectColor(int taosOutPin){

float white = colorRead(taosOutPin,0,1);
float reds = colorRead(taosOutPin,1,1);
float blues = colorRead(taosOutPin,2,1);
float greens = colorRead(taosOutPin,3,1);

//Serial.print("white ");
//Serial.println(white);

Serial.print("red ");
Serial.println(red);

Serial.print("blue ");
Serial.println(blue);

Serial.print("green ");
Serial.println(green);

}

float colorRead(int taosOutPin, int color, boolean LEDstate){ 
  taosMode(1);
  
  int sensorDelay = 1;
  if(color == 0){//white
    digitalWrite(S3, LOW); //S3
    digitalWrite(S2, HIGH); //S2
  }
  
  else if(color == 1){//red
    digitalWrite(S3, LOW); //S3
    digitalWrite(S2, LOW); //S2
  }
  
  else if(color == 2){//blue
    digitalWrite(S3, HIGH); //S3
    digitalWrite(S2, LOW); //S2 
  }
  
  else if(color == 3){//green
    digitalWrite(S3, HIGH); //S3
    digitalWrite(S2, HIGH); //S2 
  }
  
  
  float readPulse;
  
  
  if(LEDstate == 0){
      digitalWrite(LED, LOW);
  }
  
  if(LEDstate == 1){
      digitalWrite(LED, HIGH);
  }
  
  delay(sensorDelay);
  readPulse = pulseIn(taosOutPin, LOW, 80000);
  
  if(readPulse < .1){
    readPulse = 80000;
  }
  
  taosMode(0);  
  return readPulse;
}

void taosMode(int mode){
    
    if(mode == 0){
      digitalWrite(LED, LOW);
      digitalWrite(S0, LOW); //S0
      digitalWrite(S1, LOW); //S1
    }
    
    else if(mode == 1){
      digitalWrite(S0, HIGH); //S0
      digitalWrite(S1, HIGH); //S1
    }
    
    else if(mode == 2){
      digitalWrite(S0, HIGH); //S0
      digitalWrite(S1, LOW); //S1
    }
    
    else if(mode == 3){
      digitalWrite(S0, LOW); //S0
      digitalWrite(S1, HIGH); //S1 
    }
    
    return;

}

void TCS3200setup(){

    pinMode(LED,OUTPUT); //LED pinD
    pinMode(S2,OUTPUT); //S2 pinE
    pinMode(S3,OUTPUT); //s3 pinF
    pinMode(taosOutPin, INPUT); //taosOutPin pinC
    pinMode(S0,OUTPUT); //S0 pinB
    pinMode(S1,OUTPUT); //S1 pinA 
    
    return;

}
void scolor(){
  detectColor(taosOutPin);
  Serial.print("\n\n\n");
  delay(50);
}

