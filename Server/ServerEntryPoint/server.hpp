#pragma once
#include "Shared/AbstractException.hpp"
#include "Shared/GameStateController.hpp"
#include "Engines/ObjectStorage/Objects.hpp"
#include "Engines/ObjectStorage/Settings.hpp"
void server_process(std::shared_ptr<Objects> objects);
int server_main(int argc, char **argv) {
	GameStateController::initialize(ProgramMode::Server);
	auto objects = initialize_object_storage();
	initialize_render_info(objects->settings()->getStringValue("Render_Info_Path"));
	try {
		server_process(objects);
	} catch (std::exception &e) {
		e.what();
		getchar(); // Prevents Program from closing.
	}
	exit(0);
}

#include <memory>
#include <sstream>
#include <iostream>
#include <thread>
#include <map>
#include "Objects/ObjectState/ObjectQueue.hpp"
#include "Engines/Physics/PhysicsEngine.hpp"
#include "Shared/SharedFunctions.hpp"
class Map;
using Clients = std::map<std::pair<std::string, size_t>, size_t>;
inline std::thread initialize_networking(bool &server_should_close, std::shared_ptr<Objects> objects, std::shared_ptr<Map> &map, MainActorQueue &actors, Clients &clients);
inline std::thread initialize_physics(PhysicsEngine *&engine, bool &server_should_close, std::shared_ptr<Objects> objects, std::shared_ptr<Map> &map, MainActorQueue &actors, DoubleProjectileQueue &projectiles, ObjectQueue &miscellaneous);
void server_command(bool &server_should_close, std::shared_ptr<Map> &map, MainActorQueue &actors, Clients &clients, std::istream &input);

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

	std::cout << "\rRunning initializations commands:\n";
	for (auto it : Functions::split_string(objects->settings()->getStringValue("Initialization_Commands"), ";")) {
		std::cout << "Initialization_Command :: " << it << "\n";
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

inline void map_(std::shared_ptr<Map> &map, std::istream &input);
inline void actors_(MainActorQueue &actors, std::istream &input);
inline void clients_(Clients &clients, std::istream &input);
inline void help_();
inline void exit_(bool &server_should_close);
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

#include "Engines/ObjectStorage/MapStorage.hpp"
#include "Objects/Map/MapGenerator.hpp"
#include "Engines/Networking/Networking.hpp"
#include "Engines/Networking/Message.hpp"
inline void map_save(std::shared_ptr<Map> &map, std::istream &input);
inline void map_generate(std::shared_ptr<Map> &map, std::istream &input);
inline void map_load(std::shared_ptr<Map> &map, std::istream &input);
inline void map_broadcast(std::shared_ptr<Map> &map);
inline void map_help();
inline void map_(std::shared_ptr<Map> &map, std::istream &input) {
	std::string string;
	input >> string;
	if (string == "save")
		map_save(map, input);
	else if (string == "generate")
		map_generate(map, input);
	else if (string == "load")
		map_load(map, input);
	else if (string == "broadcast")
		map_broadcast(map);
	else if (string == "help")
		map_help();
	else
		std::cout << "Unsupported map-related server command.\nCall \"map help\" for list of supported ones.\n";
}
inline void map_save(std::shared_ptr<Map> &map, std::istream &input) {
	std::cout << "Saving map...";
	std::ostringstream filename;
	filename << "Map_" << std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::string string = filename.str();
	if (input) input >> string;
	if (map) {
		MapStorage storage;
		storage.save(map, string, "maps/");
		std::cout << "\rMap was saved.\n";
	} else
		std::cout << "Cannot save non-existing map. Try generating or loading one.\n";
}
inline void map_generate(std::shared_ptr<Map> &map, std::istream &input) {
	std::cout << "Generating map...";
	size_t width = 120, height = 80;
	if (input) {
		input >> width >> height;
	}
	map = std::shared_ptr<Map>(MapGenerator::generate_continious_map(width, height));
	std::cout << "\rMap was generated.\n";
	map_broadcast(map);
}
inline void map_load(std::shared_ptr<Map> &map, std::istream &input) {
	std::cout << "Loading map...";
	if (input) {
		std::string string;
		input >> string;
		MapStorage storage;
		try {
			map = std::shared_ptr<Map>(storage.load(string, "maps/"));
			std::cout << "\rMap was loaded.\n"; 
			map_broadcast(map);
		} catch (Exceptions::FileCannotBeOpennedException e) {
			e.print();
		} catch (Exceptions::FileParsingException e) {
			e.print();
		}
	} else
		std::cout << "\rUnable to load map: <filename> required.\nCall \"map help\" for more info.\n";
}
inline void map_broadcast(std::shared_ptr<Map> &map) {
	std::cout << "Broadcasting map...";
	if (map) {
		Networking::bcast_from_server(make_map_message(map));
		physics_engine->initializeCollisionSystem(map);
		std::cout << "\rMap was broadcasted.\n";
	} else
		std::cout << "\rCannot broadcast non-existing map. Try generating or loading one.\n";
}
inline void map_help() {
	std::cout << "Supported commands:\n"
		<< " - map save [<filename>] - saves currently loaded map to /maps/<filename>.mpf.\n"
		<< " - map load <filename> - loads a map from /maps/<filename>.mpf.\n"
		<< " - map generate [<width> <height>] - generates new map (<width>x<height>). Default values: width = 120 and height = 80.\n"
		<< " - map broadcast - sends map to all the client hosts.\n";
}

#include "Objects\Actors\MainActor.hpp"
#include "Engines\ObjectStorage\Objects.hpp"
#include "Engines\ObjectStorage\RenderInfoStorage.hpp"
inline void actors_broadcast(MainActorQueue &actors);
inline void actors_help();
inline void actors_(MainActorQueue &actors, std::istream &input) {
	std::string string;
	input >> string;if (string == "broadcast")
		actors_broadcast(actors);
	else if (string == "help")
		actors_help();
	else
		std::cout << "Unsupported actors-related server command.\nCall \"actors help\" for list of supported ones.\n";
}
inline void actors_broadcast(MainActorQueue &actors) {
	std::cout << "Broadcasting actor queue...";
	Networking::bcast_from_server(make_actor_queue_message(actors));
	std::cout << "\rActor queue was broadcasted.\n";
}
inline void actors_help() {
	std::cout << "Supported commands:\n"
		<< " - [[deprecated]] actors initialize - initializes server main actor queue with 9 actors (and their default colors).\n"
		<< " - [[deprecated]] actors clean - cleans actor data.\n"
		<< " - actors broadcast - sends actor data to all the clients.\n";
}
#include "Objects/EquipableItems/EnergyStorage.hpp"
#include "Objects/EquipableItems/FlyEngine.hpp"
#include "Objects/EquipableItems/Shield.hpp"
#include "Objects/EquipableItems/Trinket.hpp"
#include "Objects/EquipableItems/Weapon.hpp"
inline void actors_add(std::shared_ptr<Objects> objects, MainActorQueue &actors) {
	std::cout << "\rAdding new actor...";
	auto actor = std::make_shared<MainActor>(60.f, mgl::math::vector{0.f,0.f}, mgl::math::vector{0.f,0.f}, mgl::math::vector{30.f,50.f}, mgl::math::vector{2.f, 4.f}, RenderInfoStorage::getRenderInfo("MainActor", actors.size()));

	auto energy_storage = objects->get_energy_storage("");
	actor->giveEnergyStorage(energy_storage);

	auto fly_engine = objects->get_fly_engine("");
	actor->giveFlyEngine(fly_engine);

	auto left_weapon = objects->get_weapon("");
	actor->giveLeftWeapon(left_weapon);

	auto right_weapon = objects->get_weapon("");
	actor->giveRightWeapon(right_weapon);

	auto shield = objects->get_shield_generator("");
	actor->giveShieldGenerator(shield);

	auto trinket = objects->get_trinket("");
	actor->giveTrinket(trinket);

	std::cout << "\rActor was given:   \n\t"
		<< energy_storage->name() << ",\n\t" << fly_engine->name() << ",\n\t" << left_weapon->name() << ",\n\t"
		<< right_weapon->name() << ",\n\t" << shield->name() << " and\n\t";
	if (trinket)
		std::cout << trinket->name() << ".\n";
	else
		std::cout << "no trinket.\n";

	actors.add(actor);
	std::cout << "\rActor was generated.\n";
	actors_broadcast(actors);
}

inline void clients_list(Clients &clients);
inline void clients_help();
inline void clients_(Clients &clients, std::istream &input) {
	std::string string;
	input >> string;
	if (string == "list")
		clients_list(clients);
	else if (string == "help")
		clients_help();
	else
		std::cout << "Unsupported map-related server command.\nCall \"map help\" for list of supported ones.\n";
}
inline void clients_list(Clients &clients) {
	std::cout << "List of clients:\n";
	for (auto &it : clients)
		std::cout << " - " << it.second << "\t- " << it.first.first << "\t- " << it.first.second <<  '\n';
}
inline void clients_help() {
	std::cout << "Supported commands:\n"
		<< " - clients list - shows the list of all the clients with their IDs.\n";
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

#include "Engines/Networking/NetworkController.hpp"
#include "Engines/Networking/ParseMessage.hpp"
inline std::thread initialize_networking(bool &server_should_close, std::shared_ptr<Objects> objects, std::shared_ptr<Map> &map, MainActorQueue &actors, Clients &clients) {
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
inline std::thread initialize_physics(PhysicsEngine *&engine, bool &server_should_close, std::shared_ptr<Objects> objects, std::shared_ptr<Map> &map, MainActorQueue &actors, DoubleProjectileQueue &projectiles, ObjectQueue &miscellaneous) {
	engine = new PhysicsEngine([&server_should_close](void) { return server_should_close; }, actors, projectiles, miscellaneous);
	engine->changeUpdateInterval(1'000'000 / objects->settings()->getUintValue("Physical_Updates_Per_Second"));
	engine->initializeCollisionSystem(map);
	return std::thread(&PhysicsEngine::loop, engine, false);
}