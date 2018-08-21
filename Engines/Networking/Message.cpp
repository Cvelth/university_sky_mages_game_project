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
Message make_actor_queue_update_message(MainActorQueue const& queue) {
	Message message(NetworkChannel::ActorData, false);
	return make_message(message, MessageType::ActorQueueUpdate, update(queue));
}
Message make_projectile_queue_message(ProjectileQueue const& queue) {
	Message message(NetworkChannel::ProjectileData, false);
	return make_message(message, MessageType::ProjectileQueueUpdate, queue);
}
Message make_control_event_message(ControlEvent const& ev, bool dir) {
	Message message(NetworkChannel::ControlsData, false);
	return make_message(message, MessageType::ControlEvent, ev, dir);
}
Message make_aim_event_message(float x, float y) {
	Message message(NetworkChannel::AimData, false);
	return make_message(message, MessageType::AimEvent, x, y);
}