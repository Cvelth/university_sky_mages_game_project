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

	SettingValue& operator[](std::string name);
	const SettingValue& operator[](std::string name) const;
	SettingValue& getValue(std::string name);
	const SettingValue& getValue(std::string name) const;
	const bool& getValue(std::string name, bool& value_reference) const;
	const signed& getValue(std::string name, signed& value_reference) const;
	const unsigned& getValue(std::string name, unsigned& value_reference) const;
	const float& getValue(std::string name, float& value_reference) const;
	const std::string& getValue(std::string name, std::string& value_reference) const;
	const bool getBoolValue(std::string name) const;
	const signed int getSintValue(std::string name) const;
	const unsigned int getUintValue(std::string name) const;
	const float getFloatValue(std::string name) const;
	const std::string getStringValue(std::string name) const;
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
public:
	std::string getProgramVersionInfo();
};

std::ostream& operator<<(std::ostream &stream, const SettingValue &value);
std::istream& operator>>(std::istream &stream, SettingValue &value);

#include "Shared\AbstractException.hpp"
DefineNewException(SettingsAccessException);
DefineNewException(SettingsVersionException);
DefineNewException(SettingsUsageException);