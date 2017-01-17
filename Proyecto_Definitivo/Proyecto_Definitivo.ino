#include <Arduino.h>
#include <Keypad.h> //incluye librería de teclado
#include <Wire.h> //incluye librería de comunicación 
#include "rgb_lcd.h" //inclye librería de pantalla LCD
#include <Servo.h> // incluye librería de servo

Servo myservo; //declara Servo
//Servo
int val;
//Display
rgb_lcd lcd; //declara pantalla LCD
const int colorR = 255; 
const int colorG = 255;
const int colorB = 255;
//Teclado
const byte filas =4;
const byte columnas=4;

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
int blue=50;// valor ASCII para Azules
int red=51; // valor ASCII para rojos
int green=52; // valor ASCII para verdes
int x=0;
int a;
int b;
int c;
int z=0;
int d;
char car1;
char car2;
int cuentas; // cuenta para cada ciclo
int cazul; // cuenta de azules
int crojo; // cuenta de rojos
int cverde; // cuenta de verdes

//Variables de Sensor de Color
int S0 = 8;//pinB
int S1 = 9;//pinA
int S2 = 12;//pinE
int S3 = 11;//pinF
int taosOutPin = 10;//pinC
int LED = 13;
float whites;
float reds;
float blues;
float greens;

//LDR
int pinLDR1 = 1;
int pinLDR2 = 0;
int pinLDR3 = 2;
int valorLDR1;
int valorLDR2;
int valorLDR3;

//motores
int motor1 = 4;
int motor2 = 5;
int motor3 = 6;
int motor_principal=7;

int automatico=15;// botón de selección automático
int manual=16;//botón de selección manual
int boton_paro=17;
const byte interruptPin=2;

void setup() {
  attachInterrupt(interruptPin, loop, LOW);
  pinMode(boton_paro, INPUT);
  pinMode(automatico, INPUT);
  pinMode(manual, INPUT);
  valorLDR1 = analogRead(pinLDR1);
  valorLDR2 = analogRead(pinLDR2);
  valorLDR3 = analogRead(pinLDR3);
  myservo.attach(14);
  //Inicio de Pantalla
  pinMode(blue,INPUT); //inicio de pin azul
  pinMode(red,INPUT); // inicio de pin rojo
  pinMode(green, INPUT); // inicio de pin verde
  lcd.begin(16, 2); //inicio de LCD
  lcd.setRGB(colorR, colorG, colorB); // pantalla blanca
  Serial.begin(115200); // inicia comunicación serial 
  TCS3200setup(); //inicio de Sensor de Color
  Serial.print("\n\n\nready\n\n\n");
  delay(100);
  lcd.setCursor(0,0); 
  lcd.print("Bienvenido");
  Serial.println("hola1");
  delay(3000);
  lcd.clear();
}

void loop() {
  Serial.println("error");
  if (digitalRead(automatico)==HIGH){
    int constante_conteo=10;
    Serial.println("automatico");
    while (constante_conteo>0){
      analogWrite(motor_principal, HIGH);
      while(reds>20){
        seleccion_color();
        Serial.println("hola");
      }
      servo_color();
      constante_conteo--;
    }
    analogWrite(motor_principal, LOW);
    seleccion();
    conteo();
  }
  if (digitalRead(manual)==HIGH){
    seleccion_color();
    servo_color();
  }
}


void seleccion(){
  while (z==0){ // cantidad de azules a seleccionar
    lcd.setRGB(0,0,255);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Cantidad. Azules");
    cuenta();
    x=0;
    //Serial.print("Cantidad de Azules:   ");
    //Serial.println(cuentas);
    lcd.clear();  
    cazul=cuentas; // cantidad de azules seleccionados
    //Serial.println(z);
  }
  z=0;
  while(z==0){ //cantidad de rojos a seleccionar
    lcd.setRGB(255,0,0);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Cantidad. Rojos");
    cuenta();
    x=0;
    //Serial.print("Cantidad de Rojos:   ");
    //Serial.println(cuentas);
    lcd.clear();  
    //Serial.println(z);
    crojo=cuentas; // cantidad de rojos seleccionados
  }
  z=0;
  while(z==0){ //cantidad de verdes a seleccionar
    lcd.setRGB(0,255,0);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Cantidad. Verdes");
    cuenta();
    x=0;
    //Serial.print("Cantidad de Verdes:   ");
    //Serial.println(cuentas);
    lcd.clear(); 
    cverde=cuentas; //cantidad de verdes seleccionados
  }
  z=0;
  Serial.println(cazul);
  Serial.println(crojo);
  Serial.println(cverde);
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
            z=1;
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

void seleccion_color(){
  detectColor(taosOutPin);
  Serial.print("\n\n\n");
  delay(50);
}

int detectColor(int taosOutPin){
  
  whites = colorRead(taosOutPin,0,1);
  reds = colorRead(taosOutPin,1,1);
  blues = colorRead(taosOutPin,2,1);
  greens = colorRead(taosOutPin,3,1);

  green=green-10;
  red=red+1;
  blue=blue-3;
  
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
    
    //color mode selection
    pinMode(S2,OUTPUT); //S2 pinE
    pinMode(S3,OUTPUT); //s3 pinF
    
    pinMode(taosOutPin, INPUT); //taosOutPin pinC
    
    pinMode(S0,OUTPUT); //S0 pinB
    pinMode(S1,OUTPUT); //S1 pinA 
    
    return;
}

void servo_color(){
  val = map(val, 0, 1023, 0, 180);
  red=red+3;
  green=green-1.5;
  blue=blue-1;
  if (red<blue && red<green){ 
    myservo.write(75);                  
  }
  if (blue<red && blue<green){ 
    myservo.write(128);                  
  }  
  if (green<10 && red>green){ 
    myservo.write(180);                  
  }
}

void conteo(){
   while(crojo!=0){
     analogWrite(motor1, 360);
     if (analogRead(pinLDR1)<valorLDR1){
      crojo=crojo-1;
     }
   }
   while(cazul!=0){
     analogWrite(motor2, 360);
     if (analogRead(pinLDR2)<valorLDR2){
      cazul=cazul-1;
     }
   }
   while(cverde!=0){
     analogWrite(motor3, 360);
     if (analogRead(pinLDR3)<valorLDR3){
      cverde=cverde-1;
     }
   }
}

