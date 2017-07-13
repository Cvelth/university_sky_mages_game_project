#include "GameWindow.hpp"
#include "GameMap.hpp"
#include "AbstractExceptions.hpp"
#include "WindowExceptions.hpp"

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