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
const char* ssid = "Bee-proj";
const char* password = "123456789";

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

server.on("/numero", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", get_valor());
  });

  //start server
  server.begin();
}


void loop(){
	char  mensagem[31] = "valor";
  int timer=0;
 Serial.print("a enviar: "); Serial.print(String(mensagem));
 //envia para o rfm95
  rf95.send((uint8_t *)mensagem ,20); 
  // // aguarda o envio do rfm95
  Serial.println("envio efetuado"); //confirma o envio
  
    do{ //agurada por aproximadamente 2 segundo até que volta a enviar nova mensagem
      delay(1);
      timer++;
      if(timer>2000){
         break;
      }
    }while((rf95.available()== false ));
     
     if (rf95.available()){ 
        Serial.print("recebeu mensagem");
        // Should be a message for us now   
        uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);
     
        if (rf95.recv(buf, &len)){
            RH_RF95::printBuffer("Received: ", buf, len); // mostra no serial o RFM 
            Serial.print("Got: ");
            Serial.println((char*)buf);
            Serial.print("RSSI: ");
            Serial.println(rf95.lastRssi(), DEC);
            valor = String((char*)buf);
        }
 
    }
    else{
        Serial.print("Não Obteve resposta");
       valor = String("-/");
    }
}

String get_valor(){
  return String(valor);
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
