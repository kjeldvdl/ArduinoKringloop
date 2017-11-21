/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo myservoX;  // create servo object to control a servo
Servo myservoY;
// twelve servo objects can be created on most boards

int posX = 0;    // variable to store the servo position
int posY = 0;
const int joyX = 3;
const int joyY = 2;
const int joyButton = 2;

void setup() {
  Serial.begin(9600);
  myservoX.attach(9);  // attaches the servo on pin 9 to the servo object
  myservoY.attach(10);
  pinMode(joyX, INPUT);
  pinMode(joyY, INPUT);
  pinMode(joyButton, INPUT);
  
  
}

void loop() {
  posX = analogRead(joyX);
  posY = analogRead(joyY);

    Serial.print("PosX: ");
  Serial.println(posX);
  Serial.print("PosY: ");
  Serial.println(posY);
  
  posX = map(posX, 0, 1023, 20, 150);
  posY = map(posY, 0, 1023, 0, 179);

  myservoX.write(posX);
  myservoY.write(posY);
  delay(500);
  
  
  
  
  
  /*for (pos = 0; pos <= 160; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo1.write(pos);              // tell servo to go to position in variable 'pos'
    myservo2.write(pos);
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 160; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo1.write(pos);              // tell servo to go to position in variable 'pos'
    myservo2.write(pos);
    delay(15);                       // waits 15ms for the servo to reach the position
  }*/
}

