#include "SoftwareSerial.h"

SoftwareSerial esp8266(6, 7); // RX, TX

void setup()
{
  Serial.begin(115200); // serial port used for debugging
  esp8266.begin(9600);  // your ESP's baud rate might be different
}
 
void loop()
{
  if(esp8266.available())  // check if the ESP is sending a message
  {
    while(esp8266.available())
    {
      char c = esp8266.read();  // read the next character.
      Serial.write(c);  // writes data to the serial monitor
    }
  }
 
  if(Serial.available())
  {
    delay(10);  // wait to let all the input command in the serial buffer

    // read the input command in a string
    String cmd = "";
    while(Serial.available())
    {
      cmd += (char)Serial.read();
    }

    // print the command and send it to the ESP
    Serial.println("---------------------");
    Serial.print(">> ");
    Serial.println(cmd);
    Serial.println("---------------------");
    esp8266.println(cmd); // send the read character to the esp8266
  }
}
