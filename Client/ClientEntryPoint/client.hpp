#pragma once
#include "Shared/AbstractException.hpp"
#include "Engines/ObjectStorage/Objects.hpp"
void game_process(Objects *s);
int client_main() {
	Objects *o = initialize_object_storage(ProgramMode::Client);
	initialize_render_info();
	try {
		game_process(o);
	}
	catch (Exceptions::AbstractException& e) {
		e.print();
		getchar(); // Prevents Program from closing.
	}
	delete o;
	return 0;
}

#include "Shared/GameMode.hpp"
#include "Client/Window/Window.hpp"
#include "Client/Controller/ControllerInterface.hpp"
#include "Engines/Physics/PhysicsEngine.hpp"
#include "Engines/Camera/Camera.hpp"
#include "Objects/Map/Map.hpp"
#include "Objects/Map/MapGenerator.hpp"
#include "Objects/Actors/MainActor.hpp"
#include "Objects/EquipableItems/EnergyStorage.hpp"
#include "Objects/EquipableItems/FlyEngine.hpp"
#include "Objects/EquipableItems/Weapon.hpp"
#include "Engines/Graphics/HUD_RenderInfo.hpp"
#include "Objects/ObjectState/ObjectQueue.hpp"
#include "Engines/ObjectStorage/Settings.hpp"
#include "Engines/Networking/Networking.hpp"

#include <thread>
void game_process(Objects *o) {
	GameModeController::startInitialization();
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

	auto networking_thread = initialize_client([&window](void) {
		return window->isWindowClosed();
	}, [](std::string const& data) /*packed received*/ {
		throw Exceptions::AbstractException(data.c_str());
	}, o->settings()->getStringValue("Server_IP"));
	
	PhysicsEngine* physics_engine = new PhysicsEngine([&window](void) {
		return window->isWindowClosed();
	}, main_object_queue, projectile_queue, miscellaneous_object_queue);
	physics_engine->changeUpdateInterval(1'000'000 / o->settings()->getUintValue("Physical_Updates_Per_Second"));

	MainActor* main_actor = new MainActor(60.f, 2.f, 4.f, 40.f, 30.f);
	auto energy_storage = o->get_energy_storage();
	main_actor->giveEnergyStorage(energy_storage);
	main_actor->giveFlyEngine(o->get_fly_engine());
	main_actor->giveRightWeapon(o->get_weapon());

	controller->setMainActor(main_actor);
	main_object_queue->add(main_actor);

	Map *map = MapGenerator::generate_continious_map(120, 80);
	Camera *camera = new Camera(map, main_actor, window->currentAspectRatio(), 100.f);
	window->insertCamera(camera);
	physics_engine->initializeCollisionSystem(map);

	auto hud = new HUD_RenderInfo(energy_storage);
	window->insertHUDRenderInfo(hud);

	std::thread physics_thread(&PhysicsEngine::loop, physics_engine, false);
	window->loop(false);
	physics_thread.join();
	networking_thread.join();

	delete hud;
	delete camera;
	delete map;
	delete main_actor;
	delete physics_engine;
	delete window;
	delete main_object_queue;
	delete projectile_queue;
	delete miscellaneous_object_queue;
	delete controller;
}