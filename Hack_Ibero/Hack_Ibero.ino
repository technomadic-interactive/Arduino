#include <Arduino.h>



#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include <MAX17043.h>
#include <Wire.h>
#include <Math.h>

// PIN Number
#define PINNUMBER ""
#define GPRS_APN       "m2mtrial.telefonica.com" // replace your GPRS APN
#define GPRS_LOGIN     ""    // replace with your GPRS login
#define GPRS_PASSWORD  "" // replace with your GPRS password
// initialize the library instance
MAX17043 batteryMonitor;
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
  
}

void loop() {
  // put your main code here, to run repeatedly:
  sendMsg();
delay(30000);
}
void sendMsg(){
  String atcomm;
  StaticJsonBuffer<125> jsonBuffer;
  char resCont[125];
  JsonObject& root = jsonBuffer.createObject();
  root["imei"] = "861075021409743";
  root["lat"] = "19.3711508";
  root["lon"] = "-99.2639328";
  root["caida"] = "true";
  root["bpm"] = "95";
  root["temp"] = "38";
  root["temp_decimal"] = "0.5";
  root.printTo(resCont, sizeof(resCont));
  String res(resCont);
  Serial.println(res);
  //sendATCommandWithResponse("AT+QHTTPURL=19,80", "http:187.237.14.240:2083");
  sendATCommandWithResponse("AT+QHTTPURL=48,100", "http:hacking_health.herokuapp.com/api/datos/raul");
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
  int count = 10;                       // Number of 100ms intervals before 
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
  sendATCommand("AT+QICSGP=1,1,\"m2mtrial.telefonica.com\",\"\",\"\",1", 100);
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


