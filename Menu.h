#pragma once

#include "Settings.h"

class Menu{
public:
	void render();
	void onEncoderRotate(int dir);
	void onEncoderClick();
	void onPedalClick();
};


class MainMenu : public Menu{
public:
	int selected_item = 0;
	bool selected = false;
	int current_weight = 5678;

	void render();
	void onEncoderRotate(int dir);
	void onEncoderClick();
	void onPedalClick();
};
