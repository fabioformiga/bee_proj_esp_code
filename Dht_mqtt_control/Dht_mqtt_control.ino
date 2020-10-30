//ESP
#include <ESP8266WiFi.h>
//MQTT
#include <PubSubClient.h>
// wifi
const  char * ssid = "NOS_Internet_93AD" ;
const  char * password = "58241024" ;

// MQTT
const String HOSTNAME  = "NODE_MCU_ESP8266"; //NOME DO DEVICE, deverá ter um nome unico.
const char * TOPIC = "esp8266"; //Topico onde o Device subscreve.
const char* MQTT_SERVER = "192.168.1.104"; //IP ou DNS do Broker MQTT
long lastMsg = 0;
char msg[50];
int value = 0;

// Credrenciais ao broker mqtt. Caso nao tenha AUTH meter a false.
#define MQTT_AUTH true
#define MQTT_USERNAME "username"
#define MQTT_PASSWORD "admin"

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
      Serial.println("Ligado ao servidor mqtt " + String(MQTT_SERVER));
      //SUBSCRIÇÃO DE TOPICOS
      client.subscribe(TOPIC);
	  Serial.println("cliente subscreveu o topico" + String(TOPIC));
          client.publish("esp8266", "ola rasp");

    }
  }
  return client.connected();
}

void setup() {
  Serial.begin(115200);
  LigarWifi();
  client.setCallback(callback); // resposta as mensagens vindas do MQTT
    pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level

}

//Chamada de recepção de mensagem
void callback(char* topic, byte* payload, unsigned int length){
    Serial.print("callback\n");
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  if ((char)payload[0] == '0') {
    Serial.print("Led OFF");
    digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else if((char)payload[0] == '1') {
    Serial.print("Led ON");
    digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  }
    

}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    if (checkMqttConnection()) {
      client.loop();
    }
  }
}
