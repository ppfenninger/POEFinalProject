

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

Adafruit_DCMotor *myMotor = AFMS.getMotor(1);


int sensor1Pin = A0; 
int sensor2Pin = A1; 
int sensor3Pin = A2;
int sensor1Value = 0;
int sensor2Value = 0;
int sensor3Value = 0; 
int averageValues = 0;
int averageDistance = 50;  
uint8_t baseSpeed = 20;
boolean isClockwise = true;  
int high = 1; 
int lastHigh = 1; 


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  AFMS.begin();

  myMotor->setSpeed(150);
  myMotor->run(FORWARD);
  myMotor->run(RELEASE);
}

void loop() {
  sensor1Value = analogRead(sensor1Pin);
  sensor2Value = analogRead(sensor2Pin); 
  sensor3Value = analogRead(sensor3Pin);
  
  averageValue = (sensor1Value + sensor2Value + sensor3Value) /4; 

  {if (sensor1Value > averageValue + averageDistance) {
     high = 1; 
  }
  else if (sensor2Value > averageValue + averageDistance) {
     high = 2;
  }
  else if (sensor3Value > averageValue + averageDistance) {
     high = 3;
  }}


  {if (lastHigh != high) {
    if (lastHigh < high) {
      isClockwise = true;
    }
    else {
      isClockwise = false; 
    }
  }}

  {if (isClockwise) {
    myMotor->run(FORWARD); 
  }
  else {
    myMotor->run(BACKWARD); 
  }}
  lastHigh = high; 
}

