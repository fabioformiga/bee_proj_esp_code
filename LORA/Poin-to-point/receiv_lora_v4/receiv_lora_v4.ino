//transmissão lora utilizando RFM95 e esp8266
///                 esp8266      RFM95/96/97/98
///                 GND----------GND   (ground in)
///                 3V3----------3.3V  (3.3V in)
/// interrupt 0 pin D1-----------DIO0  (interrupt request out)
///	interrupt 1 pin D4-----------DIO1  (interrupt request out)
///          SS pin D8-----------NSS   (CS chip select in)
///         SCK pin D5-----------SCK   (SPI clock in)
///        MOSI pin D7-----------MOSI  (SPI Data in)
///        MISO pin D12----------MISO  (SPI Data out)
///        RESET pin D0----------RST  (RESET PIN)

#include <SPI.h>
#include <RH_RF95.h>


#define CS 15
#define RST 16
#define DIO0 5
#define RF95_FREQ 868

RH_RF95 rf95(CS, DIO0);

void setup(){
	Serial.begin(9600);
	pinMode(RST, OUTPUT);
	pinMode(DIO0, OUTPUT);
	digitalWrite(DIO0, HIGH);
	digitalWrite(RST, HIGH);
	delay(100);
	Serial.println();
	Serial.println("Gateway Module starting Reception…");
	while (!rf95.init()) { //detects if LoRa radio fail
		Serial.println("LoRa radio init failed");
	}
	Serial.println("LoRa radio init OK!");
	if (!rf95.setFrequency(RF95_FREQ)) { //detects if LoRa set frequency fails
		Serial.println("setFrequency failed");
	}
	Serial.print("Set Freq to: ");
	Serial.println(RF95_FREQ);

}
void loop(){
	char* recebido= "";
	if (rf95.available()){ 
		uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
		uint8_t len = sizeof(buf);
		if (rf95.recv(buf, &len)){
			RH_RF95::printBuffer("Received: ", buf, len); // mostra no serial o RFM 
			Serial.print("Got: ");
			Serial.println((char*)buf);
			recebido = (char*)buf;
			Serial.print("RSSI: ");
			Serial.println(rf95.lastRssi(), DEC);
			// Serial.print("ESP deep Sleep");
			// ESP.deepSleep(20e6);
			delay(1000);
		}
	}
	else{
	}
}
