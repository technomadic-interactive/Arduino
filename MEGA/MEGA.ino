#include <Arduino.h>

int c0=12;
int c1=11;
int d0=10;
int d1=9;
int BM=8;

int Y1=3;
int Y2=2;
int Y3=5;
int Y4=4;

int a0;
int a1;
int b0;
int b1;
int bm;
int y1;
int y2;
int y3;
int y4;
int m;

void setup() {
  pinMode(c0, INPUT);
  pinMode(c1, INPUT);
  pinMode(d0, INPUT);
  pinMode(d1, INPUT);
  pinMode(BM, INPUT);
  pinMode(Y1, OUTPUT);
  pinMode(Y2, OUTPUT);
  pinMode(Y3, OUTPUT);
  pinMode(Y4, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  a0=digitalRead(c0);
  a1=digitalRead(c1);
  b0=digitalRead(d0);
  b1=digitalRead(d1);
  bm=digitalRead(BM);

  Serial.print("c0: ");
  Serial.println(a0);
  Serial.print("c1: ");
  Serial.println(a1);
  Serial.print("d0: ");
  Serial.println(b0);
  Serial.print("d1: ");
  Serial.println(b1);
  Serial.print("BM: ");
  Serial.println(bm);
  Serial.print("M:  ");
  Serial.println(m);
  Serial.println("\n\n\n");

  if(((a0&&b0&bm&(!m))||(a0&&b1))==1){
    digitalWrite(Y1, HIGH);
    Serial.println("Y1");
    delay(10);
  }
  
  if (((a1&&b0&&(!m))||(a1&&b1))==1){
    digitalWrite(Y2, HIGH);
    Serial.println("Y2");
    delay(10);
  }

  if (((a1&&b0)||(b0&&a1&&m))==1){
    digitalWrite(Y3, HIGH);
    Serial.println("Y3");
    delay(10);
  }
  
  if (((b1&&a0)||(b0&&a1&&m))==1){
    digitalWrite(Y2, HIGH);
    Serial.println("Y4");
    delay(10);
  }
 apagar();
  if ((a0&&b1)==1){
    m=1;
  }
  if ((a1&&b1)==1){
    m=0;
  }
  delay(1000);
}

void apagar(){
    digitalWrite(Y1, LOW);
    digitalWrite(Y2, LOW);
    digitalWrite(Y3, LOW);
    digitalWrite(Y4, LOW);
    delay(1);
}

