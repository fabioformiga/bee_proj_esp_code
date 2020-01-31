//gateway_apiario_v3
#include <SPI.h>
#include <RH_RF95.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define RFM95_CS 15
#define RFM95_RST 16
#define RFM95_INT 5
#define RF95_FREQ 868.0
#define LED 2

RH_RF95 rf95(RFM95_CS, RFM95_INT);

//access point credentials
const char* ssid = "esp8266";
const char* password = "123456789";
const char* clientNameNum = "http://192.168.4.10/valor";

//Variables LoRa Receive mensage
char* mensagem_lora;
char mensagem_lora_envio[22];
String mensagem_http;

int count = 0;

void setup() 
{
  Serial.println();
  Serial.print("Setting AP ...");
  WiFi.softAP(ssid, password);
  
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP ip address"); Serial.println(IP);
  Serial.print("AP configuration Finished");
  
  rfm95_work();
}

void loop()
{
    int timer=0;
    char* recebido= "";
    mensagem_http = httpGETRequest(clientNameNum) + " :" + count++;//efetua um request http o cliente desejado e o request ficará armazenado na mensagem_http
    mensagem_http.toCharArray(mensagem_lora_envio,(mensagem_http.length()));
    rf95.send((uint8_t *)mensagem_lora_envio,22); 
    // aguarda o envio do rfm95
    Serial.print("\n... -->> " + mensagem_http + " :" + count++);
    rf95.waitPacketSent();
    Serial.println(" <<--  " ); //confirma o envio
    delay(1000);//Delay between each http request 
 }

void rfm95_work(){
  
  pinMode(LED, OUTPUT);
  digitalWrite(LED,HIGH);   
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  Serial.begin(9600);
  delay(100);

  Serial.println();

  Serial.println("Gateway Module starting Reception…");

  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) { //detects if LoRa radio fail
    Serial.println("LoRa radio init failed");
    while (1);
  }
  
  Serial.println("LoRa radio init OK!");
  if (!rf95.setFrequency(RF95_FREQ)) { //detects if LoRa set frequency fails
    Serial.println("setFrequency failed");
    while (1);
  }
  
  Serial.print("Set Freq to: ");
  Serial.println(RF95_FREQ);
  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95 / 96/97/98 modules using the transmitter pin PA_BOOST, then
  // you can set transmission powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);  //define the power of transmition
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
