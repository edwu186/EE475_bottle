#include <SPI.h>
#include <BTLE.h>
#include <LowPower.h>

#include "packets.h"

RF24 radio(9,10);

BTLE btle(&radio);

int timer;
bool closed;
uint8_t sensorPin = 2; 


void setup() {

  Serial.begin(9600);
  //while (!Serial) { }
  Serial.println("BTLE advertisement sender");
  
  timer = 0;
  closed = true;
  pinMode(sensorPin, INPUT);
  
  btle.begin("PPal");
}

void loop() {
  //Serial.print("Advertising Packet ");
  //Serial.print(i);
  //Serial.println(digitalRead(sensorPin));

  // sensor HIGH when light intensity lower than set threshold
  // sensor LOW when light intensity higher than set threshold
  closed = digitalRead(sensorPin);
  
  if (closed) {

    // first 10s sending REGISTER Package
    if (timer < 10) {
      bool succ = btle.advertise((void *) REGISTER_P, (uint8_t) PACKET_LEN);
    } else {
      bool succ = btle.advertise((void *) BEACON_P, (uint8_t) PACKET_LEN); 
    }
  } else { // lid is open
      bool succ = btle.advertise((void *) OPEN_P, (uint8_t) PACKET_LEN);
  }
  
  timer++;
  btle.hopChannel();

  // timer delay 1s
  delay(1000);
}
