
#include <AESLib.h>

AESLib aesLib;

void setup() {
    Serial.begin(115200);
  
  byte key[] = { 0x13, 0xAF, 0x55, 0x39, 0x41, 0x6C, 0x97, 0xC3, 0x51, 0xAC, 0xF4, 0xF9, 0xA9, 0x4F, 0x88, 0xBA }; //16
    
  byte my_iv[N_BLOCK] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  aesLib.gen_iv(my_iv);

  Serial.println("AES Key: ");
  int i;
  for (i = 0; i < sizeof(my_iv); i++)
  {
     Serial.print(my_iv[i], DEC);
     Serial.print(",");
  }

  Serial.println("");
  
  String msg = "1 20 30 16";
  int bits = msg.length();
  unsigned long ms = micros ();
  String encMsg = aesLib.encrypt(msg, key, bits, my_iv);
  Serial.println("encMsg: " + encMsg);  

  String  sc = aesLib.decrypt(encMsg, key, bits, my_iv);
  Serial.println(sc);
}

void loop() {
  // put your main code here, to run repeatedly:
}
