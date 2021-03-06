/*
  LoRa Simple Arduino Server :
  Support Devices: 
  * LoRa Mini
  * LoRa Shield + Arduino;
  * LoRa GPS Shield + Arduino. 
  
  Example sketch showing how to create a simple messageing server, 
  with the RH_RF95 class. RH_RF95 class does not provide for addressing or
  reliability, so you should only use RH_RF95 if you do not need the higher
  level messaging abilities.

  It is designed to work with the other example LoRa Simple Client

  modified 16 11 2016
  by Edwin Chen <support@dragino.com>
  Dragino Technology Co., Limited
*/
#define BAUDRATE 115200
#include <RH_RF95.h>
#include <Console.h>
// Singleton instance of the radio driver
RH_RF95 rf95;

int led = A2;
float frequency = 915.0;

String name;
void setup() 
{
  pinMode(led, OUTPUT);     
  // Initialize Console and wait for port to open:
  Bridge.begin(BAUDRATE);
  Console.begin();
  while (!Console) ; // Wait for Console port to be available
  Console.println("Start Sketch");
  if (!rf95.init())
    Console.println("init failed");
  // Setup ISM frequency
  rf95.setFrequency(frequency);
  // Setup Power,dBm
  rf95.setTxPower(13);
  // Defaults BW Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
  Console.print("Listening on frequency: ");
  Console.println(frequency);
}

void loop()
{
  if (rf95.available())
  {
    // Should be a message for us now   
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf95.recv(buf, &len))
    {
      digitalWrite(led, HIGH);
      RH_RF95::printBuffer("request: ", buf, len);
      Console.print("got request: ");
      Console.println((char*)buf);
      Console.print("RSSI: ");
      Console.println(rf95.lastRssi(), DEC);
      
      // Send a reply
      uint8_t data[] = "Received";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent();
      Console.println("Sent a reply");
      digitalWrite(led, LOW);
    }
    else
    {
      Console.println("recv failed");
    }
  }
}


