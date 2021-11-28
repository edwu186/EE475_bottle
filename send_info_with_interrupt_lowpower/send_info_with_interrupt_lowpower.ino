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
  // the total time for 1 cycle = time of sending packages + time of low power mode
  
  // time of sending packages = 6 : the for loop sends REGISTER Packages 30 times with delay 0.2s
  // time of low power mode = 8 : Using SLEEP_8S
  // this give us 14 second for 1 cycle, mutiply by 3, give us total of 42s to send REGISTER Packages

  // BEACON_P
  // after timer reach 3, will always sending BEACON Packages
  // the time range is same as REGISTER_P

  // NOTE: the reason sending REGISTER or BEACON Packages mutiple time:
  //       during testing, I found out that the reason cellphone able to catch the OPEN_P so quickly and display PPal on the divice list is because
  //       the rate of sending OPEN_P is very fast.
  
  //       if there isn't a constant rate of package being send out, it's really hard for cellphone to recieve it, and this happens very frequently during testing.
  //       for this reason, by giving a time slot to send one package mutiple times solves the issue.
  
  //       we defintely could adjust these time range later, but for the current setting it performs very stable.

  if (timer < 3) {

    for (int i = 0; i < 30; i++) {
      btle.advertise((void *) REGISTER_P, (uint8_t) PACKET_LEN);
      Serial.print(i);
      delay(200);
    }
  } else {
    for (int j = 30; j < 60; j++) { // use 30 to 60 is for easy visulaize which branch currently running in serial monitor
    btle.advertise((void *) BEACON_P, (uint8_t) PACKET_LEN);
      Serial.print(j);
      delay(200);
    }
  }
  
  timer++;
  btle.hopChannel();
  //delay(100);
  
  Serial.print(" Wait on SLEEP Mode to end");
  // enter low power mode for 8S, we could use for loop here to extend it, the current design is basiclly run 6s, turn off 8s, run 6s, turn off...
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);

  detachInterrupt(digitalPinToInterrupt(sensorPin));
}
