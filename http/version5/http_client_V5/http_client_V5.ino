//http_client_v5 created by Fábio Formiga Gonçalves, LEET 3ºano.
//importa bibliotecas necessarias 
#include <ESP8266WiFi.h> 
#include "ESPAsyncWebServer.h"
#include "HX711.h"
#include <DHT.h>
#include <Adafruit_Sensor.h>
//------------------Wi-fi and server http configuration--------------------------

//SSID e password de conexão ao access point 
const char* ssid = "esp8266";
const char* password = "123456789";

// Set your Static IP address
IPAddress local_IP(192, 168, 4, 11); //conecta com um endereço de ip fixo
// Set your Gateway IP address
IPAddress gateway(192, 168, 4, 1); //dados da gateway
IPAddress subnet(255, 255, 0, 0); // mascara de rede 

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
//------------------configuração HX711 peso ADC-------------------------- 
HX711 scale(2, 14);
  //variables to HX711
float calibration_factor = 18.2; // this calibration factor is adjusted according to my load cell
int units;
long zero_factor =112210;
//-------------------configuração DHT11---------------------------
#define DHTPIN 4          // D3 
#define DHTTYPE DHT11     // DHT 11
DHT dht(DHTPIN, DHTTYPE);
//---------------------------------------------
int humidade ;
int temperatura;
//------------------------configuração colmeia--------------------
String id_hive = "1";
bool sleep = false;

String http_reply(){//esta função é chamada aquando de um request http a http://192.168.4.10/valor 
	String text = id_hive + " " + String(temperatura) + " " + String(humidade)+ " "  + String(units/1000);
	Serial.println();Serial.print(text);
	sleep = !sleep;
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
	scale.set_scale();
	dht.begin();
}
void loop(){
	//-------------------estabilidade wi-fi-------------------------
	if(WiFi.status() != WL_CONNECTED){ //avisa quando o device é desconectado e volta a fazer a reconexão com o AP, sem ter que reiniciar o esp da colmeia.
		Serial.print("Device disconected from AP \n");
		Serial.print(" Try to reconect");
		wifi_connect();
	}
	collect_data();
	
	if(sleep ==true){
		Serial.print("ESP deepSleep");
		ESP.deepSleep(30e6);
	}
}

void wifi_connect(){ //efetua a conexão a um ponto de acesso previamente estipulado
	Serial.println();
	WiFi.mode(WIFI_STA);
	// Configures static IP address
	if (!WiFi.config(local_IP, gateway, subnet)) {
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

void collect_data(){
			//-----------------deteção peso---------------------------
	scale.set_scale(calibration_factor);
	units = scale.get_units(10),100;
	if(units < 0){
		units=0;
	}
	//------------------deteção temperatura--------------------------
	humidade = dht.readHumidity();
	temperatura = dht.readTemperature();
    //------------------Construção da string para envio---------------
	String text = id_hive + " " + String(temperatura) + " " + String(humidade)+ " "  + String(units/1000);
	Serial.println();Serial.print(text);
}
