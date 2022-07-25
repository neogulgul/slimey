#pragma once

#include <string>
#include <vector>

class Setting {
	public:
		std::string name;
		bool active;
		Setting();
		Setting(std::string name, bool active);
};

class SettingsManager {
	public:
		Setting music;
		Setting SFX;
		Setting FPS;
		Setting crosshair;
		Setting debugMode;

		std::vector<Setting*> settings;

		SettingsManager();
};
