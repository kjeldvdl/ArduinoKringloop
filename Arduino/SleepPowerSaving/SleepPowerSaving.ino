//**********************Arduino Code***********************************
/*
Example program for using sleep modes in Arduino. This example code was used in a sleep mode tutorial video on the ForceTronics YouTube Channel.
This code is open for anybody to use at their own risk

The 5 different modes are:
     *     SLEEP_MODE_IDLE         -the least power savings
     *     SLEEP_MODE_ADC
     *     SLEEP_MODE_PWR_SAVE
     *     SLEEP_MODE_STANDBY
     *     SLEEP_MODE_PWR_DOWN
These are the arguments used to set the sleep mode in the function set_sleep_mode()
     */
     
#include <avr/sleep.h>

int led = 13; //variable for pin that the LED is on
int count = 0; //variable to control how many times LED blinks before sleep

void setup() {
   //sleep_enable(); //enable the sleep capability
   set_sleep_mode(SLEEP_MODE_PWR_SAVE); //set the type of sleep mode. Default is Idle
   pinMode(led, OUTPUT); //set up the LED pin to output
}

void loop() {
  
  if(count < 4) { //For first four loops blink the LED
    digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(900);               // wait 
    digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
    delay(900);       // wait 
    count++; //increment count
  }
  else { //after blinking LED setup interrupt and then go to sleep. Note that sleep will only happen once since it is disabled in ISR
    sleep_enable(); //enable the sleep capability
    attachInterrupt(0, interruptFunction, HIGH); 
    sleep_cpu(); //enter sleep mode. Next code that will be executed is the ISR when interrupt wakes Arduino from sleep
    count = 0; //Set the count back to zero 
  }
   
}

//This is the function called when the interrupt occurs (pin 2 goes high)
//this is often referred to as the interrupt service routine or ISR
//This cannot take any input arguments or return anything
void interruptFunction() {
 detachInterrupt(0); //this function call will turn the interrupt off
 sleep_disable(); //Disable the sleep mode so even if call to sleep is executed again it will be ignored
}
