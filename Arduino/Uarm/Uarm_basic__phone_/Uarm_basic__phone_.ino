// Demo 
#include "uArm.h"

#define USE_SERIAL_CMD	0	// 1: use serial for control	0: just use arduino to control(release ROM and RAM space)

unsigned long tickStartTime = millis(); // get timestamp;

char val; // variable to receive data from the serial port

 

void setup()
{
	Serial.begin(115200);
	Init(); // Don't remove

	debugPrint("debug start"); // uncomment DEBUG in uArmConfig.h to use debug function
	
	// TODO
	service.setButtonService(false);	// disable build in button service
	moveTo(0, 150, 150);

}

void loop(){
	run(); // Don't remove
  if( Serial.available() );       // if data is available to read
 {
   val = Serial.read();         // read it and store it in 'val'
   Serial.print("I received: ");
   Serial.println(val);
 }
 if( val == 'A' ){  // turn ON the LED
  moveTo(0, 150, 150); //Base pos
  moveTo(100, 150, 83);
  pumpOn();
  moveTo(-150, 200, 150);
  pumpOff();
  delay(100);                                   // wait 100ms
 }

 if( val == 'B' ){
  moveTo(0,175,83);
  pumpOn();
  moveTo(0,250,150);
  moveTo(0,350,100);
  pumpOff();
  moveTo(0,300,200);
  moveTo(0,200,150);
  delay(100);
  
 }
	// TODO
  //(x,z,y)
  moveTo(0,150,150);
}

// time out every TICK_INTERVAL(50 ms default)
void tickTimeOut()
{
	
}

////////////////////////////////////////////////////////////
// DO NOT EDIT
void Init()
{
	uArmInit();	// Don't remove
	service.init();

	#if USE_SERIAL_CMD == 1
	serialCmdInit();
	

	#endif
}

void run()
{
	#if USE_SERIAL_CMD == 1
	handleSerialCmd();
	#endif

	manage_inactivity(); // Don't remove
}

void tickTaskRun()
{
	tickTimeOut();

    buttonPlay.tick();
    buttonMenu.tick();
#ifdef MKII
    ledRed.tick();
    service.btDetect();
#endif    
}

void manage_inactivity(void)
{
#if USE_SERIAL_CMD == 1
	getSerialCmd();	// for serial communication
#endif
	service.run();	// for led, button, bt etc.

	// because there is no other hardware timer available in UNO, so use a soft timer
	// it's necessary for button,led, bt
	// so Don't remove it if you need them
	if(millis() - tickStartTime >= TICK_INTERVAL)
	{
		tickStartTime = millis();
		tickTaskRun();
	}   
}
