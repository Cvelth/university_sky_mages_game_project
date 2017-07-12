#include "GameWindow.hpp"
#include "GameMap.hpp"

int main() {
	try {
		GameMap map(500, 500, DefaultMapFilling::HorizontalRows);
		GameWindow window("SkyMages Dev", 1280, 1024, &map);
		return window.loop();
	} catch (AbstractException e) {
		return -1;
	}
}