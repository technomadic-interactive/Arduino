#include <Arduino.h>

void setup() {
  // initialize the serial communication:
  Serial.begin(9600);
  pinMode(10, INPUT); // Setup for leads off detection LO +
  pinMode(11, INPUT); // Setup for leads off detection LO -

}

void loop() {
//  
  if((digitalRead(10) == 1)||(digitalRead(11) == 1)){
    Serial.println('!');
  }
  else{
    // send the value of analog input 0:
    analogRead(A5);
    int y=0;
    int x=0;
    if (analogRead(A5)==360){
      x=1;
    }
    if (analogRead(A5)==359){
      while(x==1){
        analogRead(A5);
        delay(10);
        y=y+1;
        if (analogRead(A5)==380){
          x=0;
        }
      }
      int z=y*(0.01);
      int bpm=(1/z)*(60);
      Serial.println(bpm);
      y=0;
    }
    delay(10);
  }
}
