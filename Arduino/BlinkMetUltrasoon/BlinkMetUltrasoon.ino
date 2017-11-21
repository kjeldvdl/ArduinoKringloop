#include <NewPing.h>

#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 300 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

int afstand = 0;

// the setup function runs once when you press reset or power the board
void setup() {

  Serial.begin(115200);
  
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(2, OUTPUT);
  pinMode(7, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {

  delay(250);

  // Bepaal Afstand
  unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
  afstand = uS / US_ROUNDTRIP_CM;


  Serial.println( afstand );

  if (afstand >0 && afstand <30) {

    digitalWrite(2, HIGH);
    digitalWrite(7, LOW);   
    delay(1000);                       
  
    digitalWrite(2, LOW);    
    digitalWrite(7, HIGH);   
    delay(1000);

  } else {

    digitalWrite(2, LOW);    
    digitalWrite(7, LOW);   
    
  }
  
}
