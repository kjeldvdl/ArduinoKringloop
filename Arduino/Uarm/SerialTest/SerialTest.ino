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
    serialResponse = Serial.readStringUntil('\r\n');

    // Convert from String Object to String.
    char buf[sizeof(sz)];
    serialResponse.toCharArray(buf, sizeof(buf));
    char *p = buf;
    char *str;

    char* cmd[8];
    int i = 0;

    while ((str = strtok_r(p, " ", &p)) != NULL) {
      Serial.print(i);
      Serial.print("\t");
      Serial.println(str);
      cmd[i] = removeSpaces(str);
      i++;
    }

    Serial.println(cmd[0]);

    if (cmd[0][0] == 'm') {
      Serial.print("moveTo(");
      Serial.print(cmd[1]);
      Serial.print(",");
      Serial.print(cmd[2]);
      Serial.print(",");
      Serial.print(cmd[3]);
      Serial.println(");");

      String xtemp;
      String ztemp;
      String ytemp;
      
      moveTo(xtemp.toInt(),ztemp.toInt(),ytemp.toInt());
      xx = xtemp.toInt();
      y = ytemp.toInt();
      z = ztemp.toInt();
    } else if (cmd[0][0] == 'g') {
      Serial.print(xx);
      Serial.print(",");
      Serial.print(y);
      Serial.print(",");
      Serial.println(z);
    } else if (cmd[0][5] == 'n') {
      pumpOn();
    } else if (cmd[0][5] == 'f') {
      pumpOff();
    } else {
      Serial.println("Unknown command.");
    }

    Serial.println("");
  }
}

char* removeSpaces(char* s)
{
    char* cpy = s;  // an alias to iterate through s without moving s
    char* temp = s;

    while (*cpy)
    {
        if (*cpy != ' ')
            *temp++ = *cpy;
        cpy++;
    }
    *temp = 0;

    return s; // This prints out the desired result: abbcccd
}
