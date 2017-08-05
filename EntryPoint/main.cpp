#include "GameWindow\GameWindow.hpp"
#include "Controller\GameControllerInterface.hpp"
#include "PhysicsEngine\PhysicsEngine.hpp"
#include "RenderTools\RenderInfoStorage.hpp"
#include "LogicEngine\GameMap.hpp"
#include "GameObjects\AbstractActor.hpp"
#include "Settings\Settings.hpp"
#include "Shared\AbstractException.hpp"

#include <thread>

void game_process(Settings& s) {
	GameControllerInterface* controller = new GameControllerInterface();
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

	ControllableActor* main_actor = new ControllableActor(renderInfoStorage->getMainActorRenderInfo(),
														  75.f, 1.f, 4.f, 10.5f, 30.5f);
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