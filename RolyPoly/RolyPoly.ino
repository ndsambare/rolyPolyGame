#include <Wire.h>                 // Must include Wire library for I2C
#include "SparkFun_MMA8452Q.h"    // Click here to get the library: http://librarymanager/All#SparkFun_MMA8452Q

MMA8452Q accel;                   // create instance of the MMA8452 class

void setup() {
  Serial.begin(9600);
  Serial.println("MMA8452Q Basic Reading Code!");
  Wire.begin();

  if (accel.begin() == false) {
    Serial.println("Not Connected. Please check connections and read the hookup guide.");
    while (1);
  }
  Serial.begin(9600);               //serial
  for (int i = 2; i <= 13; i++) {   //pinmode
    pinMode(i, OUTPUT);
  }
  for (int i = 2; i <= 8; i++) {    //starts with everything off
    digitalWrite(i, LOW);
  }
  for (int i = 9; i <= 13; i++) {
    digitalWrite(i, HIGH);
  }
}
int prey[2];                         //[0] is row, [1] is col
int pred[2];                         //lower left corner of pred.
unsigned long now;
unsigned long nextTimeGame = 0;
unsigned long nextTimePrey = 0;
unsigned long nextTimeMovePrey = 0;
unsigned long nextTimePred = 0;
unsigned long nextTimeMovePred = 0;
unsigned long deltaGame = 15000;
unsigned long deltaPrey = 250;
unsigned long deltaMovePrey = 250;
unsigned long deltaPred = 125;
unsigned long deltaMovePred = 2 * deltaMovePrey;
bool preyOn = false; 
bool predOn = false;
void loop() {
  now = millis();
  if (now >= nextTimeGame) {            // starts game
    prey[0] = random(2,9);
    prey[1] = random(9,14);
    pred[0] = random(2, 8);
    pred[1] = random(10,14);
    nextTimeGame += deltaGame;
  }

  /*
  if (now >= nextTimePrey) {            // makes the prey blink
    if(!preyOn) {
      digitalWrite(prey[0], HIGH);
      digitalWrite(prey[1], LOW);
      preyOn = true;
    }
    else {
      for (int i = 2; i <= 8; i++) {    // turns everything off
        digitalWrite(i, LOW);
      }
      for (int i = 9; i <= 13; i++) {
        digitalWrite(i, HIGH);
      }
      preyOn = false;
    }
    nextTimePrey += deltaPrey;
  }
  */
/*
  if (now >= nextTimePrey) {          // shows prey
    for (int i = 2; i <= 8; i++) {    // turns everything off
      digitalWrite(i, LOW);
    }
    for (int i = 9; i <= 13; i++) {
      digitalWrite(i, HIGH);
    }
    digitalWrite(prey[0], HIGH);
    digitalWrite(prey[1], LOW);
  }
*/
  if (now >= nextTimeMovePrey) {        // move prey based on accel reading
    if (accel.available()) {                        // move x direction
      if (accel.getCalculatedX() > 0.2) {           // move right
        prey[1] = move(false, 9, 13, prey[1]);
      }
      else if(accel.getCalculatedX() < -0.2) {      // move left
        prey[1] = move(true, 9, 13, prey[1]);
      }
      if (accel.getCalculatedY() > 0.2) {           // move up
        prey[0] = move(true, 2, 8, prey[0]);
      }
      else if(accel.getCalculatedY() < -0.2) {      // move down
        prey[0] = move(false, 2, 8, prey[0]);
      }
    }
    nextTimeMovePrey += deltaMovePrey;
  }

  if (now >= nextTimePred) {
    if(!predOn) {
      digitalWrite(pred[0], HIGH);
      digitalWrite(pred[0] + 1, HIGH);
      digitalWrite(pred[1], LOW);
      digitalWrite(pred[1] - 1, LOW);
      predOn = true;
    }
    else {
      for (int i = 2; i <= 8; i++) {    // turns everything off
        digitalWrite(i, LOW);
      }
      for (int i = 9; i <= 13; i++) {
        digitalWrite(i, HIGH);
      }
      predOn = false;
    }
    nextTimePred += deltaPred;
  }
}

int move(bool pos, int mini, int maxi, int cur) {
  if (pos) {
    if (cur == maxi) {
      return maxi;
    }
    else {
      return cur + 1;
      }
  }
  else {
    if (cur == mini) {
      return mini;
    }
    else {
      return cur - 1;
    }
  }
}

bool touch(int prey[], int pred[]) {
  if (prey[0] == pred[0] ||
      prey[0] == pred[0] + 1 ||
      prey[1] == pred[1] ||
      prey[1] == pred[1] -1) {
        return true;
      }
  else {
    return false;
  }
}
