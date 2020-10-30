// Gateway LoRa » Wi-fi apiario 
// configuração para o servidor


//inclui bibliotecas necessarias
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include <SPI.h>
#include <RH_RF95.h>
//define os pins do RFM95
#define RFM95_CS 15
#define RFM95_RST 16
#define RFM95_INT 5

RH_RF95 rf95(RFM95_CS, RFM95_INT);

//define a frequencia do RFM95
#define RF95_FREQ 868.0

//access point credentials
const char* ssid = "esp8266";
const char* password = "123456789";


//ip for obtain number 
const char* clientNameNum = "http://192.168.4.2/numero";

//Variables LoRa Receive mensage
uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
uint8_t len = sizeof(buf);
char* mensagem_lora;
char mensagem_lora_envio[40];
String mensagem_http;

void setup(){
  
	Serial.begin(9600);
	RFM95_work(); //active de rfm95 in specific pins
  Serial.println();
  Serial.print("Setting AP ...");
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP ip address"); Serial.println(IP);
  Serial.print("AP configuration Finished");
}

void loop(){
	
	if (rf95.available()){
    // Should be a message here
		if (rf95.recv(buf, &len)){
		  Serial.print("Got: "); 
		  
		  mensagem_lora =((char*)buf);
		  Serial.println(mensagem_lora); // show: "Got: mensage
			if (String(mensagem_lora) == "numero"){
        Serial.print("a efetuar request HTTP");
				mensagem_http = httpGETRequest(clientNameNum);//efetua um request http o cliente desejado e o request ficará armazenado na mensagem_http
				Serial.print("a enviar: "); Serial.print(mensagem_http);
				mensagem_http.toCharArray(mensagem_lora_envio,(mensagem_http.length()));
				//envia para o rfm95
				rf95.send((uint8_t *)mensagem_lora_envio ,20);  //send(const uint8_t* data, uint8_t len);
				// aguarda o envio do rfm95
				Serial.print("\n a aguardar envio \n");
				rf95.waitPacketSent();
				Serial.println("envio efetuado"); //confirma o envio
			}
		}
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


String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // Your IP address with path or Domain name with URL path 
  http.begin(client, serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "--"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}
