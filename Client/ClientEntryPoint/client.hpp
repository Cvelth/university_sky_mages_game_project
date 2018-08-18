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
#include "Engines/ObjectStorage/MapStorage.hpp"
#include "Engines/Networking/Networking.hpp"
#include "Client/Controller/Camera.hpp"
#include "Engines/Graphics/HUD_RenderInfo.hpp"
#include "Engines/Networking/NetworkController.hpp"
void game_process(Objects *o, size_t &client_index) {
	ControllerInterface controller;
	auto keys = o->settings()->getKeysValue("Keys_Layout");
	controller.startKeyControl(&keys);

	MainActorQueue actors;
	ProjectileQueue projectiles;
	ObjectQueue miscellaneous;

	Window window(o->get_program_version().c_str(),
				  o->settings()->getUintValue("Screen_Width"),
				  o->settings()->getUintValue("Screen_Height"),
				  o->settings()->getBoolValue("Fullscreen_Window"),
				  &actors, &projectiles, &miscellaneous);
	window.insertController(&controller);
	window.changeUpdateInterval(1'000'000 / o->settings()->getUintValue("Graphical_Updates_Per_Second"));

	PhysicsEngine physics_engine([&window]() { return window.isWindowClosed(); }, &actors, &projectiles, &miscellaneous);
	physics_engine.changeUpdateInterval(1'000'000 / o->settings()->getUintValue("Physical_Updates_Per_Second"));

	std::shared_ptr<MainActor> main_actor;
	std::shared_ptr<Map> map;
	std::shared_ptr<Camera> camera;
	std::shared_ptr<HUD_RenderInfo> hud;
	auto on_packet_received = [&main_actor, &map, &camera, &hud, &client_index, &window, &physics_engine, &actors, &projectiles, &miscellaneous, &o](std::string const& data) /*packed received*/ {
		std::istringstream s(data);
		std::string string;
		std::getline(s, string);
		if (string == "Index") {
			s >> client_index;
		} else if (string == "Map") {
			auto temp = std::shared_ptr<Map>(MapStorage::string_to_map(data.substr(string.size() + 1)));
			if (!temp)
				throw Exceptions::NetworkingException(("Received map is corrupted: " + data).c_str());

			physics_engine.initializeCollisionSystem(temp);
			if (camera)
				camera->changeMap(temp);
			map = std::shared_ptr<Map>(temp);
		} else if (string == "MainActorQueue") {
			std::getline(s, string);
			if (string != "Queue")
				throw Exceptions::NetworkingException(("Received data is corrupted: " + data).c_str());
			if (actors.size() != 0 && client_index != -1)
				main_actor = actors.at(client_index);

			actors.clear();
			while (std::getline(s, string)) {
				actors.add(string_to_main_actor(string, o));
			}
			if (client_index != -1) {
				auto temp = actors.at(client_index);
				if (hud) hud->change_energy_source(temp->energy_storage());
				if (camera) camera->changeCenterFigure(temp);
				main_actor = temp;
			}
		} else if (string == "QueueUpdate") {
			if (client_index != -1 && map && actors.size() != 0)
				NetworkController::update_state(data, &actors, &projectiles, &miscellaneous);
		} else
			throw Exceptions::NetworkingException(("Unsupported data was received from server: " + data).c_str());
	};

	auto networking_thread = Networking::initialize_client([&window](void) { return window.isWindowClosed(); }, 
														   on_packet_received, o->settings()->getStringValue("Server_IP"));

	//Waiting for initial server answer.
	while (client_index == -1 || !map || actors.size() == 0)
		std::this_thread::sleep_for(std::chrono::microseconds(window.getUpdateInterval()));

	physics_engine.initializeCollisionSystem(map);
	camera = std::make_shared<Camera>(map, main_actor, window.currentAspectRatio(), 100.f);
	window.insertCamera(camera);

	controller.setMainActor(main_actor);

	hud = std::make_shared<HUD_RenderInfo>(main_actor->energy_storage());
	window.insertHUDRenderInfo(&*hud);

	std::thread physics_thread(&PhysicsEngine::loop, physics_engine, false);
	window.loop(false);
	physics_thread.join();
	networking_thread.join();
}