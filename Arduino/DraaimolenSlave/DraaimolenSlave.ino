#include <Wire.h>
#include <AccelStepper.h>

#define Draaimolen 8
#define HALFSTEP 8

// Motor pin definitions
#define motorPin1  2     // IN1 on the ULN2003 driver 1
#define motorPin2  3     // IN2 on the ULN2003 driver 1
#define motorPin3  4     // IN3 on the ULN2003 driver 1
#define motorPin4  5     // IN4 on the ULN2003 driver 1

int ledGroen = 7; // Groen betekent dat de attractie draait
int ledRood = 6; // Rood betekent dat de attractie stilstaat

int attractieDraait = 0; // 0 = Attractie staat stil, 1 = attractie draait
int geluidAfspelen = 0; 

char(c) = 'x';
int i = 0;

// Define a stepper and the pins it will use
AccelStepper stepper(HALFSTEP, motorPin1, motorPin3, motorPin2, motorPin4);

void setup() {

  Serial.begin(115200);

  pinMode(ledGroen, OUTPUT);  
  pinMode(ledRood, OUTPUT);  


  digitalWrite(ledGroen, LOW);
  digitalWrite(ledRood, HIGH);
  
  Wire.begin(Draaimolen);
  Wire.onRequest(requestEvent); // data request to slave
  Wire.onReceive(receiveEvent); // data slave recieved

  Serial.println("Een ritje in de draaimolen");

  stepper.setMaxSpeed(4000);
  stepper.setSpeed(2000);
  stepper.setAcceleration(25);
  
}

void loop() {

  if (c == 'B') { // Master stuurt een 'B' als er op de startknop van de draaimolen geklikt is.

    // We hebben de boodschap ontvangen dat de attractie mag gaan draaien
      digitalWrite(ledGroen, HIGH);
      digitalWrite(ledRood, LOW);

      attractieDraait = 1;
      c = 'A';

      Serial.println("Draaimolen draait");

      // Even het soundPaneel melden dat hij een melding mag geven
      geluidAfspelen = 1;
      Serial.println("Verzoek soundPanel verstuurd");

      // Even een pauze inlassen zodat eerst het geluid gespeeld wordt en daarna de draaimolen daadwerkelijk gaat draaien.
      delay(3500);
      
      stepper.moveTo(15000);    
      while (stepper.distanceToGo() > 0) {
        stepper.run();      
      }
         
      Serial.println("Draaimolen is klaar met draaien");

      // Even de zaken resetten zodat er een nieuwe draaimolenrun gedaan kan worden.
      digitalWrite(ledGroen, LOW);
      digitalWrite(ledRood, HIGH);
      attractieDraait = 0;
      stepper.setCurrentPosition(0);

  }

}

void receiveEvent() {
  Serial.println("Gegevens ontvangen van Master");
  while (0 < Wire.available()) {
    byte x = Wire.read();
    c = x;
  
  }
}

void requestEvent() {
  // respond to the question
    Serial.print("Verzoek van master, attractieDraait = ");
    Serial.println(attractieDraait);
    
    if (attractieDraait == 1){

      if (geluidAfspelen == 1) {
          Wire.write("DS");
          geluidAfspelen = 0;
      } else {
        Wire.write("D1");
        delay(500);
      }
      
    } else {
      Wire.write("D0");
      delay(5000);
    }
}
