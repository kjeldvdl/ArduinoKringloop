// 888b    888                   8888888b.  d8b                   888 
// 8888b   888                   888   Y88b Y8P                   888 
// 88888b  888                   888    888                       888 
// 888Y88b 888  .d88b.   .d88b.  888   d88P 888 888  888  .d88b.  888 
// 888 Y88b888 d8P  Y8b d88""88b 8888888P"  888 `Y8bd8P' d8P  Y8b 888 
// 888  Y88888 88888888 888  888 888        888   X88K   88888888 888 
// 888   Y8888 Y8b.     Y88..88P 888        888 .d8""8b. Y8b.     888 
// 888    Y888  "Y8888   "Y88P"  888        888 888  888  "Y8888  888 
//
// NeoPixel-KnightRider
//
// A highly configurable Knight Rider (larson display) routine for your NeoPixels
// (WS2812 RGB LED)
//-----------------------------------------------------------------------------
// The MIT License (MIT)
//
// Copyright (c) 2013 Technobly - technobly@gmail.com - August 13th 2013
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//-----------------------------------------------------------------------------
// ASCII GEN http://patorjk.com/software/taag/#p=display&f=Colossal&t=NeoPixel
//
//-----------------------------------------------------------------------------

#include <Adafruit_NeoPixel.h>

// SETUP YOUR OUTPUT PIN AND NUMBER OF PIXELS
#define PIN A5
#define NUM_PIXELS  2

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIN, NEO_GRB);

void setup() {
  strip.begin();
  
  strip.setPixelColor(0, 0x000000);
  strip.setPixelColor(1, 0x000000);
   
  strip.show();

  delay(500);
}

void loop() {


  strip.setPixelColor(0, 0xFF0000);
  strip.setPixelColor(1, 0xFF0000);
  strip.show();
  delay(500);
  strip.setPixelColor(0, 0xFFFFFF);
  strip.setPixelColor(1, 0xFFFFFF);
  strip.show();
  delay(500);
  
  strip.setPixelColor(0, 0x000000);
  strip.setPixelColor(1, 0x000000);
  strip.show();
  delay(500);
  



  
}
