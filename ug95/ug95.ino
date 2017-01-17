#include <SoftwareSerial.h>
char numero_cell[]="554181711";
SoftwareSerial UC20(2,3);
void setup()
{
  pinMode(5, OUTPUT);
  pinMode(7, INPUT);
  digitalWrite(5, HIGH);
  Serial.begin(9600);
  void init_UC20();
  delay(200);
}
void loop(){
 Serial.println(digitalRead(7));
 leer_mensaje();
 enviar_mensaje();
 delay(1000);
 Serial.println("Listo");
  
}
void init_UC20()
{
  UC20.println("AT+IPR=9600");
  UC20.println("AT+CMGF=1");
  UC20.println("AT+CMGR=?");
  UC20.println("AT+CNMI=2,2,0,0");
 }
void leer_mensaje()
{
  UC20.println("AT+CMGF=1");
  UC20.println("AT+CMGR=?");
  UC20.println("AT+CNMI=2,2,0,0");
  
  if (UC20.available() >0 ){
    
    char DAT= UC20.read();
    Serial.print(DAT);
  }
}
void enviar_mensaje()
{
  UC20.println("AT+CMGF=1");
  UC20.println("AT+CMGS=");
  UC20.println((char)34);
  UC20.println(numero_cell);
  UC20.println((char)34);
  delay(200);
  UC20.println("HOLA soy JuanA");
  UC20.println((char)26);
}