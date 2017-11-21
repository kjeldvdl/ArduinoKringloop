/*
 *  This sketch sends a message to a TCP server
 *
 */

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti WiFiMulti;

void setup() {
    Serial.begin(115200);
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
    String url = "/gpio/1";
    
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

