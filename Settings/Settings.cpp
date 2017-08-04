#include "Settings.hpp"
#include "DefaultSettings.hpp"
#include "..\Exceptions\OtherExceptions.hpp"

Settings::Settings(std::string filename) : SettingFileName(filename) {
	addSetting("Program_Name", Program_Name);
	addSetting("Program_Major_Version", Program_Major_Version);
	addSetting("Program_Minor_Version", Program_Minor_Version);
	addSetting("Program_Build_Version", Program_Build_Version);
	addSetting("Program_Version_Suffix", Program_Version_Suffix);
	addSetting("Settings_Syntax_Major_Version", Settings_Syntax_Major_Version);
	addSetting("Settings_Syntax_Minor_Version", Settings_Syntax_Minor_Version);
}
Settings::~Settings() {}

SettingValue & Settings::operator[](std::string  name) {
	return getValue(name);
}
const SettingValue & Settings::operator[](std::string  name) const {
	return getValue(name);
}
SettingValue & Settings::getValue(std::string  name) {
	try {
		return m_data.at(name);
	} catch (std::out_of_range&) {
		throw Exceptions::NonExistingSettingException();
	}
}
const SettingValue & Settings::getValue(std::string  name) const {
	try {
		auto t = m_data.at(name);
		return m_data.at(name);
	} catch (std::out_of_range&) {
		throw Exceptions::NonExistingSettingException();
	}
}
const bool& Settings::getValue(std::string name, bool & value_reference) const {
	return value_reference = getBoolValue(name);
}
const signed& Settings::getValue(std::string name, signed & value_reference) const {
	return value_reference = getSintValue(name);
}
const unsigned& Settings::getValue(std::string name, unsigned & value_reference) const {
	return value_reference = getUintValue(name);
}
const float& Settings::getValue(std::string name, float & value_reference) const {
	return value_reference = getFloatValue(name);
}
const std::string& Settings::getValue(std::string name, std::string & value_reference) const {
	return value_reference = getStringValue(name);
}
const bool Settings::getBoolValue(std::string name) const {
	return std::get<bool>(getValue(name));
}
const signed int Settings::getSintValue(std::string name) const {
	return std::get<signed>(getValue(name));
}
const unsigned int Settings::getUintValue(std::string name) const {
	auto t = std::get<unsigned>(getValue(name));
	return t;
}
const float Settings::getFloatValue(std::string name) const {
	return std::get<float>(getValue(name));
}
const std::string Settings::getStringValue(std::string name) const {
	return std::get<std::string>(getValue(name));
}
const std::string Settings::getType(SettingValue value) const {
	return std::visit([](auto&& arg) {
		using Type = std::decay_t<decltype(arg)>;
		if (std::is_same_v<Type, bool>)
			return "boolean";
		else if (std::is_same_v<Type, signed int>)
			return "s_int";
		else if (std::is_same_v<Type, unsigned int>)
			return "u_int";
		else if (std::is_same_v<Type, float>)
			return "float";
		else if (std::is_same_v<Type, std::string>)
			return "string";
		else
			throw Exceptions::UnsupportedSettingTypeException();
	}, value);
}
const std::string Settings::getType(std::string name) const {
	return getType(getValue(name));
}
void Settings::addSetting(std::string name, bool value) {
	SettingValue t;
	t = value;
	m_data.insert(Setting(name, t));
}
void Settings::addSetting(std::string name, int value) {
	SettingValue t;
	t = value;
	m_data.insert(Setting(name, t));
}
void Settings::addSetting(std::string name, unsigned value) {
	SettingValue t;
	t = value;
	m_data.insert(Setting(name, t));
}
void Settings::addSetting(std::string name, float value) {
	SettingValue t;
	t = value;
	m_data.insert(Setting(name, t));
}
void Settings::addSetting(std::string name, std::string value) {
	SettingValue t;
	t = value;
	m_data.insert(Setting(name, t));
}

void Settings::default() {
	addSetting("Screen_Width", Screen_Width);
	addSetting("Screen_Height", Screen_Height);
	addSetting("Fullscreen_Window", Fullscreen_Window);
}

#include <fstream>
void Settings::load() {
	std::ifstream f;
	f.open(SettingFileName);
	if (!f.is_open())
		throw Exceptions::FileAccessException();

	bool first = true;
	std::string line;
	while (std::getline(f, line)) {
		if (line.size() > 0u && line.at(0) != '#') {
			if (first) {
				loadFirstLine(line);
				first = false;
			} else
				loadLine(line);
		}
	}
}

void Settings::save() {
	std::ofstream f;
	f.open(SettingFileName);
	if (!f.is_open())
		throw Exceptions::FileAccessException();
	
	f << SettingFileName << " v"
		<< getValue("Settings_Syntax_Major_Version") << "."
		<< getValue("Settings_Syntax_Minor_Version") << " for "
		<< getValue("Program_Name") << " v"
		<< getValue("Program_Major_Version") << "."
		<< getValue("Program_Minor_Version") << "."
		<< getValue("Program_Build_Version")
		<< getValue("Program_Version_Suffix") << std::endl;

	f << "# Use '#' at the beginning of the string to make a commentary.\n"
		<< "# Structure of a line : \"- field_type field-name = field_value\"\n"
		<< std::endl;

	for (auto it : m_data)
		f << getType(it.second) << " " << it.first << " = " << it.second << std::endl;
}
void Settings::backup() {
	copy_file(SettingFileName, (SettingFileName + std::string(".backup")).c_str());
}
void Settings::unbackup() {
	copy_file((SettingFileName + std::string(".backup")).c_str(), SettingFileName);
}

template <class... Ts> struct overloaded : Ts... { overloaded(Ts... f) : Ts(f)... {} };
template <class... Ts> auto overload(Ts... f) { return overloaded<Ts...>(f...); }
std::ostream& operator<<(std::ostream &stream, const SettingValue &value) {
	std::visit(overload(
		[&stream](bool arg) {
			if (arg)
				stream << "true";
			else
				stream << "false";
		},
		[&stream](auto arg) {
			stream << arg;
		}), value);
	return stream; 
}

std::istream & operator>>(std::istream & stream, SettingValue & value) {
	std::visit([&stream](auto&& arg) {
		stream >> arg;
	}, value);
	return stream;
}

void Settings::copy_file(std::string from, std::string to) {
	std::ifstream fi;
	std::ofstream fo;
	fi.open(from);
	fo.open(to);
	if (!fi.is_open() || !fo.is_open())
		throw Exceptions::FileAccessException();
	fo << fi.rdbuf();
}

std::string Settings::getProgramVersionInfo() {
	return std::get<std::string>(getValue("Program_Name")) + " " +
		std::to_string(std::get<unsigned int>(getValue("Program_Major_Version"))) + "." +
		std::to_string(std::get<unsigned int>(getValue("Program_Minor_Version"))) + "." +
		std::to_string(std::get<unsigned int>(getValue("Program_Build_Version"))) + " " +
		std::get<std::string>(getValue("Program_Version_Suffix"));
}

#include <sstream>
void Settings::loadFirstLine(std::string line) {
	std::stringstream iss(line);
	std::string s;
	iss >> s;
	if (s != "Settings.ini")
		throw Exceptions::UnsupportedSettingFileException();

	char c;
	unsigned int maj, min, bld;
	iss >> c >> maj >> c >> min;
	if (maj != std::get<unsigned int>(getValue("Settings_Syntax_Major_Version"))
		|| min != std::get<unsigned int>(getValue("Settings_Syntax_Minor_Version"))) {

		throw Exceptions::DifferentSettingFileVersionException();
	}

	iss >> s >> s; 
	if (s != std::get<std::string>(getValue("Program_Name")))
		throw Exceptions::UnsupportedSettingFileException();
	
	iss >> c >> maj >> c >> min >> c >> bld >> s;
	if (maj != std::get<unsigned int>(getValue("Program_Major_Version"))
		|| min != std::get<unsigned int>(getValue("Program_Minor_Version"))
		|| bld != std::get<unsigned int>(getValue("Program_Build_Version"))
		|| s != std::get<std::string>(getValue("Program_Version_Suffix"))) {

		throw Exceptions::DifferentProgramVersionException();
	}
}

void Settings::loadLine(std::string line) {
	std::stringstream iss(line);
	std::string s;
	iss >> s;
	char c;
	if (s == "boolean") {
		std::string v;
		iss >> s >> c >> v;
		addSetting(s, (v == "true") ? true : false);
	} else if (s == "s_int") {
		signed int v;
		iss >> s >> c >> v;
		addSetting(s, v);
	} else if (s == "u_int") {
		unsigned int v;
		iss >> s >> c >> v;
		addSetting(s, v);
	} else if (s == "float") {
		float v;
		iss >> s >> c >> v;
		addSetting(s, v);
	} else if (s == "string") {
		std::string v;
		iss >> s >> c >> v;
		addSetting(s, v);
	} else
		throw Exceptions::UnsupportedSettingTypeException();
}