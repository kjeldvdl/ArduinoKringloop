#include <DigisparkOLED.h>
#include <Wire.h>

void setup() 
{
  //initialise the display
  oled.begin();

  //make sure that displays blank...
  oled.clear(); //all black
}

void loop() 
{
  oled.setCursor(0, 0); //top left
  oled.setFont(FONT8X16);
  oled.println("You can print");
  oled.println("  \\ or )  ");
}
