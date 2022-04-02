#include "Scale.h"

HX711 Scale::scale;

float Scale::median_buff[3];
int Scale::median_index = 0;
float Scale::median = 0;

const int Scale::mean_n = 5;
float Scale::mean_buff[Scale::mean_n];
int Scale::mean_index = 0;
float Scale::mean_median = 0;

void Scale::init(){
	pinMode(MCP4921_CS_PIN, OUTPUT);
	digitalWrite(MCP4921_CS_PIN, HIGH);


	scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

	scale.set_scale(237872.0f / 600.0f); // uncalibrated scale.get_units(10) / <actual wetight>
	scale.tare();
}

float Scale::read(){
	if(scale.is_ready()){
		float s = scale.get_units();

		// median filter
		if(s < 1000 && s > -10){ // remove obious outliers
			median_buff[median_index] = s;
			median_index = (median_index + 1) % 3;
		}


		if((median_buff[1] <= median_buff[0] && median_buff[0] <= median_buff[2]) || (median_buff[2] <= median_buff[0] && median_buff[0] <= median_buff[1]) ){
			median = median_buff[0];
		}
		else if((median_buff[0] <= median_buff[1] && median_buff[1] <= median_buff[2]) || (median_buff[2] <= median_buff[1] && median_buff[1] <= median_buff[0]) ){
			median = median_buff[1];
		}
		else if((median_buff[0] <= median_buff[2] && median_buff[2] <= median_buff[1]) || (median_buff[1] <= median_buff[2] && median_buff[2] <= median_buff[0]) ){
			median = median_buff[2];
		}
		else{
			median = -1000;
		}

		// sliding mean
		mean_buff[mean_index] = median;
		mean_index = (mean_index + 1) % mean_n;
		float sum = 0;
		for(int i=0; i<mean_n; i++){
			sum += mean_buff[i];
		}
		mean_median = sum / mean_n;


		// Serial.print("\t"); Serial.print(s);
		// Serial.print("\t"); Serial.print(median);
		// Serial.print("\t"); Serial.print(mean_median);
		// Serial.print("\t");	Serial.print(0);
		// Serial.print("\t");	Serial.print(300);
		// Serial.println();
		// s = (s-300)/(460-300); // 300 - 460
		// if(s <= 0.0f) s = 0.0f;
		// if(s >= 1.0f) s = 1.0f;
		// value = (uint16_t) (s * 4095.0f);

	}
	return mean_median;
}