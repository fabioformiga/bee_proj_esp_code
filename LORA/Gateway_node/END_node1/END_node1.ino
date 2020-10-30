 

#include <SPI.h>
#include <LoRa.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include "HX711.h"


int counter = 0;
const long frequency = 868E6;  // LoRa Frequency

const int ss = 15;        // LoRa radio chip select
const int rst = 0;        // LoRa radio reset
const int dio0 = 5;          // change for your board; must be a hardware interrupt pin

//-------------------configuração DHT11---------------------------
#define DHTPIN 4          // D3
#define DHTTYPE DHT11     // DHT 11
DHT dht(DHTPIN, DHTTYPE);
//------------------configuração HX711 peso ADC--------------------------
HX711 scale(2, 14); //dout, sck
  //variables to HX711
float calibration_factor = 18.2; // this calibration factor is adjusted according to my load cell
long zero_factor =112210;
int units;

//-------------------configuração DHT11---------------------------
int humidade ;
int temperatura;

//------------------------configuração colmeia--------------------
String id_hive = "1";

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa node");
  LoRa.setPins(ss, rst, dio0);
  LoRa.begin(frequency);
  LoRa.setTxPower(17);

//
//  LoRa.setSyncWord(0x01);
//  LoRa.enableInvertIQ();                // active invert I and Q signals
//  LoRa.setTxPower(20);
//  LoRa.setSpreadingFactor(12);
//  LoRa.setCodingRate4(8);
//  LoRa.setSignalBandwidth(125e3);
  Serial.println("LoRa Initializing OK!");


}

void loop() {
      //-----------------deteção temperatura/humudade---------------------------
    dht.begin();
    // collect dht data
    humidade = dht.readHumidity();
    temperatura = dht.readTemperature();

    //-----------------deteção peso---------------------------
    scale.power_up();
    while(scale.is_ready() != true){
      delay(5);
    }
    scale.set_scale(calibration_factor);
    units = scale.get_units(10),100;
    if(units < 0){
      units=0;
    }
    scale.power_down();
    String text = id_hive + " " + String(temperatura) + " " + String(humidade)+ " "  + String(units/1000);

  // send packet
  LoRa.beginPacket();
  LoRa.print(text);
  LoRa.endPacket();
  Serial.print("Packet sent sucessfull");
  delay(5000);

}
