#include <EmonLib.h>;                    // Incluimos Emon Library
 
EnergyMonitor emon1;                   // Creamos la instancia de la librería
 
void setup() {
  Serial.begin(9600);
  emon1.current(1, 60.60);             // Inicializamos la librería: input pin, calibration.
}
 
void loop() {
  double Irms = emon1.calcIrms(1480);  // Calculamos el irms
 
  if(Irms <0.20) Irms = 0.0;
 
  Serial.print(" Consumo: ");
  Serial.println(Irms*125); //Nota: Posible rango de error de +10W, en caso de exactitud restarle 10W a este resultado
 
}

//Fuente: http://www.gonzalogalvan.es/medidor-de-consumo-lectura-de-la-corriente-con-arduino/
