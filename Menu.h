#pragma once

#include "Settings.h"

class Menu{
public:
	virtual void render();
	virtual void onEncoderRotate(int dir);
	virtual void onEncoderClick();
	virtual void onPedalClick();
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

	virtual void render();
	virtual void onEncoderRotate(int dir);
	virtual void onEncoderClick();
	virtual void onPedalClick();
};

class SettingsMenu : public Menu{
public:
	int selected_item = 0;
	bool selected = false;

	SettingElement elements[6] = {
		{"STOPPING WEIGHT", 0, 0, 1000},
		{"START ACCELERATION", 0, 0, 100},
		{"TARE ON START", 0, 0, 1},
		{"SOUND", 0, 0, 1},
		{"CALIBRATION", 0, 0, 1},
		{"SAVE SETTINGS", 0, 0, 1},
	};

	virtual void render();
	virtual void onEncoderRotate(int dir);
	virtual void onEncoderClick();
	virtual void onPedalClick();
};


