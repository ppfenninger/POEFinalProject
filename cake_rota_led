#include <Wire.h>

//start LED initialization
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6
int theChosenOne; //random number: the chosen animal!
int runOnce = 0; //int to make the rng run once

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

//end LED initialization

#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *cakeMotor = AFMS.getMotor(1);
Adafruit_DCMotor *rotaryMotor = AFMS.getMotor(2);


//pins and values for encoder sensors
const int enc1Pin = 10;
const int enc2Pin = 9;
const int enc3Pin = 8;
const int enc4Pin = 7;
int enc1 = 0;
int enc2 = 0;
int enc3 = 0;
int enc4 = 0;
byte encoderValue = B0000;
byte lastEncoderValue = B0000;

//pins for IR sensors
int sensor1Pin = A0;
int sensor2Pin = A1;
int sensor3Pin = A2;
int sensVals[3] = {0, 0, 0};
int threshold = 145;          //activation threshold for sensors

//pins and values for LEDs
int led0 = 13;
int led1 = 12;
int led2 = 11;
int sensor1Value = 0;
int sensor2Value = 0;
int sensor3Value = 0;

uint8_t CSpeed = 100;         //base speed of cakeMotor
uint8_t RSpeed = 20;          //base speed of rotaryMotor
boolean isClockwise = true;   //if cakeMotor is clockwise
int clockNumber = 0;          //at 12 position
unsigned long lastTime = 0;   //how long it's been since the rotary face moved
unsigned long lastWalk = 0;   //how long it's been since someone walked around the cake clock
int rotaryThreshold = 5000;   //wait 5 seconds after face doesn't move to bring back to 12 position
int cakeThreshold = 30000;    //wait 30 seconds after ppl stop walking around to stop moving


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  AFMS.begin();
  pinMode(led0, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  cakeMotor->setSpeed(baseSpeed);
  cakeMotor->run(FORWARD);
  cakeMotor->run(RELEASE);
  rotaryMotor->setSpeed(baseRotarySpeed);
}


void funColor(int led){
  //what a fun color!
  
  strip.setPixelColor(led, strip.Color(13, 6, 0));
  strip.show();
}

void loop() {

`//random number generator: generates randint theChosenOne
  if (runOnce == 0){
    theChosenOne = random(11);
    runOnce++;
  }

  funColor(theChosenOne);

  
  enc1 = digitalRead(enc1Pin);
  enc2 = digitalRead(enc2Pin);
  enc3 = digitalRead(enc3Pin);
  enc4 = digitalRead(enc4Pin);
  encoderValue = 1 * enc1 + 2 * enc2 + 4 * enc3 + 8 * enc4;

  sensor1Value = analogRead(sensor1Pin);
  sensor2Value = analogRead(sensor2Pin);
  sensor3Value = analogRead(sensor3Pin);

  if ((sensor1Value > threshold) && sensVals[0] == 0) {
    sensVals[0] = sensVals[0] + sensVals[1] + sensVals[2] + 1;
  }
  if ((sensor2Value > threshold) && sensVals[1] == 0) {
    sensVals[1] = sensVals[0] + sensVals[1] + sensVals[2] + 1;
  }
  if ((sensor3Value > threshold) && sensVals[2] == 0) {
    sensVals[2] = sensVals[0] + sensVals[1] + sensVals[2] + 1;
  }


  switch (encoderValue) {
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
  //Serial.println(clockNumber);
  //Serial.print(sensVals[0]);

  //Serial.print(',');
  //Serial.print(sensVals[1]);
  //Serial.print(',');
  //Serial.println(sensVals[2]);

{ if (sensVals[0] + sensVals[1] + sensVals[2] == 7) {
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
      lastWalk = millis();
      cakeMotor->setSpeed(CSpeed);
    }
  }

  digitalWrite(led0, sensVals[0]);
  digitalWrite(led1, sensVals[1]);
  digitalWrite(led2, sensVals[2]);

  lastEncoderValue = encoderValue;

  if ((millis() - lastTime > rotaryThreshold) && (encoderValue != B0100)) {
    rotaryMotor->setSpeed(RSpeed);
    
    while (encoderValue != B0100) { // until it reaches the 0 degrees marker
      cakeMotor->run(BACKWARD);

      //run cakeMotor whichever direction it was going to run anyway
      if (isClockwise) {
        cakeMotor->run(FORWARD);
      }
      else {
        cakeMotor->run(BACKWARD);
      }
    }

    rotaryMotor->setSpeed(0);
    clockNumber = 0;
    Serial.println("returned to 12");
  }

  if((millis() - lastWalk > cakeThreshold)) {
    cakeMotor->setSpeed(0);
  }
}

lastTime = millis();

{ if (isClockwise) {
    cakeMotor->run(FORWARD);
  }
  else {
    cakeMotor->run(BACKWARD);
  }
  delay(1000);
}
