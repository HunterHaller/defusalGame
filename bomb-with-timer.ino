int red_led = 12;
int green_led = 11;
int yellow_led = 10;
int blue_led = 9;

int input1 = 7;
int input2 = 6;

int btnSltn1[4] = {1, 2, 2, 1};
int mySltn[4] = {1, 2, 2, 1};

int wireSltn[2] = {2, 1};
int myWire[2] = {2, 1};

void setup() {
  Serial.begin(9600);

  pinMode(red_led, OUTPUT);
  pinMode(green_led, OUTPUT);
  pinMode(yellow_led, OUTPUT);
  pinMode(blue_led, OUTPUT);

  pinMode(A0, OUTPUT);

  //4 LEDs
  pinMode(A5, OUTPUT); //blue
  pinMode(A6, OUTPUT); //green

  pinMode(input1, INPUT);
  pinMode(input2, INPUT);

  //Rotary
  pinMode(2, INPUT);
  pinMode(3, INPUT);

  //Start Switch
  pinMode(A4, INPUT);

  //4 buttons
  pinMode(A8, INPUT); //A - left
  pinMode(A15, INPUT); //B - right

  //1 rotary switch (4 inputs)
  //also requires power and GND

  //switch wires (4 inputs)
}

void loop() {
  /* Wait for user to hit start button/switch*/
  int startBomb = digitalRead(A4);

  if (startBomb == HIGH) {
    Serial.println("Bomb activated...");
    int result = puzzle_1_1();
    //On level 3 completion, the timer stops and flashes, displaying the completion time

    if (result == 0) {  //If time runs out:
      Serial.println("You lost!");
      digitalWrite(A5, LOW);
      delay(1000);
    }

    else if (result == 1) {  //If all challenges complete:
      Serial.println("You won!");
      delay(1000);
    }
  }
}

int puzzle_1_1() { //PUSH BUTTON SEQUENCE
  digitalWrite(red_led, HIGH);
  delay(100);
  digitalWrite(red_led, LOW);
  delay(100);
  digitalWrite(blue_led, HIGH);
  delay(100);
  digitalWrite(blue_led, LOW);
  delay(100);
  digitalWrite(red_led, HIGH);
  delay(100);
  digitalWrite(red_led, LOW);
  delay(100);
  digitalWrite(blue_led, HIGH);
  delay(100);
  digitalWrite(blue_led, LOW);
  delay(1000);

  //Looking for sequence left - right - right - left
  for (int i = 0; i < 4; i = i + 1) {
    Serial.print("i = ");  //Serial print current array index
    Serial.println(i);

    while (digitalRead(A8) == LOW && digitalRead(A15) == LOW) {
      //Serial.println("Waiting for input...");
    }

    if (digitalRead(A8) == HIGH) {
      mySltn[i] = 1;
      delay(200);
      Serial.println("left press!");

      if (btnSltn1[i] != mySltn[i]) {
        i = -1;
        Serial.println("WRONG");
      }
    }

    else if (digitalRead(A15) == HIGH) {
      mySltn[i] = 2;
      delay(200);
      Serial.println("right press!");

      if (btnSltn1[i] != mySltn[i]) {
        i = -1;
        Serial.println("WRONG");
      }
    }
  }
  Serial.println("Hey, you did it!");
  digitalWrite(A5, LOW);
  return puzzle_2_1(); //if win
  return 0;
}

int puzzle_2_1() {
  Serial.println("WELCOME TO PUZZLE 2");

  digitalWrite(green_led, HIGH);
  delay(100);
  digitalWrite(green_led, LOW);
  delay(100);
  digitalWrite(yellow_led, HIGH);
  delay(100);
  digitalWrite(yellow_led, LOW);
  delay(100);

  while (digitalRead(6) == LOW) {
    //Serial.println("Waiting for input...");
  }

  if (digitalRead(6) == HIGH) {
    Serial.println("farther wire!");
    while (digitalRead(7) == LOW) {}
    if (digitalRead(7) == HIGH) {
      Serial.println("You got the second one!");
      return finish(); //if win
    }
  }
}

int finish() {
  //Wait for rotary switch to be on the right segment, then user closes main (or pushes big button?) to complete the sequence
  Serial.println("WELCOME TO PUZZLE 3");
  while (digitalRead(3) == LOW) {}
  while (digitalRead(3) == HIGH) {
    while (digitalRead(A4) == HIGH) {}
    while (digitalRead(A4) == LOW) {
      return 1;
    }
  }
}

void printInput() {
  for (int j = 0; j < 4; j = j + 1) {
    Serial.print(mySltn[j]);
  }
  Serial.println();
}
