/*
 Setup your scale and start the sketch WITHOUT a weight on the scale
 Once readings are displayed place the weight on the scale
 Press +/- or a/z to adjust the calibration_factor until the output readings match the known weight
 Arduino pin 14 -> HX711 CLK
 Arduino pin 2 -> HX711 DOUT
 Arduino pin 5V -> HX711 VCC
 Arduino pin GND -> HX711 GND 
*/

#include "HX711.h"

HX711 scale(2, 14);

float calibration_factor = 18.2; // this calibration factor is adjusted according to my load cell
float units;
float ounces;
long zero_factor =112210;
void setup() {
  Serial.begin(9600);
  Serial.println("HX711 calibration sketch");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press + or a to increase calibration factor");
  Serial.println("Press - or z to decrease calibration factor");

  scale.set_scale();
  //scale.tare();  //Reset the scale to 0
  delay(1000);
  //zero_factor = scale.read_average(); //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);
}

void loop() {

  scale.set_scale(calibration_factor); //Adjust to this calibration factor
  //scale.set_offset(zero_factor);

  Serial.print("Reading: ");
  units = scale.get_units(10),100;
  if(units < 0){
	  units=0;
  }
  
  ounces = units * 0.035274;
  Serial.print(int(units/1000));
  Serial.print(" kg"); 
  Serial.print(" calibration_factor: ");
  Serial.print(calibration_factor);
  Serial.println();
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);

  if(Serial.available())
  {
    char temp = Serial.read();
    if(temp == '+' || temp == 'a')
		calibration_factor += 0.1;
    else if(temp == '-' || temp == 'z')
		calibration_factor -= 0.1;
    else if(temp == 'r' || temp == 'r'){
		scale.tare();
		delay(1000);
		zero_factor = scale.read_average(); //Get a baseline reading
		Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
		Serial.println(zero_factor);
	}
  }
}
