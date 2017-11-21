#include "LedControl.h"
 
LedControl lc=LedControl(12,11,10,2);  // Pins: DIN,CLK,CS, # of Display connected
 
unsigned long delayTime=200;  // Delay between Frames
 
// Put values in arrays
byte invader1a[] =
{
   B00111100,  // First frame of eye #1
   B01000010,
   B10000001,
   B10011001,
   B10011001,
   B10000001,
   B01000010,
   B00111100
};
 
byte invader1b[] =
{
   B00111100,  // Second frame of eye #1
   B01000010,
   B10000001,
   B10011001,
   B10011001,
   B10000001,
   B01000010,
   B00111100
};
 
 
void setup() {
  lc.shutdown(0,false);  // Wake up displays
  lc.shutdown(1,false);
  lc.setIntensity(0,5);  // Set intensity levels
  lc.setIntensity(1,5);
  lc.clearDisplay(0);  // Clear Displays
  lc.clearDisplay(1);
}
 
 
//  Take values in Arrays and Display them
void sinvader1a() {
  for (int i = 0; i < 8; i++)  
  {
    lc.setRow(0,i,invader1a[i]);
  }
}
 
void sinvader1b() {
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0,i,invader1b[i]);
  }
}

 
void loop() {
// Put #1 frame on both Display
    sinvader1a();
    delay(delayTime);
 
 
// Put #2 frame on both Display
    sinvader1b();
    delay(delayTime);
 
}
