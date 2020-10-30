#include <SPI.h>
#include <LoRa.h>

#define ss 15
#define rst 0
#define dio0 5
#define frequency 866E6

void setup() {
  Serial.begin(115200);
  while (!Serial);
  LoRa.setPins(ss, rst, dio0);
  Serial.println("LoRa Receiver");

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
    // read packet
    if (LoRa.available()) {
      Serial.print((char)LoRa.read());
          // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
    }


  
}
