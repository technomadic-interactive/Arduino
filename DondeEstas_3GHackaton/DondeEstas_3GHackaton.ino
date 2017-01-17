//Bloque de librerias
#include <MAX17043.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#include "Geocoordinate.h"
#include "VirtualFence.h"

//Declaracion general de variables Generales
long previousMillis = 0;
unsigned long currentMillis = 0;
int totalSend = 0;
String geoLecture;
boolean uConfig = false;
int gnAct = 0;

//Variables Configurables por el Usuario
int maxDistance = 10; //Distancia maxima en metros
long maxTime = 10; // Tiempo entre lecturas en segundos

//Generacion de Objetos
Geocoordinate prevCoordinate; //Coordenadas anteriores
Geocoordinate lastCoordinate; //Ultimas Coordenadas
VirtualFence securityFence; //Cerca de Seguridad
MAX17043 batteryMonitor; //Lector de la bateria
SoftwareSerial Uc20(7, 8); //rX, tX

//Definicion de funciones
void getPetLocation();
String converterLocation(String coordinate);
boolean isFarEnough(float dist);
void checkVirtualFence();
float checkBatteryLv();
void sendMsg();

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Uc20.begin(9600);

  delay(10000);
  Serial.println("QUC20 inicializado a 115200");
  cfgInit();
  sessionsInit();
  userDataInit();
}

void loop() {
  currentMillis = millis();
  if (currentMillis - previousMillis > maxTime) {
    if (totalSend <= 20) {
      previousMillis = currentMillis;

      getPetLocation();
      isFarEnough(prevCoordinate.getDistanceBetween(lastCoordinate));
      checkVirtualFence();
      //checkBatteryLv();
      sendMsg();
      Serial.println( "lat=" + lastCoordinate.getLatitude() + "&lon=" + lastCoordinate.getLongitude());
    } else {
      powerOff();
    }
  }
}

void cfgInit() {
  //Iniclializacion de la bateria
  batteryMonitor.reset();
  batteryMonitor.quickStart();
  batteryMonitor.setAlertThreshold(25);
  //Iniclializacion del gps
  sendATCommand("AT+QGPSCFG=\"nmeasrc\", 1", 100);
  if (gnAct == 1) {
    sendATCommand("AT+QGPSCFG=\"glonassenable\", 1", 100);
  } else {
    sendATCommand("AT+QGPSCFG=\"glonassenable\", 0", 100);
  }
  /*if (getDataResponse(getBodyReadResponse(sendATCommand("AT+IPR?", 100))) != 9600) {
    sendATCommand("AT+IPR=9600", 100);
    sendATCommand("AT&W");
  }*/
}

void sessionsInit() {
  //Activar GPS
  sendATCommand("AT+QGPS=1", 100);
  //Activar conexion 3G
  sendATCommand("AT+QHTTPCFG=\"contextid\",1", 100);
  sendATCommand("AT+QHTTPCFG=\"responseheader\",1", 100);
  sendATCommand("AT+QICSGP=1,1,\"m2m.com.attz\",\"\",\"\",1", 100);
  sendATCommand("AT+QIACT=1", 100);
}

void userDataInit() {
  maxDistance = 10; // En metros
  maxTime = 15000; //15 Seg || representado en ms
  securityFence.setNWC(Geocoordinate("19.362204", "-99.156798")); //Esquina superior izquierda de la cerca
  securityFence.setSEC(Geocoordinate("19.362139", "-99.156753")); //Esquina inferior derecha de la cerca
  //securityFence.switchFence(); //Por defecto esta desabilitada, si se desea habilitar, solo quitar las primeras barras de esta linea
}

void sessionsClose() {
  sendATCommand("AT+QGPSEND", 100); //Termina sesion de GPS
  sendATCommand("AT+QIDEACT=1", 100); //Termina sesion de 3G
  delay(1000);
}

void restartUC() {
  sessionsClose();
  sendATCommand("AT+QGPSEND", 100);
  sendATCommand("AT+QIDEACT=1", 100);
  delay(10000);
}

void powerOff() {
  sessionsClose();
  sendATCommand("AT+QPOWD", 300);
}

//Funcion qu eobtiene una localizacion del GPS y la asigna a sus respectivos objetos
void getPetLocation() {
  String loc;
  loc = sendATCommand("AT+QGPSLOC?", 300);
  prevCoordinate.updateLocation(lastCoordinate.getLatitude(), lastCoordinate.getLongitude());
  lastCoordinate.updateLocation(converterLocation(parseCSV(getDataResponse(getBodyResponse(loc)), 1)), converterLocation(parseCSV(getDataResponse(getBodyResponse(loc)), 2)));
  Serial.println("Localizacion Obtenida");
}

//Funcion para mapear una coordenada geografica a UTM
String converterLocation(String coordinate) {
  int aux = 0, grades = 0;
  String utmLocation;
  char carDir = coordinate.charAt(coordinate.length() - 1);

  coordinate.remove(coordinate.length() - 1, 1);
  grades = (coordinate.substring(0, 2)).toInt();
  //Serial.println(coordinate);
  coordinate.remove(0, 2);
  //Serial.println(coordinate);
  aux = coordinate.indexOf('.');
  coordinate.remove(aux, 1);
  //Serial.println(coordinate);
  utmLocation.concat(grades);
  utmLocation.concat(".");
  utmLocation.concat((int)(coordinate.toFloat() / 60));
  //Serial.println(utmLocation);
  if (carDir == 'W' || carDir == 'S') {
    utmLocation = '-' + utmLocation;
  }
  return utmLocation;
}

//Funcion que evalua la distancia en metros entre dos distancias
boolean isFarEnough(float dist) {
  boolean farEnough = false;
  if (dist >= maxDistance) {
    farEnough = true;
    Serial.println("Far Enough");
  } else {
    farEnough = false;
    Serial.println("Not Enough");
  }
  return farEnough;
}

//Funcion que revisa si la cerca esta activada y realiza el calculo con la ubicacion actual
void checkVirtualFence() {
  if (securityFence.isActive()) {
    Serial.println("Fence status: active");
    if (securityFence.isStillInside(prevCoordinate) == 0) {
      Serial.println("La mascota salio de la cerca!");
      //Mecanismo para enviar mensajes con la notificacion
    } else {
      Serial.println("Chill, it's inside");
    }
  } else {
    Serial.println("Fence status: inactive");
  }
}

//funcion de supervision para la bateria, la parte comentada corresponde a el circuito sin 
//el chip MAX
float checkBatteryLv() {
  /*int currentCharge = 0, battStat = 0;
    float currentVolt = 0;
    //currentVolt = analogRead(5);
    currentVolt = 250;
    currentCharge = map(currentVolt, 0, 1023, 0, 100);
    //Serial.println(currentCharge);
    if (currentCharge <= 25) {
    Serial.println("La bateria esta a niveles menores del 25%");
    battStat = 1;
    }*/
  float stateOfCharge = batteryMonitor.getSoC();
  Serial.print("State of charge:\t");
  Serial.print(stateOfCharge);
  Serial.println("%");

  return stateOfCharge;
}

//Funcion de envio de comandos AT
String sendATCommand(String command, int ms) {
  char c;
  String res;
  Uc20.println(command);     
  int count = 5;                      
  // assuming there is no more data
  while (count-- != 0) {               

    delay(100);                       

    while (Uc20.available() > 0) { 
      c = (char)Uc20.read();     
      res += c;
      count = 5;
    }
  }
  Serial.println(res);
  return res;
}


//Limpia una respuesta de un comando para dejar solo la respuesta
String getBodyResponse(String msg) {
  int startW = 0, endsW = 0;
  String bodyRes;

  startW = msg.indexOf('\n');
  endsW = msg.indexOf('\n', startW + 1);
  return msg.substring(startW + 1, endsW);
}

//Limpia una respuesta de un comando para dejar solo la respuesta
//Esta sobrecarga permite lidiar con las diferencias entre comandos
//de GPS y de Datos 3G
String getBodyResponse(String msg, int mode) {
  int startW = 0, endsW = 0;
  String bodyRes;

  startW = msg.indexOf('\n');
  //Serial.println(startW);
  if (mode == 1) {
    endsW = msg.indexOf('\n', startW + 2);
  } else {
    endsW = msg.indexOf('\n', startW + 1);
  }
  //Serial.println(endsW);
  return msg.substring(startW + 1, endsW);
}

//funcion para quitar la cabecera de una respuesta a un comando
String getDataResponse(String data) {
  int ndx = 0;
  ndx = data.indexOf(':');
  data.trim();
  return data.substring(ndx + 1);
}

//Limpia una respuesta de un comando para dejar solo la respuesta
//Esta sobrecarga permite lidiar con los formatos de respuestas 
//a comandos que necesitaban argumentos
String getBodyReadResponse(String msg) {
  int startW = 0, endsW = 0, fLn = 0;
  String bodyRes;

  fLn = msg.indexOf('\n');
  startW = msg.indexOf('\n', fLn + 1);
  endsW = msg.indexOf('\n', startW + 1);
  return msg.substring(startW + 1, endsW);
}

//Funcion para obtener un campo en particular de una cadena csv
String parseCSV(String csv, int field) {
  String sField;
  int fCom, sCom;
  for (int i = 0; i < field; i++) {
    fCom = csv.indexOf(',');
    sCom = csv.indexOf(',', fCom + 1);
    sField = csv.substring(fCom + 1, sCom);
    csv.remove(fCom, sField.length());
  }
  return sField;
}

//Funcion de envio de datos a traves de 3G
void sendMsg() {
  String res, atcomm;
  res = "lat=" + lastCoordinate.getLatitude() + "&lon=" + lastCoordinate.getLongitude();
  sendATCommandWithResponse("AT+QHTTPURL=52,80", "http://roatech.dyndns.info:91/DondeEstas/seePost.php");
  delay(3000);
  sendATCommand("AT+QIGETERROR", 100);
  atcomm = "AT+QHTTPPOST=";
  atcomm += res.length();
  atcomm += ",80,80";
  Serial.println(atcomm);
  sendATCommandWithResponse(atcomm, res);
  delay(3000);
  sendATCommand("AT+QIGETERROR", 100);
  delay(2000);
  Serial.println(sendATCommand("AT+QHTTPREAD=80", 100));
  delay(3000);
}

//Funcion para enviar comandos que necesiten argumentos una vez que son enviados
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

