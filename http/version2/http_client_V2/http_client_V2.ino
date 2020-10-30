
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "ESPAsyncWebServer.h"

const char* ssid = "esp8266";
const char* password = "123456789";



// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

int num=0;

String readnum(){
	num++;
  Serial.print("Number is: "); Serial.print(num);
	return String(num);
 
}


void setup(){

Serial.begin(9600);
Serial.println();
wi_fi_connection();

  
server.on("/numero", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readnum().c_str());
  });
  
  //start server
  server.begin();
}

void wi_fi_connection(){
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

void loop(){

  
}
