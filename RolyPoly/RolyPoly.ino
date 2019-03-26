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
int scorePrey;
int scorePred;
unsigned long now;
unsigned long nextTimeGame = 0;
unsigned long nextTimePrey = 0;
unsigned long nextTimeMovePrey = 0;
unsigned long nextTimePred = 0;
unsigned long nextTimeMovePred = 0;
unsigned long nextTimeM = 0;
unsigned long deltaGame = 15000;
unsigned long deltaPrey = 250;
unsigned long deltaMovePrey = 100;
unsigned long deltaPred = 125;
unsigned long deltaMovePred = 2 * deltaMovePrey;
unsigned long deltaM = 3;
unsigned long deltaScore = 4000;
unsigned long m = 1;
bool preyOn = false; 
bool predOn = false;
bool game = true;
bool preyWin = false;
String dots = "";
void loop() {
  now = millis();
  if (now >= nextTimeGame) {                              // starts game
    if (!game) {
      prey[0] = random(2,9);
      prey[1] = random(9,14);
      pred[0] = random(2, 8);
      pred[1] = random(10,14);
      while (touch(prey, pred)) {                        // if prey and pred are touching, randomly pick new positions
        prey[0] = random(2,9);
        prey[1] = random(9,14);
        pred[0] = random(2, 8);
        pred[1] = random(10,14);
      }
      nextTimeGame += deltaGame;
      preyWin = true;
      game = true;
    }
    else {
      if (preyWin) {
        scorePrey++;
      }
      else {
        scorePred++;
      }
      dots = score(scorePrey, scorePred, preyWin);
      nextTimeGame += deltaScore;
      game = false;
    }
  }

  if (game) {
    if (touch(prey, pred)){                           // check if they are touching
      preyWin = false;
      nextTimeGame = now;
    }
    
    if (now >= nextTimeMovePrey) {                    // update location of prey based on accel reading
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
  
    if (now >= nextTimeMovePred) {                    // update location of pred based on serial?
      if (Serial.available() > 0) {
        char c = Serial.read();
        if (c == 'w') {
          pred[0] = move(true, 2, 7, pred[0]);
        }
        else if (c == 's') {
          pred[0] = move(false, 2, 7, pred[0]);
        }
        else if (c == 'd') {
          pred[1] = move(false, 10, 13, pred[1]);
        }
        else if (c == 'a') {
          pred[1] = move(true, 10, 13, pred[1]);
        }
        else {
        }
        while(Serial.available()){
          char throwaway = Serial.read();
        }
      }
      nextTimeMovePred += deltaMovePred;
    }

    if (now >= nextTimeM) {                           // multiplexing, display everything?
      int col = m % 5 + 9;
      for (int i = 2; i <= 8; i++) {                  // turns everything off
        digitalWrite(i, LOW);
      }
      for (int i = 9; i <= 13; i++) {
        digitalWrite(i, HIGH);
      }
      digitalWrite(col, LOW);                   // turn row on
      if (prey[1] == col) {
        digitalWrite(prey[0], HIGH); 
      }
      if (pred[1] == col || pred[1] -1 == col) {
        digitalWrite(pred[0], HIGH);
        digitalWrite(pred[0] + 1, HIGH);
      }
      m++;
      nextTimeM += deltaM;
    }
  }

  else {                         // show the score etc.
    if (now >= nextTimeM) {
      int col = m % 5 + 9;
      for (int i = 2; i <= 8; i++) {                  // turns everything off
        digitalWrite(i, LOW);
      }
      for (int i = 9; i <= 13; i++) {
        digitalWrite(i, HIGH);
      }
      
      digitalWrite(col, LOW);                           // shows the winner
      if (col == 11) {
        digitalWrite(5, HIGH);
        if (!preyWin) {
          digitalWrite(4, HIGH);
        }
      }
      if (col == 10 && !preyWin) {
        digitalWrite(4, HIGH);
        digitalWrite(5, HIGH);
      }
      if (dots.charAt(m % 5) == '1') {
        digitalWrite(2, HIGH);    
      }
      m++;
      nextTimeM += deltaM;  
    }
  }

}

String score(int prey, int pred, bool preyWin) {                    // calculates the dots for the score
  if (preyWin) {
    if (prey > pred + 2) {
      return "11100";
    }
    else if (prey > pred) {
      return "01100";
    }
    else if (prey == pred) {
      return "00100";
    }
    else if (prey > pred - 3) {
      return "00110";
    }
    else {
      return "00111";
    }
  }
  else {
    if (pred > prey + 2) {
      return "11100";
    }
    else if (pred > prey) {
      return "01100";
    }
    else if (pred == prey) {
      return "00100";
    }
    else if (pred > prey - 3) {
      return "00110";
    }
    else {
      return "00111";
    }
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
  if (prey[0] == pred[0] && prey[1] == pred[1]||
      prey[0] == pred[0] + 1 && prey[1] == pred[1]||
      prey[0] == pred[0] && prey[1] == pred[1] - 1||
      prey[0] == pred[0] + 1 && prey[1] == pred[1] - 1) {
        return true;
      }
  else {
    return false;
  }
}
