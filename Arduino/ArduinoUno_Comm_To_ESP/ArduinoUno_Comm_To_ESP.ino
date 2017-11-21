// Basic serial communication with ESP8266
// Uses serial monitor for communication with ESP8266
//
//  Pins
//  Arduino pin 2 (RX) to ESP8266 TX
//  Arduino pin 3 to voltage divider then to ESP8266 RX
//  Connect GND from the Arduiono to GND on the ESP8266
//  Pull ESP8266 CH_PD HIGH
//
// When a command is entered in to the serial monitor on the computer 
// the Arduino will relay it to the ESP8266
//

#include "VoiceRecognitionV3.h"
#include <SoftwareSerial.h>
SoftwareSerial ESPserial(6,7); // RX | TX

/**        
  Connection
  Arduino    VoiceRecognitionModule
   2   ------->     TX
   3   ------->     RX
*/
VR myVR(4,5);    // 5:RX 4:TX, you can choose your favourite pins.

uint8_t records[7]; // save record
uint8_t buf[64];

#define DayRecord    (1)
#define NightRecord   (2) 

void printSignature(uint8_t *buf, int len)
{
  int i;
  for(i=0; i<len; i++){
    if(buf[i]>0x19 && buf[i]<0x7F){
      Serial.write(buf[i]);
    }
    else{
      Serial.print("[");
      Serial.print(buf[i], HEX);
      Serial.print("]");
    }
  }
}

/**
  @brief   Print signature, if the character is invisible, 
           print hexible value instead.
  @param   buf  -->  VR module return value when voice is recognized.
             buf[0]  -->  Group mode(FF: None Group, 0x8n: User, 0x0n:System
             buf[1]  -->  number of record which is recognized. 
             buf[2]  -->  Recognizer index(position) value of the recognized record.
             buf[3]  -->  Signature length
             buf[4]~buf[n] --> Signature
*/
void printVR(uint8_t *buf)
{
  Serial.println("VR Index\tGroup\tRecordNum\tSignature");

  Serial.print(buf[2], DEC);
  Serial.print("\t\t");

  if(buf[0] == 0xFF){
    Serial.print("NONE");
  }
  else if(buf[0]&0x80){
    Serial.print("UG ");
    Serial.print(buf[0]&(~0x80), DEC);
  }
  else{
    Serial.print("SG ");
    Serial.print(buf[0], DEC);
  }
  Serial.print("\t");

  Serial.print(buf[1], DEC);
  Serial.print("\t\t");
  if(buf[3]>0){
    printSignature(buf+4, buf[3]);
  }
  else{
    Serial.print("NONE");
  }
  Serial.println("\r\n");
}
 
void setup() 
{
    Serial.begin(115200);     // communication with the host computer
    //while (!Serial)   { ; }
 
    // Start the software serial for communication with the ESP8266
    ESPserial.begin(9600);
    delay(1000);
    myVR.begin(9600);
 
    if(myVR.clear() == 0){
    Serial.println("Recognizer cleared.");
  }else{
    Serial.println("Not find VoiceRecognitionModule.");
    Serial.println("Please check connection and restart Arduino.");
    while(1);
  }
  
  if(myVR.load((uint8_t)DayRecord) >= 0){
    Serial.println("onRecord loaded");
  }
  
  if(myVR.load((uint8_t)NightRecord) >= 0){
    Serial.println("offRecord loaded");
  } 
}
 
void loop() 
{

    // listen for communication from the ESP8266 and then write it to the serial monitor
    if ( ESPserial.available() )   {  Serial.write( ESPserial.read() );  }

    int ret;
  ret = myVR.recognize(buf, 50);
  //Serial.println(ret);
  if(ret>0){
    switch(buf[1]){
      case NightRecord:
        // turn on LED 
        delay(100);
        ESPserial.write( "/gpio/1" );
        printVR(buf);
        break;
      case DayRecord:
        // turn off LED
        delay(100);
        ESPserial.write( "/gpio/0" );
        printVR(buf);
        break;
      default:
        Serial.println("Record function undefined");
        break;
    }
    // voice recognized 

      //delay(1000);

    }
}
