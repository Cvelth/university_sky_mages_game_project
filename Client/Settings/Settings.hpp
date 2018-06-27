#pragma once
#include <unordered_map>
#include <variant>
#include <string>
#include "Shared/KeyLayout.hpp"
using SettingValue = std::variant<bool, signed int, unsigned int, float, std::string, KeyLayout>;
using Setting = std::pair<const std::string, SettingValue>;
using SettingsMap = std::unordered_map<std::string, SettingValue>;

class Settings {
private:
	std::string SettingFileName;
protected:
	SettingsMap m_data;
public:
	Settings(std::string filename = "Settings.ini");
	~Settings();

	SettingValue& operator[](std::string const &name);
	const SettingValue& operator[](std::string const &name) const;
	SettingValue& getValue(std::string const &name);
	const SettingValue& getValue(std::string const &name) const;
	const bool& getValue(std::string const &name, bool& value_reference) const;
	const signed& getValue(std::string const &name, signed& value_reference) const;
	const unsigned& getValue(std::string const &name, unsigned& value_reference) const;
	const float& getValue(std::string const &name, float& value_reference) const;
	const std::string& getValue(std::string const &name, std::string& value_reference) const;
	const KeyLayout& getValue(std::string const &name, KeyLayout& value_reference) const;
	const bool getBoolValue(std::string const &name) const;
	const signed int getSintValue(std::string const &name) const;
	const unsigned int getUintValue(std::string const &name) const;
	const float getFloatValue(std::string const &name) const;
	const KeyLayout getKeysValue(std::string const &name) const;
	const std::string getStringValue(std::string const &name) const;
	const std::string getType(SettingValue value) const;
	const std::string getType(std::string const &name) const;
protected:
	void addSetting(std::string const &name, const bool value);
	void addSetting(std::string const &name, const int value);
	void addSetting(std::string const &name, const unsigned value);
	void addSetting(std::string const &name, const float value);
	void addSetting(std::string const &name, const std::string& value);
	void addSetting(std::string const &name, const KeyLayout& value);
public:
	void default();
	void load();
	void save();
	void backup();	
	void unbackup();
protected:
	void loadFirstLine(std::string const &line);
	void loadLine(std::string const &line);
	void copy_file(std::string const &from, std::string const &to);
public:
	std::string getProgramVersionInfo();
};

std::ostream& operator<<(std::ostream &stream, const SettingValue &value);
std::istream& operator>>(std::istream &stream, SettingValue &value);

#include "Shared/AbstractException.hpp"
DefineNewException(SettingsAccessException);
DefineNewException(SettingsVersionException);
DefineNewException(SettingsUsageException);