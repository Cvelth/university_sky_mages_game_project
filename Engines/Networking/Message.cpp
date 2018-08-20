#include "Message.hpp"
#include "MessageStream.hpp"
Message make_connection_message(uint8_t index) {
	Message message(NetworkChannel::ImportantData, true);
	return make_message(message, MessageType::Connection, index);
}
Message make_map_message(std::shared_ptr<Map> const map) {
	Message message(NetworkChannel::ImportantData, true);
	return make_message(message, MessageType::Map, map);
}
Message make_actor_queue_message(MainActorQueue const& queue) {
	Message message(NetworkChannel::ImportantData, true);
	return make_message(message, MessageType::ActorQueue, queue);
}
