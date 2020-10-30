#include <SPI.h>
#include <RH_RF95.h>

#define RFM95_CS 15
#define RFM95_RST 16
#define RFM95_INT 5

#define RF95_FREQ 868.0

#define LED 2

RH_RF95 rf95(RFM95_CS, RFM95_INT);


void setup() 
{
  rfm95_work();

}

void loop()
{
  char* recebido= "";
  if (rf95.available())
  {
    // Should be a message for us now   
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    
    if (rf95.recv(buf, &len))
    {
      digitalWrite(LED, HIGH);
      RH_RF95::printBuffer("Received: ", buf, len); // mostra no serial o RFM 
      Serial.print("Got: ");
      Serial.println((char*)buf);
      recebido = (char*)buf;
       Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);
        char  mensagem[31] = "Recebido";
  rf95.send((uint8_t *)mensagem ,20); 
  // aguarda o envio do rfm95
  Serial.print("\n a aguardar envio \n");
  rf95.waitPacketSent();
  Serial.println("envio efetuado de " + String(mensagem)); //confirma o envio
    }
    else
    {
      Serial.println("Receive failed");
    }
  }

  
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
