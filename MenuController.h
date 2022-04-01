#pragma once

#include "Display.h"
#include "Menu.h"

class MenuController{
public:
	MainMenu main_menu;
	SettingsMenu settings_menu;
	Menu* pages[2] = {&main_menu, &settings_menu};
	int current_page = 0;

	void render();
	void onEncoderRotate(int dir);
	void onEncoderClick();
	void onPedalClick();
};

extern MenuController menuController;
