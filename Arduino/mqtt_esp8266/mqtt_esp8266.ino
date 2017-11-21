/*
 Basic ESP8266 MQTT example

 This sketch demonstrates the capabilities of the pubsub library in combinationm
 with the ESP8266 board/library.

 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.

 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"

*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <Wire.h>
#include "font.h"

// Update these with values suitable for your network.

const char* ssid = "JKLM-2.4";
const char* password = "mnlk-5xgj-ccfk";

const char* mqtt_server = "192.168.2.166";
//const char* mqtt_server = "eu.thethingsnetwork";


#define OLED_address  0x3D      // Adresse de l'écran OLED sur le bus I2C

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {

  // OLed communication
  Wire.pins(0, 2);                  // Brochaes sda, scl sur lequel est branché l'écran OLD SD1306 sur l'ESP-01.
  Wire.begin();
  StartUp_OLED(); 
  clear_display();

  sendStrXY("Welkom......", 0, 1);
  
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");

  sendStrXY("Wifi Connected", 0, 1);
  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  //sendStrXY(WiFi.localIP(), 1, 1);
}

void callback(char* topic, byte* payload, unsigned int length) {

  clear_display();
  
  char ingelezen[] = "";
  
  sendStrXY("Message arrived", 0, 1);
  Serial.print("Message arrived [");
  Serial.print(topic);
  sendStrXY(topic, 1, 1);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    ingelezen[i] = (char)payload[i];
  }
  
  Serial.println(ingelezen);
  sendStrXY(ingelezen, 4, 1);  

  Serial.println(); 

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
      client.subscribe("test");
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 15000) {
    lastMsg = now;
    ++value;
    snprintf (msg, 75, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("outTopic", msg);
  }
}







//////// OLED LIBRARY ///////////
static void reset_display(void)
{
  displayOff();
  clear_display();
  displayOn();
}


void StartUp_OLED()
{
  init_OLED();
  reset_display();
  displayOff();
  setXY(0,0);
  clear_display();
  displayOn();
}


void displayOn(void)
{
    sendcommand(0xaf);        //display on
}


void displayOff(void)
{
  sendcommand(0xae);    //display off
}


static void clear_display(void)
{
  unsigned char i,k;
  for(k=0;k<8;k++)
  { 
    setXY(k,0);    
    {
      for(i=0;i<128;i++)     //clear all COL
      {
        SendChar(0);         //clear all COL
      }
    }
  }
}


// Actually this sends a byte, not a char to draw in the display. 
static void SendChar(unsigned char data) 
{
  Wire.beginTransmission(OLED_address);  // begin transmitting
  Wire.write(0x40);                      //data mode
  Wire.write(data);
    Wire.endTransmission();              // stop transmitting
}


// Prints a display char (not just a byte) in coordinates X Y,
static void sendCharXY(unsigned char data, int X, int Y)
{
  //if (interrupt && !doing_menu) return; // Stop printing only if interrupt is call but not in button functions
  setXY(X, Y);
  Wire.beginTransmission(OLED_address); // begin transmitting
  Wire.write(0x40);//data mode
  
  for(int i=0;i<8;i++)
    Wire.write(pgm_read_byte(myFont[data-0x20]+i));
    
  Wire.endTransmission();    // stop transmitting
}


static void sendcommand(unsigned char com)
{
  Wire.beginTransmission(OLED_address);     //begin transmitting
  Wire.write(0x80);                          //command mode
  Wire.write(com);
  Wire.endTransmission();                    // stop transmitting
}


// Set the cursor position in a 16 COL * 8 ROW map.
static void setXY(unsigned char row,unsigned char col)
{
  sendcommand(0xb0+row);                //set page address
  sendcommand(0x00+(8*col&0x0f));       //set low col address
  sendcommand(0x10+((8*col>>4)&0x0f));  //set high col address
}


// Prints a string regardless the cursor position.
static void sendStr(unsigned char *string)
{
  unsigned char i=0;
  while(*string)
  {
    for(i=0;i<8;i++)
    {
      SendChar(pgm_read_byte(myFont[*string-0x20]+i));
    }
    *string++;
  }
}


// Prints a string in coordinates X Y, being multiples of 8.
// This means we have 16 COLS (0-15) and 8 ROWS (0-7).
static void sendStrXY( char *string, int X, int Y)
{
  setXY(X,Y);
  unsigned char i=0;
  while(*string)
  {
    for(i=0;i<8;i++)
    {
      SendChar(pgm_read_byte(myFont[*string-0x20]+i));
    }
    *string++;
  }
}


// Inits oled and draws logo at startup
static void init_OLED(void)
{
  sendcommand(0xae);                //display off
  sendcommand(0xa6);                //Set Normal Display (default)
    sendcommand(0xAE);              //DISPLAYOFF
    sendcommand(0xD5);              //SETDISPLAYCLOCKDIV
    sendcommand(0x80);              // the suggested ratio 0x80
    sendcommand(0xA8);              //SSD1306_SETMULTIPLEX
    sendcommand(0x3F);
    sendcommand(0xD3);              //SETDISPLAYOFFSET
    sendcommand(0x0);               //no offset
    sendcommand(0x40 | 0x0);        //SETSTARTLINE
    sendcommand(0x8D);              //CHARGEPUMP
    sendcommand(0x14);
    sendcommand(0x20);              //MEMORYMODE
    sendcommand(0x00);              //0x0 act like ks0108
    //sendcommand(0xA0 | 0x1);      //SEGREMAP   //Rotate screen 180 deg
    sendcommand(0xA0);
    //sendcommand(0xC8);            //COMSCANDEC  Rotate screen 180 Deg
    sendcommand(0xC0);
    sendcommand(0xDA);              //0xDA
    sendcommand(0x12);              //COMSCANDEC
    sendcommand(0x81);              //SETCONTRAS
    sendcommand(0xCF);
    sendcommand(0xd9);              //SETPRECHARGE 
    sendcommand(0xF1);
    sendcommand(0xDB);              //SETVCOMDETECT                
    sendcommand(0x40);
    sendcommand(0xA4);              //DISPLAYALLON_RESUME        
    sendcommand(0xA6);              //NORMALDISPLAY             

  clear_display();
  sendcommand(0x2e);            // stop scroll
  //----------------------------REVERSE comments----------------------------//
  //  sendcommand(0xa0);    //seg re-map 0->127(default)
  //  sendcommand(0xa1);    //seg re-map 127->0
  //  sendcommand(0xc8);
  //  delay(1000);
  //----------------------------REVERSE comments----------------------------//
  // sendcommand(0xa7);  //Set Inverse Display  
  // sendcommand(0xae);   //display off
  sendcommand(0x20);            //Set Memory Addressing Mode
  sendcommand(0x00);            //Set Memory Addressing Mode ab Horizontal addressing mode
  // sendcommand(0x02);         // Set Memory Addressing Mode ab Page addressing mode(RESET)  
  
 //  setXY(0,0);
  // Display Logo here :)
//  for(int i=0;i<128*8;i++)     // show 128* 64 Logo
 // {
  //  SendChar(pgm_read_byte(logo+i));
 // }
 // sendcommand(0xaf);    //display on
  
 // delay(5000); 
}


/*
------------------------------------------------------------------------------------
              Added to Mikes routine Draw routines
------------------------------------------------------------------------------------
*/


void Draw_WIFI()
{
   clear_display();
   setXY(0,0);
   // Display Logo here :)
   for(int i=0;i<128*8;i++)     // show 128* 64 Logo
   {
    SendChar(pgm_read_byte(WIFI1+i));
   }
   sendcommand(0xaf);   //display on
}


void Draw_WAVES()
{
   clear_display();
   setXY(0,0);
   // Display Logo here :)
   for(int i=0;i<128*8;i++)     // show 128* 64 Logo
   {
    SendChar(pgm_read_byte(rfwaves+i));
   }
   sendcommand(0xaf);   //display on
}

