
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "NOS_Internet_93AD";                   // wifi ssid
const char* password =  "58241024";         // wifi password

const char* mqttServer = "192.168.1.102";    // endereço ip do Raspberry Pi
const int mqttPort = 1883;
const char* mqttUser = "username";      // nome utilizador MQTT
const char* mqttPassword = "admin";  // Password MQTT
char* mensagem = " " ;
char* mensagem2 = " " ;

String valor_str;
String valor_str2;


    WiFiClient espClient;
    PubSubClient client(espClient);

void setup()
{    
  Serial.begin(115200);  
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
  //converte um valor float e envia a mensagem por MQTT


    for (unsigned int valor = 0; valor <= 700000; valor++){
      
       valor_str2 = "O numero recebido é: ";
        valor_str2.toCharArray(mensagem2, valor_str2.length() + 1);
        client.publish("esp8266", mensagem2); //indica o topico e a mensagem
     Serial.println(mensagem2);
       valor_str = String(valor);
        valor_str.toCharArray(mensagem, valor_str.length() + 1);
        client.publish("esp8266", mensagem); //indica o topico e a mensagem
     Serial.println(mensagem);

        
    client.subscribe("esp8266");
    client.loop();
    delay(100);
    }
    digitalWrite(2,HIGH);

} // fim do loop
