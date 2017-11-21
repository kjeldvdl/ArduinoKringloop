#include <Wire.h>
#include <Servo.h>

Servo myservo;

int pos = 0;
//Slave for Motor

char c;

int S = 0;

void setup() {
  Wire.begin(18);
  Wire.onRequest(requestEvent); // data request to slave
  Wire.onReceive(receiveEvent); // data slave recieved
  Serial.begin(9600);
  myservo.attach(9);
}

void loop() {
}

void receiveEvent() {
  while (0 < Wire.available()) {
    byte x = Wire.read();
    c = x;
  }
}

void requestEvent() {
  // respond to the question
  if (c == 'B') {
    S = 1;
    Serial.println(S);
    Wire.write(S);
    Serial.println("Still going");

    if (S == 1) {
    
      myservo.write(180);              // tell servo to go to position in variable 'pos'
      delay(5);                       // waits 15ms for the servo to reach the position
    }
  }
    else {
      Serial.println("");
      myservo.write(0);              // tell servo to go to position in variable 'pos'
      delay(5); 
    }

  
  if (c != 'B'){
    S = 0;
    Wire.write(0);
    Serial.println("Stop");
  }
}
