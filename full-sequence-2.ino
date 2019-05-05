//This version is mostly functional, but was a bit buggy as I recall.  Only has one built in sequence.

#include <TimedAction2.h>
#include <SevSeg.h>

//Requires SevSeg library from here: http://www.circuitbasics.com/wp-content/uploads/2017/05/SevSeg.zip
//and TimedAction library from here: https://playground.arduino.cc/uploads/Code/TimedAction-1_6/index.zip

//#include <TimedAction2.h>
SevSeg sevseg;

int red_led = 28;
int green_led = 26;
int yellow_led = 32;
int blue_led = 30;

int start_switch = 29;

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
  pinMode(red_led, OUTPUT);
  pinMode(green_led, OUTPUT);
  pinMode(yellow_led, OUTPUT);
  pinMode(blue_led, OUTPUT);

  //Start Switch
  pinMode(start_switch, INPUT);

  //4 buttons
  //Note that the current button puzzles are written as if there are only two buttons for testing purposes
  pinMode(A12, INPUT); //A
  pinMode(A13, INPUT); //B
  pinMode(A14, INPUT); //C
  pinMode(A15, INPUT); //D

  //1 rotary switch (3 inputs)
  //also requires power and GND
  pinMode(27, INPUT);
  pinMode(25, INPUT);
  pinMode(23, INPUT);

  //switch wires (4 inputs)
  pinMode(A8, INPUT);
  pinMode(A9, INPUT);
  pinMode(A10, INPUT);
  pinMode(A11, INPUT);
}

int timer = 120; //intialized at ten here, but set to real time when countdown starts

void displayTime() { //Refresh display, keep showing the time
  sevseg.setNumber(timer, 0);
  sevseg.refreshDisplay();
}

void timeDown() { //Decrement time variable, counting in seconds
  timer--;
  Serial.println(timer);
  if (timer < 0) {
    Serial.println("You ran out of time!");
    tone(52, 300, 500); //out pin 46, random freq, for 300 ms
    while (digitalRead(29) == HIGH) {
      //Do nothing, wait for switch to be reset
    }
    loop(); //reset loop()
    //THIS METHOD OF RESETTING IS DEEPLY FLAWED, THE PROBLEM WILL CRASH EVENTUALLY BY DOING THIS.
    //I believe it's a fine enough solution for us, as there isn't a lot of memory being used, and we can always manually reset if needed.
  }
}

TimedAction timeDisp = TimedAction(1, displayTime); //should run every millisecond
TimedAction timeIterate = TimedAction(1000, timeDown); //should run every second

int rotary_finish() {
  timeDisp.check(); //Refresh digit display every millisecond
  timeIterate.check(); //start counting down time variable.  Will decrement every second.

  int solved = 0;

  Serial.println("WELCOME TO THE FINISHER: THE ONE ABOUT THE ROTARY SWITCH AND THE BUTTON");

  while (solved == 0) {
    while (digitalRead(23) == LOW) {
      timeDisp.check(); //Refresh digit display every millisecond
      timeIterate.check(); //counting down time variable.  Will decrement every second.
    }

    while (digitalRead(23) == HIGH) {
      timeDisp.check(); //Refresh digit display every millisecond
      timeIterate.check(); //start counting down time variable.  Will decrement every second.

      //The correct pin has been turned too, now just wait for main switch to be flipped off
      
      //WEIRD NOTE: For some reason, the big white button reads HIGH when not pressed and LOW when pressed, so I programmed the following two if statements around that fact.
      //Also the button has a habit of being crazy sensitive and firingn without being pressed intentionally.  Be very careful with it!
      if (digitalRead(31) == HIGH) { //BIG BUTTON
        timeDisp.check(); //Refresh digit display every millisecond
        timeIterate.check(); //start counting down time variable.  Will decrement every second.
      }
      else if (digitalRead(31) == LOW) {
        timeDisp.check(); //Refresh digit display every millisecond
        timeIterate.check(); //start counting down time variable.  Will decrement every second.
        Serial.println("YOU TURNED THE THING AND HIT THE BUTTON, YAY!");
        solved = 1;
        return 1;
      }
    }
  }
}


int puzzle_3_1() {
  timeDisp.check(); //Refresh digit display every millisecond
  timeIterate.check(); //counting down time variable.  Will decrement every second.

  //Assuming all wires are plugged in at this point...
  Serial.println("WELCOME TO PUZZLE 3: THE ONE WHERE YOU UNDO THE WIRES");
  int solved = 0;
  delay(1000);
  while (solved == 0) {
    timeDisp.check(); //Refresh digit display every millisecond
    timeIterate.check(); //counting down time variable.  Will decrement every second.

    while (digitalRead(A8) == HIGH && digitalRead(A9) == HIGH && digitalRead(A10) == HIGH && digitalRead(A11) == HIGH) {
      timeDisp.check(); //Refresh digit display every millisecond
      timeIterate.check(); //counting down time variable.  Will decrement every second.
    }

    while (digitalRead(A9) == LOW || digitalRead(A10) == LOW || digitalRead(A11) == LOW) {
      timeDisp.check(); //Refresh digit display every millisecond
      timeIterate.check(); //counting down time variable.  Will decrement every second. //starting high on any other wire yields nothing
    }

    while (digitalRead(A8) == LOW && solved == 0) {
      timeDisp.check(); //Refresh digit display every millisecond
      timeIterate.check(); //start counting down time variable.  Will decrement every second.
      Serial.println("Purple, eh? You're on the right track.");
      while (digitalRead(A9) == HIGH && solved == 0) {
        timeDisp.check(); //Refresh digit display every millisecond
        timeIterate.check(); //start counting down time variable.  Will decrement every second.
      }
      while (digitalRead(A9) == LOW && solved == 0) {
        timeDisp.check(); //Refresh digit display every millisecond
        timeIterate.check(); //start counting down time variable.  Will decrement every second.
        Serial.println("Grey too?! That's it!?");
        while (digitalRead(A11) == HIGH && solved == 0) {
          timeDisp.check(); //Refresh digit display every millisecond
          timeIterate.check(); //start counting down time variable.  Will decrement every second.
          while (digitalRead(A11) == LOW && solved == 0) {
            timeDisp.check(); //Refresh digit display every millisecond
            timeIterate.check(); //start counting down time variable.  Will decrement every second.
            Serial.println("On to Orange, very nice!");
            while (digitalRead(A11) == HIGH && solved == 0) {
              timeDisp.check(); //Refresh digit display every millisecond
              timeIterate.check(); //start counting down time variable.  Will decrement every second.
            }
            if (digitalRead(A11) == LOW && solved == 0) {
              Serial.println("Green too! You beat the wires!");
              solved = 1;
            }
          }
        }
      }//THIS ONE DEALS WITH WIRING
      return rotary_finish(); //if win, go another level down
    }
  } //This one is about unwiring
}

int puzzle_2_1() {
  Serial.println("WELCOME TO PUZZLE 2: THE ONE ABOUT BUTTONS");

  timeDisp.check(); //Refresh digit display every millisecond
  timeIterate.check(); //start counting down time variable.  Will decrement every second.

  Serial.println("Sequence 1!");
  delay(1000);

  int solved = 0;
  int val = 0;
  int pushCounter = 0;

  while (solved == 0) {
    timeDisp.check(); //Refresh digit display every millisecond
    timeIterate.check(); //start counting down time variable.  Will decrement every second.
    if (digitalRead(A15) == HIGH && val == 0) { //first press = blue button
      timeDisp.check(); //Refresh digit display every millisecond
      timeIterate.check(); //start counting down time variable.  Will decrement every second.
      Serial.println("Green pressed, good job!");
      pushCounter++;
      val = 1;
      delay(300);
    }
    else if (digitalRead(A13) == HIGH && val == 1) { //Second press = white button
      timeDisp.check(); //Refresh digit display every millisecond
      timeIterate.check(); //start counting down time variable.  Will decrement every second.
      Serial.println("Red pressed, nice work!");
      val = 2;
      pushCounter++;
      delay(100);
    }
    else if (digitalRead(A12) == HIGH && val == 2) { //third press = red button
      timeDisp.check(); //Refresh digit display every millisecond
      timeIterate.check(); //start counting down time variable.  Will decrement every second.
      Serial.println("Blue!");
      pushCounter++;
      val = 3;
      delay(300); //this one waits longer than usual to really force two intentional presses
    }
    else if (digitalRead(A12) == HIGH && val == 3) { //fourth press = green button
      timeDisp.check(); //Refresh digit display every millisecond
      timeIterate.check(); //start counting down time variable.  Will decrement every second.
      Serial.println("Blue!");
      pushCounter++;
      val = 4;
      delay(100);
    }

    if (val == 4 && pushCounter == 4) {
      timeDisp.check(); //Refresh digit display every millisecond
      timeIterate.check(); //start counting down time variable.  Will decrement every second.
      solved = 1;
      Serial.println("You did it!");
    }
    else if (val < 4 && pushCounter == 4) {
      timeDisp.check(); //Refresh digit display every millisecond
      timeIterate.check(); //start counting down time variable.  Will decrement every second.
      Serial.println("Something went wrong!  Try again!");
      pushCounter = 0;
      val = 0;
    }
  } //THIS ONE DEALS WITH BUTTONS
  Serial.println("You beat the buttons!");
  return puzzle_3_1(); //if win
}

int puzzle_1_1() {    //WIRING var. 1
  timeDisp.check(); //Refresh digit display every millisecond
  timeIterate.check(); //start counting down time variable.  Will decrement every second.
  Serial.println("WELCOME TO PUZZLE 1: THE ONE ABOUT WIRES");

  int solved = 0;
  delay(1000);
  //Red-blue flash
  Serial.println("Sequence 1!!!!");
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
  delay(100);
  while (solved == 0) {
    timeDisp.check(); //Refresh digit display every millisecond
    timeIterate.check(); //start counting down time variable.  Will decrement every second.

    while (digitalRead(A8) == LOW && digitalRead(A9) == LOW && digitalRead(A10) == LOW && digitalRead(A11) == LOW) {
      timeDisp.check(); //Refresh digit display every millisecond
      timeIterate.check(); //start counting down time variable.  Will decrement every second.
    }

    while (digitalRead(A8) == HIGH || digitalRead(A10) == HIGH || digitalRead(A11) == HIGH) {
      timeDisp.check(); //Refresh digit display every millisecond
      timeIterate.check(); //start counting down time variable.  Will decrement every second.} //starting high on any other wire yields nothing
    }

    while (digitalRead(A9) == HIGH && solved == 0) {
      timeDisp.check(); //Refresh digit display every millisecond
      timeIterate.check(); //start counting down time variable.  Will decrement every second.
      Serial.println("Grey, eh? You're on the right track.");
      while (digitalRead(A10) == LOW && solved == 0) {
        timeDisp.check(); //Refresh digit display every millisecond
        timeIterate.check(); //start counting down time variable.  Will decrement every second.
      }
      while (digitalRead(A10) == HIGH && solved == 0) {
        timeDisp.check(); //Refresh digit display every millisecond
        timeIterate.check(); //start counting down time variable.  Will decrement every second.
        Serial.print("Green + grey?! That's it!?");
        while (digitalRead(A8) == LOW && solved == 0) {
          timeDisp.check(); //Refresh digit display every millisecond
          timeIterate.check(); //start counting down time variable.  Will decrement every second.
        }
        while (digitalRead(A8) == HIGH && solved == 0) {
          timeDisp.check(); //Refresh digit display every millisecond
          timeIterate.check(); //start counting down time variable.  Will decrement every second.
          Serial.println("On to Purple, very nice!");
          while (digitalRead(A11) == LOW && solved == 0) {
            timeDisp.check(); //Refresh digit display every millisecond
            timeIterate.check(); //start counting down time variable.  Will decrement every second.
          }
          while (digitalRead(A11) == HIGH && solved == 0) {
            timeDisp.check(); //Refresh digit display every millisecond
            timeIterate.check(); //start counting down time variable.  Will decrement every second.
            Serial.println("Orange too! You beat the wires!");
            solved = 1;
          }
        }
      }
    }//THIS ONE DEALS WITH WIRING
    return puzzle_2_1(); //if win, go another level down
  }
}

int countdown() {
  timeDisp.check(); //Refresh digit display every millisecond
  timer = 5;
  timeIterate.check(); //start counting down time variable.  Will decrement every second.
  int firstPuzzle = random(1, 1);
  if (firstPuzzle == 1) {
    return puzzle_1_1(); //initiate puzzles...
  }
  else if (firstPuzzle == 2) {
    //return puzzle_1_2();
  }
}

void loop() {
  // Wait for user to hit start button/switch
  int startBomb = digitalRead(start_switch);
  timer = 0;
  timeDisp.check(); //Refresh digit display every millisecond

  if (startBomb == HIGH) {
    timeDisp.check(); //Refresh digit display every millisecond
    Serial.println("Bomb activated...");
    int result = countdown();
    //On level 3 completion, the timer stops and flashes, displaying the completion time
    Serial.println("Your work here is done.   Turn the switch off to turn off the game and reset everything!");
    while (digitalRead(start_switch) == HIGH) {
      //Timer should keep displaying final time but not iterate
      timeDisp.check(); //Refresh digit display every millisecond
    } //Do nothing while we wait for the switch to go down
    //Once switch goes down, should move past that loop and exit the sequence, ready to start again.
    Serial.print("Game exited, ready to go again!");
    delay(2000);
  }
}
