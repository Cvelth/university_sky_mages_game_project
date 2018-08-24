#include "server.hpp"
#include <thread>
#include <iostream>
#include "Objects/Objects/ObjectQueue.hpp"
#include "Objects/Actors/MainActor.hpp"

#include "Engines/Networking/Message.hpp"
#include "Engines/Networking/Networking.hpp"
#include "Engines/Networking/NetworkController.hpp"
#include "Engines/Networking/ParseMessage.hpp"
std::thread initialize_networking(bool &server_should_close, std::shared_ptr<Objects> objects, std::shared_ptr<Map> &map, MainActorQueue &actors, Clients &clients) {
	auto on_peer_connect = [&map, &actors, objects, &clients](std::string const& name, size_t port, std::function<void(Message const&)> send_back) {
		std::cout << "\rClient " << name << ":" << port << " is attempting to connect.\n";
		auto id = actors.size();
		send_back(make_connection_message(uint8_t(id)));
		clients.insert(std::make_pair(std::make_pair(name, port), id));
		actors_add(objects, actors);
		map_broadcast(map);
		std::cout << "\rClient " << name << ":" << port << " (id - " << id << ") has been connected.\n";

		std::cout << ": ";
	};
	auto on_peer_disconnect = [&clients, &actors](std::string const& name, size_t port) {
		std::cout << "\rClient " << name << ":" << port << " (id - " << clients[std::make_pair(name, port)] << ") has been disconnected.\n: ";
		actors[clients[std::make_pair(name, port)]]->die();
	};
	auto on_packet_received = [&clients, &actors](std::string const& name, size_t port, Message const& data) {
		auto id = clients[std::make_pair(name, port)];
		parse_message_from_client(data, overload([&actors, &id](ControlEvent const& ev, bool dir) {
			NetworkController::accept_control_event(actors[id], ev, dir);
		}, [&actors, &id](float x, float y) {
			NetworkController::accept_aim_event(actors[id], x, y);
		}));
	};

	return Networking::initialize_server(server_should_close, on_peer_connect, on_peer_disconnect, on_packet_received);
}

#include "Engines/ObjectStorage/Objects.hpp"
#include "Engines/ObjectStorage/Settings.hpp"
#include "Engines/Physics/PhysicsEngine.hpp"
std::thread initialize_physics(PhysicsEngine *&engine, bool &server_should_close, std::shared_ptr<Objects> objects, std::shared_ptr<Map> &map, MainActorQueue &actors, DoubleProjectileQueue &projectiles, ObjectQueue &miscellaneous) {
	engine = new PhysicsEngine([&server_should_close](void) { return server_should_close; }, actors, projectiles, miscellaneous);
	engine->changeUpdateInterval(1'000'000 / objects->settings()->getUintValue("Physical_Updates_Per_Second"));
	engine->initializeCollisionSystem(map);
	return std::thread(&PhysicsEngine::loop, engine, false);
}

inline void help_() {
	std::cout << "Supported commands:\n"
		<< " - map - access to map-related commands. Call \"map help\" for more details.\n"
		<< " - actors - access to actor-related commands. Call \"actors help\" for more details.\n"
		<< " - clients - access to clients-related commands. Call \"clients help\" for more details.\n"
		<< " - exit - closes the server after cleaning up.\n";
}
inline void exit_(bool &server_should_close) {
	server_should_close = true;
}
void server_command(bool &server_should_close, std::shared_ptr<Map> &map, MainActorQueue &actors, Clients &clients, std::istream &input) {
	std::string string;
	input >> string;
	if (string == "map")
		map_(map, input);
	else if (string == "actors")
		actors_(actors, input);
	else if (string == "clients")
		clients_(clients, input);
	else if (string == "help")
		help_();
	else if (string == "exit")
		exit_(server_should_close);
	else
		std::cout << "Unsupported server command.\nCall \"help\" for list of supported ones.\n";
}

#include "Shared/SharedFunctions.hpp"
PhysicsEngine *physics_engine = nullptr;
void server_process(std::shared_ptr<Objects> objects) {
	std::cout << "Starting server...";
	bool server_should_close = false;
	Clients clients;

	std::shared_ptr<Map> map;
	MainActorQueue actors;
	DoubleProjectileQueue projectiles;
	ObjectQueue miscellaneous;

	std::cout << "\rInitializing networking engine...";
	auto networking_thread = initialize_networking(server_should_close, objects, map, actors, clients);
	std::cout << "\rInitializing physics engine...";
	auto physics_thread = initialize_physics(physics_engine, server_should_close, objects, map, actors, projectiles, miscellaneous);
	std::cout << '\r' << objects->get_program_version() << " server has been started.\n";

	for (auto it : Functions::split_string(objects->settings()->getStringValue("Initialization_Commands"), ";")) {
		std::cout << "\r: " << it << "\n";
		std::istringstream input(it);
		server_command(server_should_close, map, actors, clients, input);
	}

	while (!server_should_close) {
		std::cout << ": ";
		std::string string;
		std::getline(std::cin, string);
		std::istringstream input(string);
		server_command(server_should_close, map, actors, clients, input);
	}

	std::cout << "Cleaning up...";
	networking_thread.join();
	physics_thread.join();
}