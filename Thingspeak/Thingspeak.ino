#include <Bridge.h>
#include <Console.h>
#include <Process.h>

int x=0;


#define ARRAY_SIZE           1

void setup() {
  Serial.begin(9600);
  Bridge.begin();
  Console.begin(); 
  
};

void postToThingSpeak(String key, float value[]) {
  Process p;
  String cmd = "curl --data \"key="+key;
  for (int i=0;i<ARRAY_SIZE;i++) {
    cmd = cmd + "&field"+ (i+1) + "=" + value[i];
  }
  cmd = cmd + "\" http://api.thingspeak.com/update";
  p.runShellCommand(cmd);
  Console.println(cmd);
  p.close();
  x++;
  Serial.println(x);
}
 
void loop() {
  float vol[ARRAY_SIZE];
  vol[0]=x;
  postToThingSpeak("PY72P188F8HYGL8M",vol);
  delay(30000);
  
}
