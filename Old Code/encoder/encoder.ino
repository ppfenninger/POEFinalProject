#include <Wire.h>
// 3 = 1101 (11)
// 6 = 0010(2)
// 9 = 1101 (6)
// pins the sensors are on. Note that these are DIGITAL so we need to set up a threshold.
const int enc1Pin = 10;
const int enc2Pin = 9;
const int enc3Pin = 8;
const int enc4Pin = 7;

// sensor values. enc1 is far outer edge, enc4 is inner edge.
int enc1 = 0;
int enc2 = 0;
int enc3 = 0;
int enc4 = 0;

byte encoderValue = B0000;
byte lastEncoderValue = B0000;

int clockNumber = 0;          //at 12 position
int rotaryDirection = 0;      // takes -1, 0, and 1 values, where 1 is clockwise
unsigned long lastTime = 0;   // helps us tell how long it's been since the face moved
int timeThreshold = 5000;     // wait 5 seconds after face doesn't move to bring back to 12 position
uint8_t RSpeed = 100; 
uint8_t baseRotarySpeed = 0; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
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
  Serial.print(enc1);
  Serial.print(enc2);
  Serial.print(enc3);
  Serial.print(enc1);
  Serial.print("    "); 
  Serial.println(clockNumber); 
  
}
