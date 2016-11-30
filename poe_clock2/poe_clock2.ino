#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

Adafruit_DCMotor *myMotor = AFMS.getMotor(1);


int sensor1Pin = A0; 
int sensor2Pin = A1; 
int sensor3Pin = A2;
int led0 = 13;
int led1 = 12;
int led2 = 11;
int sensor1Value = 0;
int sensor2Value = 0;
int sensor3Value = 0;   
uint8_t baseSpeed = 30;
boolean isClockwise = true;  
int sensVals[3] = {0, 0, 0};
int threshold = 300; 
//int clockwise1[3] = {1, 2, 4};
//int clockwise2[3] = {2, 4, 1};
//int clockwise3[3] = {4, 1, 2};
//int counterclockwise1[3] =  {1, 4, 2};
//int counterclockwise2[3] =  {2, 1, 4};
//int counterclockwise3[3] =  {4, 2, 1};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  AFMS.begin();
  pinMode(led0, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  myMotor->setSpeed(baseSpeed);
  myMotor->run(FORWARD);
  myMotor->run(RELEASE);
}

void loop() {
   
  sensor1Value = analogRead(sensor1Pin);
  sensor2Value = analogRead(sensor2Pin); 
  sensor3Value = analogRead(sensor3Pin);


  {if ((sensor1Value > threshold) && sensVals[0] == 0) {
     sensVals[0] = sensVals[0] + sensVals[1] + sensVals[2] + 1;
  }}
  {if ((sensor2Value > threshold) && sensVals[1] == 0) {
     sensVals[1] = sensVals[0] + sensVals[1] + sensVals[2] + 1; 
  }}
  {if ((sensor3Value > threshold) && sensVals[2] == 0) {
     sensVals[2] = sensVals[0] + sensVals[1] + sensVals[2] + 1; 
  }}
  
  Serial.print(sensor1Value); 
  Serial.print(','); 
  Serial.print(sensor2Value); 
  Serial.print(',');
  Serial.println(sensor3Value); 
  
  {if (sensVals[0] + sensVals[1] + sensVals[2] == 7) {
    if (sensVals[0] == 1) {
      if (sensVals[1] == 2) {
        isClockwise = true;
      }
      else {
        isClockwise = false;
      }
    }
    else if (sensVals[0] == 2) {
      if (sensVals[1] == 4) {
        isClockwise = true;
      }
      else {
        isClockwise = false;
      } 
    }
    else if (sensVals[0] == 4) {
      if (sensVals[1] == 1) {
        isClockwise = true;
      }
      else {
        isClockwise = false; 
      }
    }
    sensVals[0] = 0;
    sensVals[1] = 0; 
    sensVals[2] = 0; 
  }}

  digitalWrite(led0, sensVals[0]);
  digitalWrite(led1, sensVals[1]);
  digitalWrite(led2, sensVals[2]);
  

  {if (isClockwise) {
    myMotor->run(FORWARD); 
  }
  else {
    myMotor->run(BACKWARD); 
  }}  
}


