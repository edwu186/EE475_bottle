#include <SPI.h>
#include <BTLE.h>
#include <LowPower.h>

#include "packets.h"

RF24 radio(9,10);

BTLE btle(&radio);

int timer = 0;
uint8_t sensorPin = 2;  // connect the DO pin to the Pin 2 of Arduino
volatile uint16_t seqNum = 0;

byte BOTTLE_ID[4] = {0x00, 0x00, 0x00, 0x00}; // TODO: store in EEPROM

volatile byte BEACON_P[9]    = {UUID[0], UUID[1], BOTTLE_ID[0], BOTTLE_ID[1], BOTTLE_ID[2], BOTTLE_ID[3], 0x0, 0x0, BEACON_ID};
volatile byte REGISTER_P[9]  = {UUID[0], UUID[1], BOTTLE_ID[0], BOTTLE_ID[1], BOTTLE_ID[2], BOTTLE_ID[3], 0x0, 0x0, REGISTER_ID};
volatile byte OPEN_P[9]      = {UUID[0], UUID[1], BOTTLE_ID[0], BOTTLE_ID[1], BOTTLE_ID[2], BOTTLE_ID[3], 0x0, 0x0, OPEN_ID};

void updateSeqNum() {
  seqNum++;
  byte msb = (byte) (seqNum >> 4);
  byte lsb = (byte) (seqNum & 0xFF);
  BEACON_P[6] = msb;
  BEACON_P[7] = lsb;
  OPEN_P[6] = msb;
  OPEN_P[7] = lsb;
}

void lidISR()
{
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (!digitalRead(sensorPin) && interrupt_time - last_interrupt_time >= 200) {
      // broadcast every ~45 ms for 2 seconds
      Serial.println("In interrupt - lid open");
      for (int i = 0; i < 40; i++) {
        btle.advertise((void *) OPEN_P, (uint8_t) PACKET_LEN);
        btle.hopChannel();
        delayMicroseconds(15000);
        delayMicroseconds(15000);
        delayMicroseconds(15000);
      }
      Serial.println("completed all broadcasting");
      updateSeqNum();
      Serial.println("updated sequence number");
  }
  last_interrupt_time = interrupt_time;
}

void setup() {
  Serial.begin(9600);
  pinMode(sensorPin, INPUT);
  btle.begin("PPal");
}


// NOTE: in order to open up serial moniter when connect to the arduino, put finger onto the photoresistor to avoid jumping to interrupt.
// also work without open up serial moniter
void loop() {
  // sensor LOW when light intensity higher than set threshold, indicate that lid is open
  attachInterrupt(digitalPinToInterrupt(sensorPin), lidISR, FALLING);

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
    for (int j = 30; j < 120; j++) { // use 30 to 120 is for easy visulaize which branch currently running in serial monitor
      btle.advertise((void *) BEACON_P, (uint8_t) PACKET_LEN);
      btle.hopChannel();
      Serial.print(j);
      delay(200);
    }

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
