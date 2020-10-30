//gateway_apiario_v3
#include <SPI.h>
#include <RH_RF95.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define CS 15
#define RST 16
#define DIO0 5
#define RF95_FREQ 868.0

RH_RF95 rf95(CS, DIO0);

//access point credentials
const char* ssid = "esp8266";
const char* password = "123456789";
const char* clientNameNum1 = "http://192.168.4.11/valor";
const char* clientNameNum2 = "http://192.168.4.12/valor";

//Variables LoRa Receive mensage
char* mensagem_lora;
char mensagem_lora_envio[22];
String mensagem_http;
int count = 0;

void setup() 
{
	Serial.begin(9600);
  Serial.println();
  Serial.print("Setting AP ...");
  WiFi.softAP(ssid, password);
  
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP ip address"); Serial.println(IP);
  Serial.print("AP configuration Finished");
  
  rfm95_work();
}

void loop()
{
	for (int i = 1; i <= 2; i++) {
		if(i==1){
			mensagem_http = httpGETRequest(clientNameNum1);//efetua um request http o cliente desejado e o request ficará armazenado na mensagem_http
		}
		if(i==2){
			mensagem_http = httpGETRequest(clientNameNum2);//efetua um request http o cliente desejado e o request ficará armazenado na mensagem_http
		}
		mensagem_http.toCharArray(mensagem_lora_envio,(mensagem_http.length()));
		rf95.send((uint8_t *)mensagem_lora_envio,22); 
		// aguarda o envio do rfm95
		Serial.print(mensagem_http);
		rf95.waitPacketSent();	//confirma o envio
		delay(1000);//Delay between each http request 
	}
	Serial.println("ESP dorme função");
 	// ESP.deepSleep(30e6);
 }

void rfm95_work(){
  	pinMode(RST, OUTPUT);
	pinMode(DIO0, OUTPUT);
	digitalWrite(DIO0, HIGH);
	digitalWrite(RST, HIGH);
	delay(100);
	Serial.println();
	Serial.println("Gateway Module starting Reception…");
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
