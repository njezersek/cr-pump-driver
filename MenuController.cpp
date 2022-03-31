#include "MenuController.h"

void MenuController::render(){
	u8g2.clearBuffer();
	pages[current_page]->render();
	u8g2.sendBuffer();
}

void MenuController::onEncoderRotate(int dir){
	pages[current_page]->onEncoderRotate(dir);
}
void MenuController::onEncoderClick(){
	pages[current_page]->onEncoderClick();
}
void MenuController::onPedalClick(){
	pages[current_page]->onPedalClick();
}

MenuController menuController;