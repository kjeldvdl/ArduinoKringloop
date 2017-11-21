#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Adafruit_NeoPixel.h>

#define LEDSTRIP 1
#define NUMPIXELS 8

int ledje = 1;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LEDSTRIP, NEO_GRB + NEO_KHZ400);

const char* ssid = "JKLM-2.4";
const char* password = "mnlk-5xgj-ccfk";

ESP8266WebServer server(80);

void handleRoot() {

  server.send(200, "text/plain", "on air lamp");

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

  server.on("/rood", [](){
    spinningLed(255, 0, 0);
    server.send(200, "text/plain", "rode lampjes aangezet");
  });

  server.on("/groen", [](){
    spinningLed(0, 255, 0);
    server.send(200, "text/plain", "groene lampjes aangezet");
  });

 server.on("/oranje", [](){
    spinningLed(235, 132, 1);
    server.send(200, "text/plain", "oranje lampjes aangezet");
  });

 server.on("/uit", [](){
    spinningLed(0, 0, 0);
    server.send(200, "text/plain", "alle lampjes uitzetten");
 });
 




  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}

void spinningLed(int r, int g , int b) {

int j = 0;

  for(j=0; j<8; j++) {
    pixels.setPixelColor(j, pixels.Color(r,g,b)); // Moderately bright green color.
  }
  pixels.show();

}

