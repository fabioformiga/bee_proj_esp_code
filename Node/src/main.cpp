#include <Arduino.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <LoRa.h>
#include "HX711.h"
#include <SPI.h>   

int sleep_time_min=60; 
//-------------------configuração hx711---------------------------

#define DOUT 2
#define CLK  14
HX711 scale;

float calibration_factor = 15.015; 
float units;
long zero_factor = 388619;

//-------------------configuração DHT11---------------------------

#define DHTPIN 4          // D2
#define DHTTYPE DHT11     // DHT 11
DHT dht(DHTPIN, DHTTYPE);
int humidade ;
int temperatura;
//-------------------configuração LoRa---------------------------

const int ss = 15;        // LoRa radio chip select
const int rst = 0;        // LoRa radio reset
const int dio0 = 5;          // change for your board; must be a hardware interrupt pin


String id_hive = "1";

void setup() {
    Serial.begin(9600);
    dht.begin();

    Serial.print("Node started... \n");


}


void loop() {
    // collect dht data
    humidade = dht.readHumidity();
    temperatura = dht.readTemperature();

    scale.begin(DOUT, CLK);
    scale.set_offset(zero_factor);
    scale.set_scale(calibration_factor); //Adjust to this calibration factor

    //collect hx711 units 
    digitalWrite(14,LOW);
    units = scale.get_units();
    if (units < 0) {
        units = 0.00;
    }
    //construc string to send 
    String text = id_hive + " " + String(temperatura) + " " + String(humidade)+ " "  + String(int(units/1000))+" ";
    Serial.print(text); //show string 
    
    LoRa.setPins(ss, rst, dio0);
    LoRa.begin(868E6);
    // begining the packet send 

    LoRa.beginPacket();
    LoRa.print(text);
    LoRa.endPacket();
    Serial.print("packet sent");
    LoRa.end();
    // end packet send 

    delay(100);
   // ESP.deepSleep(sleep_time_min*60E6);

}

