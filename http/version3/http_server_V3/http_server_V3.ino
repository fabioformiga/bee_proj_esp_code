// configuração para o servidor

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>


//access point credentials
const char* ssid = "esp8266";
const char* password = "123456789";


//ip for obtain number 
const char* clientNameNum = "http://192.168.4.10/valor";

String num;


void setup(){
	Serial.begin(9600);
	Serial.println();
	Serial.print("Setting AP ...");
	WiFi.softAP(ssid, password);
	
	
	IPAddress IP = WiFi.softAPIP();
	Serial.print("AP ip address"); Serial.println(IP);
	Serial.print("AP configuration Finished");

}

void loop(){
  Serial.print("doing a reply to"); Serial.print(clientNameNum);
  
		num = httpGETRequest(clientNameNum);
		      Serial.println("request reply: " + num );
         delay(500);
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
