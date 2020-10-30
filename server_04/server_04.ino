#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h> // Allows us to connect to, and publish to the MQTT broker
#include "ArduinoJson.h"
#include "Client.h"
#include <vector> 
using namespace std; 

//SSID and Password of your WiFi router and Raspberry
const char* ssid = "Test ESP WIFI";
const char* password = "123456789";

const char* server_raspi_ssid = "Bee-proj";
const char* server_raspi_password = "123456789";

//Declare a global object variable from the ESP8266WebServer class.
ESP8266WebServer server(80); //Server on port 80
WiFiClient wifi_client;

// MQTT
// Make sure to update this for your own MQTT Broker!
const char* mqtt_server = "10.6.2.243";
const char* mqtt_topic = "measure";
const char* mqtt_username = "username";
const char* mqtt_password = "admin";
// The client id identifies the ESP8266 device. Think of it a bit like a hostname (Or just a name, like Greg).
const char* clientID = "Client ID";

PubSubClient MQTT_client(mqtt_server, 1883, wifi_client); // 1883 is the listener port for the Broker

// JSON library to parse object when receive message
DynamicJsonBuffer jsonBuffer;

unsigned long delayStart = 0; // the time the delay started

// Client object
vector<ESPClient> ESPclients;

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleSentVar() {
  String sensor_values;
  float tempValue;
  float weightValue;
  String IP_clients;
  String IP_client;
  String MAC_clients;
  String MAC_client;

  if (server.hasArg("sensor_reading"))
  {
    sensor_values = server.arg("sensor_reading");
  }
  JsonObject& root_sensor_value = jsonBuffer.parseObject(sensor_values);
  tempValue = root_sensor_value["temp_reading"].as<int>();
  weightValue = root_sensor_value["weight_reading"].as<int>();

  if (server.hasArg("client_ip_number"))
  {
    IP_clients = server.arg("client_ip_number");
  }
  JsonObject& root_IP_client = jsonBuffer.parseObject(IP_clients);
  IP_client = root_IP_client["client_ip_number"].as<String>();

  if (server.hasArg("client_mac_number"))
  {
    MAC_clients = server.arg("client_mac_number");
  }
  JsonObject& root_MAC_client = jsonBuffer.parseObject(MAC_clients);
  MAC_client = root_MAC_client["client_mac_number"].as<String>();

  std::string new_IP = IP_client.c_str();
  std::string new_MAC = MAC_client.c_str();

  int indexClient = isClientAlreadyExist(MAC_client);
  if(indexClient == 9999 && MAC_client.length() != 0)
  {
    // Create new client
    ESPClient espClient(new_MAC);
    espClient.setIP(new_IP);
    espClient.setTempMeasure(tempValue);
    espClient.setWeightMeasure(weightValue);
    espClient.setIsMeasureSend(true);
    ESPclients.push_back(espClient);
  } 
  else
  {
    // Change values of specified client
    ESPClient espClient = ESPclients.at(indexClient);
    espClient.setIP(new_IP);
    espClient.setTempMeasure(tempValue);
    espClient.setWeightMeasure(weightValue);
    espClient.setIsMeasureSend(true);
  }
  
  server.send(200, "text/html", "Receive data");
}

//===============================================================
// CLIENT CLASS
//===============================================================
int isClientAlreadyExist(String new_MAC)
{
  int isClientAlreadyExist = 9999; // value if you don't see client with same Mac adress
  int index = 0;
  for ( auto espClient : ESPclients ) {
    // Iterate all clients in Vector
    if(new_MAC.equals(espClient.getMacAdress().c_str()))
    {
      // If you already have this client, put the location of client in Vector
      isClientAlreadyExist = index;
    }
    index++;
  }
  return isClientAlreadyExist;
}

void SendMeasure(){
//  String message;
//  char messageArray[100];
  Serial.println("------------------------------------------");
  Serial.println("send data");
  for ( auto espClient : ESPclients ) {
    if(espClient.getMacAdress().c_str() != "") {
      String message;
      char messageArray[100];
      Serial.println("------------------------------------------");
      Serial.print("UserMAC : ");
      Serial.println(espClient.getMacAdress().c_str());
      Serial.print("UserIP : ");
      Serial.println(espClient.getIP().c_str());
      Serial.print("Temperature measure : ");
      Serial.println(espClient.getTempMeasure());
      Serial.print("Weight measure : ");
      Serial.println(espClient.getWeightMeasure());
      message = ESPclients.size();
      message = message + "/";
      message = message + espClient.getMacAdress().c_str();
      message = message + "/";
      message = message + espClient.getTempMeasure();
      message = message + "/";
      message = message + espClient.getWeightMeasure();
      //message = message + ";";
      
      message.toCharArray(messageArray, 60);
      if (MQTT_client.publish(mqtt_topic, messageArray)) {
        Serial.println("message sent!");
      }
      else {
        Serial.println("Message failed to send. Reconnecting to MQTT Broker and trying again");
        MQTT_client.connect(clientID, mqtt_username, mqtt_password);
        delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
        MQTT_client.publish(mqtt_topic, messageArray);
      } 
    }
  }
//  message.toCharArray(messageArray, 60);
//  if (MQTT_client.publish(mqtt_topic, messageArray)) {
//    Serial.println("message sent!");
//  }
//  else {
//    Serial.println("Message failed to send. Reconnecting to MQTT Broker and trying again");
//    MQTT_client.connect(clientID, mqtt_username, mqtt_password);
//    delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
//    MQTT_client.publish(mqtt_topic, messageArray);
//  }
}

//===============================================================
// SETUP
//===============================================================
void setup() {
  Serial.begin(115200);

  // Connection to the raspberry
  WiFi.mode(WIFI_STA);
  WiFi.begin(server_raspi_ssid, server_raspi_password);
  Serial.println();
  Serial.println("Connection to the raspberry server");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Connected to the raspberry server");

  // Connect to MQTT Broker
  // client.connect returns a boolean value to let us know if the connection was successful.
  // If the connection is failing, make sure you are using the correct MQTT Username and Password (Setup Earlier in the Instructable)
  if (MQTT_client.connect(clientID, mqtt_username, mqtt_password)) {
    Serial.println("Connected to MQTT Broker!");
  }
  else {
    Serial.println("Connection to MQTT Broker failed...");
  }

  // Create Webserver
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.println(myIP);  //IP address assigned to your ESP

  // Handle HTTP request of client
  server.on("/data/", handleSentVar);

  // Start server
  server.begin();
  Serial.println("HTTP server started");

  // Start delay
  delayStart = millis();   
}

//===============================================================
// LOOP
//===============================================================
void loop() {
  if (((millis() - delayStart) >= 10000)) {
    delayStart = millis();
    SendMeasure(); // Send request each 10 sec to the client for have sensor measure
  }
  
  server.handleClient(); //Handle client requests
}
