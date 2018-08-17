#pragma once
#include <string>
#include <memory>
#include <list>
class ObjectStorage;
class Settings;
enum class ProgramMode { Client, Server };
class EnergyStorage;
class FlyEngine;
class Weapon;
class MainActor;

class Objects {
	friend ObjectStorage;
protected:
	ProgramMode m_program_mode;
	Settings *m_settings;
	std::list<std::unique_ptr<EnergyStorage>> m_energy_storage;
	std::list<std::unique_ptr<FlyEngine>> m_fly_engine;
	std::list<std::unique_ptr<Weapon>> m_weapon;
public:
	Objects(ProgramMode);
	~Objects();
	void set_settings_to_default();
	inline ProgramMode mode() { return m_program_mode; }
	inline bool isServer() { return m_program_mode == ProgramMode::Server; }
	inline bool isClient() { return m_program_mode == ProgramMode::Client; }

	inline Settings* settings() { return m_settings; }

	static std::string get_program_version();

	EnergyStorage* get_energy_storage(std::string const& name = "") const;
	FlyEngine* get_fly_engine(std::string const& name = "") const;
	Weapon* get_weapon(std::string const& name = "") const;

	size_t size() const;
};

//Initialization functions;
Objects* initialize_object_storage(ProgramMode);
void initialize_render_info();
MainActor* string_to_main_actor(std::string &data, Objects *o);