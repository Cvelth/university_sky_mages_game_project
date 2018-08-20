#pragma once
#include <cstdint>
#include <memory>
#include "MessageTypes.hpp"
class Message;
class Map;

class MessageInputStream {
private:
	Message const& data;
	size_t pos;
protected:
	uint8_t read();
public:
	MessageInputStream(Message const& data);
	Message const& message() const { return data; }

	friend MessageInputStream& operator>>(MessageInputStream &s, uint8_t &mt);
	friend MessageInputStream& operator>>(MessageInputStream &s, MessageType &mt);
	friend MessageInputStream& operator>>(MessageInputStream &s, std::shared_ptr<Map> mt);
};

class MessageOutputStream {
private:
	Message &data;
protected:
	void add(uint8_t v);
public:
	MessageOutputStream(Message &data);
	Message const& message() const { return data; }

	friend MessageOutputStream& operator<<(MessageOutputStream &s, uint8_t const& mt);
	friend MessageOutputStream& operator<<(MessageOutputStream &s, MessageType const& mt);
	friend MessageOutputStream& operator<<(MessageOutputStream &s, std::shared_ptr<Map> const mt);
};

template <typename Head>
inline void _make_message(MessageOutputStream &message, Head head) {
	message << head;
}

template <typename Head, typename... Tail>
inline void _make_message(MessageOutputStream &message, Head head, Tail ...tail) {
	_make_message(message, head);
	_make_message(message, tail...);
}

template <typename Head, typename... Tail>
inline Message make_message(Message &message, Head head, Tail ...tail) {
	MessageOutputStream stream(message);
	_make_message(stream, head, tail...);
	return message;
}