// gateway internet 

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "ESPAsyncWebServer.h"

#include <SPI.h>
#include <RH_RF95.h>

//define os pins do RFM95
#define RFM95_CS 15
#define RFM95_RST 16
#define RFM95_INT 5

RH_RF95 rf95(RFM95_CS, RFM95_INT);

//define a frequencia do RFM95
#define RF95_FREQ 868.0

// conexão com a rede wifi destinada ao escritorio do apicultor
const char* ssid = "ASUS_D8_2G";
const char* password = "abelhas2019";
// Set your Static IP address
IPAddress local_IP(192, 168, 1, 20); //conecta com um endereço de ip fixo

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
	int timer=0;
  do{ //agurada por aproximadamente 1 segundo até que volta a enviar nova mensagem
    delay(1);
    timer++;
    if(timer>1000){
      break;
    }
  }while((rf95.available()== false ));
  
    if (rf95.available())
    {
      Serial.print("\n...recebeu mensagem:");
      // Should be a message for us now   
      uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
      uint8_t len = sizeof(buf);
     
      if (rf95.recv(buf, &len))
      {
        RH_RF95::printBuffer("\nReceived LORA: ", buf, len+1) // mostra no serial o RFM 
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
	//coloca o RFM95 em funcionamento
	pinMode(RFM95_RST, OUTPUT);
	digitalWrite(RFM95_RST, HIGH);
	delay(100);
	Serial.println("Gateway Module starting Reception…");

	digitalWrite(RFM95_RST, LOW);
	delay(10);
	digitalWrite(RFM95_RST, HIGH);
	delay(10);

	while (!rf95.init()) { //detects if LoRa radio fail

		Serial.println("LoRa radio init failed");

		while (1);

  }

	Serial.println("LoRa radio init OK!");

	if (!rf95.setFrequency(RF95_FREQ)) { //detects if LoRa set frequency fails

		Serial.println("setFrequency failed");

		while (1);

  }

	Serial.print("Set Freq to: ");
	Serial.println(RF95_FREQ);

    rf95.setTxPower(23, false);  //define the power of transmition to 23 dbm

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
