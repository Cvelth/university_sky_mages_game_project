#pragma once
#include "../../Engines/ObjectStorage/Objects.hpp"
#include "../../Shared/AbstractException.hpp"
#include <iostream>
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
#include "../../Engines/ObjectStorage/Settings.hpp"
#include "../../Engines/ObjectStorage/MapStorage.hpp"
#include "../../Objects/Map/MapGenerator.hpp"
#include "../../Objects/Map/Map.hpp"
#include "../../Engines/Networking/Networking.hpp"
#include "../../Engines/ObjectStorage/RenderInfoStorage.hpp"
#include <sstream>
#include <chrono>
void server_process(Objects *objects) {
	Map *map = nullptr;
	bool server_should_close = false;

	auto networking_thread = initialize_server(server_should_close,
					  [&](std::string const& name, size_t port) /*peer connected*/ {
		std::cout << "\b\bClient " << name << ":" << port << " has been connected.\n";
		if (map) {
			bcast_from_server("Map\n" + MapStorage::map_to_string(map), 0, true);
			std::cout << "Map was broadcasted.\n";
		}
		std::cout << ": ";
	},
					  [](std::string const& name, size_t port) /*peer disconnected*/ {
		std::cout << "\b\bClient " << name << ":" << port << " has been disconnected.\n: "; 
	},
					  [](std::string const& data) /*packet received*/ {
		std::cout << "\b\bA packet with " << data << " was received.\n: ";
	});
	
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
				filename << "Map_" << std::chrono::high_resolution_clock::now().time_since_epoch().count();
				string = filename.str();
				if (input) input >> string;
				if (map) {
					MapStorage storage;
					storage.save(map, string, "maps/");
					std::cout << "Map was saved.\n";
				} else
					std::cout << "Cannot save non-existing map. Try generating or loading one.\n";
			} else if (string == "generate") {
				size_t width = 120, height = 80;
				if (input) {
					input >> width >> height;
				}
				if (map) delete map;
				map = MapGenerator::generate_continious_map(width, height);
				std::cout << "Map was generated.\n";
				if (map) {
					bcast_from_server("Map\n" + MapStorage::map_to_string(map), 0, true);
					std::cout << "Map was broadcasted.\n";
				}
			} else if (string == "load") {
				if (input) {
					input >> string;
					MapStorage storage;
					try {
						map = storage.load(string, "maps/");
						std::cout << "Map was loaded.\n";
						if (map) {
							bcast_from_server("Map\n" + MapStorage::map_to_string(map), 0, true);
							std::cout << "Map was broadcasted.\n";
						}
					} catch (Exceptions::FileCannotBeOpennedException e) {
						e.print();
					} catch (Exceptions::FileParsingException e) {
						e.print();
					}
				} else
					std::cout << "<filename> required.\nCall \"map help\" for more info.\n";
			} else if (string == "help") {
				std::cout << "Supported commands:\n"
					<< " - map save [<filename>] - saves currently loaded map to /maps/<filename>.mpf.\n"
					<< " - map load <filename> - loads a map from /maps/<filename>.mpf.\n"
					<< " - map generate [<width> <height>] - generates new <width>x<height> map. Default values (if no others were provided) are 120x80.\n";
			} else
				std::cout << "Unsupported map-related server command.\nCall \"map help\" for list of supported ones.\n";
		} else if (string == "help") {
			std::cout << "Supported commands:\n"
				<< " - map - access to map-related commands. Call \"map help\" for more details.\n"
				<< " - exit - closes the server after cleaning up.\n";
		} else if (string == "exit") {
			server_should_close = true;
		} else
			std::cout << "Unsupported server command.\nCall \"help\" for list of supported ones.\n";
	}

	std::cout << "Cleaning up...";
	networking_thread.join();
	if (map) delete map;
}