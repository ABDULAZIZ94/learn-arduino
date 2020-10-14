#include <dht.h>

dht DHT;

#define DHT11_PIN 8

void setup(){
  Serial.begin(9600);
}

void loop(){
  int chk = DHT.read11(DHT11_PIN);
  if(DHT.temperature != -999){
    Serial.print("Temperature = ");
    Serial.print(DHT.temperature);
  }
  if(DHT.humidity != -999){
    Serial.print("Humidity = ");
    Serial.println(DHT.humidity);
  }

  delay(1000);
}
