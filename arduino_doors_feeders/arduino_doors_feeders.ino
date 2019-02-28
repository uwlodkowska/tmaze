/* 
This is a test sketch for the Adafruit assembled Motor Shield for Arduino v2
It won't work with v1.x motor shields! Only for the v2's with built in PWM
control

For use with the Adafruit Motor Shield v2 
---->	http://www.adafruit.com/products/1438
*/

#include <Wire.h>
#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
//Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Or, create it with a different I2C address (say for stacking)
//Adafruit_MotorShield AFMS2 = Adafruit_MotorShield(0x62); 
const int shieldsNumber = 2;
const int motorsNoPerDirection = 4;
const int motorsPerShield = 2;
const int feedersQ = 2;
const int openTime = 4000;
const int feederOpenTime = 1000;

int doorIdx;

Adafruit_MotorShield shields[shieldsNumber];

Adafruit_DCMotor *upMotors[motorsNoPerDirection];
Adafruit_DCMotor *downMotors[motorsNoPerDirection];
Adafruit_DCMotor *feeders[feedersQ];

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
 
  shields[0] = Adafruit_MotorShield(0x61); 
  shields[1] = Adafruit_MotorShield(0x64); 


  for (int shieldIdx = 0; shieldIdx < shieldsNumber-1; shieldIdx++)  {
    upMotors[shieldIdx*motorsPerShield] = shields[shieldIdx].getMotor(2);
    downMotors[shieldIdx*motorsPerShield] = shields[shieldIdx].getMotor(3);
    upMotors[shieldIdx*motorsPerShield+1] = shields[shieldIdx].getMotor(1);
    downMotors[shieldIdx*motorsPerShield+1] = shields[shieldIdx].getMotor(4);
  }

  upMotors[motorsPerShield] = shields[shieldsNumber-1].getMotor(2);
  downMotors[motorsPerShield] = shields[shieldsNumber-1].getMotor(1);

  feeders[0] = shields[shieldsNumber-1].getMotor(3);
  feeders[1] = shields[shieldsNumber-1].getMotor(4);

  for (int shieldIdx = 0; shieldIdx < shieldsNumber; shieldIdx++)  {
    shields[shieldIdx].begin();
  }
  // Set the speed to start, from 0 (off) to 255 (max speed)
  for (int motorIdx = 0; motorIdx < motorsNoPerDirection; motorIdx++)  {
    upMotors[motorIdx]->setSpeed(255);
    downMotors[motorIdx]->setSpeed(255);
  }
  for (int idx = 0; idx < feedersQ; idx++) {
    feeders[idx]->setSpeed(255);
  }
}

void loop() {

  if (Serial.available() >0) {
    String strIdx = Serial.readStringUntil('\n');
    doorIdx = strIdx.toInt();
    int absVal = abs(doorIdx);
    if (absVal == 10) {
      if (doorIdx < 0) {
        feeders[0]->run(FORWARD);
        delay(feederOpenTime);
        feeders[0]->run(RELEASE);
      } else {
        feeders[1]->run(FORWARD);
        delay(feederOpenTime);
        feeders[1]->run(RELEASE);
      }
    } else {
      absVal--;
      if (doorIdx < 0) {
        downMotors[absVal]->run(FORWARD);
        delay(openTime);  
        downMotors[absVal]->run(RELEASE);     
      } else {
        upMotors[absVal]->run(FORWARD);
        delay(openTime);  
        upMotors[absVal]->run(RELEASE);  
      }
    }
  }
  
//  up1->run(FORWARD);
//  delay(3000);
//  up1->run(RELEASE);
//  down1->run(FORWARD);
//  delay(3000);
//  down1->run(RELEASE);
 
}
