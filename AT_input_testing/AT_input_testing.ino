#include <SoftwareSerial.h>
//Con ArduinoMega//SoftwareSerial Uc20(11,10); //rX, tX
SoftwareSerial Uc20(2, 3); //rX, tX
////////////////////, ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  
  Serial.begin(9600);
  Uc20.begin(115200);
  delay(1000);
  sendATCommand("AT+IPR=9600", 100);
  Uc20.begin(9600);
  
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  sendATCommand("AT+IPR?", 100);
  delay(2000);
  /*Referente al Voltaje*/
  Serial.print("Voltaje Actual: ");
  sendATCommand("AT+CBC", 100); //Lee el voltaje de alimentación
  delay(2000);
  
  /*Información del UC20*/
   Serial.print("Info del modulo: ");
  sendATCommand("ATI", 100);
  delay(2000);
  Serial.print("IMEI: ");
  sendATCommand(" AT+GSN", 100); //IMEI DEL UC20
  delay(2000);
  sendATCommand("AT+IPR?", 100);
  delay(2000);
  
  /*Verificar tarjeta SIM*/
  sendATCommand("AT+CPIN?", 100); //R=+CPIN: READY  --->SIM insertada o no. Locked o no.
  delay(2000);
  sendATCommand("AT+CIMI", 100); //R=310170231367775
  delay(2000);
  sendATCommand(" AT+QCCID", 100); //R=89011704252313677756
  delay(2000);
  sendATCommand(" AT+QSIMDET?", 100); //R=+QSIMDET:0,0-->Detección de la SIM
  delay(2000);
  sendATCommand("AT+QSIMSTAT?", 100); //R=+QSIMSTAT: 0,1 --->¿La sim esta insertada?
  delay(2000);
  
  /*Estado de la Red*/ //Los resultados alfanumericos estan en función de la antena y ubicacion
  Serial.print("Calidad de conexionl: ");
  sendATCommand("AT+CSQ", 100); //R=+CSQ: 2,99 --->current signal quality
  delay(2000);
  Serial.print("Registro en la red: ");
  sendATCommand("AT+CREG?", 100); //R=+CREG: 0,5 --->network registration status
  delay(2000);
  Serial.print("Operador de la red: ");
  sendATCommand("AT+COPS?", 100); //R=+COPS: 0,0,"movistar",2 --->Network Operator
  delay(2000);
  
} 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

