#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Adafruit_NeoPixel.h>

#define LEDSTRIP 2
#define TOON 0
#define NUMPIXELS 16

int ledje = 1;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LEDSTRIP, NEO_GRB + NEO_KHZ400);

const char* ssid = "k-wlan";
const char* password = "kwlan2011";

ESP8266WebServer server(80);

void handleRoot() {

  server.send(200, "text/plain", "hello from esp8266!");

}

void handleNotFound(){
  
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  
}

void setup(void){

  pinMode(TOON, OUTPUT);

  pixels.begin();
  pixels.show(); // Initialize all pixels to 'off'

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inschrijving", [](){

    
    analogWrite(TOON, 500);
    delay(1000);
    analogWrite(TOON, 0);
    
    spinningLed();


    server.send(200, "text/plain", "Er is een inschrijving binnengekomen....");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}

void spinningLed() {

  for(int loop=1;loop<4;loop++){
    for(int spin=0;spin<NUMPIXELS;spin++){

      pixels.setPixelColor(spin, pixels.Color(0,loop*50,0)); // Moderately bright green color.
      pixels.show();

      
      delay(50);
    }
  }

  delay(5000);

    for(int loop=2;loop>=0;loop--){
    for(int spin=0;spin<NUMPIXELS;spin++){

      pixels.setPixelColor(spin, pixels.Color(0,loop*50,0)); // Moderately bright green color.
      pixels.show();
      delay(50);
    }
  }

}

