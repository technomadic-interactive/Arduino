#include <SoftwareSerial.h>
//Con ArduinoMega//SoftwareSerial Uc20(11,10); //rX, tX
SoftwareSerial Uc20(2, 3); //rX, tX
String resp = "t=12&a='alta'";
////////////////////, ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  
  Serial.begin(9600);
  Uc20.begin(115200);
  delay(1000);
  sendATCommand("AT+IPR=9600", 100);
  Uc20.begin(9600);
  sendATCommand("AT+QHTTPCFG=\"contextid\",1", 100);
  sendATCommand("AT+QHTTPCFG=\"requestheader\",1", 100);
  sendATCommand("AT+QICSGP=1,1,\"internet.itelcel.com\",\"webgprs \",\"webgprs2002\",1", 100);
  sendATCommand("AT+QIACT=1", 100);
  
}

void loop(){
  String atcomm;
  //sendATCommandWithResponse("AT+QHTTPURL=28,80", "http://35.163.214.225/add.php");
  //delay(3000);
  //sendATCommand("AT+QIGETERROR", 100);
  atcomm = "AT+QHTTPPOST=";
  atcomm += resp.length();
  atcomm += ",80,80";
  Serial.println(atcomm);
  Serial.println(resp);
  //sendATCommandWithResponse(atcomm, resp);
  //delay(3000);
  //sendATCommand("AT+QIGETERROR", 100);
  //delay(2000);
  //Serial.println(sendATCommand("AT+QHTTPREAD=80", 100));
  delay(30000);
}

String sendATCommand(String command, int ms) {
  char c;
  String res;
  Uc20.println(command);     // Send request
  int count = 5;                       // Number of 100ms intervals before 
                                       // assuming there is no more data
  while(count-- != 0) {                // Loop until count = 0

    delay(100);                        // Delay 100ms

    while (Uc20.available() > 0){  // If there is data, read it and reset
       c = (char)Uc20.read();      // the counter, otherwise go try again
       res += c;
       count = 5;        
    }
  }
  Serial.println(res);
  return res;
}

void sendATCommandWithResponse(String command, String response) {
  char incoming_char;
  Uc20.println(command);
  delay(5000);
  Serial.println(Uc20.available());
  while (Uc20.available() > 0) {
    incoming_char = Uc20.read();
    Serial.print(incoming_char);
  }
  Uc20.println(response);
  delay(3000);
  Serial.println(Uc20.available());
  while (Uc20.available() > 0) {
    incoming_char = Uc20.read();
    Serial.print(incoming_char);
  }
  delay(5000);
}