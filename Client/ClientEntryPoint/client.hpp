#pragma once
#include "Shared/AbstractException.hpp"
#include "Shared/GameStateController.hpp"
#include "Engines/ObjectStorage/Objects.hpp"
void game_process(Objects *s, size_t &client_index);
int client_main() {
	GameStateController::initialize(ProgramMode::Client);
	size_t client_index = -1;
	Objects *o = initialize_object_storage();
	initialize_render_info();
	try {
		game_process(o, client_index);
	}
	catch (std::exception& e) {
		e.what();
		getchar(); // Prevents Program from closing.
	}
	delete o;
	return 0;
}

#include "Client/Controller/ControllerInterface.hpp"
#include "Engines/ObjectStorage/Settings.hpp"
#include "Objects/ObjectState/ObjectQueue.hpp"
#include "Client/Window/Window.hpp"
#include "Engines/Physics/PhysicsEngine.hpp"
#include "Engines/Networking/Networking.hpp"
#include "Engines/ObjectStorage/MapStorage.hpp"
#include "Client/Controller/Camera.hpp"
#include "Objects/Actors/MainActor.hpp"
#include "Engines/Graphics/HUD_RenderInfo.hpp"
#include "Engines/Networking/NetworkController.hpp"
void game_process(Objects *o, size_t &client_index) {
	ControllerInterface* controller = new ControllerInterface();
	auto keys = o->settings()->getKeysValue("Keys_Layout");
	controller->startKeyControl(&keys);

	auto main_object_queue = new MainActorQueue();
	auto projectile_queue = new ProjectileQueue();
	auto miscellaneous_object_queue = new ObjectQueue();

	Window* window = new Window(o->get_program_version().c_str(),
								o->settings()->getUintValue("Screen_Width"),
								o->settings()->getUintValue("Screen_Height"),
								o->settings()->getBoolValue("Fullscreen_Window"),
								main_object_queue, projectile_queue, miscellaneous_object_queue);
	window->insertController(controller);
	window->changeUpdateInterval(1'000'000 / o->settings()->getUintValue("Graphical_Updates_Per_Second"));

	PhysicsEngine* physics_engine = new PhysicsEngine([&window](void) {
		return window->isWindowClosed();
	}, main_object_queue, projectile_queue, miscellaneous_object_queue);
	physics_engine->changeUpdateInterval(1'000'000 / o->settings()->getUintValue("Physical_Updates_Per_Second"));

	Map *map = nullptr;
	auto networking_thread = Networking::initialize_client([&window](void) {
		return window->isWindowClosed();
	}, [&window, &physics_engine, &map, &client_index, &main_object_queue, &projectile_queue, &miscellaneous_object_queue, &o](std::string const& data) /*packed received*/ {
		std::istringstream s(data);
		std::string string;
		std::getline(s, string);
		if (string == "Index") {
			s >> client_index;
		} else if (string == "Map") {
			map = MapStorage::string_to_map(data.substr(string.size() + 1));
			if (!map)
				throw Exceptions::NetworkingException(("Received map is corrupted: " + data).c_str());
		} else if (string == "MainActorQueue") {
			std::getline(s, string);
			if (string != "Queue")
				throw Exceptions::NetworkingException(("Received data is corrupted: " + data).c_str());
			main_object_queue->clear();
			while (std::getline(s, string)) {
				main_object_queue->add(string_to_main_actor(string, o));
			}
		} else if (string == "QueueUpdate") {
			if (client_index != -1 && map && main_object_queue->size() != 0)
				NetworkController::update_state(data, main_object_queue, projectile_queue, miscellaneous_object_queue);
		} else
			throw Exceptions::NetworkingException(("Unsupported data was received from server: " + data).c_str());
	}, o->settings()->getStringValue("Server_IP"));

	//Waiting for initial server answer.
	while (client_index == -1 || !map || main_object_queue->size() == 0)
		std::this_thread::sleep_for(std::chrono::microseconds(window->getUpdateInterval()));

	MainActor *main_actor = &*main_object_queue->at(client_index);

	physics_engine->initializeCollisionSystem(map);
	Camera *camera = new Camera(map, main_actor, window->currentAspectRatio(), 100.f);
	window->insertCamera(camera);

	controller->setMainActor(main_actor);

	auto hud = new HUD_RenderInfo(main_actor->energy_storage());
	window->insertHUDRenderInfo(hud);

	std::thread physics_thread(&PhysicsEngine::loop, physics_engine, false);
	window->loop(false);
	physics_thread.join();
	networking_thread.join();

	delete hud;
	delete camera;
	delete physics_engine;
	delete window;
	delete main_object_queue;
	delete projectile_queue;
	delete miscellaneous_object_queue;
	delete controller;
}