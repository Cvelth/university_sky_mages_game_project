#include "ObjectStorage.hpp"
#include "DefaultObjectStorageData.hpp"
#include "Objects.hpp"

ObjectStorage::ObjectStorage(Objects *objects, std::string const& path)
	: m_objects(objects), m_current_object(ObjectType::Empty),
	m_current_object_counter(0), m_current_object_number(0) { load(path); }

void ObjectStorage::save(std::string const& filename, std::string const& path) {
	static_assert("Not yet implemented.");
}

ObjectType _switch(std::string const& type) {
	if (type == "ClientSettings")
		return ObjectType::ClientSettings;
	else if (type == "ServerSettings")
		return ObjectType::ServerSettings;
	else if (type == "EnergyStorage")
		return ObjectType::EnergyStorage;
	else if (type == "FlyEngine")
		return ObjectType::FlyEngine;
	else if (type == "Weapon")
		return ObjectType::Weapon;
	else
		return ObjectType::Empty;
}

#include <string>
#include <sstream>
void ObjectStorage::parse_line(std::string const& line) {
	std::istringstream s(line);
	std::string string;
	s >> string;
	if (string == "Object") {
		char placeholder;
		size_t number;
		s >> number >> placeholder >> string;
		if (number == ++m_current_object_counter && number <= m_current_object_number)
			initialize_object(_switch(string), s);
		else
			throw Exceptions::FileParsingException("File seems to be corrupted");
	} else if (m_current_object != ObjectType::Empty) {
		return parse_object_line(line);
	} else
		throw Exceptions::FileParsingException("File seems to be corrupted");
}

#include "../../Objects/EquipableItems/EnergyStorage.hpp"
#include "../../Objects/EquipableItems/FlyEngine.hpp"
#include "../../Objects/EquipableItems/Weapon.hpp"
void ObjectStorage::initialize_object(ObjectType type, std::istream &s) {
	m_current_object = type;
	float value;
	char placeholder;
	switch (type) {
	case ObjectType::EnergyStorage:
		s >> placeholder >> value;
		m_objects->m_energy_storage.push_back(std::unique_ptr<EnergyStorage>(new EnergyStorage(value)));
		break;
	case ObjectType::FlyEngine:
		m_objects->m_fly_engine.push_back(std::unique_ptr<FlyEngine>(new FlyEngine()));
		break;
	case ObjectType::Weapon:
		m_objects->m_weapon.push_back(std::unique_ptr<Weapon>(new Weapon()));
		break;
	}
}

#include <sstream>
#include "../../Shared/KeyLayout.hpp"
template<class function> void parse_field(std::string const& line, function f) {
	std::stringstream iss(line);
	std::string s;
	iss >> s;
	char c;
	if (s == "boolean") {
		std::string v;
		iss >> s >> c >> v;
		f(s, (v == "true") ? true : false);
	} else if (s == "s_int") {
		signed int v;
		iss >> s >> c >> v;
		f(s, v);
	} else if (s == "u_int") {
		unsigned int v;
		iss >> s >> c >> v;
		f(s, v);
	} else if (s == "float") {
		float v;
		iss >> s >> c >> v;
		f(s, v);
	} else if (s == "string") {
		std::string v;
		iss >> s >> c >> v;
		f(s, v);
	} else if (s == "Keys") {
		KeyLayout v;
		iss >> s >> c >> v;
		f(s, v);
	} else
		throw Exceptions::FileParsingException("Unsupported field type was encountered.");
}
#include "Settings.hpp"
#include "Shared/GameStateController.hpp"
void ObjectStorage::parse_object_line(std::string const& line) {
	switch (m_current_object) {
	case ObjectType::ClientSettings:
		if (GameStateController::is_client())
			parse_field(line, [this](std::string const& name, auto value) { m_objects->m_settings->addSetting(name, value); });
		break;
	case ObjectType::ServerSettings:
		if (GameStateController::is_server())
			parse_field(line, [this](std::string const& name, auto value) { m_objects->m_settings->addSetting(name, value); });
		break;
	case ObjectType::EnergyStorage:
		if (!m_objects->m_energy_storage.empty())
			parse_field(line, [this](std::string const& name, auto value) { m_objects->m_energy_storage.back()->set_value(name, value); });
		break;
	case ObjectType::FlyEngine:
		if (!m_objects->m_fly_engine.empty())
			parse_field(line, [this](std::string const& name, auto value) { m_objects->m_fly_engine.back()->set_value(name, value); });
		break;
	case ObjectType::Weapon:
		if (!m_objects->m_weapon.empty())
			parse_field(line, [this](std::string const& name, auto value) { m_objects->m_weapon.back()->set_value(name, value); });
		break;
	default:
		throw Exceptions::FileParsingException("Unsupported object type was encountered.");
		break;
	}
}

void ObjectStorage::parse_file_type_info(std::string const& line) {
	std::istringstream s(line);
	std::string string;
	size_t temp;
	s >> string >> temp >> string;
	if (string != "objects")
		throw Exceptions::FileParsingException("File seems to be corrupted");

	m_current_object_number = temp;
	m_current_object_counter = 0;
}
std::string ObjectStorage::generate_file_type_info() {
	std::ostringstream s;
	s << "with " << m_objects->size() << " objects";
	return s.str();
}
#include "../../Objects/Actors/MainActor.hpp"
#include "RenderInfoStorage.hpp"
std::shared_ptr<MainActor> string_to_main_actor(std::string &data, Objects *o) {
	std::istringstream s(data);
	std::string string;
	s >> string;
	if (string != "MainActor")
		throw Exceptions::FileParsingException("MainActor data seems to be corrupted.");

	float x, y;
	s >> x;
	auto mass = x;
	s >> x >> y;
	auto acceleration = mgl::math::vector(x, y);
	s >> x >> y;
	auto speed = mgl::math::vector(x, y);
	s >> x >> y;
	auto position = mgl::math::vector(x, y);
	s >> x >> y;
	auto size = mgl::math::vector(x, y);

	s >> string;
	auto render_info = RenderInfoStorage::getRenderInfo(string);
	auto ret = std::make_shared<MainActor>(mass, acceleration, speed, position, size, render_info);

	s >> string;
	if (string != "NO")
		ret->giveEnergyStorage(o->get_energy_storage(string));
	s >> string;
	if (string != "NO")
		ret->giveFlyEngine(o->get_fly_engine(string));
	s >> string;
	if (string != "NO")
		ret->giveLeftWeapon(o->get_weapon(string));
	s >> string;
	if (string != "NO")
		ret->giveRightWeapon(o->get_weapon(string));

	return ret;
}