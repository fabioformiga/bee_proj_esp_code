#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "HX711.h"
#include <DHT.h>
#include <Adafruit_Sensor.h>
HX711 scale(2,14); // pins de comuncação NODEMCU com HX711
float calibration_factor = 6.60;
float humidade;
float temperatura;
int units;

//definição de paramentros do DHT11  
#define DHTPIN 0          // define o pin d3 como dados do DHT11
#define DHTTYPE DHT11     // DHT 11
DHT dht(DHTPIN, DHTTYPE);

//dados conectividade ao router wifi
const char* ssid = "Bee-proj";                   // wifi ssid
const char* password =  "123456789";         // wifi password

//dados conectividade mqtt server
const char* mqttServer = "192.168.1.10";    // endereço ip do Raspberry Pi
const int mqttPort = 1883;
const char* mqttUser = "username";      // nome utilizador MQTT
const char* mqttPassword = "admin";  // Password MQTT
char mensagem;
char* mensagem1 = " " ; //envio mensagem peso
char* mensagem2 = " " ; //envio mensagem humidade
char* mensagem3 = " " ; //envio mensagem temperatura

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i< length ;i++) {
   mensagem=(char)payload[i];
    //Serial.print((char)payload[i]);  
  }
  Serial.println();
  Serial.println("a mensagem e:");
  Serial.print(mensagem);

  
}

void wificonnect(){
Serial.print(" Connecting to ");
   Serial.println(ssid);
   WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected to ");
    Serial.println(ssid);
  Serial.println(" IP address: ");  Serial.println(WiFi.localIP());

}

void mqttconnect(){
 client.setServer(mqttServer, mqttPort);
 Serial.println("A aguardar conectividade com MQTT");
 while (!client.connected()) { // a enquanto aguarda a conectividade com a o MQTT
 Serial.print(".");
 if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
  Serial.println("Conectado ao MQTT");
  client.subscribe("esp8266");
  client.publish("esp8266","");
 } 
 else {
   Serial.print("Conexão falhada com o estado: ");
   Serial.print(client.state());
   delay(2000);
    }
  }
}
void setup() {
  delay(5000);    
     pinMode(LED_BUILTIN, OUTPUT);
   //inicio do sensor DHT
   dht.begin();
  // put your setup code here, to run once:
   Serial.begin(19200); 
   wificonnect();
   mqttconnect();
   
   scale.set_scale();
  //scale.tare(); //reset the scale to 0
  long zero_factor =scale.read_average();
  Serial.print("Fator zero:");
  Serial.print(zero_factor);
}

void loop() {
  client.setCallback(callback);
  client.loop();
 scale.set_scale(calibration_factor);

/*
  Serial.print(units, 1);
  Serial.print(" Kg");

  Serial.print(", fator de calibração: ");
  Serial.print(calibration_factor);
  Serial.println();
  */
    if(Serial.available())
  {
    char temp = Serial.read();
    if(temp == '+' || temp == 'a')
      calibration_factor += 0.1;
    else if(temp == '-' || temp == 'z')
      calibration_factor -= 0.1;
    else if (temp =='r')
      scale.tare();
    else if (temp == 'f')
      Serial.print(calibration_factor);
    else if (temp == 'v')
      Serial.print(scale.get_units()/1000);
         
  }
  switch (mensagem){
  case 'w':
   digitalWrite(LED_BUILTIN, LOW);  
   Serial.print(" Peso: ");
  units = scale.get_units()/1000; //recolhe dados sensor peso
  //otimização da entrega de dados ao utilizador 
  //valores negativos de peso são invalidos e valores superiores a 200 são considerados 200 
  if (units <0){
    units = 0.00;
  } 
  else if (units >200){
    units=200;
  }
   Serial.print(units, 1);
   String(units).toCharArray(mensagem1, String(units).length() + 1);
   client.publish("esp8266", mensagem1); //indica o topico e a mensagem
   //client.publish("esp8266", unit_char);
   delay(500);
   mensagem = 's'; //permite que o resultado apenas seja mostrado 1 vez
  break;
  case 'h':
  //recolhe dados do sensor humidade e temperatura
     humidade = dht.readHumidity();
     String(humidade).toCharArray(mensagem2, String(humidade).length() + 1);
     client.publish("esp8266", mensagem2); //indica o topico e a mensagem
    mensagem = 's';

  break;  
  case 't':
  //recolhe dados do sensor humidade e temperatura
     temperatura = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
     String(temperatura).toCharArray(mensagem3, String(temperatura).length() + 1);
     client.publish("esp8266", mensagem3); //indica o topico e a mensagem
    mensagem = 's';
  break;  
}
}
