// Include the required Wire library for I2C
#include <Wire.h>

int x = 0;
void setup() {

  Serial.begin(9600);
  delay(500);
  Serial.println("Kermis controlpanel");
  
  // Start the I2C Bus as Master
  Wire.begin(1); 

  Wire.beginTransmission(9); // transmit to device #9
  Wire.write(1);              // sends x 
  Wire.endTransmission();    // stop transmitting

    // Attach a function to trigger when something is received.
  Wire.onReceive(receiveEvent);
}

void receiveEvent(int bytes) {
  x = Wire.read();    // read one character from the I2C

}

void loop() {

  
  Wire.requestFrom(9, 1);    // request 6 bytes from slave device #9

  while(Wire.available())    // slave may send less than requested
  { 
    char c = Wire.read();    // receive a byte as character
    Serial.print(c);         // print the character
  }

  delay(500);
  
    
}
