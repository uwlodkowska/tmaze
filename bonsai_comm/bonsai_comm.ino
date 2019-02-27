#include <Firmata.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);
Adafruit_DCMotor *solenoid = AFMS.getMotor(1);
int no_of_revs;

void turnOnFirstMotor(byte port, int value)
{
 
  
  for (int i=0; i<TOTAL_PORTS; i++){
    myMotor->step(200, FORWARD, DOUBLE);
    delay(1000); 
  } 
}

void setup() {
  // put your setup code here, to run once:
  AFMS.begin();  // create with the default frequency 1.6KH
  myMotor->setSpeed(255);  // 10 rpm 
  solenoid->setSpeed(255);  // 10 rpm   
  //Serial.begin(57600);  
  //Serial.begin(38400);
//  Serial.begin(19200);
//  Serial.begin(14400);
  Serial.begin(9600);
    
//  Firmata.setFirmwareVersion(FIRMATA_MAJOR_VERSION, FIRMATA_MINOR_VERSION);
//  Firmata.attach(DIGITAL_MESSAGE, turnOnFirstMotor);
//  Firmata.begin(57600);
}

void loop() {
  // put your main code here, to run repeatedly:
    if (Serial.available()) {
      no_of_revs = Serial.parseInt();

      if(no_of_revs == 0) {
        solenoid->run(FORWARD);
        delay(55);                     
        solenoid->run(RELEASE);
      } else {
        for (int i=0; i<no_of_revs; i++){
          myMotor->step(200, FORWARD, DOUBLE);
          delay(1000); 
        }
      }
      
      
    } 
           
}
