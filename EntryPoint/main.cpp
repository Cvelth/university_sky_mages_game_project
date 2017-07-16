#include "Graphics\GameWindow\GameWindow.hpp"
#include "GameLogicEngine\GameMap.hpp"
#include "Exceptions\AbstractExceptions.hpp"

int main() {
	try {
		GameMap *map = new GameMap(500, 500, DefaultMapFilling::HorizontalRows);
		GameWindow window("SkyMages Dev", 1280, 1024, map);
		return window.loop();
	} catch (Exceptions::AbstractException& e) {
		e.print();
		auto t = getchar(); //Prevents program from closing.
		return EXIT_FAILURE;
	}
}