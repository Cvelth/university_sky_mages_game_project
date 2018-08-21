#include "Networking.hpp"
#define NOMINMAX
#include "enet/enet.h"

size_t const client_number = 9;

ENetHost *client_host;
ENetHost *server_host;
ENetPeer *server_peer = nullptr;

#include "Message.hpp"
#include "Shared/GameStateController.hpp"
std::thread Networking::initialize_server(bool const& should_close, std::function<void(std::string const& ip, size_t port, std::function<void(Message const&)>)> peer_connected, std::function<void(std::string const& ip, size_t port)> peer_disconnected, std::function<void(std::string const& ip, size_t port, Message const& data)> packet_received, size_t port) {
	if (!GameStateController::is_server())
		throw Exceptions::NetworkingException("Networking server must not be initialized on client program.");
	if (port > std::numeric_limits<uint16_t>::max())
		throw Exceptions::NetworkingException("Unsupported 'port' value was passed.");

	std::thread thread([&should_close, peer_connected, peer_disconnected, packet_received, port]() {
		if (enet_initialize() != 0)
			throw Exceptions::NetworkingException("Enet library cannot be initialized.");

		ENetAddress address;
		address.host = ENET_HOST_ANY;
		address.port = static_cast<uint16_t>(port);

		server_host = enet_host_create(&address, client_number, size_t(NetworkChannel::NoData), 0, 0);
		if (server_host == NULL)
			throw Exceptions::NetworkingException("Server host cannot be initialized. There's a possibility of another copy of server running on the target machine.");

		ENetEvent event;
		const size_t max_name_length = 100;
		char host_name[max_name_length];
		GameStateController::change_networking_loop_state(true);
		while (!should_close) {
			enet_host_service(server_host, &event, 3);
			switch (event.type) {
				case ENET_EVENT_TYPE_CONNECT:
					enet_address_get_host(&event.peer->address, host_name, max_name_length);
					peer_connected(host_name, event.peer->address.port, [&event](Message const& data) {
						enet_peer_send(event.peer, 0, enet_packet_create(data->data(), data->size() + 1, ENET_PACKET_FLAG_RELIABLE));
					});
					break;
				case ENET_EVENT_TYPE_DISCONNECT:
					enet_address_get_host(&event.peer->address, host_name, max_name_length);
					peer_disconnected(host_name, event.peer->address.port);
					break;
				case ENET_EVENT_TYPE_RECEIVE:
					enet_address_get_host(&event.peer->address, host_name, max_name_length);
					packet_received(host_name, event.peer->address.port, Message(event.packet->data, 
																				 event.packet->dataLength));
					enet_packet_destroy(event.packet);
					break;
				case ENET_EVENT_TYPE_NONE:
					break;
				default:
					throw Exceptions::NetworkingException("Unsupported packet was received");
			}
		}

		GameStateController::change_networking_loop_state(false);
		enet_host_destroy(server_host);
		enet_deinitialize();
	});
	return thread;
}

std::thread Networking::initialize_client(std::function<bool()> should_close, std::function<void(Message const&)> packet_received, std::string ip, size_t port) {
	if (!GameStateController::is_client())
		throw Exceptions::NetworkingException("Networking client must not be initialized on server program.");
	if (port > std::numeric_limits<uint16_t>::max())
		throw Exceptions::NetworkingException("Unsupported 'port' value was passed.");

	std::thread thread([should_close, packet_received, ip, port]() {
		if (enet_initialize() != 0)
			throw Exceptions::NetworkingException("Enet library cannot be initialized.");

		client_host = enet_host_create(NULL, 1, size_t(NetworkChannel::NoData), 0, 0);
		if (client_host == NULL)
			throw Exceptions::NetworkingException("Client host cannot be initialized.");

		ENetAddress address;
		enet_address_set_host(&address, ip.c_str());
		address.port = static_cast<uint16_t>(port);

		server_peer = enet_host_connect(client_host, &address, size_t(NetworkChannel::NoData), 0);
		if (server_peer == NULL)
			throw Exceptions::NetworkingException("Connection to cannot be established.");

		ENetEvent event;
		if (enet_host_service(client_host, &event, 15000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
			//connection was successful.
		} else {
			enet_peer_reset(server_peer);
			throw Exceptions::ConnectionException(("Cannot connect to " + ip).c_str());
		}

		GameStateController::change_networking_loop_state(true);
		while (!should_close()) {
			enet_host_service(client_host, &event, 3);
			switch (event.type) {
				case ENET_EVENT_TYPE_DISCONNECT:
					throw Exceptions::ConnectionException(("Disconnected from " + ip).c_str());
					break;
				case ENET_EVENT_TYPE_RECEIVE:
					packet_received(Message(event.packet->data, 
											event.packet->dataLength));
					enet_packet_destroy(event.packet);
					break;
				case ENET_EVENT_TYPE_NONE:
					break;
				default:
					throw Exceptions::NetworkingException("Unsupported packet was received");
			}
		}
		GameStateController::change_networking_loop_state(false);
		bool was_successful = false;
		enet_peer_disconnect(server_peer, 0);
		while (enet_host_service(client_host, &event, 3000) > 0) {
			switch (event.type) {
				case ENET_EVENT_TYPE_RECEIVE:
					packet_received(Message(event.packet->data, 
											event.packet->dataLength));
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

void Networking::send_to_server(Message const& message) {
	if (!message)
		throw Exceptions::ConnectionException("Corrupted message was attempted to be sent.");
	enet_peer_send(server_peer, message.channel(), enet_packet_create(message->data(), message->size() + 1, message.is_important() ? ENET_PACKET_FLAG_RELIABLE : 0));
}
void Networking::bcast_from_server(Message const& message) {
	if (!message)
		throw Exceptions::ConnectionException("Corrupted message was attempted to be sent.");
	enet_host_broadcast(server_host, message.channel(), enet_packet_create(message->data(), message->size() + 1, message.is_important() ? ENET_PACKET_FLAG_RELIABLE : 0));
}