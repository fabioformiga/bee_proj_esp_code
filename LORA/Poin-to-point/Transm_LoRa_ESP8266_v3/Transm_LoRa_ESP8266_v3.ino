#include <SPI.h>
#include <RH_RF95.h>


#define RFM95_CS 15
#define RFM95_RST 16
#define RFM95_INT 5
#define DIO0 5
#define DIO1 2


#define RF95_FREQ 868

#define LED 2


RH_RF95 rf95(RFM95_CS, RFM95_INT);

// Singleton instance of the radio driver
//RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup()
{
  rfm95_work();
 
}

int numero = 0;

void loop(){
  int timer=0;
  char  mensagem[31] = " ";
  itoa(numero, mensagem, 10 );
  Serial.print("a enviar: "); Serial.print(mensagem);
 //envia para o rfm95
  rf95.send((uint8_t *)mensagem ,20); 
  // aguarda o envio do rfm95
  Serial.print("\n a aguardar envio \n");
  //rf95.waitPacketSent();
  Serial.println("envio efetuado"); //confirma o envio
  numero++;
  Serial.print("rf95 sleep");
  // rf95.sleep();
  // delay(60000);
 delay(1000);
}




void rfm95_work(){
  pinMode(DIO0, OUTPUT);
  pinMode(DIO1, OUTPUT);
  digitalWrite(DIO0,LOW);   
  digitalWrite(DIO1,LOW);   

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

  }

  Serial.println("LoRa radio init OK!");

  if (!rf95.setFrequency(RF95_FREQ)) { //detects if LoRa set frequency fails

    Serial.println("setFrequency failed");

  }

  Serial.print("Set Freq to: ");

  Serial.println(RF95_FREQ);

     // The default transmitter power is 13dBm, using PA_BOOST.
    // If you are using RFM95 / 96/97/98 modules using the transmitter pin PA_BOOST, then
    // you can set transmission powers from 5 to 23 dBm:

    rf95.setTxPower(23, false);  //define the power of transmition

}
