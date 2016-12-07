#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// cake clock setup code
Adafruit_DCMotor *cakeMotor = AFMS.getMotor(1);
Adafruit_NeoPixel sensorStrip = Adafruit_NeoPixel(3, 5, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel animalStrip = Adafruit_NeoPixel(12, 6, NEO_GRB + NEO_KHZ800);
int sensor1Pin = A0; 
int sensor2Pin = A1; 
int sensor3Pin = A2;
int sensor1Value = 0;
int sensor2Value = 0;
int sensor3Value = 0;   
uint8_t cakeSpeed = 30;
boolean isClockwise = true;  
int sensVals[3] = {0, 0, 0};
int threshold = 300; 
boolean cakeTriggered = false; 
int animalNumber = -1; //number of animal that is lit up

// rotary clock variables
const int enc1Pin = 13;
const int enc2Pin = 12;
const int enc3Pin = 11;
const int enc4Pin = 10;

// sensor values. enc1 is far outer edge, enc4 is inner edge.
int enc1 = 0;
int enc2 = 0;
int enc3 = 0;
int enc4 = 0;

byte encoderValue = B0000;
byte lastEncoderValue = B0000;

int clockNumber = 0;          //at 12 position


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  AFMS.begin();

  //cake clock setup code
  sensorStrip.begin();
  sensorStrip.show(); 
  animalStrip.begin();
  animalStrip.show();
  cakeMotor->setSpeed(100);
  cakeMotor->run(FORWARD);
  cakeMotor->run(RELEASE);

}

void loop() {
  //initial cake clock code 

  sensor1Value = analogRead(sensor1Pin);
  sensor2Value = analogRead(sensor2Pin); 
  sensor3Value = analogRead(sensor3Pin);

  //tests which sensors are on
  {if ((sensor1Value > threshold) && sensVals[0] == 0) {
     sensVals[0] = sensVals[0] + sensVals[1] + sensVals[2] + 1;
     sensorStrip.setPixelColor(0, sensorStrip.Color(0, 55, 0)); 
     sensorStrip.show();
  }}    
  {if ((sensor2Value > threshold) && sensVals[1] == 0) {
     sensVals[1] = sensVals[0] + sensVals[1] + sensVals[2] + 1;
     sensorStrip.setPixelColor(1, sensorStrip.Color(0, 55, 0));
     sensorStrip.show();   
  }}
  {if ((sensor3Value > threshold) && sensVals[2] == 0) {
     sensVals[2] = sensVals[0] + sensVals[1] + sensVals[2] + 1; 
     sensorStrip.setPixelColor(2, sensorStrip.Color(0, 55, 0));
     sensorStrip.show(); 
  }}

  if (sensVals[0] + sensVals[1] + sensVals[2] == 7) {
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
      // resets sensor values
      sensVals[0] = 0;
      sensVals[1] = 0;
      sensVals[2] = 0;

      if (isClockwise) {
        cakeMotor->run(FORWARD); 
      }
      else {
        cakeMotor->run(BACKWARD); 
      }
      cakeMotor->setSpeed(cakeSpeed);  //turns cake motor on
      cakeTriggered = true; //variable to let rotary clock know that cake clock has been triggered
      
      delay(1000);
      sensorStrip.setPixelColor(0, 0); 
      sensorStrip.setPixelColor(1, 0); 
      sensorStrip.setPixelColor(2, 0); 
      sensorStrip.show(); 

      if (animalNumber < 0) {
        animalNumber = random(9);
        Serial.println(animalNumber); 
        animalStrip.setPixelColor(animalNumber, animalStrip.Color(0, 200, 0));
        animalStrip.show();   
      }   
    }

  if (cakeTriggered){ //runs all of the rotary clock code

    enc1 = digitalRead(enc1Pin);
    enc2 = digitalRead(enc2Pin);
    enc3 = digitalRead(enc3Pin);
    enc4 = digitalRead(enc4Pin);
  
    encoderValue = 1*enc1 + 2*enc2 + 4*enc3 + 8*enc4;
  
    switch(encoderValue){
      case B0000:
        clockNumber = 0;
        break;
      case B1000:
        clockNumber = 1;
        break;
      case B1100:
        clockNumber = 2;
        break;
      case B1110:
        clockNumber = 3;
        break;
      case B1111:
        clockNumber = 4;
        break;
      case B0111:
        clockNumber = 5;
        break;
      case B0011:
        clockNumber = 6;
        break;
      case B0001:
        clockNumber = 7;
        break;
      case B0101:
        clockNumber = 8;
        break;
      case B1101:
        clockNumber = 9;
        break;
      case B1001:
        clockNumber = 10;
        break;
      case B1011:
        clockNumber = 11;
        break;
    }
    Serial.print(animalNumber); 
    Serial.println(clockNumber); 
    if (clockNumber == animalNumber) { //we have achieved our goal
      sensorStrip.setPixelColor(0, sensorStrip.Color(0, 0, 50)); // three LEDs turn blue to indicate you have won
      sensorStrip.setPixelColor(1, sensorStrip.Color(0, 0, 50)); 
      sensorStrip.setPixelColor(2, sensorStrip.Color(0, 0, 50));
      sensorStrip.show();

      //animal LEDs reset
      animalStrip.setPixelColor(animalNumber, 0);
      animalNumber = -1;

      //stops cake motor if that hasn't happened already
      cakeMotor->setSpeed(0);
    }
  }
}

