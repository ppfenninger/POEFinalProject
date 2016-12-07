int analogInPin = A5; 
int sensorValue = 0; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  //pinMode(5, INPUT_PULLUP);  
}

void loop() {
  // put your main code here, to run repeatedly:
  sensorValue = analogRead(analogInPin); 
  Serial.println(sensorValue); 

}
