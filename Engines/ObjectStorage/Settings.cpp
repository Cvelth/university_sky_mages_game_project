#include "Settings.hpp"
SettingValue & Settings::operator[](const std::string &name) {
	return getValue(name);
}
const SettingValue & Settings::operator[](const std::string &name) const {
	return getValue(name);
}
SettingValue & Settings::getValue(const std::string &name) {
	try {
		return m_data.at(name);
	}
	catch (std::out_of_range&) {
		throw Exceptions::SettingsUsageException("There is no Setting with given name. Maybe settings weren't loaded(or set by default).");
	}
}
const SettingValue & Settings::getValue(const std::string &name) const {
	try {
		auto t = m_data.at(name);
		return m_data.at(name);
	}
	catch (std::out_of_range&) {
		throw Exceptions::SettingsUsageException("There is no Setting with given name. Maybe settings weren't loaded(or set by default).");
	}
}
const bool& Settings::getValue(const std::string &name, bool & value_reference) const {
	return value_reference = getBoolValue(name);
}
const signed& Settings::getValue(const std::string &name, signed & value_reference) const {
	return value_reference = getSintValue(name);
}
const unsigned& Settings::getValue(const std::string &name, unsigned & value_reference) const {
	return value_reference = getUintValue(name);
}
const float& Settings::getValue(const std::string &name, float & value_reference) const {
	return value_reference = getFloatValue(name);
}
const std::string& Settings::getValue(const std::string &name, std::string & value_reference) const {
	return value_reference = getStringValue(name);
}
const KeyLayout& Settings::getValue(std::string const & name, KeyLayout & value_reference) const {
	return value_reference = getKeysValue(name);
}
const bool Settings::getBoolValue(const std::string &name) const {
	return std::get<bool>(getValue(name));
}
const signed int Settings::getSintValue(const std::string &name) const {
	return std::get<signed>(getValue(name));
}
const unsigned int Settings::getUintValue(const std::string &name) const {
	auto t = std::get<unsigned>(getValue(name));
	return t;
}
const float Settings::getFloatValue(const std::string &name) const {
	return std::get<float>(getValue(name));
}
const KeyLayout Settings::getKeysValue(std::string const & name) const {
	return std::get<KeyLayout>(getValue(name));
}
const std::string Settings::getStringValue(const std::string &name) const {
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
		else if (std::is_same_v<Type, KeyLayout>)
			return "Keys";
		else
			throw Exceptions::SettingsUsageException("Setting type is currently unsupported.");
	}, value);
}
const std::string Settings::getType(const std::string &name) const {
	return getType(getValue(name));
}
void Settings::addSetting(const std::string &name, bool value) {
	SettingValue t;
	t = value;
	m_data.insert(Setting(name, t));
}
void Settings::addSetting(const std::string &name, int value) {
	SettingValue t;
	t = value;
	m_data.insert(Setting(name, t));
}
void Settings::addSetting(const std::string &name, unsigned value) {
	SettingValue t;
	t = value;
	m_data.insert(Setting(name, t));
}
void Settings::addSetting(const std::string &name, float value) {
	SettingValue t;
	t = value;
	m_data.insert(Setting(name, t));
}
void Settings::addSetting(const std::string &name, const std::string &value) {
	SettingValue t;
	t = value;
	m_data.insert(Setting(name, t));
}

void Settings::addSetting(std::string const & name, const KeyLayout & value) {
	SettingValue t;
	t = value;
	m_data.insert(Setting(name, t));
}
void Settings::clear() {
	m_data.clear();
}

/*
void Settings::parse_line(std::string const &line) {
	std::stringstream iss(line);
	std::string s;
	iss >> s;
	char c;
	if (s == "boolean") {
		std::string v;
		iss >> s >> c >> v;
		addSetting(s, (v == "true") ? true : false);
	}
	else if (s == "s_int") {
		signed int v;
		iss >> s >> c >> v;
		addSetting(s, v);
	}
	else if (s == "u_int") {
		unsigned int v;
		iss >> s >> c >> v;
		addSetting(s, v);
	}
	else if (s == "float") {
		float v;
		iss >> s >> c >> v;
		addSetting(s, v);
	}
	else if (s == "string") {
		std::string v;
		iss >> s >> c >> v;
		addSetting(s, v);
	}
	else if (s == "Keys") {
		KeyLayout v;
		iss >> s >> c >> v;
		addSetting(s, v);
	}
	else
		throw Exceptions::SettingsUsageException("Setting type is currently unsupported.");
}*/
//std::string Settings::generate_line() const {
//	return std::string(); //to be implemented;
//}
/*
#include "DefaultSettings.hpp"

#define addDefaultSetting(name) addSetting(#name, name)

#include "../MyGraphicsLibrary/MGL/Events/EventEnums.hpp"

void Settings::initialize() {
	addDefaultSetting(Program_Name);
	addDefaultSetting(Program_Major_Version);
	addDefaultSetting(Program_Minor_Version);
	addDefaultSetting(Program_Patch_Version);
	addDefaultSetting(Program_Build_Version);
	addDefaultSetting(Program_Version_Suffix);
	addDefaultSetting(Settings_Syntax_Major_Version);
	addDefaultSetting(Settings_Syntax_Minor_Version);
}
void Settings::nullify() {
	addDefaultSetting(Screen_Width);
	addDefaultSetting(Screen_Height);
	addDefaultSetting(Fullscreen_Window);
	addDefaultSetting(Graphical_Updates_Per_Second);
	addDefaultSetting(Physical_Updates_Per_Second);
	addDefaultSetting(Keys_Layout);
}


#include <fstream>
void Settings::load() {
	std::ifstream f;
	f.open(SettingsFileName);
	if (!f.is_open())
		throw Exceptions::NoSettingsFileException("Settings file cannot be opened.");

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
	f.open(SettingsFileName);
	if (!f.is_open())
		throw Exceptions::NoSettingsFileException("Settings file cannot be opened.");
	
	f << SettingsFileName << " v"
		<< getValue("Settings_Syntax_Major_Version") << "."
		<< getValue("Settings_Syntax_Minor_Version") << " for "
		<< getValue("Program_Name") << " v"
		<< getValue("Program_Major_Version") << "."
		<< getValue("Program_Minor_Version") << "."
		<< getValue("Program_Patch_Version") << "."
		<< getValue("Program_Build_Version") << "_"
		<< getValue("Program_Version_Suffix") << std::endl;

	f << "# Use '#' at the beginning of the string to make a commentary.\n"
		<< "# Structure of a line : \"field_type field-name = field_value\"\n"
		<< std::endl;

	for (auto it : m_data)
		f << getType(it.second) << " " << it.first << " = " << it.second << std::endl;
}
void Settings::backup() {
	copy_file(SettingsFileName, (SettingsFileName + std::string(".backup")).c_str());
}
void Settings::unbackup() {
	copy_file((SettingsFileName + std::string(".backup")).c_str(), SettingsFileName);
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

void Settings::copy_file(std::string const &from, std::string const &to) {
	std::ifstream fi;
	std::ofstream fo;
	fi.open(from);
	fo.open(to);
	if (!fi.is_open() || !fo.is_open())
		throw Exceptions::NoSettingsFileException("Settings file cannot be opened.");
	fo << fi.rdbuf();
}
*/