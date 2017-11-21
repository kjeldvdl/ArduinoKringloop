#include <NewPing.h>
#include <SoftwareSerial.h>

//Pin connected to latch pin (ST_CP) of 74HC595/ blauw
const int latchPin = 12;
//Pin connected to clock pin (SH_CP) of 74HC595/ groen
const int clockPin = 11;
////Pin connected to Data in (DS) of 74HC595 / geel
const int dataPin = 13;

#define TRIGGER_PIN  6  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     7  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 300 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

boolean WeRijden = false;
int afstand = 0;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

//This lets you run the loop a single time for testing
boolean run = true;

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);

  /*

  digitalWrite(latchPin, LOW);
   // shift the bits out:
  shiftOut(dataPin, clockPin, MSBFIRST, 85);

  // turn on the output so the LEDs can light up:
  digitalWrite(latchPin, HIGH);

  delay(4000);
  
  digitalWrite(latchPin, LOW);
   // shift the bits out:
  shiftOut(dataPin, clockPin, MSBFIRST, 255);

  // turn on the output so the LEDs can light up:
  digitalWrite(latchPin, HIGH);

  delay(4000);

  digitalWrite(latchPin, LOW);
   // shift the bits out:
  shiftOut(dataPin, clockPin, MSBFIRST, 170);

  // turn on the output so the LEDs can light up:
  digitalWrite(latchPin, HIGH);

  delay(4000);

    digitalWrite(latchPin, LOW);
    // shift the bits out:
    shiftOut(dataPin, clockPin, MSBFIRST, 255);
    // turn on the output so the LEDs can light up:
    digitalWrite(latchPin, HIGH);  
  



  // turn on the output so the LEDs can light up:
  digitalWrite(latchPin, HIGH);  

  */

  
}

void loop() {


    unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
    afstand = uS / US_ROUNDTRIP_CM;

    if (afstand >0 && afstand < 30 ) {

      if (WeRijden == true) {
      
        // Remmen!
        digitalWrite(latchPin, LOW);
        // shift the bits out:
        shiftOut(dataPin, clockPin, MSBFIRST, 255);
        // turn on the output
        digitalWrite(latchPin, HIGH);  

        delay(1000);

        // Linksachteruit
        digitalWrite(latchPin, LOW);
        // shift the bits out:
        shiftOut(dataPin, clockPin, MSBFIRST, 34);  // 100
        // turn on the output
        digitalWrite(latchPin, HIGH);  

        delay(3000);


        WeRijden = false;
      }
      
    } else {

      if (WeRijden == false) {
        digitalWrite(latchPin, LOW);
        // shift the bits out:
        shiftOut(dataPin, clockPin, MSBFIRST, 85);  //85
        // turn on the output
        digitalWrite(latchPin, HIGH);

        WeRijden = true;
      }
    }
    
}


