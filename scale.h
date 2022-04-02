#pragma once
#include <HX711.h>

#include "pinout.h"

namespace Scale{

	extern HX711 scale;

	extern float median_buff[];
	extern int median_index;
	extern float median;

	extern const int mean_n;
	extern float mean_buff[];
	extern int mean_index;
	extern float mean_median;

	extern void init();

	extern float read();
}

