#include "server.hpp"
#include <iostream>
#include <string>
void clients_list(Clients &clients) {
	if (clients.size() != 0) {
		std::cout << "List of clients:\n";
		for (auto &it : clients)
			std::cout << " - " << it.second << "\t- " << it.first.first << "\t- " << it.first.second << '\n';
	} else
		std::cout << "No clients connected.\n";
}
void clients_help() {
	std::cout << "Supported commands:\n"
		<< " - clients list - shows the list of all the clients with their IDs.\n";
}
void clients_(Clients &clients, std::istream &input) {
	std::string string;
	input >> string;
	if (string == "list")
		clients_list(clients);
	else if (string == "help")
		clients_help();
	else
		std::cout << "Unsupported clients-related server command.\nCall \"clients help\" for list of supported ones.\n";
}