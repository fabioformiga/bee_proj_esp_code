
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const char* ssid = "esp8266";
const char* password = "123456789";



//ip for obtain number 
const char* serverNameNum = "http://192.168.4.1/numero";


String num;

void setup(){

Serial.begin(9600);
Serial.println();
wi_fi_connection(); 
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
	if(WiFi.status() == WL_CONNECTED){ 
		num = httpGETRequest(serverNameNum);
		      Serial.println("Numero: " + num );
			  
	}
	else {
      Serial.println("WiFi Disconnected");
    }
	delay(1000);
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
