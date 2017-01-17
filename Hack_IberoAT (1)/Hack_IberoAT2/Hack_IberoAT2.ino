#include <Arduino.h>



#include <ArduinoJson.h>
#include <SoftwareSerial.h>

#include <Wire.h>


// PIN Number
#define PINNUMBER ""
#define GPRS_APN       "internet.itelcel.com" // replace your GPRS APN
#define GPRS_LOGIN     "webgprs"    // replace with your GPRS login
#define GPRS_PASSWORD  "webgprs2002" // replace with your GPRS password
// initialize the library instance

SoftwareSerial Uc20(3,2); //rX, tX
// URL, path & port (for example: arduino.cc)


// initialize the library instance


int gnAct = 0;
String IMEI = "";
void setup() {
  // put your setup code here, to run once:

Serial.begin(9600);
Serial.println("Inicializando conexion a red celular");
Serial.begin(9600);
  SoftwareSerial Uc20(3,2); //rX, tX
  
  delay(10000);
  Serial.println("QUC20 inicializado a 115200");
  cfgInit();
  sessionsInit();
  // userDataInit();
  pinMode(10, INPUT); // Setup for leads off detection LO +
  pinMode(11, INPUT); // Setup for leads off detection LO -
  pinMode(6, INPUT);
}

void loop() {
  int x=0;
  while (x<30000){
    Serial.println(analogRead(A3));
    delay(1);
    x=x+1;
    if (analogRead(A3)>950){
      Serial.println("caida");
      while(digitalRead(6)==0){
        if((digitalRead(10) == 1)||(digitalRead(11) == 1)){
          Serial.println('!');
        }
        else{
          // send the value of analog input 0:
            Serial.println(analogRead(A0));
      //  }
      //  //Wait for a bit to keep serial data from saturating
        delay(10);
        }
      }
    }
  }
  sendMsg();
}
void sendMsg(){
  String atcomm;
  StaticJsonBuffer<100> jsonBuffer;
  char resCont[100];
  JsonObject& root = jsonBuffer.createObject();
  root["imei"] = "861075021409743";
  root["lat"] = "19.3711508";
  root["lon"] = "-99.2639328";
  root.printTo(resCont, sizeof(resCont));
  String res(resCont);
  Serial.println(res);
  //sendATCommandWithResponse("AT+QHTTPURL=49,80", "http://199.195.116.177/dondestas/cgi-bin/uc20.php");
  sendATCommandWithResponse("AT+QHTTPURL=49,2083", "http://192.100.192.110/");
    //sendATCommand("AT+QIGETERROR", 100);
    atcomm = "AT+QHTTPPOST=";
    atcomm += res.length();
    atcomm += ",80,80";
    //Serial.println(atcomm);
    sendATCommandWithResponse(atcomm, res);
    sendATCommand("AT+QIGETERROR", 100);
    sendATCommand("AT+QHTTPREAD=100", 100);
    sendATCommand("AT+QIGETERROR", 100);
  
}
void cfgInit() {
 /* batteryMonitor.reset();
  batteryMonitor.quickStart();
  batteryMonitor.setAlertThreshold(25);
  sendATCommand("AT+QGPSCFG=\"nmeasrc\", 1", 100);
  if (gnAct == 1) {
    sendATCommand("AT+QGPSCFG=\"glonassenable\", 1", 100);
  } else {
    sendATCommand("AT+QGPSCFG=\"glonassenable\", 0", 100);
  }*/
  /*if(getDataResponse(getBodyReadResponse(sendATCommand("AT+IPR?", 100))) != "9600"){
     sendATCommand("AT+IPR=9600", 100);
     sendATCommand("AT&W", 100);
  }*/
}
String sendATCommand(String command, int ms) {
  /*char incoming_char;
  String res = " ";
  int timeout = 0, lastLen = 0;
  Serial3.println(command);
  delay(ms);
  if (Serial3.available() > 0) {
    while (res.endsWith("OK") == 0) {
      incoming_char = Serial3.read();

      if (incoming_char != (char) - 1) {
        res += incoming_char;
      }

      if (res.endsWith("ERROR")) {
        while (Serial3.available() > 0) {
          incoming_char = Serial3.read();
          res += incoming_char;
        }
        res += "\nOK";
      }

      if (res.length() == lastLen) {
        if (timeout <= 3) {
          timeout++;
        } else {
          res += "\nOK";
        }
      }
      lastLen = res.length();
    }
  }
  //Serial.println(res);
  return res;*/
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


void sessionsInit() {
  sendATCommand("AT+QGPS=1", 100);
  sendATCommand("AT+QIGETERROR", 100);
  sendATCommand("AT+QHTTPCFG=\"contextid\",1", 100 );
  sendATCommand("AT+QIGETERROR", 100);
  //sendATCommand("AT+QHTTPCFG=\"responseheader\",1", 100);
  sendATCommand("AT+QIGETERROR", 100);
  //sendATCommand("AT+QICSGP=1,1,\"m2m.com.attz\",\"\",\"\",1", 100);
  sendATCommand("AT+QICSGP=1,1,\"internet.itelcel.com\",\"webgprs\",\"webgprs2002\",1", 100);
  sendATCommand("AT+QIGETERROR", 100);
  sendATCommand("AT+QIDEACT=1", 100);
  sendATCommand("AT+QIGETERROR", 100);
  sendATCommand("AT+QIACT=1", 300);
  sendATCommand("AT+QIGETERROR", 100);
  
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


