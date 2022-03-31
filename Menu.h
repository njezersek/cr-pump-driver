#pragma once

#include "Settings.h"

class Menu{
public:
	void render();
	void onEncoderRotate(int dir);
	void onEncoderClick();
	void onPedalClick();
};

struct SettingElement{
	const char* name;
	int value;
	int min;
	int max;
};

class MainMenu : public Menu{
public:
	int selected_item = 0;
	bool selected = false;
	int current_weight = 5678;

	SettingElement elements[3] = {
		{"WEIGHT", 0, 0, 2000},
		{"DRIP", 0, 0, 100},
		{"SPEED", 0, 0, 100}
	};

	void render();
	void onEncoderRotate(int dir);
	void onEncoderClick();
	void onPedalClick();
};
