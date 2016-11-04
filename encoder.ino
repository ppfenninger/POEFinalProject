// pins the sensors are on. Note that these are DIGITAL so we need to set up a threshold.
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
int rotaryDirection = 0;      // takes -1, 0, and 1 values, where 1 is clockwise
unsigned long lastTime = 0;   // helps us tell how long it's been since the face moved
int timeThreshold = 5000;     // wait 5 seconds after face doesn't move to bring back to 12 position

void setup() {
  // put your setup code here, to run once:

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

  rotaryDirection = encoderValue - lastEncoderValue;
  
  lastEncoderValue = encoderValue;

  if((millis() - lastTime > timeThreshold) && (encoderValue != B0100)){
    // take clock back to 12 position: this requires a loop where it doesn't check lastTime
    while(encoderValue != B0100){ // until it reaches the 0 degrees marker
        // run motor backward
    }
    clockNumber = 0;
  }

  lastTime = millis();
}
