#pragma once
#include "../../Engines/ObjectStorage/Objects.hpp"
#include "../../Engines/ObjectStorage/Settings.hpp"
#include "../../Engines/ObjectStorage/MapStorage.hpp"
#include "../../Objects/Map/MapGenerator.hpp"
#include "../../Objects/Map/Map.hpp"
#include <iostream>
#include <sstream>
#include <chrono>
int server_main(int argc, char **argv) {
	Objects *objects;
	Map *map = nullptr;
	bool server_should_close = false;

	objects = initialize_object_storage(ProgramMode::Server);
	std::cout << objects->get_program_version() << " server has been started.\n";	
	while (!server_should_close) {
		std::cout << ": ";
		std::string string;
		std::getline(std::cin, string);
		std::istringstream input(string);
		input >> string;
		if (string == "map") {
			input >> string;
			if (string == "save") {
				std::ostringstream filename;
				filename << "Map_" << + std::chrono::high_resolution_clock::now().time_since_epoch().count();
				string = filename.str();
				if (input) input >> string;
				if (map) {
					MapStorage storage;
					storage.save(map, string, "maps/");
					std::cout << "Map was saved.\n";
				} else
					std::cout << "Cannot save non-existing map. Try generating or loading one.\n";
			} else if (string == "generate") {
				if (map) delete map;
				map = MapGenerator::generate_continious_map(120, 80);
				std::cout << "Map was generated.\n";
			} else if (string == "load") {

			} else if (string == "help") {
				std::cout << "Supported commands:\n"
					<< " - map save [<filename>] - saves currently loaded map to /maps/<filename>.mpf.\n"
					<< " - map load <filename> - loads a map from /maps/<filename>.mpf.\n"
					<< " - map generate - generates new map.\n";
			} else
				std::cout << "Unsupported map-related server command.\nType \"map help\" for list of supported ones.\n";
		} else if (string == "help") {
			std::cout << "Supported commands:\n" 
				<< " - map - access to map-related commands. Call \"map help\" for more details.\n"
				<< " - exit - closes the server after cleaning up.\n";
		} else if (string == "exit") {
			server_should_close = true;
		} else
			std::cout << "Unsupported server command.\nType \"help\" for list of supported ones.\n";
	}

	std::cout << "Cleaning up...";
	if (map) delete map;
	delete objects;
	exit(0);
}