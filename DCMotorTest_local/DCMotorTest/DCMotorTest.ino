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
const int openTime = 3000;

Adafruit_MotorShield shields[shieldsNumber];

Adafruit_DCMotor *upMotors[motorsNoPerDirection];
Adafruit_DCMotor *downMotors[motorsNoPerDirection];

  

// Select which 'port' M1, M2, M3 or M4. In this case, M1
//Adafruit_DCMotor *up1 = AFMS.getMotor(2);
// You can also make another motor on port M2
//Adafruit_DCMotor *down1 = AFMS.getMotor(3);

//Adafruit_DCMotor *up2 = AFMS.getMotor(1);
// You can also make another motor on port M2
//Adafruit_DCMotor *down2 = AFMS.getMotor(4);

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Adafruit Motorshield v2 - DC Motor test!");

//shields[0] = Adafruit_MotorShield(0x61); 
//shields[1] = Adafruit_MotorShield(0x64); 


  shields[0] = Adafruit_MotorShield(0x60); 
  shields[1] = Adafruit_MotorShield(0x62); 


  for (int shieldIdx = 0; shieldIdx < shieldsNumber; shieldIdx++)  {
    upMotors[shieldIdx*motorsPerShield] = shields[shieldIdx].getMotor(2);
    downMotors[shieldIdx*motorsPerShield] = shields[shieldIdx].getMotor(3);
    upMotors[shieldIdx*motorsPerShield+1] = shields[shieldIdx].getMotor(1);
    downMotors[shieldIdx*motorsPerShield+1] = shields[shieldIdx].getMotor(4);
  }

  //AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  for (int shieldIdx = 0; shieldIdx < shieldsNumber; shieldIdx++)  {
    shields[shieldIdx].begin();
  }
  // Set the speed to start, from 0 (off) to 255 (max speed)
  for (int motorIdx = 0; motorIdx < motorsNoPerDirection; motorIdx++)  {
    upMotors[motorIdx]->setSpeed(255);
    downMotors[motorIdx]->setSpeed(255);
  }
}

void loop() {

  for (int motorIdx = 0; motorIdx < motorsNoPerDirection; motorIdx++)  {
    upMotors[motorIdx]->run(FORWARD);
    delay(openTime);
    upMotors[motorIdx]->run(RELEASE);
    delay(1000);
    downMotors[motorIdx]->run(FORWARD);
    delay(openTime);
    downMotors[motorIdx]->run(RELEASE);
    delay(1000);
  }
//  up1->run(FORWARD);
//  delay(3000);
//  up1->run(RELEASE);
//  down1->run(FORWARD);
//  delay(3000);
//  down1->run(RELEASE);
 
}
