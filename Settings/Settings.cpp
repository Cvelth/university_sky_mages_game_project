#include "Settings.hpp"
#include <fstream>
#include "..\Exceptions\OtherExceptions.hpp"

void Settings::load() {
	std::ifstream f;
	f.open(SettingFileName);
	if (!f.is_open())
		throw Exceptions::FileAccessException();
}

void Settings::save() {
	std::ofstream f;
	f.open(SettingFileName);
	if (!f.is_open())
		throw Exceptions::FileAccessException();
}

Settings::Settings(const char * filename) : SettingFileName(filename) {}

Settings::~Settings() {}
