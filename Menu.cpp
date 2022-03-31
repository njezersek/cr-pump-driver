#pragma once

#include "Menu.h"
#include "MenuController.h"


void MainMenu::render(){
	char c_buffer[10];
	u8g2.setCursor(48,30);
	sprintf(c_buffer, "%5d", current_weight);
	u8g2.setFont(u8g2_font_logisoso20_tn);
	u8g2.print(c_buffer);
	u8g2.setFont(u8g2_font_logisoso16_tr);
	u8g2.print("g");
	u8g2.setFont(u8g2_font_tinytim_tf);
	u8g2.setFontMode(1);
	for(int i=0; i<4; i++){
		u8g2.setDrawColor(1);
		u8g2.drawFrame(i*34, 40, 35, 24);
		if(selected_item == i){
			u8g2.drawBox(i*34, 40, 35, 24);
		}
		u8g2.setDrawColor(2);
		u8g2.setCursor(i*34+3, 48);
		u8g2.print("WEIGHT");
		u8g2.setCursor(i*34+3, 48+8);
		u8g2.print(1234);
	}
}

void MainMenu::onEncoderRotate(int dir){
	if(selected){
		if(selected_item == 0){
			settings.target_weight += dir;
			if(settings.target_weight < 0) settings.target_weight = 0;
			if(settings.target_weight > 2000) settings.target_weight = 2000;
			settings.settings_changed = true;
		}
		if(selected_item == 1){
			settings.drip_compensation += dir;
			if(settings.drip_compensation < 0) settings.drip_compensation = 0;
			if(settings.drip_compensation > 100) settings.drip_compensation = 100;
			settings.settings_changed = true;
		}
		if(selected_item == 2){
			settings.speed += dir;
			if(settings.speed < 0) settings.speed = 0;
			if(settings.speed > 100) settings.speed = 100;
			settings.settings_changed = true;
		}
	}
	else{
		selected_item = (selected_item + dir + 4) % 4;
	}
}

void MainMenu::onEncoderClick(){
	// open menu
	if(selected_item == 3){
		// menuController.current_page = 1;
	}
	else{
		selected = !selected;
	}
}
