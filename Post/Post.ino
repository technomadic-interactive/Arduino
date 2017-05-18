#include <ArduinoJson.h>
#include <Wire.h>
#include <SoftwareSerial.h>

SoftwareSerial Uc20(5,6); //rX, tX

//Variables Generales
long previousMillis = 0;
unsigned long currentMillis = 0;
String geoLecture;
boolean uConfig = false;
int gnAct = 0;
float temperatura, presion_dis, presion_sis, pulso;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Uc20.begin(115200);
  delay(1000);
  sendATCommand("AT+IPR=9600", 100);
  sendATCommand("AT+IPR?", 100);
  Uc20.begin(9600);
  delay(10000);
  Serial.println("QUC20 inicializado a 115200");
  cfgInit();
  sessionsInit();
}

void loop() {

    
  }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  void cfgInit() {
  sendATCommand("AT+QGPSCFG=\"nmeasrc\", 1", 100);
  if (gnAct == 1) {
    sendATCommand("AT+QGPSCFG=\"glonassenable\", 1", 100);
  } else {
    sendATCommand("AT+QGPSCFG=\"glonassenable\", 0", 100);
  }
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
  //sendATCommand("AT+QICSGP=1,1,\"m2mtrial.telefonica.com\",\"\",\"\",1", 100);
  //sendATCommand("AT+QICSGP=1,1,\"internet.movistar.mx\",\"movistar\",\"movistar\",1", 100);
  sendATCommand("AT+QIGETERROR", 100);
  sendATCommand("AT+QIDEACT=1", 100);
  sendATCommand("AT+QIGETERROR", 100);
  sendATCommand("AT+QIACT=1", 300);
  sendATCommand("AT+QIGETERROR", 100);
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

//********************************************************************************************************************************************************************************************
void sendMsg(){
  String atcomm;
  StaticJsonBuffer<100> jsonBuffer;
  char resCont[100];
  JsonObject& root = jsonBuffer.createObject();
  
  root["presion_sis"] = "23";
  root["pulso"] = "123";
  root["temperatura"] = "12";
  root["presion_dis"] = "1";

  root.printTo(resCont, sizeof(resCont));
  String res(resCont);
  Serial.println(res);
 
  sendATCommandWithResponse("AT+QHTTPURL=77,80", "http://technomadic.westcentralus.cloudapp.azure.com/E-health/PHP/add_data.php");

   
    atcomm = "AT+QHTTPPOST=";
    atcomm += res.length();
    atcomm += ",80,80";
  
    sendATCommandWithResponse(atcomm, res);
    sendATCommand("AT+QIGETERROR", 100);
    sendATCommand("AT+QHTTPREAD=100", 100);
    sendATCommand("AT+QIGETERROR", 100);
}
//********************************************************************************************************************************************************************************************
