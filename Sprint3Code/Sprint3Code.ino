#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

// motor and LED setup code
Adafruit_DCMotor *cakeMotor = AFMS.getMotor(1);
Adafruit_DCMotor *rotaryMotor = AFMS.getMotor(1);
Adafruit_NeoPixel sensorStrip = Adafruit_NeoPixel(3, 5, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel animalStrip = Adafruit_NeoPixel(12, 6, NEO_GRB + NEO_KHZ800);

// initialize cake clock's IR distance sensors
int sensor1Pin = A0;
int sensor2Pin = A1;
int sensor3Pin = A2;
int sensor1Value = 0;
int sensor2Value = 0;
int sensor3Value = 0;

uint8_t cakeSpeed = 30;       // speed of cakeMotor
boolean isClockwise = true;   // direction of cakeMotor
int sensVals[3] = {0, 0, 0};  // array to record order sensors are triggered
int threshold = 300;          // threshold to trigger distance sensors
boolean cakeTriggered = false;// if cakeTriggered==true, rotary clock can be triggered
int animalNumber = -1;        // number of animal that is lit up

//unsigned long lastWalk = 0;   //how long it's been since someone walked around the cake clock
//int rotaryThreshold = 5000;   //wait 5 seconds after face doesn't move to bring back to 12 position
//int cakeThreshold = 30000;    //wait 30 seconds after ppl stop walking around to stop moving

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

// what the encoder reads now and what it read on the last loop
byte encoderValue = B0000;
byte lastEncoderValue = B0000;

int clockNumber = 0;          // at 12 position


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  AFMS.begin();

  // cake clock setup code starts LEDs and cakeMotor
  sensorStrip.begin();
  sensorStrip.show();
  animalStrip.begin();
  animalStrip.show();
  cakeMotor->setSpeed(100);
  cakeMotor->run(FORWARD);
  cakeMotor->run(RELEASE);

}

void loop() {
  // read distance sensors
  sensor1Value = analogRead(sensor1Pin);
  sensor2Value = analogRead(sensor2Pin);
  sensor3Value = analogRead(sensor3Pin);

  // tests which sensors are on
{ if ((sensor1Value > threshold) && sensVals[0] == 0) {
      sensVals[0] = sensVals[0] + sensVals[1] + sensVals[2] + 1;
      sensorStrip.setPixelColor(0, sensorStrip.Color(0, 55, 0));
      sensorStrip.show();
    }
  }
{ if ((sensor2Value > threshold) && sensVals[1] == 0) {
      sensVals[1] = sensVals[0] + sensVals[1] + sensVals[2] + 1;
      sensorStrip.setPixelColor(1, sensorStrip.Color(0, 55, 0));
      sensorStrip.show();
    }
  }
{ if ((sensor3Value > threshold) && sensVals[2] == 0) {
      sensVals[2] = sensVals[0] + sensVals[1] + sensVals[2] + 1;
      sensorStrip.setPixelColor(2, sensorStrip.Color(0, 55, 0));
      sensorStrip.show();
    }
  }

  // switch to determine whether sensors were triggered CW or CCW
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

    // resets sensor values after all 3 distance sensors are triggered
    sensVals[0] = 0;
    sensVals[1] = 0;
    sensVals[2] = 0;

    if (isClockwise) {
      cakeMotor->run(FORWARD);
    }
    else {
      cakeMotor->run(BACKWARD);
    }
    cakeMotor->setSpeed(cakeSpeed); // turns cake motor on
    cakeTriggered = true;           // cake clock has been triggered

    delay(1000);
    // turns on the LEDs corresponding to distance sensors that were triggered
    sensorStrip.setPixelColor(0, 0);
    sensorStrip.setPixelColor(1, 0);
    sensorStrip.setPixelColor(2, 0);
    sensorStrip.show();

    // turns on an LED behind a random animal; animalNumber is now rotary puzzle solution
    if (animalNumber < 0) {
      animalNumber = random(11);
      Serial.println(animalNumber);
      animalStrip.setPixelColor(animalNumber, animalStrip.Color(0, 200, 0));
      animalStrip.show();
    }
  }
  // cakeMotor stops turning after [cakeThreshold] milliseconds
  //if ((millis() - lastWalk > cakeThreshold)) {
  //  cakeMotor->setSpeed(0);
  //}
  //lastTime = millis();
  
  if (cakeTriggered) { //runs all of the rotary clock code
    // read encoder reflectance sensors
    enc1 = digitalRead(enc1Pin);
    enc2 = digitalRead(enc2Pin);
    enc3 = digitalRead(enc3Pin);
    enc4 = digitalRead(enc4Pin);

    // condense sensor reads into a 4-bit binary number
    encoderValue = 1 * enc1 + 2 * enc2 + 4 * enc3 + 8 * enc4;

    switch (encoderValue) { // selects a number from 0-11 corresponding to sensor readings
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
    if (clockNumber == animalNumber) { // we have achieved our goal
      sensorStrip.setPixelColor(0, sensorStrip.Color(0, 0, 50)); // three LEDs turn blue to indicate you have won
      sensorStrip.setPixelColor(1, sensorStrip.Color(0, 0, 50));
      sensorStrip.setPixelColor(2, sensorStrip.Color(0, 0, 50));
      sensorStrip.show();

      // animal LEDs reset
      animalStrip.setPixelColor(animalNumber, 0);
      animalNumber = -1;

      // stops cake motor if that hasn't happened already
      cakeMotor->setSpeed(0);
    }

    lastEncoderValue = encoderValue;
    
    // THE FOLLOWING CODE IS NOT YET TESTED
    // checks to see how long it's been since the rotary clock was moved
    //    if ((millis() - lastTime > rotaryThreshold) && (encoderValue != B0100)) {
    // rotaryMotor rotates clock face back to 12 position
    //      rotaryMotor->setSpeed(RSpeed);
    //
    //      while (encoderValue != B0100) { // until it reaches the 0 degrees marker
    //        cakeMotor->run(BACKWARD);
    //
    //        //run cakeMotor whichever direction it was going to run anyway
    //        if (isClockwise) {
    //          cakeMotor->run(FORWARD);
    //        }
    //        else {
    //          cakeMotor->run(BACKWARD);
    //        }
    //      }
    //
    //      rotaryMotor->setSpeed(0);
    //      clockNumber = 0;
    //      Serial.println("returned to 12");
    //    }
  }
}
