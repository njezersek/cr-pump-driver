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
	//SPI.begin();

	Serial.begin(9600);

	scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

	scale.set_scale(237872.0f / 600.0f); // uncalibrated scale.get_units(10) / <actual wetight>
	scale.tare();
}


uint16_t value = 0;

unsigned long last_tick = 0;

float samples[3];
int sample_index = 0;

void loop(){
	// read the scale
	if(scale.is_ready()){
		float s = scale.get_units();

		if(s < 1000 && s > -10){ // remove obious outliers
			samples[sample_index] = s;
			sample_index = (sample_index + 1) % 3;
		}


		float median = s;
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
		Serial.print("\t");	Serial.print(0);
		Serial.print("\t");	Serial.print(300);
		Serial.print("\t");	Serial.print(median);
		// Serial.print("\t"); Serial.print((t-last_tick)/1000.0f);
		Serial.println();
		last_tick = t;
		// s = (s-300)/(460-300); // 300 - 460
		// if(s <= 0.0f) s = 0.0f;
		// if(s >= 1.0f) s = 1.0f;
		// value = (uint16_t) (s * 4095.0f);
	}

	// read pedal value
	float v = (1.0+sin((float) micros() / 200000.0))/2. * 4096;
	uint16_t value = (uint16_t) v;
	if(digitalRead(PEDAL_PIN)){
		// value = 0;
	}

	mcp4921(4000);

}