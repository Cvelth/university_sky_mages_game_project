#include "Message.hpp"
#include "MessageStream.hpp"
Message make_connection_message(uint8_t index) {
	Message message(NetworkChannel::ImportantData, true);
	return make_message(message, MessageType::Connection, index);
}
MessageType Message::type() const {
	return MessageType(m_data[0]);
}