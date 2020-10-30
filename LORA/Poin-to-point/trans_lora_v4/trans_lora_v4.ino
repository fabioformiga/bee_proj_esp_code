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

RH_RF95 rf95(15, 5);
	int numero;

void setup(){
	Serial.begin(9600);
	pinMode(RST, OUTPUT);
	pinMode(DIO0, OUTPUT);
	delay(100);
	digitalWrite(DIO0, HIGH);
	digitalWrite(RST, HIGH);
	delay(100);
	Serial.println();
	Serial.println("Gateway Module starting Transmission…");
	//rf95.setModemConfig(RH_RF95::Bw125Cr48Sf4096); 
	RH_RF95::ModemConfig modemconfig = {
		0x00, //BW_125KHZ , CODING_RATE_4_5 
		0xc0,// SPREADING_FACTOR_4096CPS
		0x01 //RH_RF95_LOW_DATA_RATE_OPTIMIZE
	};
	rf95.setTxPower(23);
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
	char  mensagem[31] = " ";
	itoa(numero, mensagem, 5 );
	Serial.print("a enviar: "); Serial.print(mensagem);
	//envia para o rfm95
	rf95.send((uint8_t *)mensagem ,5); 
	// aguarda o envio do rfm95
	Serial.print("\n a aguardar envio \n");
	//rf95.waitPacketSent();
	Serial.println("envio efetuado"); //confirma o envio
	numero++;
	// while(Serial.available() ){
	// Serial.println("ESP deep Sleep");
	// ESP.deepSleep(30e6);
	// }
	delay(1000);

}
