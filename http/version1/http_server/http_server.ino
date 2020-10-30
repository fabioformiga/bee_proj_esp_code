// configuração para o servidor

#include <ESP8266WiFi.h>
#include "ESPAsyncWebServer.h"

//access point credentials
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
	Serial.print("Setting AP ...");
	WiFi.softAP(ssid, password);
	
	
	IPAddress IP = WiFi.softAPIP();
	Serial.print("AP ip address"); Serial.println(IP);
  Serial.print("AP configuration Finished");
	
	 server.on("/numero", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readnum().c_str());
  });
  
  //start server
  server.begin();
  
}


void loop(){
	
}
