#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include "MessageTypes.hpp"
class Message;
class Block;
class Map;
class MainActor;
class MainActorQueue;
class Objects;
class ShootableObject;
class ProjectileQueue;
enum class ControlEvent;
enum class ShootableObjectType;

template <typename Type>
class StaticObject {
	static std::shared_ptr<Type> m_object;
public:
	static void initialize(std::shared_ptr<Type> object) { m_object = object; }
	static std::shared_ptr<Type> get() { return m_object; }
};
using ObjectsStatic = StaticObject<Objects>;
using ActorsStatic = StaticObject<MainActorQueue>;

template <typename Type>
class Update {
	Type &data;
public:
	explicit Update(Type &data) : data(data) {}
	Type* operator->() { return &data; }
	Type const* operator->() const { return &data; }
	Type& operator*() { return data; }
	Type const& operator*() const { return data; }
};
template <typename Type>
Update<Type> update(Type &data) { return Update<Type>(data); }

class MessageInputStream {
private:
	Message const& data;
	size_t pos;
protected:
	uint8_t read();
	uint8_t next();
public:
	MessageInputStream(Message const& data);
	Message const& message() const { return data; }
	operator bool() const;

	friend MessageInputStream& operator>>(MessageInputStream &s, uint8_t &v);
	friend MessageInputStream& operator>>(MessageInputStream &s, uint16_t &v);
	friend MessageInputStream& operator>>(MessageInputStream &s, uint32_t &v);
	friend MessageInputStream& operator>>(MessageInputStream &s, uint64_t &v);
	friend MessageInputStream& operator>>(MessageInputStream &s, float &v);
	friend MessageInputStream& operator>>(MessageInputStream &s, bool &v);
	friend MessageInputStream& operator>>(MessageInputStream &s, MessageType &v);
	friend MessageInputStream& operator>>(MessageInputStream &s, std::string &v);
	friend MessageInputStream& operator>>(MessageInputStream &s, std::shared_ptr<Block> &v);
	friend MessageInputStream& operator>>(MessageInputStream &s, std::shared_ptr<Map> &v);
	friend MessageInputStream& operator>>(MessageInputStream &s, std::shared_ptr<MainActor> &v);
	friend MessageInputStream& operator>>(MessageInputStream &s, Update<std::shared_ptr<MainActor>> &v);
	friend MessageInputStream& operator>>(MessageInputStream &s, MainActorQueue &v);
	friend MessageInputStream& operator>>(MessageInputStream &s, Update<MainActorQueue> &v);
	friend MessageInputStream& operator>>(MessageInputStream &s, std::shared_ptr<ShootableObject> &v);
	friend MessageInputStream& operator>>(MessageInputStream &s, ProjectileQueue &v);
	friend MessageInputStream& operator>>(MessageInputStream &s, ControlEvent &v);
	friend MessageInputStream& operator>>(MessageInputStream &s, ShootableObjectType &v);
};

class MessageOutputStream {
private:
	Message &data;
protected:
	void add(uint8_t v);
public:
	MessageOutputStream(Message &data);
	Message const& message() const { return data; }

	friend MessageOutputStream& operator<<(MessageOutputStream &s, uint8_t const& v);
	friend MessageOutputStream& operator<<(MessageOutputStream &s, uint16_t const& v);
	friend MessageOutputStream& operator<<(MessageOutputStream &s, uint32_t const& v);
	friend MessageOutputStream& operator<<(MessageOutputStream &s, uint64_t const& v);
	friend MessageOutputStream& operator<<(MessageOutputStream &s, float const& v);
	friend MessageOutputStream& operator<<(MessageOutputStream &s, bool const& v);
	friend MessageOutputStream& operator<<(MessageOutputStream &s, MessageType const& v);
	friend MessageOutputStream& operator<<(MessageOutputStream &s, std::string const& v);
	friend MessageOutputStream& operator<<(MessageOutputStream &s, std::shared_ptr<Block> const& v);
	friend MessageOutputStream& operator<<(MessageOutputStream &s, std::shared_ptr<Map> const& v);
	friend MessageOutputStream& operator<<(MessageOutputStream &s, std::shared_ptr<MainActor> const& v);
	friend MessageOutputStream& operator<<(MessageOutputStream &s, Update<std::shared_ptr<MainActor> const> const& v);
	friend MessageOutputStream& operator<<(MessageOutputStream &s, MainActorQueue const& v);
	friend MessageOutputStream& operator<<(MessageOutputStream &s, Update<MainActorQueue const> const& v);
	friend MessageOutputStream& operator<<(MessageOutputStream &s, std::shared_ptr<ShootableObject> const& v);
	friend MessageOutputStream& operator<<(MessageOutputStream &s, ProjectileQueue const& v);
	friend MessageOutputStream& operator<<(MessageOutputStream &s, ControlEvent const& v);
	friend MessageOutputStream& operator<<(MessageOutputStream &s, ShootableObjectType const& v);
};

template <typename Head>
inline void _make_message(MessageOutputStream &message, Head const& head) {
	message << head;
}

template <typename Head, typename... Tail>
inline void _make_message(MessageOutputStream &message, Head const& head, Tail const& ...tail) {
	_make_message(message, head);
	_make_message(message, tail...);
}

template <typename Head, typename... Tail>
inline Message make_message(Message &message, Head const& head, Tail const& ...tail) {
	MessageOutputStream stream(message);
	_make_message(stream, head, tail...);
	return message;
}