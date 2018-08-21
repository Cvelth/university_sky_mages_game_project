#pragma once
#include <thread>
#include <string>
#include <functional>
class Message;
//#include "MessageTypes.hpp"
class Networking {
public:
	static std::thread initialize_server(bool const& should_close, 
								  std::function<void(std::string const& ip, size_t port, std::function<void(Message const&)>)> peer_connected,
								  std::function<void(std::string const& ip, size_t port)> peer_disconnected, 
								  std::function<void(std::string const& ip, size_t port, Message const& data)> packet_received, size_t port = 7777);
	static std::thread initialize_client(std::function<bool()> should_close, std::function<void(Message const&)> packet_received, std::string ip = "127.0.0.1", size_t port = 7777);
	static inline std::thread initialize_client(bool const& should_close, std::function<void(Message const&)> packet_received, std::string ip = "127.0.0.1", size_t port = 7777) {
		return initialize_client([&should_close]() { return should_close; }, packet_received, ip, port);
	}

	static void send_to_server(Message const& message);
	static void bcast_from_server(Message const& message);
};
#include "../../Shared/AbstractException.hpp"
DefineNewException(NetworkingException);
DefineNewException(ConnectionException);
