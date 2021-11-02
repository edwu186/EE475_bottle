void setup()  
{
  // set digital pin to control as an output
  pinMode(9, OUTPUT);
  // set the data rate for the SoftwareSerial port
  Serial1.begin(9600);
  Serial.begin(9600);
  // Send test message to other device
  Serial.println("Hello from Arduino");

  pinMode(A0,INPUT);
}
char a; // stores incoming character from other device
void loop() 
{
  //if (Serial1.available())
  // if text arrived in from BT serial...
  //{
    a=(Serial1.read());
    if (a=='1')
    {
      digitalWrite(9, HIGH);
      Serial1.println("LED on");
    }
    if (a=='2')
    {
      digitalWrite(9, LOW);
      Serial1.println("LED off");
    }
    if (a=='?')
    {
      Serial1.println("Send '1' to turn LED on");
      Serial1.println("Send '2' to turn LED off");
    }

    int lightRead = (analogRead(A0));
    //int new_lightRead = (analogRead(A0));
    //int ledOutput = map(lightRead, 1, 1023, 0, 254);
    int ledOutput = lightRead;
    //int new_ledOutput = new_lightRead;
    
//    if (ledOutput < 1010) {
    if (ledOutput < 1002) {
      //ledOutput = 0;
      digitalWrite(9, HIGH);
      //Serial1.write(ledOutput);
      Serial1.println("Lid is open");
    } else {
      digitalWrite(9, LOW);
      //Serial1.write(ledOutput);
      Serial1.println("Lid is close");
    } 
    Serial1.println(ledOutput);
    delay(1500);
  //}

//    // Photoresistor  
//    int lightRead = (analogRead(A0));
//    //int ledOutput = map(lightRead, 1, 1023, 0, 254);
//    int ledOutput = lightRead;
//    if (ledOutput < 1000) {
//      //ledOutput = 0;
//      digitalWrite(9, LOW);
//      Serial1.write(ledOutput);
//      Serial.println("Lid is open");
//    } else {
//      digitalWrite(9, HIGH);
//      Serial1.write(ledOutput);
//      Serial.println("Lid is close");
//    }
//    Serial.println(ledOutput);
//    delay(1500);
}
