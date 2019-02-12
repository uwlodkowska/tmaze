#include <Firmata.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *solenoid1 = AFMS.getMotor(2); 

void setup() {
  // put your setup code here, to run once:

  AFMS.begin();  // create with the default frequency 1.6KH
  solenoid1->setSpeed(255);  // 10 rpm   
  Serial.begin(9600);
  
  solenoid1->run(FORWARD);
}

void loop() {
}
