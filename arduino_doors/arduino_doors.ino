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
const int openTime = 4000;



int doorIdx;
int doorCount;

Adafruit_MotorShield shields[shieldsNumber];

Adafruit_DCMotor *upMotors[motorsNoPerDirection];
Adafruit_DCMotor *downMotors[motorsNoPerDirection];

int doorsToMove[motorsNoPerDirection];

// Select which 'port' M1, M2, M3 or M4. In this case, M1
//Adafruit_DCMotor *up1 = AFMS.getMotor(2);
// You can also make another motor on port M2
//Adafruit_DCMotor *down1 = AFMS.getMotor(3);

//Adafruit_DCMotor *up2 = AFMS.getMotor(1);
// You can also make another motor on port M2
//Adafruit_DCMotor *down2 = AFMS.getMotor(4);

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
 
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
  if (Serial.available() > 0) {
    String strIdx = Serial.readStringUntil('\n');
    
    doorCount = 0;
    String token = getValue(strIdx,',',doorCount);
    while (token != "") 
    { 
      doorsToMove[doorCount] = token.toInt();
      doorCount++;
      token = getValue(strIdx,',',doorCount);
    }

    for (int doorTabIdx = 0; doorTabIdx < doorCount; doorTabIdx++){
      doorIdx = doorsToMove[doorTabIdx];
      if (doorIdx < 0) {
        doorIdx = -doorIdx-1;
        downMotors[doorIdx]->run(FORWARD);   
      } else {
        doorIdx--;
        upMotors[doorIdx]->run(FORWARD);
      }
    }

    delay(openTime); 
     
    for (int doorTabIdx = 0; doorTabIdx < doorCount; doorTabIdx++){
      doorIdx = doorsToMove[doorTabIdx];
      if (doorIdx < 0) {
        doorIdx = -doorIdx-1;
        downMotors[doorIdx]->run(RELEASE);     
      } else {
        doorIdx--;
        upMotors[doorIdx]->run(RELEASE);  
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
