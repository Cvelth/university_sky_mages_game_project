#include "Graphics\GameWindow\GameWindow.hpp"
#include "Controller\GameControllerInterface.hpp"
#include "GameLogicEngine\GameMap.hpp"
#include "Graphics\RenderInfo\GetRenderInfo.hpp"
#include "Exceptions\AbstractExceptions.hpp"
#include "Exceptions\OtherExceptions.hpp"
#include "GameObjects\AbstractActor.hpp"
#include "PhysicsEngine\PhysicsEngine.hpp"
#include "Graphics\AbstractGraphicsEngine\RenderQueue.hpp"
#include "Settings\Settings.hpp"

#include <thread>
int main() {
	Settings s;
	try {
		s.load();
	} catch (Exceptions::DifferentSettingFileVersionException& e) {
		e.print();
	} catch (Exceptions::DifferentProgramVersionException& e) {
		e.print();
	} catch (Exceptions::SettingsException& e) {
		s.backup();
		s.default();
		s.save();
		e.print();
	}

	try {
		GameWindow* window = new GameWindow("SkyMages Dev", 1280, 1024, false);

		GameControllerInterface* controller = new GameControllerInterface();
		window->insertController(controller);

		PhysicsEngine* physics_engine = new PhysicsEngine([&window](void) {
			return window->isWindowClosed();
		});

		RenderInfoStorage* renderInfoStorage = new RenderInfoStorage;
		renderInfoStorage->generateRenderInfo();

		GameMap *map = new GameMap(100, 80, renderInfoStorage, DefaultMapFilling::Continious);
		window->insertMap(map);

		ControllableActor* main_actor = new ControllableActor(renderInfoStorage->getMainActorRenderInfo(), 
															  75.f, 5.f, 0.f);
		controller->setMainActor(main_actor);
		physics_engine->addObject(main_actor);
		window->getRenderQueue()->add(main_actor);

		std::thread physics_thread(&PhysicsEngine::loop, physics_engine, true);
		physics_thread.detach();
		window->loop();

		delete main_actor;
		delete map;
		delete renderInfoStorage;
		delete window;

		return EXIT_SUCCESS;
	} catch (Exceptions::AbstractException& e) {
		e.print();
		auto t = getchar(); //Prevents program from closing.
		return EXIT_FAILURE;
	}
}