#include <Arduino.h>
#include <U8g2lib.h>
#include <TimerOne.h>
#include "debounce.h"

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

// 128x64 LCD

#define CLK 13
#define RS 12
#define RW 11
U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, CLK, RW, RS );

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

void update(){
	// read input state
	encoder_A.update();
	encoder_B.update();

	if(encoder_A.state && !encoder_A.last_state){
		if(encoder_B.state) item = ++item % 4;
		else item = --item % 4;
	}

	// Serial.println(val++);

	if(val > 1000000) val = 0;
}

char c_buffer[10];

void loop(void) {
	u8g2.clearBuffer();
	u8g2.setCursor(48,30);
	sprintf(c_buffer, "%5d", val);
	u8g2.setFont(u8g2_font_logisoso20_tn);
	u8g2.print(c_buffer);
	u8g2.setFont(u8g2_font_logisoso16_tr);
	u8g2.print("g");
	u8g2.setFont(u8g2_font_tinytim_tf);
	u8g2.setFontMode(1);
	for(int i=0; i<4; i++){
		u8g2.setDrawColor(1);
		u8g2.drawFrame(i*34, 40, 35, 24);
		if(item == i){
			u8g2.drawBox(i*34, 40, 35, 24);
		}
		u8g2.setDrawColor(2);
		u8g2.setCursor(i*34+3, 48);
		u8g2.print("WEIGHT");
		u8g2.setCursor(i*34+3, 48+8);
		u8g2.print(1234);
	}
	u8g2.sendBuffer();
}
