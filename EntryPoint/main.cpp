#include "Graphics\GameWindow\GameWindow.hpp"
#include "Controller\GameControllerInterface.hpp"
#include "GameLogicEngine\GameMap.hpp"
#include "Graphics\RenderInfo\GetRenderInfo.hpp"
#include "Exceptions\AbstractExceptions.hpp"

int main() {
	try {
		GameWindow* window = new GameWindow("SkyMages Dev", 1280, 1024, false);

		GameControllerInterface* controller = new GameControllerInterface();
		window->insertController(controller);

		RenderInfoStorage* renderInfo = new RenderInfoStorage; 
		renderInfo->generateRenderInfo();

		GameMap *map = new GameMap(500, 500, renderInfo, DefaultMapFilling::Random);
		window->insertMap(map);

		window->loop();

		delete map;
		delete renderInfo;
		delete window;
	} catch (Exceptions::AbstractException& e) {
		e.print();
		auto t = getchar(); //Prevents program from closing.
		return EXIT_FAILURE;
	}
}