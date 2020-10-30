
//http_client_v5 created by Fábio Formiga Gonçalves, LEET 3ºano.
//importa bibliotecas necessarias

#include "HX711.h"
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include "HX711.h"
#include <SPI.h>
#include <RH_RF95.h>


//-------------------configuração RF95---------------------------

#define CS 15
#define RST 16
#define DIO0 5
#define RF95_FREQ 868

//-------------------configuração DHT11---------------------------
#define DHTPIN 4          // D3
#define DHTTYPE DHT11     // DHT 11
DHT dht(DHTPIN, DHTTYPE);
//------------------configuração HX711 peso ADC--------------------------
HX711 scale(2, 14); //dout, sck
  //variables to HX711
float calibration_factor = 18.2; // this calibration factor is adjusted according to my load cell
long zero_factor =112210;
int units;

//-------------------configuração DHT11---------------------------
int humidade;
int temperatura;

//------------------------configuração colmeia--------------------
String id_hive = "2";
//----------------------define pinos rf95-------------------
RH_RF95 rf95(15, 5);

void setup(){

	delay(1000);
	Serial.begin(115200);
  // Wait for serial to initialize.
  while(!Serial) { }
//rfm 95 work
	pinMode(RST, OUTPUT);
	pinMode(DIO0, OUTPUT);
	digitalWrite(RST, HIGH);
	digitalWrite(DIO0, HIGH);
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
	rf95.sleep();

//----------- start dht----------------
	dht.begin();
// collect dht data
	humidade = 70;
	temperatura = 10;

	//-----------------deteção peso---------------------------
	// scale.power_up();
	// while(scale.is_ready() != true){
	// 	delay(5);
	// }
	// scale.set_scale(calibration_factor);
	// units = scale.get_units(10),100;
	// if(units < 0){
	// 	units=0;
	// }
	// scale.power_down();
	units = 16000;
	rf95.available();
	//------------------Envio do LoRa--------------------------
	String text = id_hive + " " + String(temperatura) + " " + String(humidade)+ " "  + String(units/1000);
	char  mensagem[10] = " ";
	text.toCharArray(mensagem,10); //coloca a string
	Serial.print("a enviar: "); Serial.print(mensagem);
	//envia para o rfm95
	rf95.send((uint8_t *)mensagem ,10);
	// aguarda o envio do rfm95
	Serial.print("\n a aguardar envio \n");
	rf95.waitPacketSent();
	rf95.sleep();
	Serial.println("envio efetuado"); //confirma o envio	//desliga modulo
	//ESP.deepSleep(10e6); // 10e6 is 10 microseconds
	// }
}

void loop(){

}
