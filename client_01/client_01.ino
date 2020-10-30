#include <ESP8266WiFi.h>
#include <DHT.h>
#include "HX711.h"
#include <Adafruit_Sensor.h>

#define DHTPIN 15
#define DHTTYPE DHT11     // DHT 11
DHT dht(DHTPIN, DHTTYPE);

//SSID and Password of your WiFi router
const char* ssid = "Test ESP WIFI";
const char* password = "123456789";

IPAddress IP(192,168,4,1);
const char * host = "192.168.4.1";

boolean isSendingMeasure = false;

HX711 scale(0, 4); // pins de comuncação NODEMCU com HX711
float calibration_factor = 6.60;

const int watchdog = 5000;
unsigned long previousMillis = millis(); 

WiFiClient client;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);           // connects to the WiFi AP
  dht.begin();
  Serial.println();
  Serial.println("Connection to the AP");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("Connected");
  Serial.print("LocalIP:"); Serial.println(WiFi.localIP());
  Serial.println("MAC:" + WiFi.macAddress());
  Serial.print("Gateway:"); Serial.println(WiFi.gatewayIP());
  Serial.print("AP MAC:"); Serial.println(WiFi.BSSIDstr());

  pinMode(LED_BUILTIN, OUTPUT);
  scale.set_scale();
  long zero_factor = scale.read_average();
}

int getWeight()
{
  digitalWrite(LED_BUILTIN, LOW); 
  scale.set_scale(calibration_factor);
  int units = scale.get_units()/1000;
  if (units <0){
    units = 0.00;
  } 
  else if (units >200){
    units=200;
  }
  return units;
}

String IPToString(const IPAddress& ipAddress)
{
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3])  ;
}

void loop() {
  unsigned long currentMillis = millis();

  if ( currentMillis - previousMillis > watchdog ) 
  {
    previousMillis = currentMillis;
    WiFiClient client;
    
    //client.connect(IP, 80);
    if (!client.connect(IP, 80)) {
      Serial.println("connection failed");
      return;
    }
    String url = "/data/";
    url += "?sensor_reading=";
    url +=  "{\"temp_reading\":\"temp_value\",\"weight_reading\":\"weight_value\"}";
    url += "&client_ip_number=";
    url +=  "{\"client_ip_number\":\"client_ip_adress\"}";
    url += "&client_mac_number=";
    url +=  "{\"client_mac_number\":\"client_mac_adress\"}";
    
    url.replace("temp_value", String((int)dht.readTemperature()));
    url.replace("weight_value", String(40));
    url.replace("client_ip_adress", WiFi.localIP().toString());
    url.replace("client_mac_adress", WiFi.macAddress());
    
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
                 
    unsigned long timeout = millis();
    
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }
  
    while(client.available()){
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
  }
}
