/*
 *  This sketch sends a message to a TCP server
 *
 */

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti WiFiMulti;

String inputString = "";         // a string to hold incoming data

void setup() {
    Serial.begin(9600);
    delay(10);

    // We start by connecting to a WiFi network
    WiFiMulti.addAP("JKLM-2.4", "mnlk-5xgj-ccfk");

    Serial.println();
    Serial.println();
    Serial.print("Wait for WiFi... ");

    while(WiFiMulti.run() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    delay(500);
}


void loop() {

    // Als er iets in de Serial geplaatst wordt, een verbinding opzetten en de string versturen

    
    if ( Serial.available() >0 )  {

      inputString = "";

      
      while (Serial.available() >0) {
        // get the new byte:
        char inChar = (char)Serial.read();
        delay(100);
        // add it to the inputString:
        inputString += inChar;
        // if the incoming character is a newline, set a flag
        // so the main loop can do something about it:
        if (inChar == '\n') {
          break;
        }
      }
      
      String url = inputString;
      Serial.println(url);

      
      const uint16_t port = 80;
      const char * host = "192.168.2.45"; // ip or dns

      Serial.print("connecting to ");
      Serial.println(host);

      // Use WiFiClient class to create TCP connections
      WiFiClient client;

      if (!client.connect(host, port)) {
        Serial.println("connection failed");
        Serial.println("wait 5 sec...");
        delay(5000);
        return;
      }

      // We now create a URI for the request
      // String url = "/gpio/1";
    
      // This will send the request to the server
      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" + 
                 "Connection: close\r\n\r\n");

      //read back one line from server
      while(client.available()){
        String line = client.readStringUntil('\r');
        Serial.println(line);
        client.println(line);
      }

      Serial.println("closing connection");
      client.stop();
    
      Serial.println("wait 5 sec...");
      delay(5000);
  }
}
