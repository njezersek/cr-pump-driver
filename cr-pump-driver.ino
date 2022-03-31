#include <Arduino.h>
#include <TimerOne.h>
#include "debounce.h"
#include "MenuController.h"

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

// Rotary encoder

#define ENCODER_A_PIN 2
#define ENCODER_B_PIN 3
#define BUTTON_PIN 4

DebounceInput encoder_A(ENCODER_A_PIN);
DebounceInput encoder_B(ENCODER_B_PIN);
DebounceInput button(BUTTON_PIN);



void setup(void) {
	encoder_A.setup();
	encoder_B.setup();
	pinMode(BUTTON_PIN, INPUT_PULLUP);
	
	Serial.begin(9600);

	Timer1.initialize(1000);
	Timer1.attachInterrupt(update);

	u8g2.begin();
}

long val = 4350;
uint8_t item = 0;

unsigned long last_encoder_update = 0;

void update(){
	// read input state
	encoder_A.update();
	encoder_B.update();
	button.update();

	unsigned long now = millis();
	if(encoder_A.state && !encoder_A.last_state){
		int multiplier = 1;
		if(now - last_encoder_update < 25){
			multiplier = 20;
		}
		else if(now - last_encoder_update < 50){
			multiplier = 10;
		}
		last_encoder_update = now;

		if(encoder_B.state) menuController.onEncoderRotate(multiplier);
		else menuController.onEncoderRotate(-multiplier);
	}

	if(button.state && !button.last_state){
		menuController.onEncoderClick();
	}
}

char c_buffer[10];

// display rendering
void loop(void) {
	menuController.render();
}
