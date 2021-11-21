#include <SPI.h>
#include <BTLE.h>

#include "packets.h"

RF24 radio(9,10);

BTLE btle(&radio);

int i;

void setup() {

  Serial.begin(9600);
  while (!Serial) { }
  Serial.println("BTLE advertisement sender");
  i = 0;
  btle.begin("PPal");

}

void loop() {
  Serial.print("Advertising Packet ");
  Serial.print(i);

  bool succ = btle.advertise((void *) REGISTER_P, (uint8_t) PACKET_LEN);
  Serial.println(succ);
  btle.hopChannel();
  delay(50);
  i++;

  // delay(100);
}
