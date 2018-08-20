#include "MessageStream.hpp"
Objects *ObjectsStatic::m_objects = nullptr;
#include "Message.hpp"
MessageInputStream::MessageInputStream(Message const& data) : pos(0), data(data) {}
MessageOutputStream::MessageOutputStream(Message &data) : data(data) { data->clear(); }
uint8_t MessageInputStream::read() { return data->at(pos++); }
uint8_t MessageInputStream::next() { return data->at(pos + 1); }
MessageInputStream::operator bool() const { return pos < data->size(); }
void MessageOutputStream::add(uint8_t v) { data->push_back(v); }

template <typename T>
union convert_to_bytes {
	T a;
	uint8_t b[sizeof(T)];
};

MessageInputStream& operator>>(MessageInputStream &s, uint8_t &v) {
	v = s.read();
	return s;
}
MessageOutputStream& operator<<(MessageOutputStream &s, uint8_t const& v) {
	s.add(v);
	return s;
}

MessageInputStream& operator>>(MessageInputStream &s, uint16_t &v) {
	convert_to_bytes<uint16_t> t;
	for (size_t i = 0; i < sizeof(uint16_t); i++)
		s >> t.b[i];
	v = t.a;
	return s;
}
MessageOutputStream& operator<<(MessageOutputStream &s, uint16_t const& v) {
	convert_to_bytes<uint16_t> t;
	t.a = v;
	for (size_t i = 0; i < sizeof(uint16_t); i++)
		s << t.b[i];
	return s;
}

MessageInputStream& operator>>(MessageInputStream &s, uint32_t &v) {
	convert_to_bytes<uint32_t> t;
	for (size_t i = 0; i < sizeof(uint32_t); i++)
		s >> t.b[i];
	v = t.a;
	return s;
}
MessageOutputStream& operator<<(MessageOutputStream &s, uint32_t const& v) {
	convert_to_bytes<uint32_t> t;
	t.a = v;
	for (size_t i = 0; i < sizeof(uint32_t); i++)
		s << t.b[i];
	return s;
}

MessageInputStream& operator>>(MessageInputStream &s, uint64_t &v) {
	convert_to_bytes<uint64_t> t;
	for (size_t i = 0; i < sizeof(uint64_t); i++)
		s >> t.b[i];
	v = t.a;
	return s;
}
MessageOutputStream& operator<<(MessageOutputStream &s, uint64_t const& v) {
	convert_to_bytes<uint64_t> t;
	t.a = v;
	for (size_t i = 0; i < sizeof(uint16_t); i++)
		s << t.b[i];
	return s;
}

MessageInputStream& operator>>(MessageInputStream &s, MessageType &v) {
	uint8_t temp;
	s >> temp;
	v = MessageType(temp);
	return s;
}
MessageOutputStream& operator<<(MessageOutputStream &s, MessageType const& v) {
	return s << uint8_t(v);
}

MessageInputStream& operator>>(MessageInputStream &s, float &v) {
	convert_to_bytes<float> t;
	for (size_t i = 0; i < sizeof(float); i++)
		s >> t.b[i];
	v = t.a;
	return s;
}
MessageOutputStream& operator<<(MessageOutputStream &s, float const& v) {
	convert_to_bytes<float> t;
	t.a = v;
	for (size_t i = 0; i < sizeof(float); i++)
		s << t.b[i];
	return s;
}

MessageInputStream& operator>>(MessageInputStream &s, std::string &v) {
	v.clear();
	uint8_t t;
	s >> t;
	while (t != uint8_t(0)) {
		v += t;
		s >> t;
	}
	return s;
}
MessageOutputStream& operator<<(MessageOutputStream &s, std::string const& v) {
	for (size_t i = 0; i < v.size(); i++)
		s << uint8_t(v[i]);
	s << uint8_t(0);
	return s;
}

#include "Objects/Map/Block.hpp"
#include "Engines/ObjectStorage/RenderInfoStorage.hpp"
MessageInputStream& operator>>(MessageInputStream &s, std::shared_ptr<Block> &v) {
	float f; 
	std::string r;
	s >> f >> r;
	v = std::make_shared<Block>(f, RenderInfoStorage::getRenderInfo(r));
	return s;
}
MessageOutputStream& operator<<(MessageOutputStream &s, std::shared_ptr<Block> const& v) {
	s << v->get() << RenderInfoStorage::getRenderInfo(v->renderInfo());
	return s;
}

#include <unordered_map>
#include "Objects/Map/Map.hpp"
class MapSharedPointerEnabler : public Map { public: MapSharedPointerEnabler(size_t width, size_t height, std::shared_ptr<Block> default_block) : Map(width, height, default_block) {} };
MessageInputStream& operator>>(MessageInputStream &s, std::shared_ptr<Map> &v) {
	std::unordered_map<uint8_t, std::shared_ptr<Block>> ids;

	uint8_t temp_id;
	std::shared_ptr<Block> temp_block;
	s >> temp_id;
	while (temp_id) {
		s >> temp_block;
		ids.insert(std::make_pair(temp_id, temp_block));
		s >> temp_id;
	}

	s >> temp_id >> temp_block;
	ids.insert(std::make_pair(temp_id, temp_block));

	uint32_t w, h;
	s >> w >> h;
	v = std::make_shared<MapSharedPointerEnabler>(w, h, temp_block);
	for (size_t y = 0; y < h; y++)
		for (size_t x = 0; x < w; x++) {
			s >> temp_id;
			v->set(ids[temp_id], x, y);
		}

	return s;
}
MessageOutputStream& operator<<(MessageOutputStream &s, std::shared_ptr<Map> const& v) {
	uint8_t current_id = 1;
	std::unordered_map<std::shared_ptr<Block>, uint8_t> ids;
	for (auto &it : v->m_blocks) {
		ids.insert(std::make_pair(it, current_id));
		s << current_id++ << it;
	}

	auto temp = std::find(v->m_blocks.begin(), v->m_blocks.end(), v->m_default_block);
	if (temp == v->m_blocks.end()) {
		ids.insert(std::make_pair(v->m_default_block, current_id));
		s << uint8_t(0) << current_id++ << v->m_default_block;
	} else
		s << uint8_t(0) << ids[*temp] << (*temp);

	s << uint32_t(v->width()) << uint32_t(v->height());
	for (size_t y = 0; y < v->height(); y++)
		for (size_t x = 0; x < v->width(); x++)
			s << ids[v->get(x, y)];

	return s;
}

#include "Objects/Actors/MainActor.hpp"
#include "Objects/EquipableItems/EnergyStorage.hpp"
#include "Objects/EquipableItems/FlyEngine.hpp"
#include "Objects/EquipableItems/Weapon.hpp"
#include "Engines/ObjectStorage/Objects.hpp"
MessageInputStream& operator>>(MessageInputStream &s, std::shared_ptr<MainActor> &v) {
	//float mass, vector const& acceleration, vector const& speed, vector const& position, vector const& size, std::shared_ptr<RenderInfo> render_info
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

	std::string string;
	s >> string;
	auto render_info = RenderInfoStorage::getRenderInfo(string);
	v = std::make_shared<MainActor>(mass, acceleration, speed, position, size, render_info);

	s >> string;
	if (string != "")
		v->giveEnergyStorage(ObjectsStatic::get()->get_energy_storage(string));
	s >> string;
	if (string != "")
		v->giveFlyEngine(ObjectsStatic::get()->get_fly_engine(string));
	s >> string;
	if (string != "")
		v->giveLeftWeapon(ObjectsStatic::get()->get_weapon(string));
	s >> string;
	if (string != "")
		v->giveRightWeapon(ObjectsStatic::get()->get_weapon(string));

	return s;
}
MessageOutputStream& operator<<(MessageOutputStream &s, std::shared_ptr<MainActor> const& v) {
	s << v->m_mass << v->m_acceleration.at(0) << v->m_acceleration.at(1)
		<< v->m_speed.at(0) << v->m_speed.at(1) << v->m_position.at(0) << v->m_position.at(0)
		<< v->m_size.at(0) << v->m_size.at(1) << RenderInfoStorage::getRenderInfo(v->m_render_info)
		<< (v->m_energy_storage ? v->m_energy_storage->name() : "")
		<< (v->m_engine ? v->m_engine->name() : "")
		<< (v->m_weapon_left_arm ? v->m_weapon_left_arm->name() : "")
		<< (v->m_weapon_right_arm ? v->m_weapon_right_arm->name() : "");
	return s;
}
#include "Objects/ObjectState/ObjectQueue.hpp"
MessageInputStream& operator>>(MessageInputStream &s, MainActorQueue &v) {
	v.clear();
	uint16_t size;
	s >> size;
	std::shared_ptr<MainActor> a;
	for (uint16_t i = 0; i < size; i++) {
		s >> a;
		v.add(a);
	}
	return s;
}
MessageOutputStream& operator<<(MessageOutputStream &s, MainActorQueue const& v) {
	s << uint16_t(v.size());
	v.for_each([&s](std::shared_ptr<MainActor> a) {
		s << a;
	});
	return s;
}
/*
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
}*/