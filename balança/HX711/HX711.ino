#include "HX711.h"
HX711 scale(2,14); // pins de comuncação NODEMCU com HX711
float calibration_factor = 8.4;
float units;

void setup(){
  Serial.begin(9600);
  scale.set_scale();
  //scale.tare(); //reset the scale to 0
   long zero_factor =scale.read_average();
  Serial.print("Fator zero:");
  Serial.print(zero_factor);
  
}

void loop(){
 
  scale.set_scale(calibration_factor);
  Serial.print(" Peso: ");
  units = scale.get_units()/1000;
  //otimização da entrega de dados ao utilizador 
  //valores negativos de peso são invalidos e valores superiores a 200 são considerados 200 
  if (units <0){
    units = 0.00;
  } 
  else if (units >200){
    units=200;
  }

  Serial.print(units, 1);
  Serial.print(" Kg");

  Serial.print(", fator de calibração: ");
  Serial.print(calibration_factor);
  Serial.println();
    if(Serial.available())
  {
    char temp = Serial.read();
    if(temp == '+' || temp == 'a')
      calibration_factor += 0.1;
    else if(temp == '-' || temp == 'z')
      calibration_factor -= 0.1;
      else if (temp =='r')
      scale.tare();
      
  }
}
