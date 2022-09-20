#pragma once

#include <string>
#include <vector>

struct Setting
{
	std::string name;
	bool active;
	Setting();
	Setting(std::string name, bool active);
};

struct SettingsManager
{
	Setting music;
	Setting SFX;
	Setting FPS;
	Setting crosshair;
	Setting debugMode;

	std::vector<Setting*> settings;

	SettingsManager();
};
