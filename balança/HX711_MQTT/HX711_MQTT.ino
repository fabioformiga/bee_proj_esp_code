//########################## SENSOR DE PESO##################
#include "HX711.h" //inclui a livraria do DHX 711
HX711 scale(2,14); // pins de comunicação NODEMCU com HX711
float calibration_factor = 2.3; //Fator de calibração padrão para os sensores correspondentes)
float units; //variavel para a unidade de peso
//########################## FIM SENSOR DE PESO##################

//##########################ESP 8266#####################
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
const char* MQTT_SERVER = "192.168.1.10"; //IP ou DNS do Broker MQTT
long lastMsg = 0;
char msg[50];
int value = 0;
// Credrenciais ao broker mqtt. Caso nao tenha AUTH meter a false.
#define MQTT_AUTH true
#define MQTT_USERNAME "username"
#define MQTT_PASSWORD "admin"
WiFiClient wclient;
PubSubClient client(MQTT_SERVER, 1883, wclient);


//#########    INICIAR A LIGAÇÃO WIFI############################
void LigarWifi() {
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);
  Serial.print("\nA tentar ligar ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(String("\nLigado a rede: (") + ssid + ")" );
    Serial.print(" Endereço ip NODE MCU Module: ");
    Serial.println(WiFi.localIP());
}
//######### FIM LIGAÇÃO WIFI############################


//###########INICIAR O MQTT##############################
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
//###########FIM INICIAR O MQTT##############################
//###########FUNÇÃO RESPOSTA MQTT##############################

//Chamada de recepção de mensagem
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}
//###########FIM FUNÇÃO RESPOSTA MQTT##############################

//#########################FIM ESP 8266######################


void setup(){
  //############COMUM SETUP###############
  Serial.begin(115200); //começa o serial e define baudrate
  pinMode(LED_BUILTIN, OUTPUT);     // INICIALIZA PINLED COMO  output
  digitalWrite(LED_BUILTIN, LOW);   // ledpin estado baixo (led ligado)

  //############SETUP SENSOR  PESO###############
  scale.set_scale(); //permite chamar a função de set scale
  //scale.tare(); //reset a escala para zero
  long zero_factor =scale.read_average(); //define o fator zero
  Serial.print("Fator zero:");
  Serial.print(zero_factor);
  //############FIM SETUP SENSOR PESO###############
  //##########INICIO ESP8266 SETUP############
    LigarWifi();
    checkMqttConnection();
  client.setCallback(callback); // resposta as mensagens vindas do MQTT
   //##########FIM  ESP8266 SETUP############

}

void loop(){
  
  //##########################LOOP SENSOR DE PESO#######################
  scale.set_scale(calibration_factor);
  units = scale.get_units()/1000; //passa o valor para kg em vez de g
  //otimização da entrega de dados ao utilizador 
  //valores negativos de peso são invalidos e valores superiores a 200 são considerados 200 
  if (units <0){
    units = 0.00;
  } 
  else if (units >200){
    units=200;
  }
  //mostra o peso e o fator de calibraçao no serial
  Serial.print(" Peso: ");
  Serial.print(units, 1);
  Serial.print(" Kg");
  Serial.print(", fator de calibração: ");
  Serial.print(calibration_factor);
  Serial.println();
  if(Serial.available()){
    char temp = Serial.read();
    if(temp == '+' || temp == 'a') //aumenta o fator de calibração
      calibration_factor += 0.1;
    else if(temp == '-' || temp == 'z') //diminui o fator de calibração
      calibration_factor -= 0.1;
     else if (temp =='r') //reseta a escala 
      scale.tare();
      }
   //#########################FIM LOOP SENSOR DE PESO#######################
   //######################## LOOP ESP8266 #################################
  if (WiFi.status() == WL_CONNECTED) {
    if (checkMqttConnection()) {
      client.loop();
    }
   }
   
  //########################FIM  LOOP ESP8266 #################################
}
