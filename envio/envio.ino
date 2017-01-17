
//char numero_cell[]="+14433455571";//meter numero de telefono Tropo1
//char numero_cell[]="+14382281357";//meter numero de telefono Tropo2
char numero_cell[]="+14433455571";//meter numero de telefono Roger
//char numero_cell[]="+17864540964";//meter numero de telefono Arranz
//char numero_cell[]="5543595053";//meter numero de telefono Anibal
//char numero_cell[]="+14089098523";//meter numero de telefono Plivo

int Push_Button= 2;

void setup()
{
  Serial.begin(9600);
  Serial3.begin(9600);
  configuracion_inicial();
  pinMode(Push_Button,INPUT);
}

void loop()
{   
  receive_sms();
  //if (Serial.available()>0){
  int State_Push= digitalRead(Push_Button);
  if(State_Push != LOW){
    Serial.println("Mandando SMS");
    send_sms();
  }
   //}    
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
    Serial.write(Serial3.read());
  }

  if (Serial.available()){
    Serial3.write(Serial.read());
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
      Serial3.print("1, 2"); //mensaje a enviar
      //Serial3.print(200); //mensaje a enviar
      Serial3.print((char)26); //ponemos el símbolo ascii 26,que corresponde a CTRL+Z,con lo que el módulo sabe que el sms termino (termina la aventura SMS)
      
      Serial.println("1, 2");//mensaje a enviar (Refeljado en el monitor serial de debugeo)
    }
