#include <SPI.h>
#include <BTLE.h>

#include "packets.h"

RF24 radio(9,10);

BTLE btle(&radio);

bool i;

void setup() {

  Serial.begin(9600);
  while (!Serial) { }
  Serial.println("BTLE advertisement sender");
  i = true;
  btle.begin("PPal");

}

void loop() {
  // Serial.print("Advertising Packet ");
  // Serial.print(i);
  bool succ = btle.advertise((void *) REGISTER_P, (uint8_t) PACKET_LEN);
  // bool succ = btle.advertise((void *) BEACON_P, (uint8_t) PACKET_LEN);
  // bool succ = btle.advertise((void *) CLOSE_P, (uint8_t) PACKET_LEN);
  // bool succ = btle.advertise((void *) OPEN_P, (uint8_t) PACKET_LEN);
  // if (i) {
  //   bool succ = btle.advertise((void *) REGISTER_P, (uint8_t) PACKET_LEN);
  //   Serial.print("Register Packet  ");
  //   Serial.println(succ);
  //   i = false;
  // } else {
  //   bool succ = btle.advertise((void *) BEACON_P, (uint8_t) PACKET_LEN);
  //   Serial.print("Beacon Packet  ");
  //   Serial.println(succ);
  //   i = true;
  // }
  btle.hopChannel();
  delay(50);

  // delay(100);
}
