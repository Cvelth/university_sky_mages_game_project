#include "Objects.hpp"
#include "Settings.hpp"
Objects::Objects(ProgramMode mode) : m_program_mode(mode), m_settings(new Settings()) {}
Objects::~Objects() { delete m_settings; }

#include "ObjectStorage.hpp"
Objects* initialize_object_storage(ProgramMode mode) {
	auto objects = new Objects(mode);
	auto storage = new ObjectStorage(objects);
	delete storage;
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

template <typename T>
T* get_object(std::string const& name, std::list<std::unique_ptr<T>> const& list) {
	if (name == "")
		return new T(*list.front());
	else
		return new T(**std::find_if(list.begin(), list.end(), [&name](std::unique_ptr<T> const& it) {
			return name == it->name();
		}));
}
#include "../../Objects/EquipableItems/EnergyStorage.hpp"
EnergyStorage* Objects::get_energy_storage(std::string const& name) const { return get_object(name, m_energy_storage); }
#include "../../Objects/EquipableItems/FlyEngine.hpp"
FlyEngine* Objects::get_fly_engine(std::string const& name) const { return get_object(name, m_fly_engine); }
#include "../../Objects/EquipableItems/Weapon.hpp"
Weapon* Objects::get_weapon(std::string const& name) const { return get_object(name, m_weapon); }

size_t Objects::size() const {
	return m_settings->size() + m_energy_storage.size() + m_fly_engine.size() + m_weapon.size();
}