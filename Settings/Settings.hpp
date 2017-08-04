#pragma once
#include <unordered_map>
#include <variant>
#include <string>
using SettingValue = std::variant<bool, signed int, unsigned int, float, std::string>;
using Setting = std::pair<std::string, SettingValue>;
using SettingsMap = std::unordered_map<std::string, SettingValue>;

class Settings {
private:
	std::string SettingFileName;
protected:
	SettingsMap m_data;
public:
	Settings(std::string filename = "Settings.ini");
	~Settings();

	SettingValue& getValue(std::string name);
	const SettingValue& getValue(std::string name) const;
	SettingValue& operator[](std::string name);
	const SettingValue& operator[](std::string name) const;
	const std::string getType(SettingValue value) const;
	const std::string getType(std::string name) const;
protected:
	void addSetting(std::string name, bool value);
	void addSetting(std::string name, int value);
	void addSetting(std::string name, unsigned value);
	void addSetting(std::string name, float value);
	void addSetting(std::string name, std::string value);
public:
	void default();
	void load();
	void save();
	void backup();	
	void unbackup();
protected:
	void loadFirstLine(std::string line);
	void loadLine(std::string line);
	void copy_file(std::string from, std::string to);
};

std::ostream& operator<<(std::ostream &stream, SettingValue &value);
std::istream& operator>>(std::istream &stream, SettingValue &value);