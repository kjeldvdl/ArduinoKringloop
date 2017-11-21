#include "uArm.h"
#include "Arduino.h"

String serialResponse = "";
char sz[] = "moveto 1024 1024 1024";

int xx = 0;
int y = 150;
int z = 150;

void setup()
{
  Serial.begin(9600);
  Serial.setTimeout(5);
  //moveTo(0,150,150);
}

void loop()
{
  if ( Serial.available()) {
    serialResponse = Serial.readStringUntil(' ');

    if (serialResponse == "moveTo") {
      String xt = Serial.readStringUntil(' ');
      String zt = Serial.readStringUntil(' ');
      String yt = Serial.readStringUntil(' ');

      Serial.print("moveTo(");
      Serial.print(xt);
      Serial.print(",");
      Serial.print(zt);
      Serial.print(",");
      Serial.print(yt);
      Serial.print(");");

      xx = xt.toInt();
      y = yt.toInt();
      z = zt.toInt();

      moveTo(xx,z,y);
    }
    if (serialResponse == "get") {
      Serial.print(xx);
      Serial.print(",");
      Serial.print(y);
      Serial.print(",");
      Serial.println(z);
    }
    if (serialResponse == "pumpOn") {
      pumpOn();
    }
    if (serialResponse == "pumpOff") {
      pumpOff();
    }

    Serial.println("");
  }
}
