//inclusao das bibliotecas necessarias
//esp e mqtt
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//sensor DHT11
#include <DHT.h>
#include <Adafruit_Sensor.h>

//definição de paramentros do DHT11  
#define DHTPIN 0          // define o pin d3 como dados do DHT11
#define DHTTYPE DHT11     // DHT 11
DHT dht(DHTPIN, DHTTYPE);

//dados conectividade À internet
const char* ssid = "NOS_Internet_93AD";                   // wifi ssid
const char* password =  "58241024";         // wifi password

//dados conectividade mqtt server
const char* mqttServer = "192.168.1.104";    // endereço ip do Raspberry Pi
const int mqttPort = 1883;
const char* mqttUser = "username";      // nome utilizador MQTT
const char* mqttPassword = "admin";  // Password MQTT


char* mensagem = " " ;


    WiFiClient espClient;
    PubSubClient client(espClient);
    
void setup() {
  //inicio da serial a 9600 de baud rate
   Serial.begin(9600); 
//inicio do sensor DHT
   dht.begin();

  // conectividade com o wifi
    Serial.print("Connecting to ");
  Serial.println(ssid);
    WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqttServer, mqttPort);
// conectividade com o MQTT
  
    Serial.println("A aguardar conectividade com MQTT");
  while (!client.connected()) { // a enquanto aguarda a conectividade com a o MQTT
    Serial.print(".");

    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {

      Serial.println("Conectado ao MQTT");
    } else {

      Serial.print("Conexão falhada com o estado: ");
      Serial.print(client.state());
      delay(2000);

    }
  }

}

void loop() {

  //recolhe dados do sensor
      float humidade = dht.readHumidity();
     float temperatura = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
// envia os dados com o protocolo MQTT

     String(humidade).toCharArray(mensagem, String(humidade).length() + 1);
     client.publish("esp8266", mensagem); //indica o topico e a mensagem
     
    client.subscribe("esp8266");
    client.loop();
    delay(1000);
    digitalWrite(2,HIGH);

}
