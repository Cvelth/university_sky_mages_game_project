#pragma once
#include <string>
#include <functional>
void initialize_server(bool &should_close, std::function<void()> peer_connected, std::function<void()> peer_disconnected, std::function<void(std::string)> packet_received, size_t port = 7777);
void initialize_client(bool &should_close, std::function<void(std::string)> packet_received, std::string ip = "127.0.0.1", size_t port = 7777);

void send_to_server(std::string const& data, size_t channel_id = 0, bool important = false);
void bcast_from_server(std::string const& data, size_t channel_id = 0, bool important = false);

#include "Shared\AbstractException.hpp"
DefineNewException(NetworkingException);
DefineNewException(ConnectionException);