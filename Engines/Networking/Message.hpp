#pragma once
#include <cstdint>
#include <vector>
#include <memory>
#include "MessageTypes.hpp"
class MessageInputStream;
class MessageOutputStream;
enum class NetworkChannel { ImportantData = 0, ActorData = 1, ControlsData = 2, ProjectileData = 3, OtherData = 4, AimData = 5, NoData = 6 };

class Message {
	friend MessageInputStream;
	friend MessageOutputStream;
protected:
	std::vector<uint8_t> m_data;
	const NetworkChannel m_channel;
	const bool m_is_important;

	std::vector<uint8_t>* operator->() { return &m_data; }
public:
	Message(NetworkChannel channel = NetworkChannel::NoData, bool is_important = false)
		: m_channel(channel), m_is_important(is_important) {}
	Message(size_t size, NetworkChannel channel = NetworkChannel::NoData, bool is_important = false)
		: m_data(size, 0u), m_channel(channel), m_is_important(is_important) {}
	Message(uint8_t *data, size_t size, NetworkChannel channel = NetworkChannel::NoData, bool is_important = false)
		: m_data(data, data + size), m_channel(channel), m_is_important(is_important) {}
	std::vector<uint8_t> const* operator->() const { return &m_data; }
	inline uint8_t channel() const { return uint8_t(m_channel); }
	inline bool is_important() const { return m_is_important; }
	virtual ~Message() {}
};
Message make_connection_message(uint8_t index);

class Map;
Message make_map_message(std::shared_ptr<Map> map);

class MainActorQueue;
Message make_actor_queue_message(MainActorQueue const& queue);