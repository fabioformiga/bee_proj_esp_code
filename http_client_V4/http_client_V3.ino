//http_client_v3
//importa bibliotecas necessarias 
#include <ESP8266WiFi.h> 
#include "ESPAsyncWebServer.h"


//SSID e password de conexão ao access point 
const char* ssid = "esp8266";
const char* password = "123456789";

// Set your Static IP address
IPAddress local_IP(192, 168, 4, 10); //conecta com um endereço de ip fixo

// Set your Gateway IP address
IPAddress gateway(192, 168, 4, 1); //dados da gateway
IPAddress subnet(255, 255, 0, 0); // mascara de rede 
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
  
  int numero = 0;

String http_reply(){//esta função é chamada aquando de um request http a http://192.168.4.10/valor 
	numero=random(0,40);
    String text= "1 20 30 " + String(numero);
return text; // retorna uma string
}

void setup(){
Serial.begin(9600);
wifi_connect();


server.on("/valor", HTTP_GET, [](AsyncWebServerRequest *request){ //quando recebe um request, executa a funão http_reply e retorna uma string 
    request->send(200, "text/plain",http_reply());
  });

  //start server
    server.begin();

}

void wifi_connect(){ //efetua a conexão a um ponto de acesso previamente estipulado
  Serial.println();
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
void loop(){
  if(WiFi.status() != WL_CONNECTED){ //avisa quando o device é desconectado e volta a fazer a reconexão com o AP, sem ter que reiniciar o esp da colmeia.
    Serial.print("Device disconected from AP \n");
    Serial.print(" Try to reconect");
    wifi_connect();
  }
}
