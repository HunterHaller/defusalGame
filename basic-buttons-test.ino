void setup() {
  // put your setup code here, to run once:
  pinMode(52, INPUT);
  pinMode(46, INPUT);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Green: "); //NOT SHOWING UP
  Serial.println(digitalRead(52));
  Serial.print("Blue: "); //SHORTING
  Serial.println(digitalRead(46));
}
