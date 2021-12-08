#include <SPI.h>
#include <BTLE.h>
#include <LowPower.h>
#include <EEPROM.h>

#include "packets.h"

RF24 radio(9,10);

BTLE btle(&radio);

int timer = 0;
uint8_t sensorPin = 2;  // connect the DO pin to the Pin 2 of Arduino
volatile uint16_t seqNum = 0;

byte BOTTLE_ID[4] = {0x00, 0x00, 0x00, 0x00}; // TODO: store in EEPROM

volatile byte BEACON_P[9]    = {UUID[0], UUID[1], 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, BEACON_ID};
volatile byte REGISTER_P[9]  = {UUID[0], UUID[1], 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, REGISTER_ID};

void lidISR();
void updateSeqNum();
void getBottleId();
void setupPackets();

void setupPackets() {
  for (int i = 2; i < 6; i++) {
    BEACON_P[i] = BOTTLE_ID[i-2];
    REGISTER_P[i] = BOTTLE_ID[i-2];
  }
}

void setup() {
  Serial.begin(9600);
  getBottleId();
  setupPackets();
  pinMode(sensorPin, INPUT);
  btle.begin("PPal");
}


// NOTE: in order to open up serial moniter when connect to the arduino, put finger onto the photoresistor to avoid jumping to interrupt.
// also work without open up serial moniter
void loop() {
  // REGISTER_P
  // for timer < 3, we want send out REGISTER Packages
  // for timer increment by 1, the loop need to complete 1 cycle
  // the total time for 1 cycle = time of sending packages

  // time of sending packages = 12s : the for loop sends REGISTER Packages 60 times with delay 0.2s
  // for three cycle have total of 36s to send REGISTER Packages

  // BEACON_P
  // after timer reach 3, will always sending BEACON Packages
  // time of sending packages = 18s
  // time of low power mode = 16 : Using SLEEP_8S for two cycle
  // every 16s, sending packages for 18s

  // NOTE: the reason sending REGISTER or BEACON Packages mutiple time:
  //       during testing, I found out that the reason cellphone able to catch the OPEN_P so quickly and display PPal on the divice list is because
  //       the rate of sending OPEN_P is very fast.

  //       if there isn't a constant rate of package being send out, it's really hard for cellphone to recieve it, and this happens very frequently during testing.
  //       for this reason, by giving a time slot to send one package mutiple times solves the issue.

  //       we defintely could adjust these time range later, but for the current setting it performs very stable.

  if (timer >= 3) {
    // sensor LOW when light intensity higher than set threshold, indicate that lid is open
    // only enable this interrupt when sending beacon packets
    attachInterrupt(digitalPinToInterrupt(sensorPin), lidISR, RISING);
    for (int j = 30; j < 120; j++) { // use 30 to 120 is for easy visulaize which branch currently running in serial monitor
      btle.advertise((void *) BEACON_P, (uint8_t) PACKET_LEN);
      btle.hopChannel();
      Serial.print(j);
      delay(200);
    }

    // TODO: remove low power mode for demo
    // enter low power mode for 8S, use for loop here to extend it, the current design is basiclly run 18s, turn off 16s, run 18s, turn off...
    for (int k = 0; k < 2; k++) {
      Serial.println("would be sleeping for 8 seconds");
      delay(8000);
      //LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
    }
  } else {
    for (int i = 0; i < 60; i++) {
      btle.advertise((void *) REGISTER_P, (uint8_t) PACKET_LEN);
      btle.hopChannel();
      Serial.print(i);
      delay(200);
    }
    timer++;
  }

  btle.hopChannel();

  //Serial.print(" Wait on SLEEP Mode to end");
  detachInterrupt(digitalPinToInterrupt(sensorPin));
}


void getBottleId() {
  // bottle id has never been generated, create a new one
  if (EEPROM.read(0) == 0xFF) {
    randomSeed(analogRead(0));
    EEPROM.write(0, (byte) random(0, 254));
    EEPROM.write(1, (byte) random(0, 255));
    EEPROM.write(2, (byte) random(0, 255));
    EEPROM.write(3, (byte) random(0, 255));
  }

  // retrieve bottle id from eeprom
  for (int i = 0; i < 4; i++) {
    BOTTLE_ID[i] = EEPROM.read(i);
  }
}

void updateSeqNum() {
  seqNum++;
  byte msb = (byte) (seqNum >> 4);
  byte lsb = (byte) (seqNum & 0xFF);
  BEACON_P[6] = msb;
  BEACON_P[7] = lsb;
}

void lidISR() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (digitalRead(sensorPin) && interrupt_time - last_interrupt_time >= 200) {
      updateSeqNum();
  }
  last_interrupt_time = interrupt_time;
}
