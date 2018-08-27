#include "Shared/AbstractException.hpp"
#include "Shared/GameStateController.hpp"
#include "Engines/ObjectStorage/Objects.hpp"
#include "Engines/ObjectStorage/Settings.hpp"
void server_process(std::shared_ptr<Objects> objects);
int server_main(int argc, char **argv) {
	GameStateController::initialize(ProgramMode::Server);
	auto objects = initialize_object_storage();
	try {
		server_process(objects);
	} catch (std::exception &e) {
		e.what();
		getchar(); // Prevents Program from closing.
	}
	exit(0);
}
int main(int argc, char **argv) { return server_main(argc, argv); }