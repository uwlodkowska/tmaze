#include <Firmata.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

const int shieldsNumber = 2;
const int motorsNoPerDirection = 4;
const int motorsPerShield = 2;
const int feedersQ = 2;
const int openTime = 4000;
const int feederOpenTime = 300;

int doorIdx;
int feederIdx;
int doorCount;
int doorsToMove[motorsNoPerDirection];

Adafruit_MotorShield shields[shieldsNumber];

Adafruit_DCMotor *upMotors[motorsNoPerDirection];
Adafruit_DCMotor *downMotors[motorsNoPerDirection];
Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x68); 
Adafruit_DCMotor *solenoid1 = AFMS.getMotor(4); 
Adafruit_DCMotor *feeders[feedersQ];

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

int open_time = 300;
int interval_btw_drops = 5000;
int led_pin = 7;
int button_pin = 4;
int ir_pin = 5;
int button_state = 0;
int diode_delay = 2500;

int click_time_limit = 1000;
long last_click_time = 0;
long current_click_time;
long click_duration;

long counter = 0;

boolean on = false;

int prev_button_state = 1;

void setup() {
  AFMS.begin();  // create with the default frequency 1.6KH
  solenoid1->setSpeed(255);  // 10 rpm   
  Serial.begin(9600);
  
  
  feeders[0] = AFMS.getMotor(1);
  feeders[1] = AFMS.getMotor(2);
  for (int idx = 0; idx < feedersQ; idx++) {
    feeders[idx]->setSpeed(255);
  }
  //pinMode(led_pin, OUTPUT);
  pinMode(button_pin, INPUT_PULLUP);
  //pinMode(ir_pin, INPUT_PULLUP);
}

void loop() {
  //solenoid->run(FORWARD);
  //delay(open_time);                     
  //solenoid->run(RELEASE);
  //delay(interval_btw_drops);
  button_state = digitalRead(button_pin);
    
  //Serial.println(digitalRead(ir_pin));
  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  
  if (button_state == LOW) {
      //digitalWrite(led_pin, HIGH);
    if (!on) {
      on = true;
      last_click_time = millis();
    }
  } else if (on) {
    current_click_time = millis(); 
    click_duration = current_click_time - last_click_time;
    Serial.println(click_duration);
    if (click_duration > 0){
      if (click_duration > click_time_limit) {
        
      } else {
        solenoid1->run(FORWARD);
        Serial.println("run");
        delay(open_time);                     
        solenoid1->run(RELEASE);
      }
    }
    on = false;
  }

  if (Serial.available() >0) {
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

      int absVal = abs(doorIdx);
      if (absVal < 10) {
        if (doorIdx < 0) {
          doorIdx = -doorIdx-1;
          downMotors[doorIdx]->run(FORWARD);   
        } else {
          doorIdx--;
          upMotors[doorIdx]->run(FORWARD);
        }
      } else {

        if (doorIdx < 0) {
          feederIdx = 0;
        } else {
          feederIdx = 1;
        }
        if (absVal <=20){
          feeders[feederIdx]->run(FORWARD);
        }
        if(absVal == 10){
          delay(feederOpenTime);
        }
        if(absVal !=20) {
          feeders[feederIdx]->run(RELEASE);
        }

      }
    }
  }

    
  
  prev_button_state = button_state;
  
}

//if(on){
//      digitalWrite(led_pin, LOW);
//      on = false; 
//    } else {
//      digitalWrite(led_pin, HIGH);
//      on = true;
//      solenoid->run(FORWARD);
//      delay(open_time);                     
//      solenoid->run(RELEASE);
//      delay(2000);
//    }
