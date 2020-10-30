/**
  Nuno Azevedo @nunoazev - 2019
  https://nunoazev.com
*/

//ESP
#include <ESP8266WiFi.h>
//MQTT
#include <PubSubClient.h>

//WIFI CONFIG
const  char * ssid = "MEUWIFI" ;
const  char * password = "PASSWORD" ;

//MQTT CONFIG
const String HOSTNAME  = "DeviceESP8266"; //NOME DO DEVICE, deverá ter um nome unico.
const char * MQTT_COMMAND_TOPIC = "ledSala/set"; //Topico onde o Device subscreve.
const char * MQTT_STATE_TOPIC = "ledSala/state"; //Topico onde o Device publica.


//EXEMPO DE BROKERS MQTT GRATUITOS PARA TESTES https://github.com/mqtt/mqtt.github.io/wiki/public_brokers
const char* MQTT_SERVER = "IP_ou_DNS"; //IP ou DNS do Broker MQTT

// Credrenciais ao broker mqtt. Caso nao tenha AUTH meter a false.
#define MQTT_AUTH true
#define MQTT_USERNAME "usernameMQTT"
#define MQTT_PASSWORD "passMQTT"

//DEFINIR PIN DO LED
#define GPIO7_LED 13


WiFiClient wclient;
PubSubClient client(MQTT_SERVER, 1883, wclient);

//INICIAR A LIGAÇÃO WIFI
void LigarWifi() {

  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);
  Serial.print("\nA tentar ligar ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(String("\nLigado a rede: (") + ssid + ")" );
}

//INICIAR O MQTT
//Verifica se o estado da ligação está ativa e se não estiver tenta conectar-se
bool checkMqttConnection() {
  if (!client.connected()) {
    if (MQTT_AUTH ? client.connect(HOSTNAME.c_str(), MQTT_USERNAME, MQTT_PASSWORD) : client.connect(HOSTNAME.c_str())) {
      Serial.println("Ligado ao broker mqtt " + String(MQTT_SERVER));
      //SUBSCRIÇÃO DE TOPICOS
      client.subscribe(MQTT_COMMAND_TOPIC);
    }
  }
  return client.connected();
}

void setup() {
  Serial.begin(115200);

  //definir gpio7 como output
  pinMode(GPIO7_LED, OUTPUT);

  //Conecta a rede wifi criada
  LigarWifi();
  client.setCallback(callback); // resposta as mensagens vindas do MQTT
}

//Chamada de recepção de mensagem
void callback(char* topic, byte* payload, unsigned int length) {


  String payloadStr = "";
  for (int i = 0; i < length; i++) {
    payloadStr += (char)payload[i];
  }
  String topicStr = String(topic);

  if (topicStr.equals(MQTT_COMMAND_TOPIC)) {
    if (payloadStr.equals("ON")) {
      digitalWrite(GPIO7_LED, HIGH);
      client.publish(MQTT_STATE_TOPIC, "ON");
      Serial.print("LED LIGOU");

    } else if (payloadStr.equals("OFF")) {
      digitalWrite(GPIO7_LED, LOW);
      client.publish(MQTT_STATE_TOPIC, "OFF");
      Serial.print("LED DESLIGOU");
    }

  }

}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    if (checkMqttConnection()) {
      client.loop();
    }
  }
}

