int redPin=11;  //set red LED pin to 11
int greenPin=10; //set green LED pin to 10
int bluePin=6; //set blue LED pin to 6
int brightness=100; //Set brightness to 100
 
void setup() {
  // put your setup code here, to run once:
 
Serial.begin(9600); //Turn on Serial port
pinMode(redPin, OUTPUT); //Set redPin to be an output
pinMode(greenPin, OUTPUT); //Set greenPin to be an output
pinMode(bluePin, OUTPUT); //set bluePin to be an output
 
}
 
void loop() {
  // put your main code here, to run repeatedly:
  
  analogWrite(redPin, 0); //turn off red pin
  analogWrite(greenPin, 0); //turn off green pin
  analogWrite(bluePin, brightness); //write 100 (brightness) to blue pin
}

