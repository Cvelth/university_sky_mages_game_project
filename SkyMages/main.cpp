#include "GameWindow.hpp"

int main() {
	try {
		GameWindow *window = new GameWindow("SkyMages Dev", 1280, 1024);
		return window->loop();
	} catch (AbstractException e) {
		return -1;
	}
}