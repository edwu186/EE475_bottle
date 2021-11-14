#include <SPI.h>
#include <RF24.h>
#include <BTLE.h>

RF24 radio(9,10);

BTLE btle(&radio);

void setup() {

  Serial.begin(9600);
  while (!Serial) { }
  Serial.println("BTLE advertisement sender");

  btle.begin("foobar");
}

void loop() {
  int photoRead = analogRead(A0);
  if (photoRead < xxx) {
    output = x;
  } else {
    output = x;
  }

  nrf_service_data buf;
  buf.value = BTLE::to_nRF_Float(output);
  btle.advertise(0x16, &buf, sizeof(buf));
  
  btle.hopChannel();
  delay(2000);
}
