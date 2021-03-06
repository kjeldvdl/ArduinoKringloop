#include <NewPing.h>

#include <SoftwareSerial.h> 
#include <SparkFunESP8266WiFi.h>

#define TRIGGER_PIN  6  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     7  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 300 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

/*
#define STILSTAND 'S'
#define VOORUIT   'V'
#define ACHTERUIT 'A'
*/

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

int remLicht = 2; // pos in array shiftByte
int knipperLinks = 0; // pos in array shiftByte
int knipperRechts = 0; // pos in array shiftByte
int kopLamp = 1; // pos in array shiftByte

boolean WeRijden = false;
char Richting = 'V';

int afstand = 0;

int lichtSensor = A5;
int lichtWaarde = 0;
int alarmAantal = 0;

// Verlichting
//Pin connected to latch pin (ST_CP) of 74HC595/ groen
const int verlichtinglatchPin = 3;
//Pin connected to clock pin (SH_CP) of 74HC595/ blauw
const int verlichtingclockPin = 2;
////Pin connected to Data in (DS) of 74HC595 / wit
const int verlichtingdataPin = 4;

// Motor
//Pin connected to latch pin (ST_CP) of 74HC595/ blauw
const int motorlatchPin = 12;
//Pin connected to clock pin (SH_CP) of 74HC595/ groen
const int motorclockPin = 11;
////Pin connected to Data in (DS) of 74HC595 / geel
const int motordataPin = 13;


// in ShiftByte wordt de waarde geplaatst van de corresponderende shiftValue
// Bij het verzenden van de gegevens worden de waardes bij elkaar opgeteld zodat 
// alle benodigde poorten aangezet worden.
int verlichtingshiftByte[] = {0,0,0,0,0,0,0,0};
int verlichtingshiftValue[] = {1,2,4,8,16,32,128};


unsigned long timerLichtCheck;

int getal = 0;
int lus = 0;

// Wifi settings
const char mySSID[] = "JKLM-2.4";
const char myPSK[] = "mnlk-5xgj-ccfk";

// Server initialiseren
ESP8266Server server = ESP8266Server(80);

const String htmlHeader = "HTTP/1.1 200 OK\r\n"
                          "Content-Type: text/html\r\n"
                          "Connection: close\r\n\r\n"
                          "<!DOCTYPE HTML>\r\n"
                          "<html>\r\n";

void setup() {

  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
  Serial.println("Kresla 1.0");

  pinMode(lichtSensor, INPUT);

  pinMode(verlichtinglatchPin, OUTPUT);
  pinMode(verlichtingdataPin, OUTPUT);  
  pinMode(verlichtingclockPin, OUTPUT);

  pinMode(motorlatchPin, OUTPUT);
  pinMode(motordataPin, OUTPUT);  
  pinMode(motorclockPin, OUTPUT);

  timerLichtCheck = millis();

  // Initialize ESP8266 shield
  if (initializeESP8266()){

    // Connect to given Wifi
    connectESP8266();

    // Toon connectie info van het gekoppelde wifi netwerk
    displayConnectInfo();

    // Opzetten van een http-server
    serverSetup();
  }
  
  
}

void loop() {
  
  delay(250);                      // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
    
  // Bepaal Afstand
  unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
  afstand = uS / US_ROUNDTRIP_CM;
  Serial.print("AF:");
  Serial.println(afstand); // Convert ping time to distance in cm and print result (0 = outside set distance range)

  if (afstand >0 && afstand < 30 ) {
  

    Serial.print("Richting: ");
    Serial.println( Richting);
    Serial.print("We rijden: ");
    Serial.println(WeRijden);

    
    if (Richting == 'V') {

      if (WeRijden == true){

        Serial.println("AC:Remmen"); 
        verlichtingshiftByte[remLicht] = verlichtingshiftValue[remLicht];
        
        // Motor stoppen
        digitalWrite(motorlatchPin, LOW);
        shiftOut(motordataPin, motorclockPin, MSBFIRST, 255);
        digitalWrite(motorlatchPin, HIGH); 
        WeRijden = false;
    
        
        Serial.println("Richting in zijn achteruit");
        Richting = 'A';
        
      }
      

    }      

    
  } else {

    // Afstand niet kunnen meten of obstakel ver genoeg verwijderd
    // Als we niet rijden kunnen we rijden

    Serial.print("geen obstakel: ");
    Serial.println(afstand);


    if (afstand == 0) {

      // Eigenlijk niets doen
      
    } else {

      if (WeRijden == false){
        Richting = 'V'; 
      }

    }
  } 


  
  
  if (Richting == 'A'){

    // Linksachteruit
    
    if (WeRijden == false){
      digitalWrite(motorlatchPin, LOW);
      shiftOut(motordataPin, motorclockPin, MSBFIRST, 34);  // 100
      digitalWrite(motorlatchPin, HIGH); 
      
      WeRijden = true;
    }

    // Alarmlichten aan om achterliggers te waarschuwen
    if (verlichtingshiftByte[knipperLinks] == 0){
      verlichtingshiftByte[knipperLinks] = verlichtingshiftValue[knipperLinks];
      verlichtingshiftByte[knipperRechts] = verlichtingshiftValue[knipperRechts];
    } else {
      verlichtingshiftByte[knipperLinks] = 0;
      verlichtingshiftByte[knipperRechts] = 0;
    }

  }

  if (Richting == 'V'){
    
    if (WeRijden == false){
      digitalWrite(motorlatchPin, LOW);
      shiftOut(motordataPin, motorclockPin, MSBFIRST, 85);  //85
      digitalWrite(motorlatchPin, HIGH);

      WeRijden = true;

      // Als we voouit rijden kunnen alarmlichten en remlichten uit
      verlichtingshiftByte[knipperLinks] = 0;
      verlichtingshiftByte[knipperRechts] = 0;
          
      Serial.println("Remlicht wordt uitgezet"); 
      verlichtingshiftByte[remLicht] = 0;
    
    }

    

  }

  // Bepaal licht i.v.m. automatisch schakelen verlichting
  if (millis()-timerLichtCheck >= 3500UL) {
    
    lichtWaarde = analogRead(lichtSensor);
    //Serial.print("LW:");
    //Serial.println(lichtWaarde);

    timerLichtCheck = millis(); // Nieuwe timer instellen
    if (lichtWaarde <410) {
      //Serial.println("ST:Te donker, lampen aan");
    
      verlichtingshiftByte[kopLamp] = verlichtingshiftValue[kopLamp];
    } else {
      verlichtingshiftByte[kopLamp] = 0;
    }
  }

  // Alle bytes uit shiftByte bij elkaar optellen en versturen
  getal = 0;

  for (lus = 0; lus<8; lus++){
    getal = getal + verlichtingshiftByte[lus];
  }
  
  digitalWrite(verlichtinglatchPin, LOW);
  shiftOut(verlichtingdataPin, verlichtingclockPin, MSBFIRST, getal);
  digitalWrite(verlichtinglatchPin, HIGH);


  // En naar de internetpagina van de draaiende server
  //serverDemo();


  
}









/////////////////////////// WIFI functions //////////////////////////////////



boolean initializeESP8266()
{
  // esp8266.begin() verifies that the ESP8266 is operational
  // and sets it up for the rest of the sketch.
  // It returns either true or false -- indicating whether
  // communication was successul or not.
  // true
  int test = esp8266.begin();
  if (test != true)
  {
    Serial.println(F("Error talking to ESP8266."));
    errorLoop(test);
  } else {
    Serial.println(F("ESP8266 Shield Present"));
  }
  return test;
}

boolean connectESP8266()
{
  // The ESP8266 can be set to one of three modes:
  //  1 - ESP8266_MODE_STA - Station only
  //  2 - ESP8266_MODE_AP - Access point only
  //  3 - ESP8266_MODE_STAAP - Station/AP combo
  // Use esp8266.getMode() to check which mode it's in:
  int retVal = esp8266.getMode();
  if (retVal != ESP8266_MODE_STA)
  { // If it's not in station mode.
    // Use esp8266.setMode([mode]) to set it to a specified
    // mode.
    retVal = esp8266.setMode(ESP8266_MODE_STA);
    if (retVal < 0)
    {
      Serial.println(F("Error setting mode."));
      errorLoop(retVal);
      return false;
    }
  }
  Serial.println(F("Mode set to station"));

  // esp8266.status() indicates the ESP8266's WiFi connect
  // status.
  // A return value of 1 indicates the device is already
  // connected. 0 indicates disconnected. (Negative values
  // equate to communication errors.)
  retVal = esp8266.status();
  if (retVal <= 0)
  {
    Serial.print(F("Connecting to "));
    Serial.println(mySSID);
    // esp8266.connect([ssid], [psk]) connects the ESP8266
    // to a network.
    // On success the connect function returns a value >0
    // On fail, the function will either return:
    //  -1: TIMEOUT - The library has a set 30s timeout
    //  -3: FAIL - Couldn't connect to network.
    retVal = esp8266.connect(mySSID, myPSK);
    if (retVal < 0)
    {
      Serial.println(F("Error connecting"));
      errorLoop(retVal);
      return false;
    }
  }

  return true;
}

void displayConnectInfo()
{
  char connectedSSID[24];
  memset(connectedSSID, 0, 24);
  // esp8266.getAP() can be used to check which AP the
  // ESP8266 is connected to. It returns an error code.
  // The connected AP is returned by reference as a parameter.
  int retVal = esp8266.getAP(connectedSSID);
  if (retVal > 0)
  {
    Serial.print(F("Connected to: "));
    Serial.println(connectedSSID);
  }

  // esp8266.localIP returns an IPAddress variable with the
  // ESP8266's current local IP address.
  IPAddress myIP = esp8266.localIP();
  Serial.print(F("My IP: ")); Serial.println(myIP);
}

void serverSetup()
{
  // begin initializes a ESP8266Server object. It will
  // start a server on the port specified in the object's
  // constructor (in global area)
  server.begin();
  Serial.print(F("Server started! Go to "));
  Serial.println(esp8266.localIP());
  Serial.println();
}

void serverDemo()
{
  // available() is an ESP8266Server function which will
  // return an ESP8266Client object for printing and reading.
  // available() has one parameter -- a timeout value. This
  // is the number of milliseconds the function waits,
  // checking for a connection.
  ESP8266Client client = server.available(500);
  
  if (client) 
  {
    
    boolean currentLineIsBlank = true;
    while (client.connected()) 
    {
      if (client.available()) 
      {
        char c = client.read();
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) 
        {
          Serial.println(F("Sending HTML page"));
          // send a standard http response header:
          client.print(htmlHeader);
          String htmlBody;
          // output the value of each analog input pin
          for (int a = 2; a < 13; a++)
          {
            htmlBody += "D";
            htmlBody += String(a);
            htmlBody += ":";
            htmlBody += String(digitalRead(a));
            htmlBody += "<br>\n";
          }
          htmlBody += "</html>\n";
          client.print(htmlBody);
          break;
        }
        if (c == '\n') 
        {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') 
        {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
   
    // close the connection:
    client.stop();
    Serial.println(F("Client disconnected"));
  }
  
}

// errorLoop prints an error code, then loops forever.
void errorLoop(int error)
{
  Serial.print(F("Error: ")); Serial.println(error);
  //Serial.println(F("Looping forever."));
  //for (;;)
  //  ;
}


