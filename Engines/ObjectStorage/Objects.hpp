#pragma once
class ObjectStorage;
class Settings;

class Objects {
	friend ObjectStorage;
protected:
	Settings *m_client_settings, *m_server_settings;
public:
	Objects();
	~Objects();
	void set_settings_to_default();
};

Objects* initialize_object_storage();