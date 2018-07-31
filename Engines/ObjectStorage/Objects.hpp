#pragma once
class ObjectStorage;
class Settings;

enum class ProgramMode { Client, Server };

class Objects {
	friend ObjectStorage;
protected:
	ProgramMode m_program_mode;
	Settings *m_settings;
public:
	Objects(ProgramMode);
	~Objects();
	void set_settings_to_default();
	inline ProgramMode mode() { return m_program_mode; }
	inline bool isServer() { return m_program_mode == ProgramMode::Server; }
	inline bool isClient() { return m_program_mode == ProgramMode::Client; }
};

Objects* initialize_object_storage(ProgramMode);