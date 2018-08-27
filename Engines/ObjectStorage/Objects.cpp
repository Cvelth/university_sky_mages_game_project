#include "Objects.hpp"
#include "Settings.hpp"
Objects::Objects() : m_settings(new Settings()) {}
Objects::~Objects() { delete m_settings; }

#include "ObjectStorage.hpp"
std::shared_ptr<Objects> initialize_object_storage() {
	auto objects = std::make_shared<Objects>();
	ObjectStorage s(objects);
	s.load(objects->settings()->getStringValue("Object_Storage_Path"));
	return objects;
}
void Objects::set_settings_to_default() {
	m_settings->clear();
}

#include "DefaultObjectStorageData.hpp"
std::string Objects::get_program_version() {
	return Program_Name + " v" +
		std::to_string(Program_Major_Version) + "." +
		std::to_string(Program_Minor_Version) + "." +
		std::to_string(Program_Patch_Version) + "." +
		std::to_string(Program_Build_Version) + "_" +
		Program_Version_Suffix;
}

#include <random>
static std::mt19937 g((std::random_device())());
template <typename T>
std::shared_ptr<T> get_object(std::string const& name, std::vector<std::unique_ptr<T>> const& list, bool can_be_empty = false) {
	if (name != "")
		if (auto ret = std::find_if(list.begin(), list.end(), [&name](std::unique_ptr<T> const& it) { return name == it->name(); }); ret != list.end())
			return std::make_unique<T>(**ret);

	std::uniform_int_distribution<size_t> d(0, can_be_empty ? list.size() : list.size() - 1);

	auto index = d(g);
	if (index == list.size())
		return nullptr;
	else
		return std::make_unique<T>(*list.at(index));

}
#include "../../Objects/EquipableItems/EnergyStorage.hpp"
std::shared_ptr<EnergyStorage> Objects::get_energy_storage(std::string const& name) const { return get_object(name, m_energy_storage, false); }
#include "../../Objects/EquipableItems/FlyEngine.hpp"
std::shared_ptr<FlyEngine> Objects::get_fly_engine(std::string const& name) const { return get_object(name, m_fly_engine, false); }
#include "../../Objects/EquipableItems/Weapon.hpp"
std::shared_ptr<Weapon> Objects::get_weapon(std::string const& name) const { return get_object(name, m_weapon, false); }
#include "../../Objects/EquipableItems/Shield.hpp"
std::shared_ptr<ShieldGenerator> Objects::get_shield_generator(std::string const& name) const { return get_object(name, m_shield_generator, false); }
#include "../../Objects/EquipableItems/Trinket.hpp"
std::shared_ptr<Trinket> Objects::get_trinket(std::string const& name) const { return get_object(name, m_trinket, true); }
#include "../../Objects/EquipableItems/Upgrade.hpp"
std::shared_ptr<Upgrade> Objects::get_upgrade(std::string const& name) const { return get_object(name, m_upgrade, true); }

size_t Objects::size() const {
	return m_settings->size() + m_energy_storage.size() + m_fly_engine.size() + m_weapon.size() + m_shield_generator.size() + m_trinket.size();
}