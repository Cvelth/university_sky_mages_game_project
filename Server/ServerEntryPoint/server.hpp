#pragma once
#include "../../Engines/ObjectStorage/Objects.hpp"
#include "../../Engines/ObjectStorage/Settings.hpp"
int server_main(int argc, char **argv) {
	auto objects = initialize_object_storage(ProgramMode::Server);
	return 0;
}