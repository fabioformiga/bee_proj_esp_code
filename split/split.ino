
#include "Arduino.h"
int id_beekeper = 1;

void setup()
{
  Serial.begin(9600);
  while (!Serial);
}

void loop()
{
  String obtain = "1 20 12 31";
  Serial.println("esta é a string");
  Serial.print(obtain);
  Serial.println("string convertida");
  Serial.print(split(obtain));
  delay(500);
}

String split(String mystring){
  String end_string;
  int ind1, ind2, ind3, ind4;
  String id_hive, temperature, humidity, weight;

  ind1 = mystring.indexOf(' ');
  id_hive = mystring.substring(0,ind1);
  ind2 = mystring.indexOf(' ',ind1+1);
  temperature = mystring.substring(ind1+1,ind2);
  ind3 = mystring.indexOf(' ',ind2+1);
  humidity = mystring.substring(ind2+1,ind3);
  ind4 = mystring.indexOf(' ',ind3+1);
  weight = mystring.substring(ind4);
  end_string = "idbeekeper" + (String)id_beekeper + "&idhive=" + (String)id_hive + "&temperature=" + (String)temperature + "&humidity=" + (String)humidity + "&weight=" + (String)weight;
  
  return end_string;
}
