#include "ESP8266wifi.h"
#include "SoftwareSerial.h"

#define sw_serial_rx_pin 2 //  Connect this pin to TX on the esp8266
#define sw_serial_tx_pin 3 //  Connect this pin to RX on the esp8266
#define esp8266_reset_pin 5 // Connect this pin to CH_PD on the esp8266, not reset. (let reset be unconnected)
#define SERV '0'

String ssid = "Hackathon1";
String password = "hackces2017";

SoftwareSerial swSerial(sw_serial_rx_pin, sw_serial_tx_pin);

// the last parameter sets the local echo option for the ESP8266 module..
ESP8266wifi wifi(swSerial, swSerial, esp8266_reset_pin, Serial);//adding Serial enabled local echo and wifi debug

String inputString;
boolean stringComplete = false;
String data = "id=arduino";
String server = "199.195.116.177"; // Moonki server
String uri = "/alexaHack/getAddress.php"; 

void setup() {
  inputString.reserve(20);
  swSerial.begin(115200);
  Serial.begin(115200);

  while (!Serial);

  Serial.println("Starting wifi");    
  wifi.setTransportToTCP();// this is also default
  wifi.endSendWithNewline(true); // Will end all transmissions with a newline and carrage return ie println.. default is true

  bool result;
  result =  wifi.begin(); 
  Serial.println(result);
  Serial.println("waiting for wifi");  
  //while (!wifi.begin()) ;
    
 
  Serial.println("WiFi connected");
  // Print the IP address

  while (!wifi.connectToAP(ssid, password)){
    delay(500) ;
    Serial.print(".");
  }
  Serial.println("WiFi Sign In OK");
  
  //boolean serverConnected =  wifi.connectToServer("54.245.148.171", "80");
}

boolean find_OK(){
  String ok = "";
  char c;
  while((c=swSerial.read()) != -1)
    ok=ok+c;
  Serial.println("find_0k()= " + ok + "\r\n");
  return(ok.substring(0) == "OK");
}

void loop () {
   if (!wifi.isStarted())
    wifi.begin();
   if(!wifi.isConnectedToServer())
     wifi.connectToServer("199.195.116.177", "80");
    httppost();
}

void httppost () {

  String postRequest =
    "POST " + uri + " HTTP/1.1\r\n" +
    "Host: " + server + "\r\n" +
    "Accept: *" + "/" + "*\r\n" +
    "Content-Length: " + data.length() + "\r\n" +
    "Content-Type: application/x-www-form-urlencoded\r\n" +
    "\r\n" + data + "\r\n";

  Serial.print(postRequest);
  String sendCmd = "AT+CIPSEND=";//determine the number of caracters to be sent.
  swSerial.println(sendCmd + postRequest.length());
  delay(500);
  swSerial.println(postRequest);

  Serial.println("...............");
  Serial.println(sendCmd + postRequest.length() + data);
  Serial.println("...............");
  find_OK();
}

void dat(){
  Serial.println("POST /alexaHack/getAddress.php HTTP/1.1");
  Serial.println("Host: 199.195.116.177");
  Serial.println("Accept */*");
  Serial.println("Content-Lenght: 10");
  Serial.println("Content-Type: application/x-www-form-urlencoded");
  Serial.println("");
  Serial.println("id=arduino");
}
