#include "GameWindow\GameWindow.hpp"
#include "Controller\GameControllerInterface.hpp"
#include "PhysicsEngine\PhysicsEngine.hpp"
#include "RenderTools\RenderInfoStorage.hpp"
#include "LogicEngine\GameMap.hpp"
#include "GameObjects\MainActor.hpp"
#include "GameObjects\AbstractEquipableItems.hpp"
#include "Settings\Settings.hpp"
#include "Shared\AbstractException.hpp"

#include <thread>

void game_process(Settings& s) {
	GameControllerInterface* controller = new GameControllerInterface();
	auto keys = s.getKeysValue("Keys_Layout");
	controller->startKeyControl(&keys);

	GameWindow* window = new GameWindow(s.getProgramVersionInfo().c_str(),
										s.getUintValue("Screen_Width"),
										s.getUintValue("Screen_Height"),
										s.getBoolValue("Fullscreen_Window"));
	window->insertController(controller);
	window->changeUpdateInterval(1'000'000 / s.getUintValue("Graphical_Updates_Per_Second"));

	PhysicsEngine* physics_engine = new PhysicsEngine([&window](void) {
		return window->isWindowClosed();
	});
	physics_engine->changeUpdateInterval(1'000'000 / s.getUintValue("Physical_Updates_Per_Second"));

	RenderInfoStorage* renderInfoStorage = new RenderInfoStorage;
	renderInfoStorage->generateRenderInfo();

	GameMap *map = new GameMap(100, 80, renderInfoStorage, DefaultMapFilling::Continious);
	window->insertMap(map);
	physics_engine->initializeCollisionSystem(map);

	MainActor* main_actor = new MainActor(renderInfoStorage->getMainActorRenderInfo(),
										  75.f, 1.f, 4.f, 10.5f, 30.5f);
	main_actor->giveEnergyStorage(new AbstractEnergyStorage(1000.f, 15.f));
	main_actor->giveFlyEngine(new AbstractFlyEngine(10.f, 15.f, 8.f));

	controller->setMainActor(main_actor);
	physics_engine->addObject(main_actor);
	window->addToRenderQueue(main_actor);

	std::thread physics_thread(&PhysicsEngine::loop, physics_engine, false);
	window->loop(false);
	physics_thread.join();

	delete main_actor;
	delete map;
	delete renderInfoStorage;
	delete physics_engine;
	delete window;
	delete controller;
}

int main() {
	Settings s;
	try {
		s.load();
	} catch (Exceptions::SettingsVersionException& e) {
		e.print();
	} catch (Exceptions::SettingsAccessException& e) {
		s.backup();
		s.default();
		s.save();
		e.print();
	}

	try {
		game_process(s);
	} catch (Exceptions::AbstractException& e) {
		e.print();
		getchar(); // Prevents Program from closing.
	}
}