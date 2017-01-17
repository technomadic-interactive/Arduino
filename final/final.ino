#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>
#include <Keypad.h>
#include "FPS_GT511C3.h"
#include "SoftwareSerial.h"

char numero_cell[]="+14433455571";//meter numero de telefono Tropo1
//char numero_cell[]="+14382281357";//meter numero de telefono Tropo2
//char numero_cell[]="5554181711";//meter numero de telefono Roger
//char numero_cell[]="+17864540964";//meter numero de telefono Arranz
//char numero_cell[]="5543595053";//meter numero de telefono Anibal
//char numero_cell[]="+14089098523";//meter numero de telefono Plivo

String cargo = "";
String nip = "1234";
String contra = "";
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char contrasena[4] = {1,2,3,4};
char login[4];
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {13, 12, 11, 10}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {9, 8, 7, 6}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

LiquidCrystal_PCF8574 lcd(0x27);  // set the LCD address to 0x27 for a 16 chars and 2 line display

FPS_GT511C3 fps(50,51);

int show;
int money;
int standby_huella ;
int fail;
int fail2;
int espera;
int ID= 0;
int j = 0;
char datosSerial[100];
int counter= 1;
String valor="";
int id;

void setup()
{
  int error;
  Serial.begin(9600);
  pinMode(5, OUTPUT);
  pinMode(7, INPUT);
  digitalWrite(5, HIGH);
  Serial.println(digitalRead(5));
  delay(500);
  digitalWrite(5, LOW);
  Serial3.begin(9600);
  configuracion_inicial();
  delay(100);
  fps.Open();
  fps.SetLED(false);
  Serial.println("LCD...");
  while (! Serial);
  Serial.println("Dose: check for LCD");
  Wire.begin();
  Wire.beginTransmission(0x27);
  error = Wire.endTransmission();
  lcd.begin(16, 2); // initialize the lcd
  show = 0;
  inicio();
} 

void loop(){
  char key = keypad.getKey();
  if (key){
    switch (key) {
        case 'A':
          lcd.setCursor(15,1);
          lcd.print(key);
          Serial.println(key);
          delay(1000);
          standby_huella=0;
          fail2=0;
          while (standby_huella==0){
            fps.SetLED(true);
            lee_dedo();
          }
          fail=0;
          while(fail<3){
            solcitar_NIP();
          }
          send_sms();
          espera=0;
          while (espera==0){
            receive_sms();
          }
          delay(2000);
          valor="";
          break;
        case 'B':
          lcd.setCursor(15,1);
          lcd.print(key);
          Serial.println(key);
          delay(1000);
          venta();
          standby_huella=0;
          fail2=0;
          while (standby_huella==0){
            fps.SetLED(true);
            lee_dedo();
          }
          fail=0;
          while(fail<3){
            solcitar_NIP();
          }
          send_sms2();
          espera=0;
          while (espera==0){
            receive_sms2();
          }
          delay(2000);
          valor="";
          Serial3.flush();
          break;
        default:
          break;
    }
    
    cargo="";
    j=0;
    counter=1;
    inicio();
    
  }
}

void inicio(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.setBacklight(255);
  lcd.print("Saldo(A)");
  lcd.setCursor(0,1);
  lcd.print("Compra(B)");
}

void borra(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.setBacklight(255);
}

void lee_dedo(){
  borra();
  if (fps.IsPressFinger()){
    fps.CaptureFinger(false);
    id = fps.Identify1_N();
    if (id <200){
      Serial.print("Verified ID:");
      Serial.println(id);
      lcd.print("Identificado!");
      standby_huella=1;
    }
    else{
      Serial.println("Huella no encontrada");
      lcd.print("Intente de nuevo");
      fail2+=1;
      if (fail2==3){
        standby_huella=1;
      }
    }
  }
  else{
    Serial.println("Colocar dedo");
    lcd.print("Coloque dedo");
    lcd.setCursor(0,1);
    lcd.print("sobre sensor");

  }
  delay(1000);
}


void solcitar_NIP(){

int contador=0;
  borra();
  lcd.print("NIP:");
  lcd.setCursor(0,1);
    while (contador<4 ){
      char key = keypad.getKey(); 
      if (key){
        Serial.println(key);
        lcd.print("*");
        contra+=key;
        Serial.println(contra);
        contador+=1;
      }
  }
  if (contra==nip){
    borra();
    lcd.print("conectando...");
    delay(1500);
    borra();
    lcd.print("Valido");
    Serial.println("bien");
    contra="";
    fail=4;
  }
  else{
    borra();
    lcd.print("conectando...");
    delay(1500);
    borra();
    lcd.print("Error");
    delay(1000);
    Serial.println("mal");
    contra="";
    fail+=1;
  }
}

void venta(){
  borra();
  lcd.print("Monto:");
  lcd.setCursor(0,1);
  int count=0;
  while (count==0){
    char monto = keypad.getKey();
    if (monto){
      switch (monto) {
          case 'A':
          case 'B':
          case 'C':
          case 'D':
          case '#':
          break;
          case '*':
            borra();
            lcd.print("Enviando...");
            delay(1500);
            count=1;
            cargo.toInt();
            Serial.println(cargo.toInt());
            
            break;
          default:
            lcd.print(monto);
            cargo+=monto;
      }
    }
  }
}

void configuracion_inicial()//configura los codigos de lectura de mensajes
{
  Serial3.println("AT+IPR=9600");
  delay(200);
  Serial3.println("AT+CMGF=1");
  delay(200);
  Serial3.println("AT+CMGR=?");//ACTIVAMOS CODIGO PARA RECIBIR MENSAJES
  delay(200);
  Serial3.println("AT+CNMI=2,2,0,0");//ACTIVAMOS PARA VER MENSAJES
  delay(200);
  Serial3.println("AT+CMGD=1,4");//BORRA TODOS LOS SMS EN <mem1>
  delay(1000);
  
  Serial.println("configuracion terminada");
  Serial.println("Hola Tecnomadas");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void receive_sms() {

  if (Serial3.available()){
    char character = (char)Serial3.read();  //Convierte lo que arroja el modulo cómo caracter para sr leido en ASCII
    datosSerial[j] = character;          //Almacena cada caracter ASCII en un elemento del array "datosSerial"
    j++;
      if(character == '\n'){  //Si identifica un salto de linea en una fila, borra todos los caracteres almacenados hasta el momento y vuelve a guardar desde la fila siguiente
        j=0;
        counter++;
      }
      if(counter>=13  && j>= 1){ //Recorre todas las filas hasta llegar a la 10, y recorre los caracteres en esta hasta llegar al 28.
        Serial.print(character); //Imprime caracter x caracter desde el elemento 28 (consulta saldo) o 60 (compra articulo)
        
        if (character=='Z'){
          Serial.print(valor);
          espera=1;
          
        }
        borra();
        lcd.print("Tu saldo es:");
        lcd.setCursor(0,1);
        lcd.print(valor);
        valor+=character;

      }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ENVIO DE MENSAJES
void send_sms()
    {
      Serial3.println("AT+CMGF=1");//modo texto 
      delay(2000);
      Serial3.print("AT+CMGS=");//comando de envio de mensaje, (aqui empieza la aventura SMS)
      Serial3.print((char)34); //poner el caracter "(Comilla) empleando el codigo ASCII
      Serial3.print(numero_cell); //colocamos numero de telefono
      Serial3.println((char)34); //volvemos a poner el caracter "(Comilla) empleando el codigo ASCII
      delay(200); //tiempo para que de respuesta el modulo al signo de: >
      Serial3.print("1, "); //mensaje a enviar
      Serial3.print(id); //mensaje a enviar
      Serial3.print((char)26); //ponemos el símbolo ascii 26,que corresponde a CTRL+Z,con lo que el módulo sabe que el sms termino (termina la aventura SMS)
      
      Serial.println("1, 2");//mensaje a enviar (Refeljado en el monitor serial de debugeo)
      //espera=1;
    }

void send_sms2()
    {
      Serial3.println("AT+CMGF=1");//modo texto 
      delay(2000);
      Serial3.print("AT+CMGS=");//comando de envio de mensaje, (aqui empieza la aventura SMS)
      Serial3.print((char)34); //poner el caracter "(Comilla) empleando el codigo ASCII
      Serial3.print(numero_cell); //colocamos numero de telefono
      Serial3.println((char)34); //volvemos a poner el caracter "(Comilla) empleando el codigo ASCII
      delay(200); //tiempo para que de respuesta el modulo al signo de: >
      Serial3.print("2, "); //mensaje a enviar
      Serial3.print(id);
      Serial3.print(", ");
      Serial3.print(cargo); //mensaje a enviar
      Serial3.print((char)26); //ponemos el símbolo ascii 26,que corresponde a CTRL+Z,con lo que el módulo sabe que el sms termino (termina la aventura SMS)
      
      Serial.println(cargo);//mensaje a enviar (Refeljado en el monitor serial de debugeo)
      Serial.print("2, ");
      Serial.print(id);
      Serial.print(", ");
      Serial.print(cargo);
      //espera=1;
    }

void receive_sms2() {

  if (Serial3.available()){
    char character = (char)Serial3.read();  //Convierte lo que arroja el modulo cómo caracter para sr leido en ASCII
    datosSerial[j] = character;          //Almacena cada caracter ASCII en un elemento del array "datosSerial"
    j++;
      if(character == '\n'){  //Si identifica un salto de linea en una fila, borra todos los caracteres almacenados hasta el momento y vuelve a guardar desde la fila siguiente
        j=0;
        counter++;
      }
      if(counter>=13  && j>= 1){ //Recorre todas las filas hasta llegar a la 10, y recorre los caracteres en esta hasta llegar al 28.
        Serial.print(character); //Imprime caracter x caracter desde el elemento 28 (consulta saldo) o 60 (compra articulo)
        
        if (character=='-'){
          Serial.print(valor);
          borra();
          lcd.print("Sin Saldo");
          espera=1;
         
        }

        if (character=='Z'){
          Serial.println(valor);
          espera=1;
          
        }
        borra();
        lcd.print("Tu saldo es:");
        lcd.setCursor(0,1);
        lcd.print(valor);
        valor+=character;

      }
    }
}