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

	u8g2.setFontMode(1);
	for(int i=0; i<3; i++){
		u8g2.setDrawColor(1);
		u8g2.drawFrame(i*34, 40, 35, 24);
		if(selected_item == i){
			if(selected){
				u8g2.drawBox(i*34, 40, 35, 24);
			}
			else{
				u8g2.drawTriangle(i*34+(34/2)+1, 40, i*34+(34/2)-5+1, 35, i*34+(34/2)+5+1, 35);
			}
		}
		u8g2.setDrawColor(2);
		u8g2.setCursor(i*34+3, 48);
		u8g2.setFont(u8g2_font_tinytim_tr);
		u8g2.print(elements[i].name);
		u8g2.setCursor(i*34+3, 48+12);
		u8g2.setFont(u8g2_font_profont12_tn);
		sprintf(c_buffer, "%5d", elements[i].value);
		u8g2.print(c_buffer);
	}
}

void MainMenu::onEncoderRotate(int dir){
	if(selected){
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
