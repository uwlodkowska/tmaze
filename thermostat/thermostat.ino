float ADDIT_RESISTANCE = 10000;

float U_TO_R = 1023;

#define THERMISTOR_PIN A0
int heating_pad_pin = 8;

float reading;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(heating_pad_pin, OUTPUT);

}

void loop() {
  reading = analogRead(THERMISTOR_PIN);
  reading = ADDIT_RESISTANCE/((U_TO_R/reading) - 1);
  Serial.println(reading);
  if(reading > 8500){
    digitalWrite(heating_pad_pin, HIGH);
  } else {
    digitalWrite(heating_pad_pin, LOW);
  }
  delay(5000);
  // put your main code here, to run repeatedly:

}
