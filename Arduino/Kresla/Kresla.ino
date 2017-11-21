#include <NewPing.h>

#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 300 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

int remLicht = 8;
int knipperLinks = 6;
int knipperRechts = 7;
int afstand = 0;
int kopLamp = 5;
int lichtSensor = 0;
int lichtWaarde = 0;
int alarmAantal = 0;


unsigned long timerLichtCheck;

void setup() {
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
  pinMode(remLicht, OUTPUT);
  digitalWrite(remLicht, LOW);

  pinMode(knipperLinks, OUTPUT);
  digitalWrite(knipperLinks, LOW);

  pinMode(knipperRechts, OUTPUT);
  digitalWrite(knipperRechts, LOW);

  pinMode(kopLamp, OUTPUT);
  digitalWrite(kopLamp, LOW);

  pinMode(lichtSensor, INPUT);

  timerLichtCheck = millis();
  
}

void loop() {
  
  
  delay(250);                      // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.

  
  // Bepaal Afstand
  unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
  afstand = uS / US_ROUNDTRIP_CM;
  Serial.print("AF:");
  Serial.println(afstand); // Convert ping time to distance in cm and print result (0 = outside set distance range)
 
  if (afstand >0 && afstand < 100 ) {

    Serial.println("AC:Remmen"); 
      
    if (digitalRead(remLicht) == LOW) {  
      digitalWrite(remLicht, HIGH);
    }

    if (alarmAantal++ <6) {     // Na 3 x knipperen gaan de alarmlichten uit, ook als de rem ingetrapt blijft.

      // Alarmlichten aan om achterliggers te waarschuwen
      if (digitalRead(knipperLinks) == LOW){
        digitalWrite(knipperLinks, HIGH);
        digitalWrite(knipperRechts, HIGH);
      } else {
        digitalWrite(knipperLinks, LOW);
        digitalWrite(knipperRechts, LOW);
      }

    } 
  }    
  else {
    // Remmen los, we kunnen weer
    digitalWrite(remLicht, LOW);
    digitalWrite(knipperLinks, LOW);
    digitalWrite(knipperRechts, LOW);

    alarmAantal = 0;
    
  }

  // Bepaal licht i.v.m. automatisch schakelen verlichting
  if (millis()-timerLichtCheck >= 3500UL) {
    
    lichtWaarde = analogRead(lichtSensor);
    Serial.print("LW:");
    Serial.println(lichtWaarde);

    timerLichtCheck = millis(); // Nieuwe timer instellen
    if (lichtWaarde >= 900) {
      Serial.println("ST:Te donker, lampen aan");
      digitalWrite(kopLamp, HIGH);
    } else {
      digitalWrite(kopLamp, LOW);
    }
  }

  Serial.print("D05:"); Serial.println(digitalRead(kopLamp));
  Serial.print("D06:"); Serial.println(digitalRead(knipperLinks));
  Serial.print("D07:"); Serial.println(digitalRead(knipperRechts));
  Serial.print("D08:"); Serial.println(digitalRead(remLicht));
  
}



