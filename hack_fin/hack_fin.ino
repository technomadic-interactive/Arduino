#include <Arduino.h>

int x;
void setup() {
  // initialize the serial communication:
  Serial.begin(9600);
  pinMode(10, INPUT); // Setup for leads off detection LO +
  pinMode(11, INPUT); // Setup for leads off detection LO -
  pinMode(7, INPUT);
}

void loop() {
  Serial.println(digitalRead(7));
  int y=0;
  x=0;
  if (digitalRead(7)==1 && y==0){
    
    for (x = 0; x<10000; x++){
      if (digitalRead(12)==1){
      y=1;
      x=10001;
    }
      if((digitalRead(10) == 1)||(digitalRead(11) == 1)){
       Serial.println('!');
      }
      else{
        Serial.println(analogRead(A0));
        delay(10);
      }
    }
  }
}
