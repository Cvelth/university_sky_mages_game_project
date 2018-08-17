#pragma once
#include "../../Engines/ObjectStorage/Objects.hpp"
#include "../../Shared/AbstractException.hpp"
void server_process(Objects *objects);
int server_main(int argc, char **argv) {
	Objects *objects = initialize_object_storage(ProgramMode::Server);
	initialize_render_info();
	try {
		server_process(objects);
	} catch (Exceptions::AbstractException &e) {
		e.print();
		getchar(); // Prevents Program from closing.
	}
	delete objects;
	exit(0);
}

#include <memory>
#include <sstream>
#include <iostream>
#include <thread>
class Map;
inline void map_(std::shared_ptr<Map> &map, std::istream &input);
inline void actors_(std::istream &input); 
inline void help_();
inline void exit_(bool &server_should_close);
inline std::thread initialize_networking(bool &server_should_close, std::shared_ptr<Map> &map);
void server_process(Objects *objects) {
	std::cout << "Starting server...\n";
	std::shared_ptr<Map> map;
	bool server_should_close = false;
	auto networking_thread = initialize_networking(server_should_close, map);
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
			actors_(input);
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

inline void actors_initialize();
inline void actors_clean();
inline void actors_broadcast();
inline void actors_help();
inline void actors_(std::istream &input) {
	std::string string;
	input >> string;
	if (string == "initialize")
		actors_initialize();
	else if (string == "clean")
		actors_clean();
	else if (string == "broadcast")
		actors_broadcast();
	else if (string == "help")
		actors_help();
	else
		std::cout << "Unsupported actors-related server command.\nCall \"actors help\" for list of supported ones.\n";
}
inline void actors_initialize() {
	std::cout << "Initializing actor queue...\n";
	//To be implemented.
}
inline void actors_clean() {
	std::cout << "Cleaning actor queue...\n";
	//To be implemented.
}
inline void actors_broadcast() {
	std::cout << "Broadcasting actor queue...\n";
	//To be implemented.
}
inline void actors_help() {
	std::cout << "Supported commands:\n"
		<< " - actors initialize - initializes server main actor queue with 9 actors (and their default colors).\n"
		<< " - actors clean - cleans actor data.\n"
		<< " - actors clean - sends actor data to all the clients.\n";
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

inline std::thread initialize_networking(bool &server_should_close, std::shared_ptr<Map> &map) {
	auto on_peer_connect = [&map](std::string const& name, size_t port) {
		std::cout << "\b\bClient " << name << ":" << port << " has been connected.\n";
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