#include <Firmata.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x60);  
Adafruit_DCMotor *solenoid1 = AFMS.getMotor(1);

void setup() {
  // put your setup code here, to run once:

  AFMS.begin();  // create with the default frequency 1.6KH
  solenoid1->setSpeed(255);  // 10 rpm   
  Serial.begin(9600);
}

void loop() {
  solenoid1->run(FORWARD);
  delay(2000);
  solenoid1->run(RELEASE);
  delay(2000);
}
