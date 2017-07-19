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

//Class to store game window.
//It stores and handles connection between the window handler(GLFWwindow struct) and chosen Engine.
class GameWindow {
private:
	GLFWwindow* m_window;
	AbstractGraphicsEngine* m_graphics;
	GameCamera* m_camera;

	size_t m_width;
	size_t m_height;
protected:

	//Creates engine instance, shows a window and initializes the connection between them.
	void initialize();

	//Cleans all the instances of windows and engines created during work process.
	void clean();

	//static void errorCallback(int errorCode, const char* description);
public:

	//Constructs new window and engine using initialize function (protected).
	//Needed to pass window title and dimention sizes.
	GameWindow(char* title, size_t width, size_t height, GameMap* map, bool insertDefaultCallback = true);

	//Desctructs class and all dependent on it. Uses clean function.
	~GameWindow();

	//Main game loop. 
	//Initializes engine rendering state (sends data to graphical device).
	//In loop uses the redrawing in order to update the view and polls new events in order for engines to handle them.
	//After window is manually closed by the user, it cleans rendering data using engine and finishes executing.
	int loop();
};

