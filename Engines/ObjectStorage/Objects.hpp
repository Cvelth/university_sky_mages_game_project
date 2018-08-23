#pragma once
#include <string>
#include <memory>
#include <list>
class ObjectStorage;
class Settings;
class EnergyStorage;
class FlyEngine;
class Weapon;
class ShieldGenerator;
class MainActor;

class Objects {
	friend ObjectStorage;
protected:
	Settings *m_settings;
	std::list<std::unique_ptr<EnergyStorage>> m_energy_storage;
	std::list<std::unique_ptr<FlyEngine>> m_fly_engine;
	std::list<std::unique_ptr<Weapon>> m_weapon;
	std::list<std::unique_ptr<ShieldGenerator>> m_shield_generator;
public:
	Objects();
	~Objects();
	void set_settings_to_default();

	inline Settings* settings() { return m_settings; }

	static std::string get_program_version();

	EnergyStorage* get_energy_storage(std::string const& name = "") const;
	FlyEngine* get_fly_engine(std::string const& name = "") const;
	Weapon* get_weapon(std::string const& name = "") const;
	ShieldGenerator* get_shield_generator(std::string const& name = "") const;

	size_t size() const;
};

//Initialization functions;
std::shared_ptr<Objects> initialize_object_storage();
void initialize_render_info();