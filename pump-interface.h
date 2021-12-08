#include <SoftwareSerial.h>

#include "crc16.h"

// pins
#define TE_PIN 5 	// transmit enable (driver enable)
#define RE_PIN 4	// recieve enable (inverted)
#define RX_PIN 10 	// receive signal
#define TX_PIN 11 	// transmit signal

// internal registers
#define PUMP_START_STOP 0
#define PUMP_FILLING_VOLUME 1
#define PUMP_STOP_TIME 3
#define PUMP_FILLING_TIME 5
#define PUMP_FILLING_FREQUENCY 7 // ushort
#define PUMP_FLOw_RATE 8
#define PUMP_HEAD 10 // ushort, read only
#define PUMP_TUBE 11 // ushort, read only
#define PUMP_MODE 12 // ushort, read only
#define PUMP_DIRECTON 13 // ushort
#define PUMP_FULL_SPEED 14 // ushort
#define PUMP_SUCTION_SPEED 15 // ushort
#define PUMP_SUCTION_ANGLE 16 // ushort
#define PUMP_EXTERNAL_CONTROLL_OUTPUT 19 // ushort
#define PUMP_CALIBRATION_AMOUNT 20
#define PUMP_ACTUAL_AMOUNT 22
#define PUMP_K_VALUE 252
#define PUMP_485_STATE 254 // ushort

#define DEBUG

uint8_t pumpID = 1;

SoftwareSerial softwareSerial(RX_PIN, TX_PIN, 0); // RX, TX, invert signal

void pumpInterfaceSetup(){
	softwareSerial.begin(9600);
	softwareSerial.listen();
	pinMode(RE_PIN, OUTPUT);
	pinMode(TE_PIN, OUTPUT);
}

void enableTransmit(){
	digitalWrite(RE_PIN, HIGH); 
	digitalWrite(TE_PIN, HIGH);
}

void enableReceive(){
	digitalWrite(RE_PIN, LOW); 
	digitalWrite(TE_PIN, LOW);
}

void sendCommand(uint8_t* command, size_t size){
	softwareSerial.write(command, size);
	softwareSerial.flush(); // wait for transmition to finish
}

void debugPrintCommand(uint8_t* command, size_t size){
	Serial.print("Command: ");
	for(int i=0; i<size; i++){
		Serial.print(command[i], HEX);
		Serial.print(" ");
	}
	Serial.println();
}

void setUShortReg(uint16_t reg, uint16_t value){
	uint8_t command[8];
	uint32_t v = *(uint32_t*)&value;

	command[0] = pumpID; 					// Device addres
	command[1] = 0x06; 					// Function code (set float)
	command[2] = (reg & 0xFF00) >> 8; 	// Register addres (HIGH)
	command[3] = (reg & 0x00FF) >> 0; 	// Register addres (LOW)

	// data
	command[4] = (v & 0x0000FF00) >> 8;
	command[5] = (v & 0x00000FF) >> 0;

	// CRC-16
	uint16_t c = crc16(command, 6);
	command[6] = (c & 0x00FF) >> 0;
	command[7] = (c & 0xFF00) >> 8;

	#ifdef DEBUG
	debugPrintCommand(command, sizeof(command));
	#endif

	sendCommand(command, sizeof(command));
}

void setFloatReg(uint16_t reg, float value){
	uint8_t command[13];
	uint32_t v = *(uint32_t*)&value;

	command[0] = pumpID; 					// Device addres
	command[1] = 0x10; 					// Function code (set float)
	command[2] = (reg & 0xFF00) >> 8; 	// Register addres (HIGH)
	command[3] = (reg & 0x00FF) >> 0; 	// Register addres (LOW)
	command[4] = 0x00; 					// Undefined
	command[5] = 0x02; 					// The number of registers holding a float (always 2 - LOW and HIGH)
	command[6] = 0x04; 					// The number of bytes

	// data
	command[7] = (v & 0xFF000000) >> 24;
	command[8] = (v & 0x00FF0000) >> 16;
	command[9] = (v & 0x0000FF00) >> 8;
	command[10] = (v & 0x00000FF) >> 0;

	// CRC-16
	uint16_t c = crc16(command, 11);
	command[11] = (c & 0x00FF) >> 0;
	command[12] = (c & 0xFF00) >> 8;

	#ifdef DEBUG
	debugPrintCommand(command, sizeof(command));
	#endif

	sendCommand(command, sizeof(command));
}

void enable(){
	setUShortReg(0xFE, 3);
}

void test(){
	Serial.println("Hello World!");

	//setFloatReg(1, 1, 8.9);

}
