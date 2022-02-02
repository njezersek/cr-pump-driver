#include "pinout.h"
#include "mc4921.h"
#include "scale.h"


void setup(){
	pinMode(PUMP_START_PIN, OUTPUT);
	pinMode(PUMP_DIRECTION_PIN, OUTPUT);
	pinMode(PEDAL_PIN, INPUT_PULLUP);

	digitalWrite(PUMP_START_PIN, LOW);
	digitalWrite(PUMP_DIRECTION_PIN, LOW);

	SPI.begin();

	Serial.begin(9600);

	Scale::init();
}


#define TARGET_WEIGHT 200
#define STOPPING_WEIGHT 150
#define MAX_FLOW 1000
#define DRIP_WEIGHT 25

unsigned long filling_start_time = 0;
bool filling_state = false;

float ramp_up = 0;
float pump_speed = 0;

void loop(){

	// read the scale
	float weight = Scale::read();

	if(filling_state){ // filling state

		if(millis() - filling_start_time > 100){ // increase speed after 100ms
			ramp_up += 0.01;
			if(ramp_up > 1) ramp_up = 1;
			float f = ramp_up - max(0, max(0, weight-TARGET_WEIGHT+STOPPING_WEIGHT)/STOPPING_WEIGHT);
			// float f = ramp_up - (weight/(float)TARGET_WEIGHT);
			if(f < 0) f = 0;
			if(f > 1) f = 1;
			Serial.println(f);
			pump_speed = f * (float)MAX_FLOW;
		}

		if(weight >= TARGET_WEIGHT - DRIP_WEIGHT){
			filling_state = false;

			digitalWrite(PUMP_START_PIN, LOW);

			Serial.print("Allocated ");
			Serial.print((millis() - filling_start_time) / 1000.0);
			Serial.print("s ");
			Serial.print(weight);
			Serial.print("g ");
			Serial.println();

			for(int i=0; i<10; i++){
				Serial.print(Scale::scale.get_units(20));
				Serial.print("g ");
				Serial.println();
			}
		}
	}
	else{ // idle state
		// read pedal value
		if(!digitalRead(PEDAL_PIN)){
			Scale::scale.tare();
			filling_state = true;
			filling_start_time = millis();
		}
	}

	digitalWrite(PUMP_START_PIN, filling_state);
	mcp4921((uint16_t) pump_speed);
}
