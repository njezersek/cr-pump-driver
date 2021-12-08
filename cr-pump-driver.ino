
#include "pump-interface.h"

// enable command
// unsigned char command[] = {0x01, 0x06, 0x00, 0xFE, 0x00, 0x03, 0xA8, 0x3B};

// start
// unsigned char command[] = {0x01, 0x06, 0x00, 0x00, 0x00, 0x01, 0x48, 0x0A};

// Set the address 01 peristaltic pump filling 8 times.
// unsigned char command[] = {0x01, 0x06, 0x00, 0x07, 0x00, 0x08, 0x39, 0xCD};

// Set the filling amount for the address 01 peristaltic pump to 8.9
// unsigned char command[] = {0x01, 0x10, 0x00, 0x01, 0x00, 0x02, 0x04, 0x41, 0x0E, 0x66, 0x66, 0xEC, 0x16};


void read(){
	while(softwareSerial.available()){  
		unsigned char c = softwareSerial.read();
		Serial.print(c, HEX);
		Serial.print(" ");
	}
	Serial.println(";");
}

void setup(){
	Serial.begin(9600);
	pumpInterfaceSetup(); 

	delay(100);

	Serial.println("enable");
	enableTransmit();
	enable();
	enableReceive();

	delay(100);
	read();
	delay(100);

	Serial.println("start");
	enableTransmit();
	setUShortReg(PUMP_START_STOP, 1);
	enableReceive();

	delay(100);
	read();
	delay(100);

	delay(3000);

	Serial.println("stop");
	enableTransmit();
	setUShortReg(PUMP_START_STOP, 0);
	enableReceive();

	delay(100);
	read();
	delay(100);

	while(true){}
} 
 
void loop() 
{ 
	// read output from the pump and print it to debug console
	bool received = false;
	while(softwareSerial.available()){  
		received = true;
		unsigned char c = softwareSerial.read();
		Serial.print(c, HEX);
		Serial.print(" ");
	}
	if(received) Serial.println(";");
}
