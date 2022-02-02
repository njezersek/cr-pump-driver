#include <HX711.h>

#include "pinout.h"
#include "mc4921.h"

HX711 scale;

uint32_t start, stop;


void setup(){
	pinMode(MCP4921_CS_PIN, OUTPUT);
	pinMode(PUMP_START_PIN, OUTPUT);
	pinMode(PUMP_DIRECTION_PIN, OUTPUT);
	pinMode(PEDAL_PIN, INPUT_PULLUP);

	digitalWrite(MCP4921_CS_PIN, HIGH);
	SPI.begin();

	Serial.begin(9600);

	scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

	scale.set_scale(237872.0f / 600.0f); // uncalibrated scale.get_units(10) / <actual wetight>
	scale.tare();
}


uint16_t value = 0;

unsigned long last_tick = 0;

float samples[3];
int sample_index = 0;

#define PEDAL_THRESHOLD 100
#define TARGET_WEIGHT 200
#define MAX_FLOW 500.0

unsigned long last_pedal_press = 0;
bool pedal_state = false;
bool pedal_state_filtered = false;
bool filling_state = false;

float ramp_up = 0.;

float median = 0;

void loop(){
	// read the scale
	if(scale.is_ready()){
		float s = scale.get_units();

		if(s < 1000 && s > -10){ // remove obious outliers
			samples[sample_index] = s;
			sample_index = (sample_index + 1) % 3;
		}


		if((samples[1] <= samples[0] && samples[0] <= samples[2]) || (samples[2] <= samples[0] && samples[0] <= samples[1]) ){
			median = samples[0];
		}
		else if((samples[0] <= samples[1] && samples[1] <= samples[2]) || (samples[2] <= samples[1] && samples[1] <= samples[0]) ){
			median = samples[1];
		}
		else if((samples[0] <= samples[2] && samples[2] <= samples[1]) || (samples[1] <= samples[2] && samples[2] <= samples[0]) ){
			median = samples[2];
		}
		else{
			median = -1000;
		}


		unsigned long t = micros();
		// Serial.print("\t"); Serial.print(s);
		// Serial.print("\t");	Serial.print(0);
		// Serial.print("\t");	Serial.print(300);
		// Serial.print("\t");	Serial.print(median);
		// Serial.print("\t"); Serial.print((t-last_tick)/1000.0f);
		// Serial.println();
		last_tick = t;
		// s = (s-300)/(460-300); // 300 - 460
		// if(s <= 0.0f) s = 0.0f;
		// if(s >= 1.0f) s = 1.0f;
		// value = (uint16_t) (s * 4095.0f);
	}

	// read pedal value
	float v = (1.0+sin((float) micros() / 200000.0))/2. * 4096;
	uint16_t value = (uint16_t) v;

	if(!digitalRead(PEDAL_PIN)){
		if(pedal_state == false){
			last_pedal_press = millis();
		}
		pedal_state = true;
		if(millis() - last_pedal_press > PEDAL_THRESHOLD){
			if(pedal_state_filtered == false){
				ramp_up = 0;
				filling_state = true;
				Serial.println("FILL!");
			}
			pedal_state_filtered = true;
		}
		else{
			pedal_state_filtered = false;
		}
	}
	else{
		pedal_state = false;
	}

	if(filling_state && median < TARGET_WEIGHT){
		digitalWrite(PUMP_START_PIN, HIGH);
		ramp_up += 0.01;
		if(ramp_up > 1) ramp_up = 1;
		float f = ramp_up - (median/(float)TARGET_WEIGHT);
		if(f < 0) f = 0;
		if(f > 1) f = 1;
		Serial.println(f);
		// mcp4921((uint16_t)(f*(float)MAX_FLOW));
	}
	else{
		digitalWrite(PUMP_START_PIN, LOW);
		// mcp4921(0);
		if(filling_state){
			Serial.print("Allocated ");
			Serial.print(scale.get_units(30));
			Serial.print("g ");
			Serial.println();
		}
		filling_state = false;
	}
	mcp4921(0);




}
