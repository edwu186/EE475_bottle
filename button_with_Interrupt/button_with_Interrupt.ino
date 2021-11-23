#include <LowPower.h>

uint8_t button = 2;

// task to do when the lid is open
void wakeUp()
{
  digitalWrite(9, HIGH);
  Serial.println("Lid is open");
  // CODE for send pakages
  
  //delay(8000);
}

void setup()  
{
  pinMode(9, OUTPUT);
  Serial.begin(9600);
  pinMode(button, INPUT);
}

void loop() 
{
    attachInterrupt(digitalPinToInterrupt(button), wakeUp, LOW);
    digitalWrite(9, LOW);
    Serial.println("Lid is close");

    // go into sleep mode when the lid is closed (button pressed)
    // has bug that cause the board can't be recognized by PC, need to reset the arduino board
    
    //LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
    delay(4000);
}
