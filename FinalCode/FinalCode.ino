#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
Adafruit_MotorShield AFMSbot(0x60); // rotary clock (no jumpers closed)
Adafruit_MotorShield AFMStop(0x61); // cake clock (rightmost jumper closed)

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#include <Servo.h>

// motor and LED setup code
Adafruit_StepperMotor *cakeMotor = AFMStop.getStepper(513, 1); //attached to M3 and M4 on the bottom motorshield
Adafruit_StepperMotor *rotaryMotor = AFMSbot.getStepper(513, 2); //attached to M1 and M2 on the top motershield
Adafruit_NeoPixel lighthouseLed = Adafruit_NeoPixel(1, 3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel sensorStrip = Adafruit_NeoPixel(3, 5, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel animalStrip = Adafruit_NeoPixel(11, 6, NEO_GRB + NEO_KHZ800);

// initialize cake clock's IR distance sensors
int sensor1Pin = A0;
int sensor2Pin = A1;
int sensor3Pin = A2;
int sensor1Value = 0;
int sensor2Value = 0;
int sensor3Value = 0;

boolean isClockwise = true;   // direction of cakeMotor
int sensVals[3] = {0, 0, 0};  // array to record order sensors are triggered
int threshold = 300;          // threshold to trigger distance sensors
boolean cakeTriggered = false;// if cakeTriggered==true, rotary clock can be triggered

int animalledCounter = 0;
int animalNumber = -1;
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
unsigned long interval = 500;
int ledState = 0;

unsigned long lastWalk = 0;   //how long it's been since someone walked around the cake clock
//int rotaryThreshold = 5000;   //wait 5 seconds after face doesn't move to bring back to 12 position
int cakeTimeThreshold = 30000;    //wait 30 seconds after ppl stop walking around to stop moving

// rotary clock variables
boolean rotaryTriggered = false; 
const int enc1Pin = 10;
const int enc2Pin = 9;
const int enc3Pin = 8;
const int enc4Pin = 7;

// sensor values. enc1 is far outer edge, enc4 is inner edge.
int enc1 = 0;
int enc2 = 0;
int enc3 = 0;
int enc4 = 0;

// what the encoder reads now and what it read on the last loop
byte encoderValue = B0000;
byte lastEncoderValue = B0000;

int clockNumber = 0;          // at 12 position

//krakuation variables
int krakenPin = A3; 
int pressurePadValue = 500; //this puts it above the threshhold
int krakenThreshold = 350; //value read on the kraken pad to indicate that it has been stomped on
Servo krakenServo; //kraken servo 
int inKrakenPos = 30; //good values for kracutation
int outKrakenPos = 55;
boolean lightHouseTriggered = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  AFMSbot.begin();
  AFMStop.begin();

  // cake clock setup code starts LEDs and cakeMotor
  sensorStrip.begin();
  sensorStrip.show();
  animalStrip.begin();
  animalStrip.show();
  cakeMotor->setSpeed(10);
  rotaryMotor->setSpeed(10);
  
//  lighthouse setup
  lighthouseLed.begin();
  lighthouseLed.show();
  krakenServo.attach(11); //kraken servo is attached to pwm pin 11
}

void loop() {
  Serial.println("running"); 
//   puts servo in correct position at beginning
  if (millis() < 2000) {
    krakenServo.write(inKrakenPos); 
  }
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

    cakeTriggered = true;           // cake clock has been triggered

    delay(500);
    // turns on the LEDs corresponding to distance sensors that were triggered
    sensorStrip.setPixelColor(0, 0);
    sensorStrip.setPixelColor(1, 0);
    sensorStrip.setPixelColor(2, 0);
    sensorStrip.show();

    //animalNumber is now rotary puzzle solution
    if (animalNumber < 0) {
      animalNumber = random(11);
      Serial.println(animalNumber);
      for(int x = 0; x <= 11; x ++) {
        animalStrip.setPixelColor(x, animalStrip.Color(20, 20, 20));
      }
      animalStrip.show();
      animalledCounter = animalNumber;
    }

     lastWalk = millis(); // time that the person last triggered all 3 sensors 
     Serial.println("cake clock triggered"); 
  }

//turns motor if cake clock has been triggered within cakeTimeThreshold
if ((millis() - lastWalk < cakeTimeThreshold) && cakeTriggered) {
  if (isClockwise) {
    cakeMotor->step(10, FORWARD, SINGLE);
  }
  else {
    cakeMotor->step(10, BACKWARD, SINGLE); 
  }
}
  
  if (cakeTriggered) { //runs all of the rotary clock code
    Serial.print("led counter: ");
    Serial.println(animalledCounter);
    Serial.print("animal number: "); 
    Serial.println(animalNumber);  
    //makes the LED in the light house blink
      currentMillis = millis(); 
      if (animalledCounter < animalNumber) {

        if ((currentMillis - previousMillis) > interval) {
          previousMillis = millis();
          if (ledState == 0) {
            lighthouseLed.setPixelColor(0, lighthouseLed.Color(0, 255, 0));
            ledState = 1;
            lighthouseLed.show();
            Serial.println("led triggered"); 
          }
          else {
            lighthouseLed.setPixelColor(0, lighthouseLed.Color(0, 0, 0));
            ledState = 0;
            animalledCounter = animalledCounter + 1;
            lighthouseLed.show();
          }
    
        }
    }
    if ((animalledCounter == (animalNumber))) {
      if ((currentMillis - previousMillis) > (interval + 2000)) { //creates a long pause
        if (ledState == 1) {
          Serial.println("yay1");
          previousMillis = millis();
          ledState = 0;
          animalledCounter = 0;
        }
        else { 
          Serial.println("yay2"); 
          lighthouseLed.setPixelColor(0, lighthouseLed.Color(0, 0, 0));
          lighthouseLed.show();
          previousMillis = millis();
          ledState = 1;
        }
      }
    }
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
    if (clockNumber == animalNumber) { // we have achieved our goal
      //turns sensor LEDs blue
      sensorStrip.setPixelColor(0, sensorStrip.Color(0, 0, 50)); 
      sensorStrip.setPixelColor(1, sensorStrip.Color(0, 0, 50));
      sensorStrip.setPixelColor(2, sensorStrip.Color(0, 0, 50));
      sensorStrip.show();

      //lighthouse strip turns blue
      lighthouseLed.setPixelColor(0, lighthouseLed.Color(0, 0, 255));
      lighthouseLed.show(); 

      rotaryTriggered = true;
    }

    if (rotaryTriggered && (clockNumber != 0)){
      rotaryMotor->step(10, FORWARD, SINGLE);
    }
  } //end of just rotary clock code

//start of krackuation

if (rotaryTriggered && cakeTriggered) {
  pressurePadValue = analogRead(krakenPin); 
  
  if (pressurePadValue >= krakenThreshold) {
    lighthouseLed.setPixelColor(0, lighthouseLed.Color(255, 100, 0));
    lighthouseLed.show();
    delay(1000); // pause before kractuation
    krakenServo.write(outKrakenPos);
    delay(200); //wait for the spring to get into position
    krakenServo.write(inKrakenPos);
    delay(2000); 
    lightHouseTriggered = true;
  }

  if (lightHouseTriggered) { //resets everything
    cakeTriggered = false;
    rotaryTriggered = false;
    lightHouseTriggered = false;
    
    animalNumber = -1; 
    sensVals[0] = 0;
    sensVals[1] = 0;
    sensVals[2] = 0;
    
    //turn all of the LEDs off
    lighthouseLed.setPixelColor(0, lighthouseLed.Color(0, 0, 0));
    lighthouseLed.show(); 
    for(int x = 0; x <= 11; x ++) {
        animalStrip.setPixelColor(x, animalStrip.Color(0, 0, 0));
    }
    animalStrip.show();
    sensorStrip.setPixelColor(0, 0);
    sensorStrip.setPixelColor(1, 0);
    sensorStrip.setPixelColor(2, 0);
    sensorStrip.show();
  }
}
}
