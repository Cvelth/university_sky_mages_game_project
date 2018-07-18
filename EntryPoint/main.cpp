#include "../Client/ClientEntryPoint/client.hpp"
#include "../Server/ServerEntryPoint/server.hpp"

#include <thread>

int main(int argc, char **argv) {
	server_main(argc, argv);
	client_main();
	return 0;
	//std::thread server_thread(&server_main);
}