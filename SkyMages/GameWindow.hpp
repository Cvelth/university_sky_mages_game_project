#pragma once
#include "AbstractException.hpp"
#include <cstdint>

struct GLFWwindow;
typedef uint64_t VkSurfaceKHR;
class GameCamera;
class GameMap;

class GLFWInitializationException : AbstractException {};
class WindowInitializationException  : AbstractException {};
class GLFWCallBackException : AbstractException {};

class GameWindow {
private:
	GLFWwindow* window;
	GameCamera* camera;
	VkSurfaceKHR* surface;
protected:
	void initializeGLFW();
	void initializeWindow();

	virtual void initializeRenderProcess();
	virtual void renderProcess();
	virtual void clearRenderProcess();

	static void errorCallback(int errorCode, const char* description);
public:
	GameWindow(char* name, size_t width, size_t height, GameMap* map, bool insertDefaultCallback = true);
	~GameWindow();
	int loop();
	void makeCurrent();
};

