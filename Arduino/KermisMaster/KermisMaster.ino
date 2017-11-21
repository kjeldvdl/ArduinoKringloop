#include <Wire.h>

#define Draaimolen 8
#define Botsauto 9
#define Schip 10
#define Bank 11

#define meldingBlijvenZitten 52

int statusAttractie[11] = {0,0,0,0,0,0,0,0,0,0,0};
char S;
String result;

unsigned long opgeslagenTijd = millis();


void setup() {

  Serial.begin(115200);
  
  // Poorten voor starten attracties definieren
  pinMode(Draaimolen, INPUT);

  // Poorten voor geluid definieren
  pinMode(meldingBlijvenZitten, OUTPUT);
  
  // Zorgen dat de meldingen HIGH zijn zodat ze niet afgespeeld worden.
  digitalWrite(meldingBlijvenZitten, HIGH);
  
  Serial.println("Kermis control panel");

  Wire.begin();

}

void loop() {

  Serial.print(".");
  
  if (digitalRead(Draaimolen) == HIGH && statusAttractie[Draaimolen] == 0 ){

    Serial.println("Draaimolen heeft opdracht gekregen om te gaan draaien");

    Wire.beginTransmission(Draaimolen);
    Wire.write("B");
    Wire.endTransmission();

    statusAttractie[Draaimolen] = 1; 
    
  } else if (statusAttractie[Draaimolen] == 1 ){
      
      // We willen iedere 3 seconden een vraag naar de draaimolensturen
      if (millis()-opgeslagenTijd > 3000){
        Wire.requestFrom(Draaimolen, 2);      
        Serial.println("Er wordt een verzoek naar de draaimolen gestuurd om te kijken wat de status is.");  

        opgeslagenTijd = millis();
      }
      
  }

  // Als er een waarde terugkomt
  result = "" ;
  
  while(Wire.available())    // slave may send less than requested
  { 
    char S = Wire.read();    // receive a byte as character
    Serial.print(S);         // print the character
    result += S;
  }

  Serial.print("Resultaat = ");
  Serial.println(result);


  if (result == "D0") {
      Serial.println("Draaimolen is weer gestopt");
      statusAttractie[Draaimolen] = 0;
  } else if (result == "DS") {
      Serial.println("Melding blijven zitten");
      soundPanel(meldingBlijvenZitten);
  }
  

}



void soundPanel(int pin) 
{

  digitalWrite(pin, LOW);
  delay(500);
  digitalWrite(pin, HIGH);

}

