#pragma once

#include "Menu.h"
#include "MenuController.h"

// uint8_t settingsIcon[] = {0b11111111,0b11111111,0b11111111,0b11111111,0b11000011,0b11111111,0b11111111,0b11000011,0b11111111,0b11111011,0b11000011,0b11011111,0b11110001,0b11000011,0b10001111,0b11100000,0b10000001,0b00000111,0b11110000,0b00000000,0b00001111,0b11111000,0b00000000,0b00011111,0b11111100,0b00000000,0b00111111,0b11111000,0b00111100,0b00011111,0b10000000,0b01111110,0b00000001,0b10000000,0b01111110,0b00000001,0b10000000,0b01111110,0b00000001,0b10000000,0b01111110,0b00000001,0b11111000,0b00111100,0b00011111,0b11111100,0b00000000,0b00111111,0b11111000,0b00000000,0b00011111,0b11110000,0b00000000,0b00001111,0b11100000,0b10000001,0b00000111,0b11110001,0b11000011,0b10001111,0b11111011,0b11000011,0b11011111,0b11111111,0b11000011,0b11111111,0b11111111,0b11000011,0b11111111,0b11111111,0b11111111,0b11111111};
// uint8_t settingsIcon[] = {0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11000011,0b11111111,0b11111101,0b11000011,0b10111111,0b11111000,0b10000001,0b00011111,0b11110000,0b00000000,0b00001111,0b11111000,0b00000000,0b00011111,0b11111100,0b00000000,0b00111111,0b11111000,0b00011000,0b00011111,0b11100000,0b00111100,0b00000111,0b11100000,0b01111110,0b00000111,0b11100000,0b01111110,0b00000111,0b11100000,0b00111100,0b00000111,0b11111000,0b00011000,0b00011111,0b11111100,0b00000000,0b00111111,0b11111000,0b00000000,0b00011111,0b11110000,0b00000000,0b00001111,0b11111000,0b10000001,0b00011111,0b11111101,0b11000011,0b10111111,0b11111111,0b11000011,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111,0b11111111};

void MainMenu::render(){
	char c_buffer[10];
	u8g2.setCursor(48,30);
	u8g2.setDrawColor(1);
	sprintf(c_buffer, "%5d", current_weight);
	u8g2.setFont(u8g2_font_logisoso20_tn);
	u8g2.print(c_buffer);
	u8g2.setFont(u8g2_font_logisoso16_tr);
	u8g2.print("g");

	u8g2.setFontMode(1);
	for(int i=0; i<3; i++){
		u8g2.setDrawColor(1);
		u8g2.drawFrame(i*34, 38, 35, 26);
		if(selected_item == i){
			if(selected){
				u8g2.drawBox(i*34, 38, 35, 26);
			}
			else{
				u8g2.drawTriangle(i*34+(34/2)+1, 38, i*34+(34/2)-3+1, 35, i*34+(34/2)+3+1, 35);
			}
		}
		u8g2.setDrawColor(2);
		u8g2.setCursor(i*34+3, 46);
		u8g2.setFont(u8g2_font_tinytim_tr);
		u8g2.print(elements[i].name);
		u8g2.setCursor(i*34+3, 46+12);
		u8g2.setFont(u8g2_font_profont12_tn);
		sprintf(c_buffer, "%5d", elements[i].value);
		u8g2.print(c_buffer);
	}
	u8g2.setDrawColor(1);
	u8g2.drawFrame(3*34, 38, 26, 26);
	if(selected_item == 3){
		u8g2.drawTriangle(3*34+(24/2)+1, 38, 3*34+(24/2)-3+1, 35, 3*34+(24/2)+3+1, 35);
	}
	// settings icon
	u8g2.setDrawColor(0);
	// u8g2.drawBitmap(3*34+1, 38+1, 3, 24, settingsIcon);
}

void MainMenu::onEncoderRotate(int dir){
	if(selected){
		// change setting value
		if(selected_item < 3){
			elements[selected_item].value += dir;
			if(elements[selected_item].value > elements[selected_item].max){
				elements[selected_item].value = elements[selected_item].max;
			}
			if(elements[selected_item].value < elements[selected_item].min){
				elements[selected_item].value = elements[selected_item].min;
			}
		}
	}
	else{
		// change selected item
		int sign = dir/abs(dir);
		selected_item = (selected_item + sign + 4) % 4;
	}
}

void MainMenu::onEncoderClick(){
	// open menu
	if(selected_item == 3){
		menuController.current_page = 1;
	}
	else{
		selected = !selected;
	}
}

void MainMenu::onPedalClick(){
	// do nothing
}

void SettingsMenu::render(){
	char c_buffer[10];
	u8g2.setDrawColor(1);
	u8g2.setFont(u8g2_font_tinytim_tr);
	u8g2.setCursor(1,6);
	u8g2.print("SETTINGS");
	u8g2.drawLine(0, 7, 127, 7);

	for(int i=0; i<6; i++){
		u8g2.setCursor(4, i*8+16);
		u8g2.print(elements[i].name);
	}


}

void SettingsMenu::onEncoderRotate(int dir){
	if (selected){
		// change setting value
		if(selected_item < 4){
			elements[selected_item].value += dir;
			if(elements[selected_item].value > elements[selected_item].max){
				elements[selected_item].value = elements[selected_item].max;
			}
			if(elements[selected_item].value < elements[selected_item].min){
				elements[selected_item].value = elements[selected_item].min;
			}
		}
	}
	else{
		// change selected item
		int sign = dir/abs(dir);
		selected_item = (selected_item + sign + 4) % 4;
	}
}

void SettingsMenu::onEncoderClick(){
	selected = !selected;
}

void SettingsMenu::onPedalClick(){
	// do nothing
}