void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(3,OUTPUT);
  pinMode(A0,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int lightRead = (analogRead(A0));
  //int ledOutput = map(lightRead, 1, 1023, 0, 254);
  int ledOutput = lightRead;
  if (ledOutput < 1020) {
    ledOutput = 0;
  }

  Serial.println(ledOutput);
  analogWrite(3, ledOutput);
  delay(50);
}
