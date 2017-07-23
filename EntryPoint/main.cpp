#include "Graphics\GameWindow\GameWindow.hpp"
#include "GameLogicEngine\GameMap.hpp"
#include "Exceptions\AbstractExceptions.hpp"
#include "Graphics\RenderInfo\GetRenderInfo.hpp"

int main() {
	try {
		GameWindow window("SkyMages Dev", 1280, 1024, false);
		RenderInfoStorage* renderInfo = new RenderInfoStorage; 
		renderInfo->generateRenderInfo();
		GameMap *map = new GameMap(500, 500, renderInfo, DefaultMapFilling::HorizontalRows);
		window.insertMap(map);
		window.loop();
		delete map;
		delete renderInfo;
	} catch (Exceptions::AbstractException& e) {
		e.print();
		auto t = getchar(); //Prevents program from closing.
		return EXIT_FAILURE;
	}
}