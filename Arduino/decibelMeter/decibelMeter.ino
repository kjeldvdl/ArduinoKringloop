//these define the pin connections
const int microphonePin= 0; //the microphone positive terminal will connect to analog pin A0 to be read
const int ledGroenPin=2;
const int ledGeelPin=3;
const int ledRoodPin=4;

int sample; //the variable that will hold the value read from the microphone each time

const int thresholdGroen= 350;//the microphone threshold sound level at which the LED will turn on
const int thresholdGeel= 500;//the microphone threshold sound level at which the LED will turn on
const int thresholdRood= 535;//the microphone threshold sound level at which the LED will turn on

void setup() {
pinMode (ledGroenPin, OUTPUT);
pinMode (ledGeelPin, OUTPUT);
pinMode (ledRoodPin, OUTPUT);

Serial.begin(9600); //sets the baud rate at 9600 so we can check the values the microphone is obtaining on the Serial Monitor
}

void loop(){
sample= analogRead(microphonePin); //the arduino takes continuous readings from the microphone
Serial.println(sample);
if (sample >= thresholdRood) {
  digitalWrite (ledRoodPin, HIGH); //if the reading is greater than the threshold value, LED turns on
  digitalWrite (ledGroenPin, LOW); //LED turns off
  digitalWrite (ledGeelPin, LOW); //LED turns off
} else if (sample >= thresholdGeel) {
 digitalWrite (ledGeelPin, HIGH); //if the reading is greater than the threshold value, LED turns on
  digitalWrite (ledGroenPin, LOW); //LED turns off
  digitalWrite (ledRoodPin, LOW); //LED turns off
} else if (sample >= thresholdGroen) {
  digitalWrite (ledGroenPin, HIGH); //if the reading is greater than the threshold value, LED turns on
  digitalWrite (ledGeelPin, LOW); //LED turns off
  digitalWrite (ledRoodPin, LOW); //LED turns off
}


if (sample < thresholdGroen) {
  // Er is n ieder geval een lampje aangezet, we wachten even 500ms en zetten dan alle lampjes uit
  delay (500); //LED stays on for a half a second
  digitalWrite (ledGroenPin, LOW); //LED turns off
  digitalWrite (ledGeelPin, LOW); //LED turns off
  digitalWrite (ledRoodPin, LOW); //LED turns off
}


}
