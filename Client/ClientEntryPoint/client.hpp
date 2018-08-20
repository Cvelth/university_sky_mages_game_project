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
#include "Engines/Networking/ParseMessage.hpp"
#include "Objects/Actors/MainActor.hpp"
void game_process(Objects *o, size_t &client_index) {
	ControllerInterface controller;
	auto keys = o->settings()->getKeysValue("Keys_Layout");
	controller.startKeyControl(&keys);

	MainActorQueue actors;
	DoubleProjectileQueue projectiles;
	ObjectQueue miscellaneous;

	Window window(o->get_program_version().c_str(),
				  o->settings()->getUintValue("Screen_Width"),
				  o->settings()->getUintValue("Screen_Height"),
				  o->settings()->getBoolValue("Fullscreen_Window"),
				  actors, projectiles, miscellaneous);
	window.changeController(&controller);
	window.changeUpdateInterval(1'000'000 / o->settings()->getUintValue("Graphical_Updates_Per_Second"));

	PhysicsEngine physics_engine([&window]() { return window.isWindowClosed(); }, actors, projectiles, miscellaneous);
	physics_engine.changeUpdateInterval(1'000'000 / o->settings()->getUintValue("Physical_Updates_Per_Second"));

	std::shared_ptr<MainActor> main_actor;
	std::shared_ptr<Map> map;
	std::shared_ptr<Camera> camera;
	std::shared_ptr<HUD_RenderInfo> hud;
	auto on_packet_received = [&main_actor, &map, &camera, &hud, &client_index, &window, &physics_engine, &actors, &projectiles, &miscellaneous, &o](Message const& data) /*packed received*/ {
		parse_message_from_server(data, overload(
			[&client_index, &main_actor, &actors](uint8_t _index) {
				client_index = _index;
				if (actors.size() != 0)
					main_actor = actors[client_index];
			}, [&map, &physics_engine, &camera](std::shared_ptr<Map> _map) {
				if (!_map)
					throw Exceptions::NetworkingException("Received map is corrupted.");

				physics_engine.initializeCollisionSystem(_map);
				if (camera)
					camera->changeMap(_map);

				map = _map;
			}, [&main_actor, &actors, &client_index, &hud, &camera](MainActorQueue &_actors) {
				if (_actors.size() == 0)
					throw Exceptions::NetworkingException("Received actor queue is corrupted.");
				if (actors.size() != 0 && client_index != -1)
					main_actor = actors.at(client_index);

				actors = _actors;
				if (client_index != -1) {
					auto temp = actors.at(client_index);
					if (hud) hud->change_energy_source(temp->energy_storage());
					if (camera) camera->changeCenterFigure(temp);
					main_actor = temp;
				}
			}, [&actors](Update<MainActorQueue>) -> Update<MainActorQueue> {
				return Update<MainActorQueue>(actors);
			}, [&projectiles](ProjectileQueue &_projectiles) {
				projectiles.next().clear();
				projectiles.next() = _projectiles;
				projectiles.swap();
			}), o);
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