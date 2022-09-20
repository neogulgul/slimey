#include <string>
#include "headers/SettingsManager.hpp"

Setting::Setting() {}

Setting::Setting(std::string name, bool active) {
	this->name = name;
	this->active = active;
}

SettingsManager::SettingsManager() {
	this->music     = Setting("Music", true);
	this->SFX       = Setting("SFX", true);
	this->FPS       = Setting("FPS", false);
	this->crosshair = Setting("Crosshair", false);
	this->debugMode = Setting("Debug Mode", false);

	this->settings = {
		&music,
		&SFX,
		&FPS,
		&crosshair,
		&debugMode
	};
}
