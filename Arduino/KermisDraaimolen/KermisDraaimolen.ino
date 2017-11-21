

// Include the required Wire library for I2C
#include <Wire.h>



int LED = 13;
int x = 0;
void setup() {
  
  Serial.begin(9600);
  
  // Define the LED pin as Output
  pinMode (LED, OUTPUT);
  // Start the I2C Bus as Slave on address 9
  Wire.begin(9); 
  // Attach a function to trigger when something is received.
  Wire.onReceive(receiveEvent);
}
void receiveEvent(int bytes) {
  x = Wire.read();    // read one character from the I2C

}
void loop() {

  Serial.println(x);
  //If value received is 0 blink LED for 200 ms
  if (x == 1) {
    Serial.println("Waarde 1 ontvangen van het control panel");
    x=0;
    digitalWrite(LED, HIGH);
    delay(200);
    digitalWrite(LED, LOW);
    delay(1200);

    // Terugkoppeling naar master dat we klaar zijn met de actie
    Wire.beginTransmission(1); // transmit to device #1
    Wire.write(2);              // sends x 
    Wire.endTransmission();    // stop transmitting
    digitalWrite(LED, HIGH);
    Serial.println("Getal 2 verstuurd over bus ten behoeve van master");
  } else {delay(500);}
}
