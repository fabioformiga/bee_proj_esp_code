// gateway internet

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "ESPAsyncWebServer.h"

#include <SPI.h>
#include <RH_RF95.h>

//define os pins do RFM95

#define CS 15
#define RST 16
#define DIO0 5

RH_RF95 rf95(CS, DIO0);

//define a frequencia do RFM95
#define RF95_FREQ 868.0

// conexão com a rede wifi destinada ao escritorio do apicultor
//SSID e password de conexão ao access point
const char* ssid = "Bee-proj";
const char* password = "123456789";
// Set your Static IP address
IPAddress local_IP(192, 168, 2, 100); //conecta com um endereço de ip fixo

// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1); //dados da gateway
IPAddress subnet(255, 255, 255, 0); // mascara de rede
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);


//Variables LoRa Receive mensage
uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
uint8_t len = sizeof(buf);
String valor;

void setup(){

Serial.begin(9600);
RFM95_work();

wi_fi_connection();

server.on("/valor", HTTP_GET, [](AsyncWebServerRequest *request){
	AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", valor.c_str());
    response->addHeader("Access-Control-Allow-Origin","*");
	request->send(response);
  });

  //start server
  server.begin();
}

void loop(){
    if (rf95.available())
    {
      Serial.print("\n...recebeu mensagem:");
      // exist a message for us now
      uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
      uint8_t len = sizeof(buf);

      if (rf95.recv(buf, &len))
      {
        RH_RF95::printBuffer("\nReceived LORA: ", buf, len+1); // mostra no serial o RFM
        //Serial.print("\n Received LoRa: " + String(buf) + String(len));
		Serial.print("Got: ");
  	    valor = ((char*)buf);
  	    Serial.print(valor);
      }
    }
    else{
      Serial.print("\n!Not rx");
    }
}
void RFM95_work(){
	pinMode(RST, OUTPUT);
	pinMode(DIO0, OUTPUT);
	delay(100);
	digitalWrite(DIO0, HIGH);
	digitalWrite(RST, HIGH);
	delay(100);
	Serial.println();
	Serial.println("Gateway Module starting Transmission…");
	while (!rf95.init()) { //detects if LoRa radio fail
		Serial.println("LoRa radio init failed");
	}
	Serial.println("LoRa radio init OK!");
	if (!rf95.setFrequency(RF95_FREQ)) { //detects if LoRa set frequency fails
		Serial.println("setFrequency failed");
	}
	Serial.print("Set Freq to: ");
	Serial.println(RF95_FREQ);

}

void wi_fi_connection(){
  WiFi.mode(WIFI_STA);
        // Configures static IP address

    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
	WiFi.begin(ssid, password);
	Serial.println("Connecting");
	while(WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	Serial.println("");
	Serial.print("Connected to WiFi network with IP Address: ");
	Serial.println(WiFi.localIP());
}
