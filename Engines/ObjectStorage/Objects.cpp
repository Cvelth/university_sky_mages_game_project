#include "Objects.hpp"
#include "Settings.hpp"
Objects::Objects() {
	m_client_settings = new Settings();
	m_server_settings = new Settings();
}
Objects::~Objects() {
	delete m_client_settings;
	delete m_server_settings;
}

#include "ObjectStorage.hpp"
Objects* initialize_object_storage() {
	auto objects = new Objects();
	auto storage = new ObjectStorage(objects);
	delete storage;
	return objects;
}
void Objects::set_settings_to_default() {
	m_client_settings->clear_settings();
	m_server_settings->clear_settings();
}