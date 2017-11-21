#include <IRremote.h>

int RECEIVE_PIN= 3;
IRrecv irreceiver(RECEIVE_PIN);
decode_results results;

const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;
 
void setup()
{
  Serial.begin(9600);
  irreceiver.enableIRIn(); // Start the receiver
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}
 
void loop() {
  if (irreceiver.decode(&results)) {
    Serial.println(results.value);

    switch (results.value) {
      case 356815237: {
        Serial.println("okRood");
        analogWrite(redPin, 75);
        analogWrite(greenPin, 0);
        analogWrite(bluePin, 0);
      }
      break;

      case 3056006823: {
        Serial.println("okGroen");
        analogWrite(redPin, 0);
        analogWrite(greenPin, 75);
        analogWrite(bluePin, 0);
      }
      break;

      case 1070850536: {
        Serial.println("okBlauw");
        analogWrite(redPin, 0);
        analogWrite(greenPin, 0);
        analogWrite(bluePin, 255);
      }
      break;

      case 1634033446: {
        Serial.println("okGeel");
        analogWrite(redPin, 255);
        analogWrite(greenPin, 75);
        analogWrite(bluePin, 0);
      }
      break;
      
      default:
      Serial.println("niet ok");
      analogWrite(redPin, 0);
      analogWrite(greenPin, 0);
      analogWrite(bluePin, 0);
      break;
    }
    delay(250);
    irreceiver.resume(); // Receive the next value //NEW LINE
  }
  delay(100); //NEW LINE
}
