#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>  
#include <Keypad.h>


#define I2C_ADDR    (0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

#define  LED_OFF  0
#define  LED_ON  1

const byte filas = 4; 
const byte columnas = 4; 
char matriz[filas][columnas] = {
  {'0','1','2','3'},
  {'4','5','6','7'},
  {'8','9','A','B'},
  {'C','D','E','F'}
};
byte pinfila[filas] = {3, 2, 1, 0}; 
byte pincolumna[columnas] = {7, 6, 5, 4}; 


Keypad teclado = Keypad( makeKeymap(matriz), pinfila, pincolumna, filas, columnas); 

LiquidCrystal_I2C lcd(I2C_ADDR, 4, 5, 6, 0, 1, 2, 3, 7, NEGATIVE);

int x=0;

void setup(){
  lcd.begin (16,2);  
  lcd.setBacklight(LED_ON);
  inicio();
}

void loop(){
  x=0;
  while (x<1){
    char car=datos.getKey();
    if (car){
      switch(car){
        case 'A':
          lectura_dedo();
          //Aquí va el código para recibir SMS con saldo
          break;
        case 'B':
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Cantidad:");
          lcd.setCursor(0,1);
          char precio=datos.getKey();
          if (precio)
          lectura_dedo();
          //Aquí va el código para recibir SMS con saldo y envio de cantidad de dinero
          break;
        default:
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Operacion no");
          lcd.setCursor(0,1);
          lcd.print("Valida");
          break;   
      }
    }
    x=1;
  }
}

void inicio(){
  lcd.clear();
  lcd.setBacklight(LED_OFF);
  delay(500); 

  lcd.backlight(); 
  lcd.setCursor(0,0);
  lcd.print("A para saldo"); 
  lcd.setCursor(0,1);
  lcd.print("B para deposito"); 
  delay(2000); 
}


void lectura_dedo(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Coloca dedo");
  lcd.setCursor(0,1);
  lcd.print("Sobre sensor");
}

