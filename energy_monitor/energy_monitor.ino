/*
Eric Tsai
2015-04-22
Arduino Yun code for publishing energy use to MQTT broker
Modify the "PubSubClient client" for your broker IP address
*/


#include <Bridge.h>
#include <yunclient.h>
#include <PubSubClient.h>
#include "EmonLib.h"                   // OpenEnergy Monitor project library


int LED = 3;
EnergyMonitor emon1;                   // open energy monitor
unsigned long MQTT_reconnect=0;
unsigned long read_energy=0;
double Irms;
bool conn_ok;
YunClient yun;          //equivalent of ethernet client


//really shouldn't need call back, but just in case it's needed.
void callback(char* topic, byte* payload, unsigned int length) {
  digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);              // wait for a second
  digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
}



//use yunclient to bridge to network
PubSubClient client("192.168.1.101", 1883, callback, yun);


//**********************************************************************
void setup()
{
    // initialize digital pin 13 as an output.
  pinMode(LED, OUTPUT);

  digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW

  Bridge.begin();

  client.connect("yun_energy");
  client.publish("watch", "Energy Monitor Connected!");
  client.subscribe("yun_energy");
  
  digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(3000);              // wait for a second
  digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW

  
  emon1.current(5, 60);             // Current: input pin, calibration.
}



//**********************************************************************
void loop()
{
  char buff_message[12];    //message buffer for MQTT publish
  float value;
  client.loop();    //run this every loop to maintain MQTT connection
  
  Irms = emon1.calcIrms(1480);  // Calculate Irms only, 1480 means read Irms.
  
  //publish current every 10 seconds.
  if (millis() - read_energy > 10000)
  {
    //amps
    value = Irms;
    dtostrf (value, 4, 5, buff_message);
    client.publish("2853", buff_message);
    read_energy = millis();
    
    //watts
    value = Irms*115;
    dtostrf (value, 4, 5, buff_message);
    client.publish("2852", buff_message);
  }

  
  //maintain MQTT connection
  if ((millis() - MQTT_reconnect) > 60000)
  {
    conn_ok = client.connected();
    if (conn_ok==1)
    {
      digitalWrite(LED, HIGH);
    }
    else
    {
      digitalWrite(LED, LOW);
    }
    
    //no connection, reconnect
    if (conn_ok == 0)
    {
      client.disconnect();
      delay(5000);
      while (client.connect("yun_energy") != 1)
      {
        delay(4000);
      }
      digitalWrite(LED, HIGH);
      client.publish("watch", "Energy Monitor reconnected");
    }
    
    MQTT_reconnect = millis();
  }//end Mosquitto connection check
  
}
