
// zoomkat 3-28-14 serial servo incremental test code
// using serial monitor type a character (s to increase or a 
// to decrease) and enter to change servo position 
// (two hands required, one for letter entry and one for enter key)
// use strings like 90x or 1500x for new servo position 
// for IDE 1.0.5 and later
// Powering a servo from the arduino usually *DOES NOT WORK*.

#include<Servo.h>
String readString;
Servo myservo;
int pos=1500; //~neutral value for continous rotation servo
//int pos=90;

void setup()
{
  myservo.attach(10); //, 400, 2600); //servo control pin, and range if desired
  Serial.begin(9600);
  Serial.println("serial servo incremental test code");
  Serial.println("type a character (s to increase or a to decrease)");
  Serial.println("and enter to change servo position");
  Serial.println("use strings like 90x or 1500x for new servo position");
  Serial.println();
}

void loop()
{
  while (Serial.available()) {
    char c = Serial.read();  //gets one byte from serial buffer
    readString += c; //makes the string readString
    delay(2);  //slow looping to allow buffer to fill with next character
  }
  if (readString.length() >0) {
    if(readString.indexOf('x') >0) { 
      pos = readString.toInt();
    }

    if(readString =="a"){
      (pos=pos-100); //use larger numbers for larger increments
      if(pos<0) (pos=0); //prevent negative number
    }
    if (readString =="s"){
      (pos=pos+100);
    }

    if(pos >= 400) //determine servo write method
    {
      Serial.println(pos);
      myservo.writeMicroseconds(pos);
    }
    else
    {   
      Serial.println(pos);
      myservo.write(pos); 
    }
  }
  readString=""; //empty for next input
}

