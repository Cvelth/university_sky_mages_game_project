#include "MessageStream.hpp"
#include "Message.hpp"
MessageInputStream::MessageInputStream(Message const& data) : pos(0), data(data) {}
uint8_t MessageInputStream::read() {
	return data->at(pos++);
}
MessageOutputStream::MessageOutputStream(Message &data) : data(data) {
	data->clear();
}
MessageInputStream& operator>>(MessageInputStream &s, uint8_t &mt) {
	mt = s.read();
	return s;
}
MessageInputStream& operator>>(MessageInputStream &s, MessageType &mt) {
	uint8_t temp;
	s >> temp;
	mt = MessageType(temp);
	return s;
}

void MessageOutputStream::add(uint8_t v) {
	auto size = data->size();
	data->resize(size + 1);
	data->at(size) = v;
}
MessageOutputStream& operator<<(MessageOutputStream &s, uint8_t const& mt) {
	s.add(mt);
	return s;
}
MessageOutputStream& operator<<(MessageOutputStream &s, MessageType const& mt) {
	return s << uint8_t(mt);
}
