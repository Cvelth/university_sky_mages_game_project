#include "Networking.hpp"
#define NOMINMAX
#include "enet/enet.h"

size_t const client_number = 9;
size_t const channel_number = 3;

ENetHost *client_host;
ENetHost *server_host;
ENetPeer *server_peer = nullptr;

std::thread initialize_server(bool const& should_close, std::function<void(std::string const& ip, size_t port)> peer_connected, std::function<void(std::string const& ip, size_t port)> peer_disconnected, std::function<void(std::string)> packet_received, size_t port) {
	if (port > std::numeric_limits<uint16_t>::max())
		throw Exceptions::NetworkingException("Unsupported 'port' value was passed.");

	std::thread thread([&should_close, peer_connected, peer_disconnected, packet_received, port]() {
		if (enet_initialize() != 0)
			throw Exceptions::NetworkingException("Enet library cannot be initialized.");

		ENetAddress address;
		address.host = ENET_HOST_ANY;
		address.port = static_cast<uint16_t>(port);

		server_host = enet_host_create(&address, client_number, channel_number, 0, 0);
		if (server_host == NULL)
			throw Exceptions::NetworkingException("Server host cannot be initialized.");

		ENetEvent event;
		const size_t max_name_length = 100;
		char host_name[max_name_length];
		while (!should_close) {
			enet_host_service(server_host, &event, 1000);
			switch (event.type) {
				case ENET_EVENT_TYPE_CONNECT:
					enet_address_get_host(&event.peer->address, host_name, max_name_length);
					peer_connected(host_name, event.peer->address.port);
					break;
				case ENET_EVENT_TYPE_DISCONNECT:
					enet_address_get_host(&event.peer->address, host_name, max_name_length);
					peer_disconnected(host_name, event.peer->address.port);
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

		enet_host_destroy(server_host);
		enet_deinitialize();
	});
	return thread;
}

std::thread initialize_client(std::function<bool()> should_close, std::function<void(std::string)> packet_received, std::string ip, size_t port) {
	if (port > std::numeric_limits<uint16_t>::max())
		throw Exceptions::NetworkingException("Unsupported 'port' value was passed.");

	std::thread thread([should_close, packet_received, ip, port]() {
		if (enet_initialize() != 0)
			throw Exceptions::NetworkingException("Enet library cannot be initialized.");

		client_host = enet_host_create(NULL, 1, channel_number, 0, 0);
		if (client_host == NULL)
			throw Exceptions::NetworkingException("Client host cannot be initialized.");

		ENetAddress address;
		enet_address_set_host(&address, ip.c_str());
		address.port = static_cast<uint16_t>(port);

		server_peer = enet_host_connect(client_host, &address, channel_number, 0);
		if (server_peer == NULL)
			throw Exceptions::NetworkingException("Connection to cannot be established.");

		ENetEvent event;
		if (enet_host_service(client_host, &event, 15000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
			//connection was successful.
		} else {
			enet_peer_reset(server_peer);
			throw Exceptions::ConnectionException(("Cannot connect to " + ip).c_str());
		}
		
		while (!should_close()) {
			enet_host_service(client_host, &event, 1000);
			switch (event.type) {
				case ENET_EVENT_TYPE_DISCONNECT:
					throw Exceptions::ConnectionException(("Disconnected from " + ip).c_str());
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
		bool was_successful = false;
		enet_peer_disconnect(server_peer, 0);
		while (enet_host_service(client_host, &event, 3000) > 0) {
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
		enet_host_destroy(client_host);
		enet_deinitialize();
	});
	return thread;
}

void send_to_server(std::string const& data, size_t channel_id, bool important) {
	enet_peer_send(server_peer, static_cast<uint8_t>(channel_id), enet_packet_create(data.data(), data.size() + 1, important ? ENET_PACKET_FLAG_RELIABLE : 0));
	enet_host_flush(client_host);
}
void bcast_from_server(std::string const& data, size_t channel_id, bool important) {
	enet_host_broadcast(server_host, static_cast<uint8_t>(channel_id), enet_packet_create(data.data(), data.size() + 1, important ? ENET_PACKET_FLAG_RELIABLE : 0));
	enet_host_flush(server_host);
}