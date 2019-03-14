#include <Firmata.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *solenoid1 = AFMS.getMotor(3); 
Adafruit_DCMotor *solenoid2 = AFMS.getMotor(2);

int startup_time = 10000;
int open_time = 30;
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
  solenoid2->setSpeed(255);  // 10 rpm   
  Serial.begin(9600);
  
  solenoid1->run(FORWARD);
  delay(startup_time);                     
  solenoid1->run(RELEASE);
  
  solenoid2->run(FORWARD);
  delay(startup_time);                     
  solenoid2->run(RELEASE);
  
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
    //current_click_time = millis(); 
    //click_interval = current_click_time - last_click_time;
  /*
    if(click_interval < click_time_limit){
      solenoid1->run(FORWARD);
      delay(open_time);                     
      solenoid1->run(RELEASE);
      clicked_before = false;
    } else {
      if(clicked_before){
        solenoid2->run(FORWARD);
        delay(open_time);                     
        solenoid2->run(RELEASE);
        clicked_before = false;
      } else {
        clicked_before = true;
      }
    }
  */
    //delay(diode_delay - open_time);
    //digitalWrite(led_pin, LOW);
      
  } else if (on) {
    current_click_time = millis(); 
    click_duration = current_click_time - last_click_time;
    Serial.println(click_duration);
    if (click_duration > 0){
      if (click_duration > click_time_limit) {
        solenoid2->run(FORWARD);
        delay(open_time);                     
        solenoid2->run(RELEASE);
      } else {
        solenoid1->run(FORWARD);
        delay(open_time);                     
        solenoid1->run(RELEASE);
      }
    }
    on = false;
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
