//-------------import libs----------------
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <SPI.h>
#include <LoRa.h>

//--------------------network parameters-------------

const char* ssid = "NOS_Internet_93AD";
const char* password = "58241024";

const char* serverName = "http://34.89.49.50/post.php";
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;
//-------------------configure lora pins---------------
const int ss = 15;        // LoRa radio chip select
const int rst = 0;        // LoRa radio reset
const int dio0 = 5;          // change for your board; must be a hardware interrupt pin

String message = "";
//------------------callback function
void onReceive(int packetSize) {

  while (LoRa.available()) {
    message += (char)LoRa.read();
  }
  Serial.println("mensagem recebida:"+ message);
}

//----------------wi-fi conection function---------
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
  delay(100);
}

void setup() {
  Serial.begin(9600);
  wi_fi_connection();
  Serial.println("LoRa gateway");
  //start LoRa
  LoRa.setPins(ss, rst, dio0);
  LoRa.begin(868E6);
  
  // register the receive callback
  LoRa.onReceive(onReceive);

  // put the radio into receive mode
  LoRa.receive();
}

void loop() {

  if(WiFi.status()!= WL_CONNECTED){ // try conection to wifi router in case disconected
   wi_fi_connection();
  }
  if (message !=""){
    if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
      if(WiFi.status()== WL_CONNECTED){
        HTTPClient http;
        
        // Your Domain name with URL path or IP address with path
        http.begin(serverName);

        // Specify content-type header
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        // Data to send with HTTP POST
        String httpRequestData = "message="+ message;           
        // Send HTTP POST request
        int httpResponseCode = http.POST(httpRequestData);
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
          
        // Free resources
        http.end();
        message ="";

      }
    }
  }
}
