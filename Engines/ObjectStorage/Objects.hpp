#pragma once
#include <string>
#include <memory>
#include <vector>
class ObjectStorage;
class Settings;
class EnergyStorage;
class FlyEngine;
class Weapon;
class ShieldGenerator;
class Trinket;
class MainActor;

class Objects {
	friend ObjectStorage;
protected:
	Settings *m_settings;
	std::vector<std::unique_ptr<EnergyStorage>> m_energy_storage;
	std::vector<std::unique_ptr<FlyEngine>> m_fly_engine;
	std::vector<std::unique_ptr<Weapon>> m_weapon;
	std::vector<std::unique_ptr<ShieldGenerator>> m_shield_generator;
	std::vector<std::unique_ptr<Trinket>> m_trinket;
public:
	Objects();
	~Objects();
	void set_settings_to_default();

	inline Settings* settings() { return m_settings; }

	static std::string get_program_version();

	std::shared_ptr<EnergyStorage> get_energy_storage(std::string const& name = "") const;
	std::shared_ptr<FlyEngine> get_fly_engine(std::string const& name = "") const;
	std::shared_ptr<Weapon> get_weapon(std::string const& name = "") const;
	std::shared_ptr<ShieldGenerator> get_shield_generator(std::string const& name = "") const;
	std::shared_ptr<Trinket> get_trinket(std::string const& name = "") const;

	size_t size() const;
};

//Initialization functions;
std::shared_ptr<Objects> initialize_object_storage();
void initialize_render_info(std::string const& path = "/");