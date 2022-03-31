#pragma once

#include "Display.h"
#include "Menu.h"

class MenuController{
public:
	MainMenu* pages[1] = {new MainMenu()};
	int current_page = 0;

	void render();
	void onEncoderRotate(int dir);
	void onEncoderClick();
	void onPedalClick();
};

extern MenuController menuController;
