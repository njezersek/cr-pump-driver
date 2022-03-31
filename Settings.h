#pragma once

struct Settings{
	int target_weight;
	int speed;
	int stopping_weight;
	int start_acceleration;
	int drip_compensation;
	int scale_calibration;
	bool settings_changed;
};

extern Settings settings;