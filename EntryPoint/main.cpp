#include "Graphics\GameWindow\GameWindow.hpp"
#include "Controller\GameControllerInterface.hpp"
#include "GameLogicEngine\GameMap.hpp"
#include "Graphics\RenderInfo\GetRenderInfo.hpp"
#include "Exceptions\AbstractExceptions.hpp"
#include "GameObjects\ControllableActor.hpp"
#include "PhysicsEngine\PhysicsEngine.hpp"

#include <thread>
int main() {
	try {
		GameWindow* window = new GameWindow("SkyMages Dev", 1280, 1024, false);

		GameControllerInterface* controller = new GameControllerInterface();
		window->insertController(controller);

		PhysicsEngine* physics_engine = new PhysicsEngine([&window](void) {
			return window->isWindowClosed();
		});

		RenderInfoStorage* renderInfo = new RenderInfoStorage; 
		renderInfo->generateRenderInfo();

		GameMap *map = new GameMap(100, 80, renderInfo, DefaultMapFilling::Continious);
		window->insertMap(map);

		ControllableActor* main_actor = new ControllableActor();//(renderInfo, 50.f, 30.f, 0.f, 0.f, 0.f, 0.f);
		controller->setMainActor(main_actor);
		physics_engine->addObject(main_actor);

		std::thread physics_thread(&PhysicsEngine::loop, physics_engine, true);
		physics_thread.detach();
		window->loop();

		delete main_actor;
		delete map;
		delete renderInfo;
		delete window;

		return EXIT_SUCCESS;
	} catch (Exceptions::AbstractException& e) {
		e.print();
		auto t = getchar(); //Prevents program from closing.
		return EXIT_FAILURE;
	}
}