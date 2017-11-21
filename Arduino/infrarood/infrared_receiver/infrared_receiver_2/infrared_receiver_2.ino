#include <IRremote.h>

int RECEIVE_PIN= 3;
IRrecv irreceiver(RECEIVE_PIN);
decode_results results;

const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;
int aantal = 0;
 
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
      case 16724175: {
        Serial.println("okRood");
        Rood(1000);
      }
      break;

      case 16718055: {
        Serial.println("okGroen");
        Groen(1000);
      }
      break;

      case 16743045: {
        Serial.println("okBlauw");
        Blauw(1000);
      }
      break;

      case 16716015: {
        Serial.println("okGeel");
        Geel(1000);
      }
      break;

      case 16726215: {
        Serial.println("Hoe vaak wil je de disco laten herhalen?");
        while(Serial.available() ==0) {} //doe iets anders
        aantal = Serial.parseInt();
        Serial.println("disco");
        Disco(aantal);
      }
      
      default:
      Serial.println("niet ok");
      Blank();
      break;
    }
    delay(250);
    irreceiver.resume(); // Receive the next value //NEW LINE
  }
  delay(100); //NEW LINE
}

void Rood(int Pauze) {
  analogWrite(redPin, 255);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 0);
  delay(Pauze);
}

void Groen(int Pauze) {
  analogWrite(redPin, 0);
  analogWrite(greenPin, 255);
  analogWrite(bluePin, 0);
  delay(Pauze);
}

void Geel(int Pauze) {
  analogWrite(redPin, 255);
  analogWrite(greenPin, 75);
  analogWrite(bluePin, 0);
  delay(Pauze);
}

void Blauw(int Pauze) {
  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 255);
  delay(Pauze);
}

void Blank() {
  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 0);
}

void Disco(int aantal) {
  for(int i=0; i<aantal; i++) {
    Rood(500);
    Groen(500);
    Geel(500);
    Blauw(500);
  }
}

