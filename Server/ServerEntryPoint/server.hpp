#pragma once
#include "../../Engines/ObjectStorage/Objects.hpp"
#include "../../Shared/AbstractException.hpp"
void server_process(Objects *objects);
int server_main(int argc, char **argv) {
	Objects *objects = initialize_object_storage(ProgramMode::Server);
	initialize_render_info();
	try {
		server_process(objects);
	} catch (std::exception &e) {
		e.what();
		getchar(); // Prevents Program from closing.
	}
	delete objects;
	exit(0);
}

#include <memory>
#include <sstream>
#include <iostream>
#include <thread>
#include "../../Objects/ObjectState/ObjectQueue.hpp"
class Map;
inline void map_(std::shared_ptr<Map> &map, std::istream &input);
inline void actors_(MainActorQueue &actors, std::istream &input);
inline void help_();
inline void exit_(bool &server_should_close);
inline std::thread initialize_networking(bool &server_should_close, Objects *objects, std::shared_ptr<Map> &map, MainActorQueue &actors);
void server_process(Objects *objects) {
	std::cout << "Starting server...\n";
	std::shared_ptr<Map> map;
	MainActorQueue actors;
	bool server_should_close = false;
	auto networking_thread = initialize_networking(server_should_close, objects, map, actors);
	std::cout << objects->get_program_version() << " server has been started.\n";

	while (!server_should_close) {
		std::cout << ": ";
		std::string string;
		std::getline(std::cin, string);
		std::istringstream input(string);
		input >> string;
		if (string == "map")
			map_(map, input);
		else if (string == "actors")
			actors_(actors, input);
		else if (string == "help")
			help_();
		else if (string == "exit")
			exit_(server_should_close);
		else
			std::cout << "Unsupported server command.\nCall \"help\" for list of supported ones.\n";
	}

	std::cout << "Cleaning up...";
	networking_thread.join();
}

#include "../../Engines/ObjectStorage/MapStorage.hpp"
#include "../../Objects/Map/MapGenerator.hpp"
#include "../../Engines/Networking/Networking.hpp"
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
	std::cout << "Saving map...\n";
	std::ostringstream filename;
	filename << "Map_" << std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::string string = filename.str();
	if (input) input >> string;
	if (map) {
		MapStorage storage;
		storage.save(&*map, string, "maps/");
		std::cout << "Map was saved.\n";
		map_broadcast(map);
	} else
		std::cout << "Cannot save non-existing map. Try generating or loading one.\n";
}
inline void map_generate(std::shared_ptr<Map> &map, std::istream &input) {
	std::cout << "Generating map...\n";
	size_t width = 120, height = 80;
	if (input) {
		input >> width >> height;
	}
	map = std::shared_ptr<Map>(MapGenerator::generate_continious_map(width, height));
	std::cout << "Map was generated.\n";
	map_broadcast(map);
}
inline void map_load(std::shared_ptr<Map> &map, std::istream &input) {
	std::cout << "Loading map...\n";
	if (input) {
		std::string string;
		input >> string;
		MapStorage storage;
		try {
			map = std::shared_ptr<Map>(storage.load(string, "maps/"));
			std::cout << "Map was loaded.\n"; 
			map_broadcast(map);
		} catch (Exceptions::FileCannotBeOpennedException e) {
			e.print();
		} catch (Exceptions::FileParsingException e) {
			e.print();
		}
	} else
		std::cout << "<filename> required.\nCall \"map help\" for more info.\n";
}
inline void map_broadcast(std::shared_ptr<Map> &map) {
	std::cout << "Broadcasting map...\n";
	if (map) {
		bcast_from_server("Map\n" + MapStorage::map_to_string(&*map), 0, true);
		std::cout << "Map was broadcasted.\n";
	} else
		std::cout << "Cannot broadcast non-existing map. Try generating or loading one.\n";
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
inline void actors_initialize(MainActorQueue &actors);
inline void actors_clean(MainActorQueue &actors);
inline void actors_broadcast(MainActorQueue &actors);
inline void actors_help();
inline void actors_(MainActorQueue &actors, std::istream &input) {
	std::string string;
	input >> string;
	/*if (string == "initialize")
		actors_initialize();
	else if (string == "clean")
		actors_clean();
	else*/ if (string == "broadcast")
		actors_broadcast(actors);
	else if (string == "help")
		actors_help();
	else
		std::cout << "Unsupported actors-related server command.\nCall \"actors help\" for list of supported ones.\n";
}
inline void actors_initialize(MainActorQueue &actors) {
	std::cout << "Initializing actor queue...\n";
	//To be implemented.
}
inline void actors_clean(MainActorQueue &actors) {
	std::cout << "Cleaning actor queue...\n";
	//To be implemented.
}
inline void actors_broadcast(MainActorQueue &actors) {
	std::cout << "Broadcasting actor queue...\n";
	bcast_from_server("MainActorQueue\n" + actors.to_string(), 1, false);
	std::cout << "Actor queue was broadcasted.\n";
}
inline void actors_help() {
	std::cout << "Supported commands:\n"
		<< " - [[deprecated]] actors initialize - initializes server main actor queue with 9 actors (and their default colors).\n"
		<< " - [[deprecated]] actors clean - cleans actor data.\n"
		<< " - actors clean - sends actor data to all the clients.\n";
}
inline void actors_add(Objects *objects, MainActorQueue &actors) {
	std::cout << "Adding new actor...\n";
	auto actor = new MainActor(60.f, mgl::math::vector{0.f,0.f}, mgl::math::vector{0.f,0.f}, mgl::math::vector{30.f,50.f}, mgl::math::vector{2.f, 4.f}, RenderInfoStorage::getRenderInfo("MainActor", actors.size()));
	actor->giveEnergyStorage(objects->get_energy_storage(""));
	actor->giveFlyEngine(objects->get_fly_engine(""));
	actor->giveRightWeapon(objects->get_weapon(""));

	actors.add(actor);
	std::cout << "Actor was successfully generated.\n";
	actors_broadcast(actors);
}

inline void help_() {
	std::cout << "Supported commands:\n"
		<< " - map - access to map-related commands. Call \"map help\" for more details.\n"
		<< " - actors - access to actor-related commands. Call \"actors help\" for more details.\n"
		<< " - exit - closes the server after cleaning up.\n";
}

inline void exit_(bool &server_should_close) {
	server_should_close = true;
}
inline std::string id(MainActorQueue &actors) {
	std::ostringstream s;
	s << "Index " << actors.size();
	return s.str();
}
inline std::thread initialize_networking(bool &server_should_close, Objects *objects, std::shared_ptr<Map> &map, MainActorQueue &actors) {
	auto on_peer_connect = [&map, &actors, &objects](std::string const& name, size_t port, std::function<void(std::string)> send_back) {
		std::cout << "\b\bClient " << name << ":" << port << " has been connected.\n";
		send_back(id(actors));
		actors_add(objects, actors);
		map_broadcast(map);

		std::cout << ": ";
	};
	auto on_peer_disconnect = [](std::string const& name, size_t port) {
		std::cout << "\b\bClient " << name << ":" << port << " has been disconnected.\n: ";
	};
	auto on_packet_received = [](std::string const& data) {
		std::cout << "\b\bA packet with " << data << " was received.\n: ";
	};

	return initialize_server(server_should_close, on_peer_connect, on_peer_disconnect, on_packet_received);
}