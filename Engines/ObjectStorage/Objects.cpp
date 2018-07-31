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