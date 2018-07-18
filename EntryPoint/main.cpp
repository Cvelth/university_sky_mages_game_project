#include "../Client/ClientEntryPoint/client.hpp"
#include "../Server/ServerEntryPoint/server.hpp"

#include <thread>

int main(int argc, char **argv) {
	std::thread server_thread(std::bind(&server_main, argc, argv));
	std::thread client_thread(&client_main);
	client_thread.join();
	server_thread.join();
	return 0;
}