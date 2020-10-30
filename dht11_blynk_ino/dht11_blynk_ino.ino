 
#include <ESP8266WiFi.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
 
#define DHTPIN 4          // D3
 
#define DHTTYPE DHT11     // DHT 11

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  Serial.begin(9600);
 
  dht.begin();
 
  
}
 
void loop()
{ 
    int humidade = dht.readHumidity();
    int temperatura = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

    Serial.print("Temperatura:"); Serial.print(temperatura);Serial.print(" ºC");
    Serial.println();
    Serial.print("Humidade:"); Serial.print(humidade);;Serial.print(" %");
	Serial.println();
	delay(1000);

}
