#include "Networking.hpp"
#include <thread>
#define NOMINMAX
#include "enet/enet.h"

size_t const client_number = 9;
size_t const channel_number = 3;

bool is_server = false;
bool is_client = false;

ENetHost *host;
ENetPeer *server_peer = nullptr;

void initialize_server(bool const& should_close, std::function<void()> peer_connected, std::function<void()> peer_disconnected, std::function<void(std::string)> packet_received, size_t port) {
	if (is_server || is_client) throw Exceptions::NetworkingException("Cannot initialize server. NetworkingEngine was already initialized.");
	is_server = true;

	if (port > std::numeric_limits<uint16_t>::max())
		throw Exceptions::NetworkingException("Unsupported 'port' value was passed.");

	std::thread thread([&]() {
		if (enet_initialize() != 0)
			throw Exceptions::NetworkingException("Enet library cannot be initialized.");

		ENetAddress address;
		address.host = ENET_HOST_ANY;
		address.port = static_cast<uint16_t>(port);

		host = enet_host_create(&address, client_number, channel_number, 0, 0);
		if (host == NULL)
			throw Exceptions::NetworkingException("Server host cannot be initialized.");

		ENetEvent event;
		while (!should_close) {
			enet_host_service(host, &event, 1000);
			switch (event.type) {
				case ENET_EVENT_TYPE_CONNECT:
					peer_connected();
					break;
				case ENET_EVENT_TYPE_DISCONNECT:
					peer_disconnected();
					break;
				case ENET_EVENT_TYPE_RECEIVE:
					packet_received(reinterpret_cast<char*>(event.packet->data));
					enet_packet_destroy(event.packet);
					break;
				case ENET_EVENT_TYPE_NONE:
					break;
				default:
					throw Exceptions::NetworkingException("Unsupported packet was received");
			}
		}

		enet_host_destroy(host);
		enet_deinitialize();
	});
	thread.detach();
	is_server = false;
}

void initialize_client(bool const& should_close, std::function<void(std::string)> packet_received, std::string ip, size_t port) {
	if (is_server || is_client) throw Exceptions::NetworkingException("Cannot initialize client. NetworkingEngine was already initialized.");
	is_client = true;

	if (port > std::numeric_limits<uint16_t>::max())
		throw Exceptions::NetworkingException("Unsupported 'port' value was passed.");

	std::thread thread([&]() {
		if (enet_initialize() != 0)
			throw Exceptions::NetworkingException("Enet library cannot be initialized.");

		host = enet_host_create(NULL, 1, channel_number, 0, 0);
		if (host == NULL)
			throw Exceptions::NetworkingException("Client host cannot be initialized.");

		ENetAddress address;
		enet_address_set_host(&address, ip.c_str());
		address.port = static_cast<uint16_t>(port);

		server_peer = enet_host_connect(host, &address, channel_number, 0);
		if (server_peer == NULL)
			throw Exceptions::NetworkingException("Connection to cannot be established.");

		ENetEvent event;
		if (enet_host_service(host, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
			//connection was successful.
		} else {
			enet_peer_reset(server_peer);
			throw Exceptions::ConnectionException(("Cannot connect to " + ip).c_str());
		}
		
		while (!should_close) {
			enet_host_service(host, &event, 1000);
			switch (event.type) {
				case ENET_EVENT_TYPE_DISCONNECT:
					throw Exceptions::ConnectionException(("Disconnected from " + ip).c_str());
					break;
				case ENET_EVENT_TYPE_RECEIVE:
					packet_received(reinterpret_cast<char*>(event.packet->data));
					enet_packet_destroy(event.packet);
					break;
				default:
					throw Exceptions::NetworkingException("Unsupported packet was received");
			}
		}
		bool was_successful = false;
		enet_peer_disconnect(server_peer, 0);
		while (enet_host_service(host, &event, 3000) > 0) {
			switch (event.type) {
				case ENET_EVENT_TYPE_RECEIVE:
					packet_received(reinterpret_cast<char*>(event.packet->data));
					enet_packet_destroy(event.packet);
					break;
				case ENET_EVENT_TYPE_DISCONNECT:
					was_successful = true;
					return;
			}
		}
		if (!was_successful) enet_peer_reset(server_peer);
		server_peer = nullptr;
		enet_host_destroy(host);
		enet_deinitialize();
	});
	thread.detach();
	is_client = false;
}

void send_to_server(std::string const& data, size_t channel_id, bool important) {
	if (!is_client) throw Exceptions::NetworkingException("Client wasn't initialized. Can't send data to server.");

	enet_peer_send(server_peer, static_cast<uint8_t>(channel_id), enet_packet_create(data.data(), data.size() + 1, important ? ENET_PACKET_FLAG_RELIABLE : 0));
	enet_host_flush(host);
}
void bcast_from_server(std::string const& data, size_t channel_id, bool important) {
	if (!is_server) throw Exceptions::NetworkingException("Server wasn't initialized. Can't brodcast data.");

	enet_host_broadcast(host, static_cast<uint8_t>(channel_id), enet_packet_create(data.data(), data.size() + 1, important ? ENET_PACKET_FLAG_RELIABLE : 0));
	enet_host_flush(host);
}