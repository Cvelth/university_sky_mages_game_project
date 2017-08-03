#pragma once

class Settings;
template <typename Type> class Setting {
	friend Settings;
protected:
	char* m_name;
	Type* m_data;
public:
	Setting() : m_data(nullptr) {}
	~Setting() { delete m_data; }
};

class Settings {
private:
	const char* SettingFileName;
protected:
	Setting<float> screen_width;
	Setting<float> screen_height;
public:
	Settings(const char* filename = "Settings.ini");
	~Settings();

	void load();
	void save();
};