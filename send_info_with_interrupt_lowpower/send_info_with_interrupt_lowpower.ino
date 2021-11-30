#include <SPI.h>
#include <BTLE.h>
#include <LowPower.h>

#include "packets.h"

RF24 radio(9,10);

BTLE btle(&radio);

int timer = 0;
uint8_t sensorPin = 2;  // connect the DO pin to the Pin 2 of Arduino 


void lidOpen()
{
  btle.advertise((void *) OPEN_P, (uint8_t) PACKET_LEN);
  Serial.println("In the interrupt task now");
}

void setup() {

  Serial.begin(9600);
  pinMode(sensorPin, INPUT);
  btle.begin("PPal"); 

}


// NOTE: in order to open up serial moniter when connect to the arduino, put finger onto the photoresistor to avoid jumping to interrupt.
// also work without open up serial moniter
void loop() {
  Serial.print(" timer: ");
  Serial.print(timer); 
  Serial.println("In the loop now"); 

  // sensor LOW when light intensity higher than set threshold, indicate that lid is open
  attachInterrupt(digitalPinToInterrupt(sensorPin), lidOpen, LOW);
 
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

  if (timer < 3) {
    for (int i = 0; i < 60; i++) {
      btle.advertise((void *) REGISTER_P, (uint8_t) PACKET_LEN);
      Serial.print(i);
      delay(200);
    }
  } else {
    for (int j = 30; j < 120; j++) { // use 30 to 120 is for easy visulaize which branch currently running in serial monitor
    btle.advertise((void *) BEACON_P, (uint8_t) PACKET_LEN);
      Serial.print(j);
      delay(200);
    }
    
    // enter low power mode for 8S, use for loop here to extend it, the current design is basiclly run 18s, turn off 16s, run 18s, turn off...
    for (int k = 0; k < 2; k++) { 
      LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
    }
  }
  
  timer++;
  btle.hopChannel();
  
  //Serial.print(" Wait on SLEEP Mode to end");
  detachInterrupt(digitalPinToInterrupt(sensorPin));
}
