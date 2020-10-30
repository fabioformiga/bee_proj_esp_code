

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

#include <SPI.h>
#include <LoRa.h>
//--------------------network parameters-------------
const char *ssid = "NOS_Internet_93AD";
const char *password = "58241024";
// ipv4 database server 80 port
const char *host = "192.168.1.103";


const long frequency = 868E6;  // LoRa Frequency

const int ss = 15;        // LoRa radio chip select
const int rst = 0;        // LoRa radio reset
const int dio0 = 5;          // change for your board; must be a hardware interrupt pin

IPAddress local_IP(192, 168, 2, 100); //conecta com um endereço de ip fixo

String text;

int id_beekeper = 1;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  wi_fi_connection();

  //LoRa setup
  Serial.println("LoRa Receiver Callback");
  LoRa.setPins(ss, rst, dio0);
  LoRa.begin(frequency);
  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  // register the receive callback
  LoRa.onReceive(onReceive);
  // put the radio into receive mode
  LoRa.receive();
}

void loop() {

}

void onReceive(int packetSize) {
  String postData;
  char received_packet[packetSize];
  // received a packet
  Serial.println("Received packet");
  // read packet
  for (int i = 0; i < packetSize; i++) {
    // Serial.print((char)LoRa.read());
    received_packet[i] = (char)LoRa.read();
  }
  postData = split((String)received_packet);
  Serial.println((String)received_packet);
  HTTPClient http;    //Declare object of class HTTPClient 
  http.begin("http://192.168.1.103/beeproject/postdemo.php");              //Specify request destination
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header

  int httpCode = http.POST(postData);   //Send the request
  String payload = http.getString();    //Get the response payload

  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload

  http.end();  //Close connection
}

void wi_fi_connection(){
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot

  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
}


String split(String mystring){
  String end_string;
  int ind1, ind2, ind3, ind4;
  String id_hive, temperature, humidity, weight;

  ind1 = mystring.indexOf(' ');
  id_hive = mystring.substring(0,ind1);
  ind2 = mystring.indexOf(' ',ind1+1);
  temperature = mystring.substring(ind1+1,ind2);
  ind3 = mystring.indexOf(' ',ind2+1);
  humidity = mystring.substring(ind2+1,ind3);
  ind4 = mystring.indexOf(' ',ind3+1);
  weight = mystring.substring(ind4);
  end_string = "idbeekeper" + (String)id_beekeper + "&idhive=" + (String)id_hive + "&temperature=" + (String)temperature + "&humidity=" + (String)humidity + "&weight=" + (String)weight;
  return end_string;
}
