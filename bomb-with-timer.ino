#include <SevSeg.h>
#include <TimedAction2.h>

int red_led =12;
int green_led = 11;
int yellow_led=10;
int blue_led =9;

int timer = 0; //intialized at zero here, but set to real time when countdown starts

void displayTime() { //Refresh display, keep showing the time
  sevseg.setNumber(timer, 0);
  sevseg.refreshDisplay();
}

void timeDown() { //Decrement time variable, counting in seconds
  timer--;
  if (timer < 0) {
    //TO ADD: Play beep/tone to signal failure
    delay(300);//wait a moment, then...
    loop(); //reset loop()
    //THIS METHOD OF RESETTING IS DEEPLY FLAWED, THE PROBLEM WILL CRASH EVENTUALLY BY DOING THIS.
    //I believe it's a fine enough solution for us, as there isn't a lot of memory being used, and we can always manually reset if needed.
  }
}

TimedAction timeDisp = TimedAction(1, displayTime); //should run every millisecond
TimedAction timeIterate = TimedAction(1000, timeDown); //should run every second

int wireSltn[2] = {2, 1};
int myWire[2] = {2, 1};

void setup() {
  Serial.begin(9600);

  //Display setup
  byte numDigits = 4;
  byte digitPins[] = {10, 11, 12, 13}; // REASSIGN ALL OF THESE TO AVOID CONFLICTS WITH BELOW
  byte segmentPins[] = {9, 2, 3, 5, 6, 8, 7, 4};
  bool resistorsOnSegments = true;
  bool updateWithDelaysIn = true;
  byte hardwareConfig = COMMON_CATHODE;
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(90);

  //4 LEDs
  pinMode(red_led,OUTPUT);
  pinMode(green_led,OUTPUT);
  pinMode(yellow_led,OUTPUT);
  pinMode(blue_led,OUTPUT);

  //Start Switch
  pinMode(17, INPUT);

  //4 buttons
  pinMode(18, INPUT); //A - left
  pinMode(19, INPUT); //B - right
  pinMode(20, INPUT); //C
  pinMode(21, INPUT); //D

  //1 rotary switch (4 inputs)
  //also requires power and GND
  pinMode(22, INPUT);
  pinMode(23, INPUT);
  pinMode(24, INPUT);
  pinMode(25, INPUT);

  //switch wires (4 inputs)
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
}

void loop() {
  // Wait for user to hit start button/switch
  int startBomb = digitalRead(4);
  timeDisp.check(); //Refresh digit display every millisecond

  if (startBomb == HIGH) {
    timeDisp.check(); //Refresh digit display every millisecond
    Serial.println("Bomb activated...");
    int result = countdown();
    //On level 3 completion, the timer stops and flashes, displaying the completion time

    if (result == 0) {  //If time runs out:
      timeDisp.check(); //Refresh digit display every millisecond
      Serial.println("You lost!");
      digitalWrite(3, LOW);
    }

    else if (result == 1) {  //If all challenges complete:
      timeDisp.check(); //Refresh digit display every millisecond
      Serial.println("You won!");
      lcd.print("WIN");
      delay(1000);
      lcd.clear();
    }
  }
}

int countdown() {
  timeDisp.check(); //Refresh digit display every millisecond
  int timer = 300;
  timeIterate.check(); //start counting down time variable.  Will decrement every second.
  return puzzle_1(); //initiate puzzles...
}

int puzzle_1() {    //PUSH BUTTON SEQUENCE
  timeDisp.check(); //Refresh digit display every millisecond
  timeIterate.check(); //start counting down time variable.  Will decrement every second.

  int butChoice = random(1, 3); //pick a number between 1 and 3
  //Depending on which number is pulled, pick a puzzle with different solutions (but always tied to the same LED pattern)

  if (butChoice == 1) {
    int btnSltn[4] = {1, 2, 2, 1};
    int mySltn[4] = {1, 2, 2, 1};
      
    //Red-blue flash
    digitalWrite(red_led, HIGH);  //LED signal could be tied to a TimedAction to let it repeat every so often while user is working on it
    delay(100);
    digitalWrite(red_led, LOW);
    delay(100);
    digitalWrite(blue_led, HIGH);
    delay(100);
    digitalWrite(blue_led, LOW);
    delay(100);

    //Looking for sequence left - right - right - left
    for (int i = 0; i < 4; i = i + 1) {
      timeDisp.check(); //Refresh digit display every millisecond
      timeIterate.check(); //start counting down time variable.  Will decrement every second.

      Serial.print("i = ");  //Serial print current array index
      Serial.println(i);

      while (digitalRead(5) == LOW && digitalRead(6) == LOW) {
        timeDisp.check(); //Refresh digit display every millisecond
        timeIterate.check(); //start counting down time variable.  Will decrement every second.
        //Serial.println("Waiting for input...");
      }

      if (digitalRead(5) == HIGH) {
        timeDisp.check(); //Refresh digit display every millisecond
        timeIterate.check(); //start counting down time variable.  Will decrement every second.
        mySltn[i] = 1;
        delay(200);
        Serial.println("left press!");

        printInput();

        if (btnSltn1[i] != mySltn[i]) {
          timeDisp.check(); //Refresh digit display every millisecond
          timeIterate.check(); //start counting down time variable.  Will decrement every second.
          i = -1;
          Serial.println("WRONG");
        }
      }

      else if (digitalRead(6) == HIGH) {
        timeDisp.check(); //Refresh digit display every millisecond
        timeIterate.check(); //start counting down time variable.  Will decrement every second.
        mySltn[i] = 2;
        delay(200);
        Serial.println("right press!");

        printInput();

        if (btnSltn1[i] != mySltn[i]) {
          timeDisp.check(); //Refresh digit display every millisecond
          timeIterate.check(); //start counting down time variable.  Will decrement every second.
          i = -1;
          Serial.println("WRONG");
        }
      }
    }
  }

  else if (butChoice == 2) {
    int btnSltn[4] = {2, 1, 1, 2};
    int mySltn[4] = {2, 1, 1, 2};
      
    //Red-yellow flash  
    digitalWrite(red_led, HIGH);
    delay(100);
    digitalWrite(red_led, LOW);
    delay(100);
    digitalWrite(yellow_led, HIGH);
    delay(100);
    digitalWrite(yellow_led, LOW);
    delay(100);

    //Looking for sequence right - left - left - right
    for (int i = 0; i < 4; i = i + 1) { //This for loop is universal and should work for all sequences
      timeDisp.check(); //Refresh digit display every millisecond
      timeIterate.check(); //start counting down time variable.  Will decrement every second.

      Serial.print("i = ");  //Serial print current array index
      Serial.println(i);

      while (digitalRead(5) == LOW && digitalRead(6) == LOW) {
        timeDisp.check(); //Refresh digit display every millisecond
        timeIterate.check(); //start counting down time variable.  Will decrement every second.
        //Serial.println("Waiting for input...");
      }

      if (digitalRead(5) == HIGH) {
        timeDisp.check(); //Refresh digit display every millisecond
        timeIterate.check(); //start counting down time variable.  Will decrement every second.
        mySltn[i] = 1;
        delay(200);
        Serial.println("left press!");

        printInput();

        if (btnSltn1[i] != mySltn[i]) {
          timeDisp.check(); //Refresh digit display every millisecond
          timeIterate.check(); //start counting down time variable.  Will decrement every second.
          i = -1;
          Serial.println("WRONG");
        }
      }

      else if (digitalRead(6) == HIGH) {
        timeDisp.check(); //Refresh digit display every millisecond
        timeIterate.check(); //start counting down time variable.  Will decrement every second.
        mySltn[i] = 2;
        delay(200);
        Serial.println("right press!");

        printInput();

        if (btnSltn1[i] != mySltn[i]) {
          timeDisp.check(); //Refresh digit display every millisecond
          timeIterate.check(); //start counting down time variable.  Will decrement every second.
          i = -1;
          Serial.println("WRONG");
        }
      }
    }
  }

  else if (butChoice == 3) {
    int btnSltn[4] = {1, 2, 1, 2};
    int mySltn[4] = {1, 2, 1, 2};
      
    //Red-green flash  
    digitalWrite(red_led, HIGH);
    delay(100);
    digitalWrite(red_led, LOW);
    delay(100);
    digitalWrite(green_led, HIGH);
    delay(100);
    digitalWrite(green_led, LOW);
    delay(100);

    //Looking for sequence left - right - left - right
    for (int i = 0; i < 4; i = i + 1) {
      timeDisp.check(); //Refresh digit display every millisecond
      timeIterate.check(); //start counting down time variable.  Will decrement every second.

      Serial.print("i = ");  //Serial print current array index
      Serial.println(i);

      while (digitalRead(5) == LOW && digitalRead(6) == LOW) {
        timeDisp.check(); //Refresh digit display every millisecond
        timeIterate.check(); //start counting down time variable.  Will decrement every second.
        //Serial.println("Waiting for input...");
      }

      if (digitalRead(5) == HIGH) {
        timeDisp.check(); //Refresh digit display every millisecond
        timeIterate.check(); //start counting down time variable.  Will decrement every second.
        mySltn[i] = 1;
        delay(200);
        Serial.println("left press!");

        printInput();

        if (btnSltn1[i] != mySltn[i]) {
          timeDisp.check(); //Refresh digit display every millisecond
          timeIterate.check(); //start counting down time variable.  Will decrement every second.
          i = -1;
          Serial.println("WRONG");
        }
      }

      else if (digitalRead(6) == HIGH) {
        timeDisp.check(); //Refresh digit display every millisecond
        timeIterate.check(); //start counting down time variable.  Will decrement every second.
        mySltn[i] = 2;
        delay(200);
        Serial.println("right press!");

        printInput();

        if (btnSltn1[i] != mySltn[i]) {
          timeDisp.check(); //Refresh digit display every millisecond
          timeIterate.check(); //start counting down time variable.  Will decrement every second.
          i = -1;
          Serial.println("WRONG");
        }
      }
    }
  }

  Serial.println("Hey, you did it!");

  return puzzle_2_1(); //if win, go another level down
}

int puzzle_2() {
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
  timeDisp.check(); //Refresh digit display every millisecond
  timeIterate.check(); //start counting down time variable.  Will decrement every second.
  Serial.println("WELCOME TO PUZZLE 3");
  while (digitalRead(3) == LOW) {
    timeDisp.check(); //Refresh digit display every millisecond
    timeIterate.check(); //counting down time variable.  Will decrement every second.
  }
  while (digitalRead(3) == HIGH) {
    timeDisp.check(); //Refresh digit display every millisecond
    timeIterate.check(); //start counting down time variable.  Will decrement every second.
    while (digitalRead(A4) == HIGH) {
      timeDisp.check(); //Refresh digit display every millisecond
      timeIterate.check(); //start counting down time variable.  Will decrement every second.
    }
    while (digitalRead(A4) == LOW) {
      timeDisp.check(); //Refresh digit display every millisecond
      timeIterate.check(); //start counting down time variable.  Will decrement every second.
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
