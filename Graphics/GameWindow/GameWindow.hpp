#pragma once

//The engine to handle this window is chosen here:
/* Variants of engines to chose from:
	* USE_DEFAULT_ENGINE
	* USE_VULKAN_ENGINE
	* USE_OPENGL_ENGINE
Just #define the one you want the window to use.
	USE_DEFAULT_ENGINE is recommended.
*/
#define USE_DEFAULT_ENGINE
#include "EngineUsage.hpp"

struct GLFWwindow;
class AbstractGraphicsEngine;
class GameCamera;
class GameMap;

class GameWindow {
private:
	GLFWwindow* m_window;
	AbstractGraphicsEngine* m_graphics;
	GameCamera* m_camera;

	size_t m_width;
	size_t m_height;
protected:
	void initialize();
	void clean();

	static void errorCallback(int errorCode, const char* description);
public:
	GameWindow(char* title, size_t width, size_t height, GameMap* map, bool insertDefaultCallback = true);
	~GameWindow();
	int loop();
};

