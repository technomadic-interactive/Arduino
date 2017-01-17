#include <Arduino.h>

//include the GSM library
#include <GSM.h>
// PIN number if necessary
#define PINNUMBER ""
#include <ArduinoJson.h>

// APN information obrained from your network provider
#define GPRS_APN       "m2mtrial.telefonica.com" // replace with your GPRS APN
#define GPRS_LOGIN     ""    // replace with your GPRS login
#define GPRS_PASSWORD  "" // replace with your GPRS password

// initialize the library instances
GSMClient client;
GPRS gprs;
GSM gsmAccess;

GSM gsm;

char server[] = "199.195.116.177"; // the base URL
char path[] = "/dondestas/cgi-bin/uc20hack.php"; // the path
int port = 80; // the por t, 80 for HTTP

const int groundpin = 18;             // analog input pin 4 -- ground
const int powerpin = 19;              // analog input pin 5 -- voltage
const int xpin = A3;                  // x-axis of the accelerometer
const int ypin = A2;                  // y-axis
const int zpin = A1;

void setup()
{
  pinMode(groundpin, OUTPUT);
  pinMode(powerpin, OUTPUT);
  digitalWrite(groundpin, LOW);
  digitalWrite(powerpin, HIGH);
  // initialize serial communications
  Serial.begin(9600);
  Serial.println("Starting Arduino web client.");
  // connection state
  boolean notConnected = true;

  // Start GSM shield
  // pass the PIN of your SIM as a parameter of gsmAccess.begin()
  while(notConnected)
  {
    Serial.println("conectando");
    if((gsmAccess.begin(PINNUMBER)==GSM_READY) &
        (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD)==GPRS_READY))
      notConnected = false;
    else
    {
      Serial.println("Not connected");
      delay(1000);
    }
  }

  Serial.println("connecting...");

}

void loop(){
//  Serial.print(analogRead(xpin));
//  // print a tab between values:
//  Serial.print("\t");
//  Serial.print(analogRead(ypin));
//  // print a tab between values:
//  Serial.print("\t");
//  Serial.print(analogRead(zpin));
//  Serial.println();
//  if(analogRead(xpin)>950){
//    Serial.println("caida");
//    manda_aviso();
//  }
   
  manda_aviso();
  delay(10000);
  
  
}

void manda_aviso()
{
   StaticJsonBuffer<200> jsonBuffer;
  char resCont[200];
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
  Serial.println("?json=",res);
  //  gps.encode(gpsSerial.read());
  //  displayInfo();
  if (client.connect(server, port))
  {
    Serial.println("connected");
    // Make a HTTP request:
    client.print("GET ");
    client.print(path);
    client.println(" HTTP/1.1");
    client.println();
    client.print("Host ");
    client.println(server);
    client.println("Content-Type: text/html");
    client.print("Content-Length: ");
    client.println(res.length());
    client.println(res);
    client.println();
    client.stop();
    
  }
  else
  {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
  return;
}
