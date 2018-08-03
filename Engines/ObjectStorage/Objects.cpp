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

#include "../../Objects/EquipableItems/EnergyStorage.hpp"
EnergyStorage* Objects::get_energy_storage() const {
	return new EnergyStorage(*m_energy_storage.front());
}
//#include "../../Objects/EquipableItems/FlyEngine.hpp"
//FlyEngine* Objects::get_fly_engine() const {
//	return new FlyEngine(*m_fly_engine.front());
//}
//#include "../../Objects/EquipableItems/Weapon.hpp"
//Weapon* Objects::get_weapon() const {
//	return new Weapon(*m_weapon.front());
//}