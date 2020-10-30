#include <SPI.h>
#include <LoRa.h>

int counter = 0;
const long frequency = 868E6;  // LoRa Frequency

const int ss = 15;        // LoRa radio chip select
const int rst = 0;        // LoRa radio reset
const int dio0 = 5;          // change for your board; must be a hardware interrupt pin
String packet = "pacote :D";
void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Sender");
  LoRa.setPins(ss, rst, dio0);
  LoRa.begin(frequency);
  LoRa.setTxPower(17);
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(packet);

  // send packet
  LoRa.beginPacket();
  LoRa.print(packet);
  LoRa.endPacket();
  Serial.print("Packet sent sucessfull");
  counter++;

  delay(5000);
}
