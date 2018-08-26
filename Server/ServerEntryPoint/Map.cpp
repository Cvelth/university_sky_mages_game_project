#include "server.hpp"
#include <iostream>
#include <sstream>
#include <chrono>

#include "Engines/ObjectStorage/MapStorage.hpp"
void map_save(std::shared_ptr<Map> &map, std::istream &input) {
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
void map_load(std::shared_ptr<Map> &map, std::istream &input) {
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

#include "Objects/Map/MapGenerator.hpp"
void map_generate(std::shared_ptr<Map> &map, std::istream &input) {
	std::cout << "Generating map...";
	size_t width = 120, height = 80;
	if (input) {
		input >> width >> height;
	}
	map = std::shared_ptr<Map>(MapGenerator::generate_continious_map(width, height));
	std::cout << "\rMap was generated.\n";
	map_broadcast(map);
}

#include "Engines/Networking/Networking.hpp"
#include "Engines/Networking/Message.hpp"
#include "Engines/Physics/PhysicsEngine.hpp"
extern PhysicsEngine *physics_engine;
void map_broadcast(std::shared_ptr<Map> &map) {
	std::cout << "Broadcasting map...";
	if (map) {
		Networking::bcast_from_server(make_map_message(map));
		physics_engine->initializeCollisionSystem(map);
		std::cout << "\rMap was broadcasted.\n";
	} else
		std::cout << "\rCannot broadcast non-existing map. Try generating or loading one.\n";
}
void map_help() {
	std::cout << "Supported commands:\n"
		<< " - map save [<filename>] - saves currently loaded map to /maps/<filename>.mpf.\n"
		<< " - map load <filename> - loads a map from /maps/<filename>.mpf.\n"
		<< " - map generate [<width> <height>] - generates new map (<width>x<height>). Default values: width = 120 and height = 80.\n"
		<< " - map broadcast - sends map to all the client hosts.\n";
}
void map_(std::shared_ptr<Map> &map, std::istream &input) {
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