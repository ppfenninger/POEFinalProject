
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

Adafruit_DCMotor *myMotor = AFMS.getMotor(1);

int sensorPin = A0; 
int sensorValue = 0;
uint8_t baseSpeed = 20; 
uint8_t changeSpeed = 20; // change in speed 



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  AFMS.begin();

  myMotor->setSpeed(60);
  myMotor->run(FORWARD);
  myMotor->run(RELEASE);
}

void loop() {
  sensorValue = analogRead(A0); 
  Serial.println(sensorValue); 

  myMotor->setSpeed((uint8_t)(sensorValue / 20)); 
  myMotor->run(FORWARD); 
}


