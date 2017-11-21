
#include <Wire.h>
#include <rgb_lcd.h>

// the setup function runs once when you press reset or power the board

#define PIR_MOTION_SENSOR 0
#define LIGHT_SENSOR_PIN A2 //Use analog pin A2 for the sensor
#define TEMP_SENSOR_PIN A6 //Use analog pin A6 for the sensor

#define ALARM_LED_PIN 4 //D4

#define THRESHOLD_VALUE 15 //Activation threshold
#define TEMP_THRESHOLD 20 // Temp warning threshold

char buf[10];
byte motionSR = 0;
float fLight;
float fTemp;

// Temp sensor constants
const int B = 4275;               // B value of the thermistor
const int R0 = 100000;            // R0 = 100k

rgb_lcd lcd;

void setup() {
  delay(1000);

  // initialize digital pin LED_BUILTIN as an output.
  SerialUSB.println("Configuring pins");
  pinMode(ALARM_LED_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIR_MOTION_SENSOR, INPUT);
  digitalWrite(ALARM_LED_PIN, HIGH);
  SerialUSB.println("Init LCD");
  delay(250);
  lcd.begin(16, 2);
  delay(250);
  lcd.setRGB(255, 255, 255);
  SerialUSB.println("Greeting on LCD");
  lcd.print("Green2School");
  lcd.display();
  SerialUSB.println("Init complete");
}

void ReadPIR()
{
  int sensorValue = digitalRead(PIR_MOTION_SENSOR);
  motionSR != 0x40;
  motionSR = (motionSR << 1) | sensorValue;

  SerialUSB.print("PIR: ");
  SerialUSB.print(sensorValue);
  SerialUSB.print("-> ");
  SerialUSB.println(motionSR);
}

bool PeoplePresent()
{
  return motionSR != 0;
}

void ReadLight()
{
  //Read the analog value from the light sensor
  int a = analogRead(LIGHT_SENSOR_PIN);
  //Calculate the resistance from the sensor
  fLight = (float)(1023 - a) * 10 / a;
  SerialUSB.print("light = ");
  SerialUSB.println(fLight);
}

void ReadTemp()
{
  //Read the analog value from the light sensor
  int a = analogRead(TEMP_SENSOR_PIN);
  //Calculate the resistance from the sensor
  float R = 1023.0 / a - 1.0;
  R = R0 * R;

  fTemp = 1.0 / (log(R / R0) / B + 1 / 298.15) - 273.15; // convert to temperature via datasheet

  SerialUSB.print("temperature = ");
  SerialUSB.println(fTemp);
}

// the loop function runs over and over again forever
void loop()
{
  SerialUSB.println("loop");
  ReadPIR();
  ReadLight();
  ReadTemp();
  if (PeoplePresent())
  {
    // turn the LED off
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(ALARM_LED_PIN, LOW);
  }
  else
  {
    sprintf(buf, "%f", fLight);
    lcd.clear();
    lcd.print(buf);

    //Compare the calculated resistance against the threshold
    if (fLight < THRESHOLD_VALUE)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(ALARM_LED_PIN, HIGH);
    }
    else
    {
      digitalWrite(LED_BUILTIN, LOW);   // turn the LED off
      digitalWrite(ALARM_LED_PIN, LOW);
    }
   /* 
    if (fTemp > TEMP_THRESHOLD)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(ALARM_LED_PIN, HIGH);
    }
    else
    {
      digitalWrite(LED_BUILTIN, LOW);   // turn the LED off
      digitalWrite(ALARM_LED_PIN, LOW);
    }
*/
  }
  delay(1000);
}

