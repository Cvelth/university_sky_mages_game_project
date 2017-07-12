#include "GameWindow.hpp"
#include "GameMap.hpp"

int main() {
	try {
		GameWindow window("SkyMages Dev", 1280, 1024);
		GameMap map(500, 500, DefaultMapFilling::HorizontalRows);
		return window.loop();
	} catch (AbstractException e) {
		return -1;
	}
}