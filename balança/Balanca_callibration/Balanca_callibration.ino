/*
 Setup your scale and start the sketch WITHOUT a weight on the scale
 Once readings are displayed place the weight on the scale
 Press +/- or a/z to adjust the calibration_factor until the output readings match the known weight
 Arduino pin 6 -> HX711 CLK
 Arduino pin 5 -> HX711 DOUT
 Arduino pin 5V -> HX711 VCC
 Arduino pin GND -> HX711 GND 
*/

#include "HX711.h"

HX711 scale(2, 14);

float calibration_factor = -10; // this calibration factor is adjusted according to my load cell
int units;
int peso;

void setup() {
  Serial.begin(9600);
  
  scale.set_scale();
  //scale.tare();  //Reset the scale to 0

  long zero_factor = scale.read_average(); //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);
   Serial.print("qual o peso do objeto");
}

void loop() {
  scale.set_scale(calibration_factor); //Adjust to this calibration factor
  units = scale.get_value();
  Serial.println("units:"); Serial.print(units);
	while(Serial.available()){
		Serial.println("o valor de peso inserido é: "); Serial.print(Serial.read());
		char temp = Serial.read();
		peso = int(temp);
		while(peso != units){
			calibration_factor = calibration_factor + 0.01;
			Serial.println("o fator de calibração é:"); Serial.print(calibration_factor);
			Serial.println("units:"); Serial.print(units);
			delay(1);
		}
		Serial.print("o fator de calibração obtido é:") + calibration_factor;
	
	}
	
}
